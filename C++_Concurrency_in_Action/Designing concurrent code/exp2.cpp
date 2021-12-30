//并行版`std::for_each`
template <typename Iterator, typename Func>
void parallel_for_each(Iterator first, Iterator last, Func f)
{
  unsigned long const length = std::distance(first, last);

  if (!length)
    return;

  unsigned long const min_per_thread = 25;
  unsigned long const max_threads =
      (length + min_per_thread - 1) / min_per_thread;

  unsigned long const hardware_threads =
      std::thread::hardware_concurrency();

  unsigned long const num_threads =
      std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);

  unsigned long const block_size = length / num_threads;

  std::vector<std::future<void>> futures(num_threads - 1); // 1
  std::vector<std::thread> threads(num_threads - 1);
  join_threads joiner(threads); //见exp1.cpp

  Iterator block_start = first;
  for (unsigned long i = 0; i < (num_threads - 1); ++i)
  {
    Iterator block_end = block_start;
    std::advance(block_end, block_size);
    std::packaged_task<void(void)> task( // 2
        [=]()
        {
          std::for_each(block_start, block_end, f);
        });
    futures[i] = task.get_future();
    threads[i] = std::thread(std::move(task)); // 3
    block_start = block_end;
  }
  std::for_each(block_start, last, f);
  for (unsigned long i = 0; i < (num_threads - 1); ++i)
  {
    futures[i].get(); // 4
  }
}

//使用`std::async`实现`std::for_each`
template <typename Iterator, typename Func>
void parallel_for_each(Iterator first, Iterator last, Func f)
{
  unsigned long const length = std::distance(first, last);

  if (!length)
    return;

  unsigned long const min_per_thread = 25;

  if (length < (2 * min_per_thread))
  {
    std::for_each(first, last, f); // 1
  }
  else
  {
    Iterator const mid_point = first + length / 2;
    std::future<void> first_half = // 2
        std::async(&parallel_for_each<Iterator, Func>,
                   first, mid_point, f);
    parallel_for_each(mid_point, last, f); // 3
    first_half.get();                      // 4
  }
}

//并行find算法实现
template <typename Iterator, typename MatchType>
Iterator parallel_find(Iterator first, Iterator last, MatchType match)
{
  struct find_element // 1
  {
    void operator()(Iterator begin, Iterator end,
                    MatchType match,
                    std::promise<Iterator> *result,
                    std::atomic<bool> *done_flag)
    {
      try
      {
        for (; (begin != end) && !done_flag->load(); ++begin) // 2
        {
          if (*begin == match)
          {
            result->set_value(begin); // 3
            done_flag->store(true);   // 4
            return;
          }
        }
      }
      catch (...) // 5
      {
        try
        {
          result->set_exception(std::current_exception()); // 6
          done_flag->store(true);
        }
        catch (...) // 7
        {
        }
      }
    }
  };

  unsigned long const length = std::distance(first, last);

  if (!length)
    return last;

  unsigned long const min_per_thread = 25;
  unsigned long const max_threads =
      (length + min_per_thread - 1) / min_per_thread;

  unsigned long const hardware_threads =
      std::thread::hardware_concurrency();

  unsigned long const num_threads =
      std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);

  unsigned long const block_size = length / num_threads;

  std::promise<Iterator> result;      // 8
  std::atomic<bool> done_flag(false); // 9
  std::vector<std::thread> threads(num_threads - 1);
  { // 10
    join_threads joiner(threads);

    Iterator block_start = first;
    for (unsigned long i = 0; i < (num_threads - 1); ++i)
    {
      Iterator block_end = block_start;
      std::advance(block_end, block_size);
      threads[i] = std::thread(find_element(), // 11
                               block_start, block_end, match,
                               &result, &done_flag);
      block_start = block_end;
    }
    find_element()(block_start, last, match, &result, &done_flag); // 12
  }
  if (!done_flag.load()) //13
  {
    return last;
  }
  return result.get_future().get(); // 14
}

//使用`std::async`实现的并行find算法
template<typename Iterator,typename MatchType>  // 1
Iterator parallel_find_impl(Iterator first,Iterator last,MatchType match,
                            std::atomic<bool>& done)
{
  try
  {
    unsigned long const length=std::distance(first,last);
    unsigned long const min_per_thread=25;  // 2
    if(length<(2*min_per_thread))  // 3
    {
      for(;(first!=last) && !done.load();++first)  // 4
      {
        if(*first==match)
        {
          done=true;  // 5
          return first;
        }
      }
      return last;  // 6
    }
    else
    { 
      Iterator const mid_point=first+(length/2);  // 7
      std::future<Iterator> async_result=
        std::async(&parallel_find_impl<Iterator,MatchType>,  // 8
                   mid_point,last,match,std::ref(done));
      Iterator const direct_result=
        parallel_find_impl(first,mid_point,match,done);  // 9
      return (direct_result==mid_point)?
        async_result.get():direct_result;  // 10
    }
  }
  catch(...)
  {
    done=true;  // 11
    throw;
  }
}

template<typename Iterator,typename MatchType>
Iterator parallel_find(Iterator first,Iterator last,MatchType match)
{
  std::atomic<bool> done(false);
  return parallel_find_impl(first,last,match,done);  // 12
}

//使用划分的方式进行并行求和
template <typename Iterator>
void parallel_partial_sum(Iterator first, Iterator last)
{
  typedef typename Iterator::value_type value_type;

  struct process_chunk // 1
  {
    void operator()(Iterator begin, Iterator last,
                    std::future<value_type> *previous_end_value,
                    std::promise<value_type> *end_value)
    {
      try
      {
        Iterator end = last;
        ++end;
        std::partial_sum(begin, end, begin); // 2
        if (previous_end_value)              // 3
        {
          value_type &addend = previous_end_value->get(); // 4
          *last += addend;                                // 5
          if (end_value)
          {
            end_value->set_value(*last); // 6
          }
          std::for_each(begin, last, [addend](value_type &item) // 7
                        { item += addend; });
        }
        else if (end_value)
        {
          end_value->set_value(*last); // 8
        }
      }
      catch (...) // 9
      {
        if (end_value)
        {
          end_value->set_exception(std::current_exception()); // 10
        }
        else
        {
          throw; // 11
        }
      }
    }
  };

  unsigned long const length = std::distance(first, last);

  if (!length)
    return last;

  unsigned long const min_per_thread = 25; // 12
  unsigned long const max_threads =
      (length + min_per_thread - 1) / min_per_thread;

  unsigned long const hardware_threads =
      std::thread::hardware_concurrency();

  unsigned long const num_threads =
      std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);

  unsigned long const block_size = length / num_threads;

  typedef typename Iterator::value_type value_type;

  std::vector<std::thread> threads(num_threads - 1); // 13
  std::vector<std::promise<value_type>>
      end_values(num_threads - 1); // 14
  std::vector<std::future<value_type>>
      previous_end_values;                      // 15
  previous_end_values.reserve(num_threads - 1); // 16
  join_threads joiner(threads);

  Iterator block_start = first;
  for (unsigned long i = 0; i < (num_threads - 1); ++i)
  {
    Iterator block_last = block_start;
    std::advance(block_last, block_size - 1); // 17
    threads[i] = std::thread(process_chunk(), // 18
                             block_start, block_last,
                             (i != 0) ? &previous_end_values[i - 1] : 0,
                             &end_values[i]);
    block_start = block_last;
    ++block_start;                                             // 19
    previous_end_values.push_back(end_values[i].get_future()); // 20
  }
  Iterator final_element = block_start;
  std::advance(final_element, std::distance(block_start, last) - 1); // 21
  process_chunk()(block_start, final_element,                        // 22
                  (num_threads > 1) ? &previous_end_values.back() : 0,
                  0);
}

//简单的屏障类
class barrier
{
  unsigned const count;
  std::atomic<unsigned> spaces;
  std::atomic<unsigned> generation;

public:
  explicit barrier(unsigned count_) : // 1
                                      count(count_), spaces(count), generation(0)
  {
  }

  void wait()
  {
    unsigned const my_generation = generation; // 2
    if (!--spaces)                             // 3
    {
      spaces = count; // 4
      ++generation;   // 5
    }
    else
    {
      while (generation == my_generation) // 6
        std::this_thread::yield();        // 7
    }
  }
};

//通过两两更新的方式实现partial_sum
struct barrier
{
  std::atomic<unsigned> count;
  std::atomic<unsigned> spaces;
  std::atomic<unsigned> generation;

  barrier(unsigned count_) : count(count_), spaces(count_), generation(0)
  {
  }

  void wait()
  {
    unsigned const gen = generation.load();
    if (!--spaces)
    {
      spaces = count.load();
      ++generation;
    }
    else
    {
      while (generation.load() == gen)
      {
        std::this_thread::yield();
      }
    }
  }

  void done_waiting()
  {
    --count;
    if (!--spaces)
    {
      spaces = count.load();
      ++generation;
    }
  }
};

template <typename Iterator>
void parallel_partial_sum(Iterator first, Iterator last)
{
  typedef typename Iterator::value_type value_type;

  struct process_element // 1
  {
    void operator()(Iterator first, Iterator last,
                    std::vector<value_type> &buffer,
                    unsigned i, barrier &b)
    {
      value_type &ith_element = *(first + i);
      bool update_source = false;

      for (unsigned step = 0, stride = 1; stride <= i; ++step, stride *= 2)
      {
        value_type const &source = (step % 2) ? // 2
                                       buffer[i]
                                              : ith_element;

        value_type &dest = (step % 2) ? ith_element : buffer[i];

        value_type const &addend = (step % 2) ? // 3
                                       buffer[i - stride]
                                              : *(first + i - stride);

        dest = source + addend; // 4
        update_source = !(step % 2);
        b.wait(); // 5
      }
      if (update_source) // 6
      {
        ith_element = buffer[i];
      }
      b.done_waiting(); // 7
    }
  };

  unsigned long const length = std::distance(first, last);

  if (length <= 1)
    return;

  std::vector<value_type> buffer(length);
  barrier b(length);

  std::vector<std::thread> threads(length - 1); // 8
  join_threads joiner(threads);

  Iterator block_start = first;
  for (unsigned long i = 0; i < (length - 1); ++i)
  {
    threads[i] = std::thread(process_element(), first, last, // 9
                             std::ref(buffer), i, std::ref(b));
  }
  process_element()(first, last, buffer, length - 1, b); // 10
}