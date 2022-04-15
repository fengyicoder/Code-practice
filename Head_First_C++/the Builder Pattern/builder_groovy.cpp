#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

struct Tag {
    string name;
    string text;
    vector<Tag> children;
    vector<pair<string, string>> attributes;
    friend ostream& operator<<(ostream& os, const Tag& tag) {
        return os;
    }
protected:
    Tag(const string& name, const string& text)
        :name{name}, text{text} {}
    Tag(const string& name, const vector<Tag>& children)
        :name{name}, children{children} {}
};

struct p: Tag
{
    explicit p(const string& text): Tag{"p", text} {}
    p(initializer_list<Tag> children): Tag("p", children) {}
};

struct IMG: Tag
{
    explicit IMG(const string& url): Tag{"img", ""} {
        attributes.emplace_back(make_pair("src", url));
    }
};

int main()
{
    return 0;
}
