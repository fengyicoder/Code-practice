#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

struct HtmlBuilder;

struct HtmlElement
{
    //处于自己理解添加友元，不然HtmlBuilder无法构造HtmlBuilder对象
    friend struct HtmlBuilder;
    string name;
    string text;
    vector<HtmlElement> elements;
    const size_t indent_size = 2;
    static unique_ptr<HtmlBuilder> build(const string &root_name)
    {
        return make_unique<HtmlBuilder>(root_name);
    }

protected: // hide all constructors
    HtmlElement() {}
    HtmlElement(const string &name, const string &text)
        : name{name}, text{text} {}
};

struct HtmlBuilder
{
    HtmlElement root;
    HtmlBuilder(string root_name) { root.name = root_name; }
    HtmlBuilder &add_child(string child_name, string child_text)
    {
        HtmlElement e{child_name, child_text};
        root.elements.emplace_back(e);
        return *this;
    }
    //自动类型转换
    operator HtmlElement() const { return root; }
};

int main()
{
    auto builder = HtmlElement::build("ul");
    (*builder).add_child("li", "hello").add_child("li", "world");
    return 0;
}