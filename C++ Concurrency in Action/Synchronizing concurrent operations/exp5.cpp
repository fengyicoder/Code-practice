//等待一个带超时的条件变量

#include <condition_variable>
#include <mutex>
#include <chrono>

std::condition_variable cv;
bool done;
std::mutex m;

bool wait_loop()
{
    auto const timeout = std::chrono::steady_clock::now() +
                         std::chrono::milliseconds(500);
    std::unique_lock<std::mutex> lk(m);
    while (!done)
    {
        if (cv.wait_until(lk, timeout) == std::cv_status::timeout)
            break;
    }
    return done;
}

//快速排序的顺序实现
template <typename T>
std::list<T> sequential_quick_sort(std::list<T> input)
{
    if (input.empty())
    {
        return input;
    }
    std::list<T> result;
    result.splice(result.begin(), input, input.begin());
    T const &pivot = *result.begin();
    auto divide_point = std::partition(input.begin(), input.end(),
                                       [&](T const &t)
                                       { return t < pivot; });
    std::list<T> lower_part;
    lower_part.splice(lower_part.end(), input, input.begin(), divide_point);
    auto new_lower(sequential_quick_sort(std::move(lower_part)));
    auto new_higher(sequential_quick_sort(std::move(input)));
    result.splice(result.end(), new_higher);
    result.splice(result.begin(), new_lower);
    return result;
}

//使用期望的并行快排
std::list<T> parallel_quick_sort(std::list<T> input)
{
    if (input.empty())
    {
        return input;
    }
    std::list<T> result;
    result.splice(result.begin(), input, input.begin());
    T const &pivot = *result.begin();
    auto divide_point = std::partition(input.begin(), input.end(),
                                       [&](T const &t)
                                       { return t < pivot; });
    std::list<T> lower_part;
    lower_part.splice(lower_part.end(), input, input.begin(), divide_point);
    std::future<std::list<T>> new_lower(
        std::async(&parallel_quick_sort<T>, std::move(lower_part)));
    auto new_higher(parallel_quick_sort(std::move(input)));
    result.splice(result.end(), new_higher);
    result.splice(result.begin(), new_lower.get());
    return result;
}
//spawn_task的示例实现
template <typename F, typename A>
std::future<std::result_of<F(A &&)>::type> spawn_task(F &&f, A &&a)
{
    typedef std::result_of<F(A &&)>::type result_type;
    std::packaged_task<result_type(A &&)> task(std::move(f));
    std::future<result_type> res(task.get_future());
    std::thread t(std::move(task), std::move(a));
    t.detach();
    return res;
}

//ATM逻辑类的简单实现
#include <string>
struct card_inserted
{
    std::string accout;
};

class atm
{
    messaging::receiver incoming;
    messaging::sender bank;
    messaging::sender interface_hardware;
    void (atm::*state)();

    std::string accout;
    std::string pin;

    void waiting_for_card()
    {
        interface_hardware.send(display_enter_card());
        incoming.wait().handle<card_inserted>(
            [&](card_inserted const &msg)
            {
                accout = msg.account;
                pin = "";
                interface_hardware.send(display_enter_pin());
                state = &atm::getting_pin;
            });
    }
    void getting_pin();

public:
    void run()
    {
        state = &atm::waiting_for_card;
        try
        {
            for (;;)
            {
                (this->*state)();
            }
        }
        catch (messaging::close_queue const &)
        {
        }
    }
};

void atm::getting_pin()
{
    incoming.wait().handle<digit_pressed const & msg>
    {
        unsigned const pin_length = 4;
        pin += msg.digit;
        if (pin.length() == pin_length)
        {
            bank.send(verify_pin(account, pin, incoming));
            state = &atm::verifying_pin;
        }
    }
    .handle<clear_last_pressed>(
        [&](clear_last_pressed const &msg)
        {
            if (!pin.empty())
            {
                pin.resize(pin.length() - 1);
            }
        })
        .handle<cancel_pressed>(
            [&](cancel_pressed const &msg)
            {
                state = &atm::done_processing;
            });
}

//延续
std::experimental::future<int> find_the_answer;
std::string find_the_question(std::experimental::future<int> the_answer);
auto fut = find_the_answer();
auto fut2 = fut.then(find_the_question);
assert(!fut.valid());
assert(fut2.valid());

//为并发技术规范中的期望而实现一个简单的std::async等价物
template <typename Func>
std::experimental::future<decltype(std::declval<Func>()())>
spawn_async(Func &&func)
{
    std::experimental::promise<decltype(std::declval<Func>()())> p;
    auto res = p.get_future();
    std::thread t(
        [p = std::move(p), f = std::decay_t<Func>(func)]() mutable
        {
            try
            {
                p.set_value_at_thread_exit(f());
            }
            catch (...)
            {
                p.set_exception_at_thread_exit(std::current_exception());
            }
        });
    t.detach();
    return res;
}

//处理用户登录的简单顺序函数
void process_login(std::string const &username, std::string const &password)
{
    try
    {
        user_id const id = backend.authenticate_user(username, password);
        user_data const info_to_display = backend.request_current_info(id);
        update_display(info_to_display);
    }
    catch (const std::exception &e)
    {
        display_error(e);
    }
}

//使用单个异步线程处理用户登录
void process_login(std::string const &username, std::string const &password)
{
    return std::async(std::launch::async, [=]()
                      {
                          try
                          {
                              user_id const id = backend.authenticate_user(username, password);
                              user_data const info_to_display = backend.request_current_info(id);
                              update_display(info_to_display);
                          }
                          catch (const std::exception &e)
                          {
                              display_error(e);
                          }
                      });
}

//使用延续处理用户登录
std::experimental::future<void>process_login(std::string const&username,std::string const&password)
{
    return spawn_async([=]()
                                            {returnbackend.authenticate_user(username,password); })
            .then([](std::experimental::future<user_id>id) {
                    returnbackend.request_current_info(id.get());
            }).then([](std::experimental::future<user_data>info_to_display) {
                    try
                    {
                            update_display(info_to_display.get());
                    }
                    catch(conststd::exception &e)
                    {
                            display_error(e);
                    }
            });
}

//全异步处理用户登录
std::experimental::future<void>process_login(std::string const&username,std::string const&password)
{
    return backend.async_authenticate_user(username,password).then(
        [](std::experimental::future<user_id>id) {
                    return backend.async_request_current_info(id.get());
            }).then([](std::experimental::future<user_data>info_to_display) {
                    try
                    {
                            update_display(info_to_display.get());
                    }
                    catch(conststd::exception &e)
                    {
                            display_error(e);
                    }
            });
}

//shared_future的延续
auto fut = spawn_async(some_function).share();
auto fut2 = fut.then([](std::experimental::shared_future<some_data> data){
    do_stuff(data);
}
);
auto fut3 = fut.then([](std::experimental::shared_future<some_data> data){
    do_other_stuff(data);
}
);

//使用std::async从多个期望中收集结果
std::future<FinalResult> process_data(std::vector<MyData>& vec)
{
    size_t const chunk_size = whatever;
    std::vector<std::future<ChunkResult>> results;
    for (auto begin=vec.begin(), end=vec.end(); begin!=end;) {
        size_t const remaining_size = end - begin;
        size_t const this_chunk_size = std::min(remaining_size, chunk_size);
        results.push_back(std::async(process_chunk, begin, begin+this_chunk_size), begin += this_chunk_size);
    }
    return std::async([all_results=std::move(results)](){
        std::vector<ChunkResult> v;
        v.reserve(all_results.size());
        for (auto& f: all_results)
        {
            v.push_back(f.get());
        }
        return gather_results(v);
    })
}

//使用std::experimental::when_all从多个期望值中收集结果
std::experimental::future<FinalResult> process_data(std::vector<MyData>& vec)
{
    size_t const chunk_size = whatever;
    std::vector<std::experimental::future<ChunkResult>> results;
    for (auto begin=vec.begin(), end=vec.end(); begin!=end;) {
        size_t const remaining_size = end - begin;
        size_t const this_chunk_size = std::min(remaining_size, chunk_size);
        results.push_back(std::async(process_chunk, begin, begin+this_chunk_size), begin += this_chunk_size);
    }
    return std::experimental::when_all(
        results.begin(),results.end()).then(
            [](std::future<std::vector<std::experimental::future<ChunkResult>>> ready_results){
            std::vector<std::experimental::future<ChunkResult>> all_results = ready_results.get();
            std::vector<ChunkResult> v;
            v.reserve(all_results.size());
            for (auto& f: all_results)
            {
                v.push_back(f.get());
            }
            return gather_results(v);
        )};
}

//使用std::experimental::when_any处理第一个被找到的值
std::experimental::future<FinalResult> 
find_and_process_value(std::vector<MyData>& vec)
{
    unsigned const concurrency = std::thread::hardware_concurrency();
    unsigned const num_tasks = (concurrency > 0) ? concurrency : 2;

    std::vector<std::experimental::future<MyData *>> results;
    auto const chunk_size = (data.size() + num_tasks - 1) / num_tasks;
    auto chunk_begin = data.begin();
    std::shared_ptr<std::atomic<bool>> done_flag = 
        std::make_shared<std::atomic<bool>>(false);
    for (unsigned i = 0; i < num_tasks; ++i) {
        auto chunk_end = (i < (num_tasks - 1)? chunk_begin + chunk_size : data.end());
        results.push_back(spawn_async([=] {
            for (auto entry = chunk_begin;
                !*done_flag && (entry != chunk_end);
                ++entry) {
                    if (matches_find_criteria(*entry)){
                        *done_flag = true;
                        return &*entry;
                    }
                }
            return (MyData *)nullptr;
        }));
        chunk_begin = chunk_end;
    }
    std::shared_ptr<std::experimental::promise<FinalResult>> final_result = 
        std::make_shared<std::experimental::promise<FinalResult>>();
    struct DoneCheck {
        std::shared_ptr<std::experimental::promise<FinalResult>> final_result;
        DoneCheck(
            std::shared_ptr<std::experimental::promise<FinalResult>>
            final_result_)
            : final_result(std::move(final_result_)) {}
        void operator() (
            std::experimental::future<std::experimental::when_any_result<
                std::vector<std::experimental::future<MyData *>>>>
            results_param) {
                auto results = results_param.get();
                MyData *const ready_result = 
                    results.futures[results.index].get();
            if (ready_result)
                final_result->set_value(
                    process_found_valud(*ready_result));
            else {
                results.futures.erase(
                    results.futures.begin() + results.index);
                if (!results.futures.empty()) {
                    std::experimental::when_any(
                        results.futures.begin(), results.futures.end())
                        .then(std::move(*this));
                } else {
                    final_result->set_exception(
                        std::make_exception_ptr(
                            std::runtime_error("Not found")));
                }
            }
        };
    std::experimental::when_any(results.begin(), results.end())
    .then(DoneCheck(final_result));
    return final_result->get_future();
}
}

//使用std::experimental::latch等待事件

void foo() {
    unsigned const thread_count = ...;
    latch done(thread_count);
    my_data data[thread_count];
    std::vector<std::future<void> > threads;

    for (unsigned i=0; i<thread_count; ++i)
        threads.push_back(std::async(std::launch::async, [&,i]{
            data[i] = make_data(i);
            done.count_down();
            do_more_stuff();
        }));
    done.wait();
    process_data(data, thread_count);
}

//使用std::experimental::barrier

result_chunk process(data_chunk);
std::vector<data_chunk>
divide_into_chunks(data_block data, unsigned num_threads);

void process_data(data_source &source, data_sink &sink) {
    unsigned const concurrency = std::thread::hardware_concurrency();
    unsigned const num_threads = (concurrency > 0) ? concurrency : 2;
    std::experimental::barrier sync(num_threads);
    std::vector<joining_thread> threads(num_threads);
    std::vector<data_chunk> chunks;
    result_block result;
    for (unsigned i=0; i<num_threads; ++i) {
        threads[i] = joining_thread([&, i] {
            while (!source.done()) {
                if(!i) {
                    data_block current_block = 
                        source.get_next_data_block();
                    chunks = divide_into_chunks(
                            current_block, num_threads
                        );
                }
                sync.arrive_and_wait();
                result.set_chunk(i, num_threads, process(chunks[i]));
                sync.arrive_and_wait();
                if (!i) {
                    sink.write_data(std::move(result));
                }
            }
        });
    }
}

//使用std::experimental::flex_barrier

result_chunk process(data_chunk);
std::vector<data_chunk>
divide_into_chunks(data_block data, unsigned num_threads);

void process_data(data_source &source, data_sink &sink) {
    unsigned const concurrency = std::thread::hardware_concurrency();
    unsigned const num_threads = (concurrency > 0) ? concurrency : 2;
    std::experimental::barrier sync(num_threads);
    std::vector<joining_thread> threads(num_threads);
    std::vector<data_chunk> chunks;
    auto split_source = [&] {
        if(!source.done()) {
            data_block current_block = 
                        source.get_next_data_block();
                        chunks = divide_into_chunks(
                            current_block, num_threads
                        );
        }
    };
    split_source();
    result_block result;
    std::experimental::flex_barrier sync(num_threads, [&] {
        sink.write_data(std::move(result));
        split_source();
        return -1;
    });
    std::vector<joining_thread> threads(num_threads);
    for (unsigned i=0; i<num_threads; ++i) {
        threads[i] = joining_thread([&, i] {
            while (!source.done()) {
                result.set_chunk(i, num_threads, process(chunks[i]));
                sync.arrive_and_wait();
             
        });
    }
}