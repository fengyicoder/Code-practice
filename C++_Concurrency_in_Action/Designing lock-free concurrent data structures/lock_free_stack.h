#include <iostream>
#include <memory>
#include <atomic>



namespace version_1
{
//采用引用计数的内存释放机制，但可能出现候删节点一直增加不能删除
template<typename T>
class lock_free_stack
{
private:
    struct node
    {
        std::shared_ptr<T> data;
        node* next;
        node(T const& data_): data(std::make_shared<T>(data_)) {}
    };
    std::atomic<node*> head;
    std::atomic<unsigned> threads_in_pop;
    std::atomic<node*> to_be_deleted;

    static void delete_nodes(node* nodes)
    {
        while(nodes)
        {
            node* next = nodes->next;
            delete nodes;
            nodes = next;
        }
    }

    void try_reclaim(node* old_head)
    {
        if (threads_in_pop == 1)
        {
            node* nodes_to_delete = to_be_deleted;
            if (!--threads_in_pop)
            {
                delete_nodes(nodes_to_delete);
            }
            else if(nodes_to_delete)
            {
                chain_pending_nodes(nodes_to_delete);
            }
            delete old_head;
        }
        else
        {
            chain_pending_node(old_head);
            --threads_in_loop;
        }
    }

    void chain_pending_nodes(node* nodes)
    {
        node* last = nodes;
        while (node* const next = last->next)
        {
            last = next;
        }
        chain_pending_nodes(nodes, last);
    }

    void chain_pending_nodes(node* first, node* last)
    {
        last->next = to_be_deleted;
        while(!to_be_deleted.compare_exchange_weak(last->next, first));
    }

    void chain_pending_node(node* n)
    {
        chain_pending_nodes(n, n);
    }

public:
    void push(T const& data)
    {
        node* const new_node = new node(data);
        new_node->next = head.load();
        while(!head.compare_exchange_weak(new_node->next, new_node));
    }

    std::shared_ptr<T> pop()
    {
        ++threads_in_pop;
        node* old_head = head.load();
        while(old_head && !head.compare_exchange_weak(old_head, old_head->next));
        std::shared_ptr<T> res;
        if (old_head)
        {
            res.swap(old_head->data);
        }
        try_reclaim(old_head);
        return res;
    }
};
}

namespace version_2
{
//采用风险指针检测无法回收的节点
template<typename T>
class lock_free_stack
{
private:
    struct node
    {
        std::shared_ptr<T> data;
        node* next;
        node(T const& data_): data(std::make_shared<T>(data_)) {}
    };
    struct hazard_pointer
    {
        std::atomic<std::thread::id> id;
        std::atomic<void*> pointer;
    };
    struct data_to_reclaim
    {
        void* data;
        std::function<void(void*)> deleter;
        data_to_reclaim* next;
        template<typename T>
        data_to_reclaim(T* p): data(p), deleter(&do_delete<T>), next(0);
        {}
        ~data_to_reclaim()
        {
            deleter(data);
        }
    };
    std::atomic<node*> head;
    std::atomic<data_to_reclaim*> nodes_to_reclaim;
    unsigned const max_hazard_pointers = 100;
    hazard_pointer hazard_pointers[max_hazard_pointers];
    
    class hp_owner
    {
        hazard_pointer* hp;
    public:
        hp_owner(hp_owner const&) = delete;
        hp_owner operator=(hp_owner const&) = delete;
        hp_owner(): hp(nullptr)
        {
            for (unsigned i=0; i<max_hazard_pointers; i++)
            {
                std::thread::id old_id;
                if (hazard_pointers[i].id.compare_change_strong(
                    old_id, std::this_thread::get_id()
                ))
                {
                    hp = &hazard_pointers[i];
                    break;
                }
            }
            if (!hp)
            {
                throw std::runtime_error("No hazard pointers available");
            }

        }
        std::atomic<void*>& get_pointer()
        {
            return hp->pointer;
        }
        ~hp_owner()
        {
            hp->pointer.store(nullptr);
            hp->id.store(std::thread::id());
        }
    }

    std::atomic<void*>& get_hazard_pointer_for_current_thread()
    {
        thread_local static hp_owner hazard;
        return hazard.get_pointer();
    }

    bool outstanding_hazard_pointers_for(void* p)
    {
        for (unsigned i=0; i < max_hazard_pointers; ++i)
        {
            if (hazard_pointers[i].pointer.load() == p)
            return true;
        }
        return false;
    }
    template<typename T>
    void do_delete(void* p)
    {
        delete static_cast<T*>(p);
    }

    void add_to_reclaim_list(data_to_reclaim* node)
    {
        node->next = nodes_to_reclaim.load();
        while(!nodes_to_reclaim.compare_exchange_weak(node->next, node));
    }

    template<typename T>
    void reclaim_later(T* data)
    {
        add_to_reclaim_list(new data_to_reclaim(data));
    }

    void delete_nodes_with_no_hazards()
    {
        data_to_reclaim* current = nodes_to_relaim.exchange(nullptr);
        while (current)
        {
            data_to_reclaim* const next = current->next;
            if (!outstanding_hazard_pointers_for(current->data))
            {
                delete current;
            }
            else {
                add_to_reclaim_list(current);
            }
            current = next;
        }
    }

public:
    void push(T const& data)
    {
        node* const new_node = new node(data);
        new_node->next = head.load();
        while(!head.compare_exchange_weak(new_node->next, new_node));
    }

    std::shared_ptr<T> pop()
    {
        std::atomic<void*>& hp = get_hazard_pointer_for_current_thread();
        node* old_head = head.load();
        node* temp;
        do 
        {
            do
            {
                temp = old_head;
                hp.store(old_head);
                old_head = head.load();
            } while (old_head != temp);
        }
        while (old_head && !head.compare_exchange_strong(old_head, old_head->next));
        hp.store(nullptr);
        std::shared_ptr<T> res;
        if (old_head)
        {
            res.swap(old_head->data);
            if (outstanding_hazard_pointers_for(old_head))
            {
                reclaim_later(old_head);
            }
            else
            {
                delete old_head;
            }
            delete_nodes_with_no_hazards();
        }
    }
};
}

namespace version_3
{
//采用引用计数检测正在使用的节点，在C++20中引入偏特化std::atomic<std::shared_ptr<T>>取代std::experimental::atomic_shared_ptr<T>
template<typename T>
class lock_free_stack
{
private:
    struct node
    {
        std::shared_ptr<T> data;
        std::experimental::atomic_shared_ptr<node> next;
        node(T const& data_): data(std::make_shared<T>(data_)) {}
    };
    std::experimental::atomic_shared_ptr<node> head;

public:
    void push(T const& data)
    {
        std::shared_ptr<node> const new_node = std::make_shared<node>(data);
        new_node->next = head.load();
        while(!head.compare_exchange_weak(new_node->next, new_node));
    }

    std::shared_ptr<T> pop()
    {
        std::shared_ptr<node> old_head = head.load();
        while(old_head && !head.compare_exchange_weak(old_head, old_head->next.load()));
        if (old_head)
        {
            old_head->next = std::shared_ptr<node>();
            return old_head->data;
        }
        return std::shared_ptr<T>();
    }
    ~lock_free_stack()
    {
        while(pop());
    }
};
}

namespace version_4
{
//采用分离计数的方式管理要删除的节点
template<typename T>
class lock_free_stack
{
private:
    struct node;
    struct counted_node_ptr
    {
        int external_count;
        node* ptr;
    }
    struct node
    {
        std::shared_ptr<T> data;
        std::atomic<int> internal_count;
        counted_node_ptr next;
        node(T const& data_): data(std::make_shared<T>(data_)), internal_count(0) {}
    };
    std::atomic<counted_node_ptr> head;

    void increase_head_count(counted_node_ptr& old_counter)
    {
        counted_node_ptr new_counter;
        do
        {
            new_counter = old_counter;
            ++new_counter.external_count;
        } while (!head.compare_exchange_strong(old_counter, new_counter));
        old_counter.external_count = new_counter.external_count;
        
    }

public:
    void push(T const& data)
    {
        counted_node_ptr new_node;
        new_node.ptr = new node(data);
        new_node.external_count = 1;
        new_node.ptr->next = head.load();
        while(!head.compare_exchange_weak(new_node.ptr->next, new_node));
    }

    std::shared_ptr<T> pop()
    {
        counted_node_ptr old_head = head.load();
        for (;;)
        {
            increase_head_count(old_head);
            node* const ptr = old_head.ptr;
            if (!ptr)
            {
                return std::shared_ptr<T>();
            }
            if (head.compare_exchange_strong(old_head, ptr->next))
            {
                std::shared_ptr<T> res;
                res.swap(ptr->data);
                int const count_increase = old_head.external_count - 2;
                if (ptr->internal_count.fetch_add(count_increase) == -count_increase)
                {
                    delete ptr;
                }
                return res;
            }
            else if(ptr->internal_count.fetch_sub(1) == 1)
            {
                delete ptr;
            }
        }
    }
    ~lock_free_stack()
    {
        while(pop());
    }
};
}

namespace version_5
{
//采用分离计数的方式管理要删除的节点并施加内存顺序
template<typename T>
class lock_free_stack
{
private:
    struct node;
    struct counted_node_ptr
    {
        int external_count;
        node* ptr;
    }
    struct node
    {
        std::shared_ptr<T> data;
        std::atomic<int> internal_count;
        counted_node_ptr next;
        node(T const& data_): data(std::make_shared<T>(data_)), internal_count(0) {}
    };
    std::atomic<counted_node_ptr> head;

    void increase_head_count(counted_node_ptr& old_counter)
    {
        counted_node_ptr new_counter;
        do
        {
            new_counter = old_counter;
            ++new_counter.external_count;
        } while (!head.compare_exchange_strong(old_counter, new_counter,
            std::memory_order_acquire, std::memory_order_relaxed));
        old_counter.external_count = new_counter.external_count;
        
    }

public:
    void push(T const& data)
    {
        counted_node_ptr new_node;
        new_node.ptr = new node(data);
        new_node.external_count = 1;
        new_node.ptr->next = head.load();
        while(!head.compare_exchange_weak(new_node.ptr->next, new_node,
            std::memory_order_release, std::memory_order_relaxed));
    }

    std::shared_ptr<T> pop()
    {
        counted_node_ptr old_head = head.load();
        for (;;)
        {
            increase_head_count(old_head);
            node* const ptr = old_head.ptr;
            if (!ptr)
            {
                return std::shared_ptr<T>();
            }
            if (head.compare_exchange_strong(old_head, ptr->next, std::memory_order_relaxed))
            {
                std::shared_ptr<T> res;
                res.swap(ptr->data);
                int const count_increase = old_head.external_count - 2;
                if (ptr->internal_count.fetch_add(count_increase, std::memory_order_release) == -count_increase)
                {
                    delete ptr;
                }
                return res;
            }
            else if(ptr->internal_count.fetch_sub(1, std::memory_order_relaxed) == 1)
            {
                ptr->internal_count.load(std::memory_order_acquire);
                delete ptr;
            }
        }
    }
    ~lock_free_stack()
    {
        while(pop());
    }
};
}