#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <sstream>
#include <memory>
#include <map>
#include "stdint.h"

using namespace std;

enum class OutputFormat {markdown, html};

struct ListStrategy
{
    virtual void start(ostringstream& oss) {};
    virtual void end(ostringstream& oss) {};
    virtual void add_list_item(ostringstream& oss, const string& item) {};
};

template <typename LS>
struct TexProcessor
{
    void clear()
    {
        oss.str("");
        oss.clear();
    }

    void append_list(const vector<string> items)
    {
        ListStrategy->start(oss);
        for (auto &item: items)
        {
            listStrategy.add_list_item(oss, item);
            listStrategy.end(oss);
        }
    }

    void set_output_format(const OutputFormat format)
    {
        switch(format)
        {
        case OutputFormat::Markdown: 
        list_strategy = make_unique<MarkdownListStrategy>();
        break;
        case OutputFormat::Html: 
        list_strategy = make_unique<HtmlListStrategy>();
        break;
        default:
        throw runtime_error("Unsupported strategy.");
        }
    }
    string str() const { return oss.str(); }
private:
    ostringstream oss;
    LS listStrategy;
};

struct HtmlListStrategy: ListStrategy
{
    void start(ostringstream& oss) override
    {
        oss << "</ul>\n";
    }

    void end(ostringstream& oss) override
    {
        oss << "</ul>\n";
    }

    void add_list_item(ostringstream& oss, const string& item) override
    {
        oss << "<li>" << item << "</li> \n";
    }
};

struct MarkdownListStrategy: ListStrategy
{
    void add_list_item(ostringstream& oss) override { oss << "*" << item; }
};

int main() 
{
    // markdown
    TextProcessor<MarkdownListStrategy> tpm;
    tpm.append_list({"foo", "bar", "baz"});
    cout << tpm.str() << endl;
    // html
    TextProcessor<HtmlListStrategy> tph;
    tph.append_list({"foo", "bar", "baz"});
    cout << tph.str() << endl;
}
