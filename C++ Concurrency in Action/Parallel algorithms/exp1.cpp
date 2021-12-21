
//具有内部同步并行算法的类
class X{
  mutable std::mutex m;
  int data;
public:
  X():data(0){}
  int get_value() const{
    std::lock_guard guard(m);
    return data;
  }
  void increment(){
    std::lock_guard guard(m);
    ++data;
  }
};
void increment_all(std::vector<X>& v){
  std::for_each(std::execution::par,v.begin(),v.end(),
    [](X& x){
    x.increment();
    });
}

//无内部同步并行算法的类
class Y{
  int data;
public:
  Y():data(0){}
  int get_value() const{
    return data;
  }
  void increment(){
    ++data;
  }
};
class ProtectedY{
  std::mutex m;
  std::vector<Y> v;
public:
  void lock(){
    m.lock();
  }
  void unlock(){
    m.unlock();
  }
  std::vector<Y>& get_vec(){
    return v;
  }
};
void increment_all(ProtectedY& data){
  std::lock_guard guard(data);
  auto& v=data.get_vec();
  std::for_each(std::execution::par_unseq,v.begin(),v.end(),
    [](Y& y){
      y.increment();
    });
}

//使用transform_reduce来记录网站的页面被访问的次数
#include <vector>
#include <string>
#include <unordered_map>
#include <numeric>

struct log_info {
  std::string page;
  time_t visit_time;
  std::string browser;
  // any other fields
};

extern log_info parse_log_line(std::string const &line); // 1
using visit_map_type= std::unordered_map<std::string, unsigned long long>;
visit_map_type
count_visits_per_page(std::vector<std::string> const &log_lines) {
  struct combine_visits {
    visit_map_type
    operator()(visit_map_type lhs, visit_map_type rhs) const { // 3
      if(lhs.size() < rhs.size())
        std::swap(lhs, rhs);
      for(auto const &entry : rhs) {
        lhs[entry.first]+= entry.second;
      }
      return lhs;
    }
    visit_map_type operator()(log_info log,visit_map_type map) const{ // 4
      ++map[log.page];
      return map;
    }
    visit_map_type operator()(visit_map_type map,log_info log) const{ // 5
      ++map[log.page];
      return map;
    }
    visit_map_type operator()(log_info log1,log_info log2) const{ // 6
      visit_map_type map;
      ++map[log1.page];
      ++map[log2.page];
      return map;
    }
  };
  return std::transform_reduce( // 2
    std::execution::par, log_lines.begin(), log_lines.end(),
    visit_map_type(), combine_visits(), parse_log_line);
}