#include <mutex>
#include <algorithm>
#include <thread>
#include <iostream>
#include <map>
#include <string>
#include <shared_mutex>

//once_flag与call_once使用示例
std::shared_ptr<some_resource> resource_ptr;
std::once_flag resource_flag;

void init_resource()
{
    resource_ptr.reset(new some_resource);
}

void foo()
{
    std::call_once(resource_flag, init_resource);
    resource_ptr->do_something();
}

//使用std::shared_mutex保护数据结构
class dns_entry;
class dns_cache
{
    std::map<std::string, dns_entry> entries;
    mutable std::shared_mutex entry_mutex;
public:
    dns_entry find_entry(std::string const& domain) const
    {
        std::shared_lock<std::shared_mutex> lk(entry_mutex);
        std::map<std::string, dns_entry>::const_iterator const it = entries.find(domain);
        return (it==entries.end()) ? dns_entry() : it->second;
    }
    void update_or_add_entry(std::string const& domain, dns_entry const& dns_details)
    {
        std::lock_guard<std::shared_mutex> lk(entry_mutex);
        entries[domain] = dns_details;
    }
}

int main()
{
    return 0;
}