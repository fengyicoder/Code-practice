
// 使用栈的并行快速排序算法——等待数据块排序
template <typename T>
struct sorter // 1
{
  struct chunk_to_sort
  {
    std::list<T> data;
    std::promise<std::list<T>> promise;
  };

  thread_safe_stack<chunk_to_sort> chunks; // 2
  std::vector<std::thread> threads;        // 3
  unsigned const max_thread_count;
  std::atomic<bool> end_of_data;

  sorter() : max_thread_count(std::thread::hardware_concurrency() - 1),
             end_of_data(false)
  {
  }

  ~sorter() // 4
  {
    end_of_data = true; // 5

    for (unsigned i = 0; i < threads.size(); ++i)
    {
      threads[i].join(); // 6
    }
  }

  void try_sort_chunk()
  {
    boost::shared_ptr<chunk_to_sort> chunk = chunks.pop(); // 7
    if (chunk)
    {
      sort_chunk(chunk); // 8
    }
  }

  std::list<T> do_sort(std::list<T> &chunk_data) // 9
  {
    if (chunk_data.empty())
    {
      return chunk_data;
    }

    std::list<T> result;
    result.splice(result.begin(), chunk_data, chunk_data.begin()); //实现list拼接
    T const &partition_val = *result.begin();

    typename std::list<T>::iterator divide_point = // 10 std::partition会将区间[first,last)中的元素重新排列，满足判断条件pred的元素会被放在区间的前段，不满足pred的元素会被放在区间的后段
        std::partition(chunk_data.begin(), chunk_data.end(),
                       [&](T const &val)
                       { return val < partition_val; });

    chunk_to_sort new_lower_chunk;
    new_lower_chunk.data.splice(new_lower_chunk.data.end(),
                                chunk_data, chunk_data.begin(),
                                divide_point);

    std::future<std::list<T>> new_lower =
        new_lower_chunk.promise.get_future();
    chunks.push(std::move(new_lower_chunk)); // 11
    if (threads.size() < max_thread_count)   // 12
    {
      threads.push_back(std::thread(&sorter<T>::sort_thread, this));
    }

    std::list<T> new_higher(do_sort(chunk_data));

    result.splice(result.end(), new_higher);
    while (new_lower.wait_for(std::chrono::seconds(0)) !=
           std::future_status::ready) // 13
    {
      try_sort_chunk(); // 14
    }

    result.splice(result.begin(), new_lower.get());
    return result;
  }

  void sort_chunk(boost::shared_ptr<chunk_to_sort> const &chunk)
  {
    chunk->promise.set_value(do_sort(chunk->data)); // 15
  }

  void sort_thread()
  {
    while (!end_of_data) // 16
    {
      try_sort_chunk();          // 17
      std::this_thread::yield(); // 18
    }
  }
};

template <typename T>
std::list<T> parallel_quick_sort(std::list<T> input) // 19
{
  if (input.empty())
  {
    return input;
  }
  sorter<T> s;

  return s.do_sort(input); // 20
}

//使用`std::packaged_task`的并行`std::accumulate`
template <typename Iterator, typename T>
struct accumulate_block
{
  T operator()(Iterator first, Iterator last) // 1
  {
    return std::accumulate(first, last, T()); // 2
  }
};

template <typename Iterator, typename T>
T parallel_accumulate(Iterator first, Iterator last, T init)
{
  unsigned long const length = std::distance(first, last);

  if (!length)
    return init;

  unsigned long const min_per_thread = 25;
  unsigned long const max_threads =
      (length + min_per_thread - 1) / min_per_thread;

  unsigned long const hardware_threads =
      std::thread::hardware_concurrency();

  unsigned long const num_threads =
      std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);

  unsigned long const block_size = length / num_threads;

  std::vector<std::future<T>> futures(num_threads - 1); // 3
  std::vector<std::thread> threads(num_threads - 1);

  Iterator block_start = first;
  for (unsigned long i = 0; i < (num_threads - 1); ++i)
  {
    Iterator block_end = block_start;
    std::advance(block_end, block_size);
    std::packaged_task<T(Iterator, Iterator)> task( // 4
        accumulate_block<Iterator, T>());
    futures[i] = task.get_future();                                    // 5
    threads[i] = std::thread(std::move(task), block_start, block_end); // 6
    block_start = block_end;
  }
  T last_result = accumulate_block()(block_start, last); // 7

  std::for_each(threads.begin(), threads.end(),
                std::mem_fn(&std::thread::join));

  T result = init; // 8
  for (unsigned long i = 0; i < (num_threads - 1); ++i)
  {
    result += futures[i].get(); // 9
  }
  result += last_result; // 10
  return result;
}

//异常安全版`std::accumulate`
class join_threads
{
  std::vector<std::thread>& threads;
public:
  explicit join_threads(std::vector<std::thread>& threads_):
    threads(threads_)
  {}
  ~join_threads()
  {
    for(unsigned long i=0;i<threads.size();++i)
    {
      if(threads[i].joinable())
        threads[i].join();
    }
  }
};

template<typename Iterator,typename T>
T parallel_accumulate(Iterator first,Iterator last,T init)
{
  unsigned long const length=std::distance(first,last);

  if(!length)
    return init;

  unsigned long const min_per_thread=25;
  unsigned long const max_threads=
    (length+min_per_thread-1)/min_per_thread;

  unsigned long const hardware_threads=
    std::thread::hardware_concurrency();

  unsigned long const num_threads=
    std::min(hardware_threads!=0?hardware_threads:2,max_threads);

  unsigned long const block_size=length/num_threads;

  std::vector<std::future<T> > futures(num_threads-1);
  std::vector<std::thread> threads(num_threads-1);
  join_threads joiner(threads);  // 1

  Iterator block_start=first;
  for(unsigned long i=0;i<(num_threads-1);++i)
  {
    Iterator block_end=block_start;
    std::advance(block_end,block_size);
    std::packaged_task<T(Iterator,Iterator)> task(
      accumulate_block<Iterator,T>());
    futures[i]=task.get_future();
    threads[i]=std::thread(std::move(task),block_start,block_end);
    block_start=block_end;
  }
  T last_result=accumulate_block()(block_start,last);
  T result=init;
  for(unsigned long i=0;i<(num_threads-1);++i)
  {
    result+=futures[i].get();  // 2
  }
  result += last_result;
  return result;
}

// 异常安全并行版`std::accumulate`——使用`std::async()`
template<typename Iterator,typename T>
T parallel_accumulate(Iterator first,Iterator last,T init)
{
  unsigned long const length=std::distance(first,last);  // 1
  unsigned long const max_chunk_size=25;
  if(length<=max_chunk_size)
  {
    return std::accumulate(first,last,init);  // 2
  }
  else
  {
    Iterator mid_point=first;
    std::advance(mid_point,length/2);  // 3
    std::future<T> first_half_result=
      std::async(parallel_accumulate<Iterator,T>,  // 4
        first,mid_point,init);
    T second_half_result=parallel_accumulate(mid_point,last,T());  // 5
    return first_half_result.get()+second_half_result;  // 6
  }
}

//将GUI线程和任务线程进行分离
std::thread task_thread;
std::atomic<bool> task_cancelled(false);

void gui_thread()
{
  while(true)
  {
    event_data event=get_event();
    if(event.type==quit)
      break;
    process(event);
  }
}

void task()
{
  while(!task_complete() && !task_cancelled)
  {
    do_next_operation();
  }
  if(task_cancelled)
  {
    perform_cleanup();
  }
  else
  {
    post_gui_event(task_complete);
  }
}

void process(event_data const& event)
{
  switch(event.type)
  {
  case start_task:
    task_cancelled=false;
    task_thread=std::thread(task);
    break;
  case stop_task:
    task_cancelled=true;
    task_thread.join();
    break;
  case task_complete:
    task_thread.join();
    display_results();
    break;
  default:
    //...
  }
}