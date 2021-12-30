#include <deque>
#include <mutex>
#include <future>
#include <thread>
#include <utility>

//使用std::packaged_task在GUI线程运行代码
std::mutex m;
std::deque<std::packaged_task<void()> > tasks;
bool gui_shutdown_message_received();
void get_and_process_gui_message();

void guid_thread()
{
    while(!gui_shutdown_message_received())
    {
        get_and_process_gui_message();
        std::packaged_task<void()> task;
        {
            std::lock_guard<std::mutex> lk(m);
            if (tasks.empty()) continue;
            task = std::move(tasks.front());
            tasks.pop_front();
        }
        task();
    }
    
}

std::thread gui_bg_thread(gui_thread);

template<typename Func>
std::future<void> post_task_for_gui_thread(Func f)
{
    std::packaged_task<void()> task(f);
    std::future<void> res = task.get_future();
    std::lock_guard<std::mutex> lk(m);
    tasks.push_back(std::move(task));
    return res;
}

//使用承诺处理单线程上的多个连接
#include <future>

void process_connections(connection_set& connections)
{
    while (!done(connections))
    {
        for (connection_iterator connection=connections.begin(), end=connections.end(); \
            connection!=end; ++connection)
        {
            if (connection->has_incoming_data())
            {
                data_packet data = connection->incoming();
                std::promise<payload_type> &p = connection->get_promise(data.id);
                p.set_value(data.payload);
            }
            if (connection->has_outgoing_data())
            {
                outgoing_packet data = connection->top_of_outgoing_queue();
                connection->send(data.payload);
                data.promise.set_value(true);
            }
        }
    }
}