
//简单的线程池
class thread_pool
{
  std::atomic_bool done;
  thread_safe_queue<std::function<void()>> work_queue; // 1
  std::vector<std::thread> threads;                    // 2
  join_threads joiner;                                 // 3

  void worker_thread()
  {
    while (!done) // 4
    {
      std::function<void()> task;
      if (work_queue.try_pop(task)) // 5
      {
        task(); // 6
      }
      else
      {
        std::this_thread::yield(); // 7
      }
    }
  }

public:
  thread_pool() : done(false), joiner(threads)
  {
    unsigned const thread_count = std::thread::hardware_concurrency(); // 8

    try
    {
      for (unsigned i = 0; i < thread_count; ++i)
      {
        threads.push_back(
            std::thread(&thread_pool::worker_thread, this)); // 9
      }
    }
    catch (...)
    {
      done = true; // 10
      throw;
    }
  }

  ~thread_pool()
  {
    done = true; // 11
  }

  template <typename FunctionType>
  void submit(FunctionType f)
  {
    work_queue.push(std::function<void()>(f)); // 12
  }
};

//可等待任务的线程池
class function_wrapper
{
  struct impl_base
  {
    virtual void call() = 0;
    virtual ~impl_base() {}
  };

  std::unique_ptr<impl_base> impl;
  template <typename F>
  struct impl_type : impl_base
  {
    F f;
    impl_type(F &&f_) : f(std::move(f_)) {}
    void call() { f(); }
  };

public:
  template <typename F>
  function_wrapper(F &&f) : impl(new impl_type<F>(std::move(f)))
  {
  }

  void operator()() { impl->call(); }

  function_wrapper() = default;

  function_wrapper(function_wrapper &&other) : impl(std::move(other.impl))
  {
  }

  function_wrapper &operator=(function_wrapper &&other)
  {
    impl = std::move(other.impl);
    return *this;
  }

  function_wrapper(const function_wrapper &) = delete;
  function_wrapper(function_wrapper &) = delete;
  function_wrapper &operator=(const function_wrapper &) = delete;
};

class thread_pool
{
  thread_safe_queue<function_wrapper> work_queue; // 使用function_wrapper，而非使用std::function

  void worker_thread()
  {
    while (!done)
    {
      function_wrapper task;
      if (work_queue.try_pop(task))
      {
        task();
      }
      else
      {
        std::this_thread::yield();
      }
    }
  }

public:
  template <typename FunctionType>
  std::future<typename std::result_of<FunctionType()>::type> // 1
  submit(FunctionType f)
  {
    typedef typename std::result_of<FunctionType()>::type
        result_type; // 2

    std::packaged_task<result_type()> task(std::move(f)); // 3
    std::future<result_type> res(task.get_future());      // 4
    work_queue.push(std::move(task));                     // 5
    return res;                                           // 6
  }
  // 和之前一样
};

//parallel_accumulate使用可等待任务的线程池
template <typename Iterator, typename T>
T parallel_accumulate(Iterator first, Iterator last, T init)
{
  unsigned long const length = std::distance(first, last);

  if (!length)
    return init;

  unsigned long const block_size = 25;
  unsigned long const num_blocks = (length + block_size - 1) / block_size; // 1

  std::vector<std::future<T>> futures(num_blocks - 1);
  thread_pool pool;

  Iterator block_start = first;
  for (unsigned long i = 0; i < (num_blocks - 1); ++i)
  {
    Iterator block_end = block_start;
    std::advance(block_end, block_size);
    futures[i] = pool.submit([=]
                             { accumulate_block<Iterator, T>()(block_start, block_end); }); // 2
    block_start = block_end;
  }
  T last_result = accumulate_block<Iterator, T>()(block_start, last);
  T result = init;
  for (unsigned long i = 0; i < (num_blocks - 1); ++i)
  {
    result += futures[i].get();
  }
  result += last_result;
  return result;
}

//run_pending_task()函数实现
void thread_pool::run_pending_task()
{
  function_wrapper task;
  if (work_queue.try_pop(task))
  {
    task();
  }
  else
  {
    std::this_thread::yield();
  }
}

//基于线程池的快速排序实现
template <typename T>
struct sorter // 1
{
  thread_pool pool; // 2

  std::list<T> do_sort(std::list<T> &chunk_data)
  {
    if (chunk_data.empty())
    {
      return chunk_data;
    }

    std::list<T> result;
    result.splice(result.begin(), chunk_data, chunk_data.begin());
    T const &partition_val = *result.begin();

    typename std::list<T>::iterator divide_point =
        std::partition(chunk_data.begin(), chunk_data.end(),
                       [&](T const &val)
                       { return val < partition_val; });

    std::list<T> new_lower_chunk;
    new_lower_chunk.splice(new_lower_chunk.end(),
                           chunk_data, chunk_data.begin(),
                           divide_point);

    std::future<std::list<T>> new_lower = // 3
        pool.submit(std::bind(&sorter::do_sort, this,
                              std::move(new_lower_chunk)));

    std::list<T> new_higher(do_sort(chunk_data));

    result.splice(result.end(), new_higher);
    while (!new_lower.wait_for(std::chrono::seconds(0)) ==
           std::future_status::timeout)
    {
      pool.run_pending_task(); // 4
    }

    result.splice(result.begin(), new_lower.get());
    return result;
  }
};

template <typename T>
std::list<T> parallel_quick_sort(std::list<T> input)
{
  if (input.empty())
  {
    return input;
  }
  sorter<T> s;

  return s.do_sort(input);
}

//线程池——线程具有本地任务队列
class thread_pool
{
  thread_safe_queue<function_wrapper> pool_work_queue;

  typedef std::queue<function_wrapper> local_queue_type; // 1
  static thread_local std::unique_ptr<local_queue_type>
      local_work_queue; // 2

  void worker_thread()
  {
    local_work_queue.reset(new local_queue_type); // 3
    while (!done)
    {
      run_pending_task();
    }
  }

public:
  template <typename FunctionType>
  std::future<typename std::result_of<FunctionType()>::type>
  submit(FunctionType f)
  {
    typedef typename std::result_of<FunctionType()>::type result_type;

    std::packaged_task<result_type()> task(f);
    std::future<result_type> res(task.get_future());
    if (local_work_queue) // 4
    {
      local_work_queue->push(std::move(task));
    }
    else
    {
      pool_work_queue.push(std::move(task)); // 5
    }
    return res;
  }

  void run_pending_task()
  {
    function_wrapper task;
    if (local_work_queue && !local_work_queue->empty()) // 6
    {
      task = std::move(local_work_queue->front());
      local_work_queue->pop();
      task();
    }
    else if (pool_work_queue.try_pop(task)) // 7
    {
      task();
    }
    else
    {
      std::this_thread::yield();
    }
  }
  // rest as before
};

//基于锁的任务窃取队列
class work_stealing_queue
{
private:
  typedef function_wrapper data_type;
  std::deque<data_type> the_queue; // 1
  mutable std::mutex the_mutex;

public:
  work_stealing_queue()
  {
  }

  work_stealing_queue(const work_stealing_queue &other) = delete;
  work_stealing_queue &operator=(
      const work_stealing_queue &other) = delete;

  void push(data_type data) // 2
  {
    std::lock_guard<std::mutex> lock(the_mutex);
    the_queue.push_front(std::move(data));
  }

  bool empty() const
  {
    std::lock_guard<std::mutex> lock(the_mutex);
    return the_queue.empty();
  }

  bool try_pop(data_type &res) // 3
  {
    std::lock_guard<std::mutex> lock(the_mutex);
    if (the_queue.empty())
    {
      return false;
    }

    res = std::move(the_queue.front());
    the_queue.pop_front();
    return true;
  }

  bool try_steal(data_type &res) // 4
  {
    std::lock_guard<std::mutex> lock(the_mutex);
    if (the_queue.empty())
    {
      return false;
    }

    res = std::move(the_queue.back());
    the_queue.pop_back();
    return true;
  }
};

//使用任务窃取的线程池
class thread_pool
{
  typedef function_wrapper task_type;

  std::atomic_bool done;
  thread_safe_queue<task_type> pool_work_queue;
  std::vector<std::unique_ptr<work_stealing_queue>> queues; // 1
  std::vector<std::thread> threads;
  join_threads joiner;

  static thread_local work_stealing_queue *local_work_queue; // 2
  static thread_local unsigned my_index;

  void worker_thread(unsigned my_index_)
  {
    my_index = my_index_;
    local_work_queue = queues[my_index].get(); // 3
    while (!done)
    {
      run_pending_task();
    }
  }

  bool pop_task_from_local_queue(task_type &task)
  {
    return local_work_queue && local_work_queue->try_pop(task);
  }

  bool pop_task_from_pool_queue(task_type &task)
  {
    return pool_work_queue.try_pop(task);
  }

  bool pop_task_from_other_thread_queue(task_type &task) // 4
  {
    for (unsigned i = 0; i < queues.size(); ++i)
    {
      unsigned const index = (my_index + i + 1) % queues.size(); // 5
      if (queues[index]->try_steal(task))
      {
        return true;
      }
    }
    return false;
  }

public:
  thread_pool() : done(false), joiner(threads)
  {
    unsigned const thread_count = std::thread::hardware_concurrency();

    try
    {
      for (unsigned i = 0; i < thread_count; ++i)
      {
        queues.push_back(std::unique_ptr<work_stealing_queue>( // 6
            new work_stealing_queue));
        threads.push_back(
            std::thread(&thread_pool::worker_thread, this, i));
      }
    }
    catch (...)
    {
      done = true;
      throw;
    }
  }

  ~thread_pool()
  {
    done = true;
  }

  template <typename FunctionType>
  std::future<typename std::result_of<FunctionType()>::type> submit(
      FunctionType f)
  {
    typedef typename std::result_of<FunctionType()>::type result_type;
    std::packaged_task<result_type()> task(f);
    std::future<result_type> res(task.get_future());
    if (local_work_queue)
    {
      local_work_queue->push(std::move(task));
    }
    else
    {
      pool_work_queue.push(std::move(task));
    }
    return res;
  }

  void run_pending_task()
  {
    task_type task;
    if (pop_task_from_local_queue(task) ||      // 7
        pop_task_from_pool_queue(task) ||       // 8
        pop_task_from_other_thread_queue(task)) // 9
    {
      task();
    }
    else
    {
      std::this_thread::yield();
    }
  }
};