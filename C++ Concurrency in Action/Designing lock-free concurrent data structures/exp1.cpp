//使用std::atomic_flag实现的自旋锁
#include <mutex>
#include <atomic>
class spinlock_mutex
{
  std::atomic_flag flag;

public:
  spinlock_mutex() : flag(ATOMIC_FLAG_INIT)
  {
  }
  void lock()
  {
    while (flag.test_and_set(std::memory_order_acquire))
      ;
  }
  void unlock()
  {
    flag.clear(std::memory_order_release);
  }
};

//不用锁实现push
template <typename T>
class lock_free_stack
{
private:
  struct node
  {
    T data;
    node *next;

    node(T const &data_) : // 1
                           data(data_)
    {
    }
  };

  std::atomic<node *> head;

public:
  void push(T const &data)
  {
    node *const new_node = new node(data); // 2
    new_node->next = head.load();          // 3
    while (!head.compare_exchange_weak(new_node->next, new_node))
      ; // 4
  }
};

template <typename T>
class lock_free_stack
{
public:
  void pop(T &result)
  {
    node *old_head = head.load();
    while (!head.compare_exchange_weak(old_head, old_head->next))
      ;
    result = old_head->data;
  }
};

//会泄漏节点的无锁栈
template <typename T>
class lock_free_stack
{
private:
  struct node
  {
    std::shared_ptr<T> data; // 1 指针获取数据
    node *next;

    node(T const &data_) : data(std::make_shared<T>(data_)) // 2 让std::shared_ptr指向新分配出来的T
    {
    }
  };

  std::atomic<node *> head;

public:
  void push(T const &data)
  {
    node *const new_node = new node(data);
    new_node->next = head.load();
    while (!head.compare_exchange_weak(new_node->next, new_node))
      ;
  }
  std::shared_ptr<T> pop()
  {
    node *old_head = head.load();
    while (old_head && // 3 在解引用前检查old_head是否为空指针
           !head.compare_exchange_weak(old_head, old_head->next))
      ;
    return old_head ? old_head->data : std::shared_ptr<T>(); // 4
  }
};
//没有线程访问pop时就对节点进行回收
template <typename T>
class lock_free_stack
{
private:
  std::atomic<unsigned> threads_in_pop; // 1 原子变量
  void try_reclaim(node *old_head);

public:
  std::shared_ptr<T> pop()
  {
    ++threads_in_pop; // 2 在做事之前，计数值加1
    node *old_head = head.load();
    while (old_head &&
           !head.compare_exchange_weak(old_head, old_head->next))
      ;
    std::shared_ptr<T> res;
    if (old_head)
    {
      res.swap(old_head->data); // 3 回收删除的节点
    }
    try_reclaim(old_head); // 4 从节点中直接提取数据，而非拷贝指针
    return res;
  }
};

template <typename T>
class lock_free_stack //引用计数的回收机制
{
private:
  std::atomic<node *> to_be_deleted;

  static void delete_nodes(node *nodes)
  {
    while (nodes)
    {
      node *next = nodes->next;
      delete nodes;
      nodes = next;
    }
  }
  void try_reclaim(node *old_head)
  {
    if (threads_in_pop == 1) // 1
    {
      node *nodes_to_delete = to_be_deleted.exchange(nullptr); // 2 声明“可删除”列表
      if (!--threads_in_pop)                                   // 3 是否只有一个线程调用pop()?
      {
        delete_nodes(nodes_to_delete); // 4
      }
      else if (nodes_to_delete) // 5
      {
        chain_pending_nodes(nodes_to_delete); // 6
      }
      delete old_head; // 7
    }
    else
    {
      chain_pending_node(old_head); // 8
      --threads_in_pop;
    }
  }
  void chain_pending_nodes(node *nodes)
  {
    node *last = nodes;
    while (node *const next = last->next) // 9 让next指针指向链表的末尾
    {
      last = next;
    }
    chain_pending_nodes(nodes, last);
  }

  void chain_pending_nodes(node *first, node *last)
  {
    last->next = to_be_deleted;                  // 10
    while (!to_be_deleted.compare_exchange_weak( // 11 用循环来保证last->next的正确性
        last->next, first))
      ;
  }
  void chain_pending_node(node *n)
  {
    chain_pending_nodes(n, n); // 12
  }
};

//使用风险指针实现pop
std::shared_ptr<T> pop()
{
  std::atomic<void *> &hp = get_hazard_pointer_for_current_thread();
  node *old_head = head.load();
  do
  {
    node *temp;
    do // 1 直到将风险指针设为head指针
    {
      temp = old_head;
      hp.store(old_head);
      old_head = head.load();
    } while (old_head != temp);
  } while (old_head &&
           !head.compare_exchange_strong(old_head, old_head->next));
  hp.store(nullptr); // 2 当声明完成，清除风险指针
  std::shared_ptr<T> res;
  if (old_head)
  {
    res.swap(old_head->data);
    if (outstanding_hazard_pointers_for(old_head)) // 3 在删除之前对风险指针引用的节点进行检查
    {
      reclaim_later(old_head); // 4
    }
    else
    {
      delete old_head; // 5
    }
    delete_nodes_with_no_hazards(); // 6
  }
  return res;
}

//get_hazard_pointer_for_current_thread()函数的简单实现
unsigned const max_hazard_pointers = 100;
struct hazard_pointer
{
  std::atomic<std::thread::id> id;
  std::atomic<void *> pointer;
};
hazard_pointer hazard_pointers[max_hazard_pointers];

class hp_owner
{
  hazard_pointer *hp;

public:
  hp_owner(hp_owner const &) = delete;
  hp_owner operator=(hp_owner const &) = delete;
  hp_owner() : hp(nullptr)
  {
    for (unsigned i = 0; i < max_hazard_pointers; ++i)
    {
      std::thread::id old_id;
      if (hazard_pointers[i].id.compare_exchange_strong( // 6 尝试声明风险指针的所有权
              old_id, std::this_thread::get_id()))
      {
        hp = &hazard_pointers[i];
        break; // 7
      }
    }
    if (!hp) // 1
    {
      throw std::runtime_error("No hazard pointers available");
    }
  }

  std::atomic<void *> &get_pointer()
  {
    return hp->pointer;
  }

  ~hp_owner() // 2
  {
    hp->pointer.store(nullptr);      // 8
    hp->id.store(std::thread::id()); // 9
  }
};

std::atomic<void *> &get_hazard_pointer_for_current_thread() // 3
{
  thread_local static hp_owner hazard; // 4 每个线程都有自己的风险指针
  return hazard.get_pointer();         // 5
}

bool outstanding_hazard_pointers_for(void *p)
{
  for (unsigned i = 0; i < max_hazard_pointers; ++i)
  {
    if (hazard_pointers[i].pointer.load() == p)
    {
      return true;
    }
  }
  return false;
}

//回收函数的简单实现
template <typename T>
void do_delete(void *p)
{
  delete static_cast<T *>(p);
}

struct data_to_reclaim
{
  void *data;
  std::function<void(void *)> deleter;
  data_to_reclaim *next;

  template <typename T>
  data_to_reclaim(T *p) : // 1
                          data(p),
                          deleter(&do_delete<T>),
                          next(0)
  {
  }

  ~data_to_reclaim()
  {
    deleter(data); // 2
  }
};

std::atomic<data_to_reclaim *> nodes_to_reclaim;

void add_to_reclaim_list(data_to_reclaim *node) // 3
{
  node->next = nodes_to_reclaim.load();
  while (!nodes_to_reclaim.compare_exchange_weak(node->next, node))
    ;
}

template <typename T>
void reclaim_later(T *data) // 4
{
  add_to_reclaim_list(new data_to_reclaim(data)); // 5
}

void delete_nodes_with_no_hazards()
{
  data_to_reclaim *current = nodes_to_reclaim.exchange(nullptr); // 6
  while (current)
  {
    data_to_reclaim *const next = current->next;
    if (!outstanding_hazard_pointers_for(current->data)) // 7
    {
      delete current; // 8
    }
    else
    {
      add_to_reclaim_list(current); // 9
    }
    current = next;
  }
}