#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <sstream>
#include "stdint.h"

#include <boost/bimap.hpp>
#include <boost/flyweight.hpp>
#include <boost/flyweight/key_value.hpp>
using namespace boost;
using namespace flyweights;

using namespace std;

class FormattedText{
    string plainText;
    bool *cap;
public:
    explicit FormattedText(const string& plainText): plainText{plainText} {
        cap = new bool [plainText.length()];
    }

    ~FormattedText() {
        delete [] cap;
    }

    void capitalize(int start, int end) {
        for (int i=start; i<=end; ++i) cap[i] = true;
    }

    friend ostream& operator<<(ostream& os, const Formatted& obj) {
        string s;
        for(int i=0; i<obj.plainText.length(); ++i) {
            char c = obj.plainText[i];
            s += (obj.cap[i] ? toupper(c) : c);
        }
        return os << s;
    }
};

class BetterFormattedText
{
public:
    struct TextRange
    {
        int start, end;
        bool capitalize;
        bool covers(int position) const {
            return position >= start && position <= end;
        }
    };

    TextRange& get_range(int start, int end) {
        formatting.emplace_back(TextRange{start, end});
        return *formatting.rbegin();
    }

    friend ostream& operator<<(ostream& os, const BetterFormattedText& obj) {
        string s;
        for (size_t i=0; i<obj.plain_text.length(); i++) {
            auto c = obj.plain_text[i];
            for (const auto& rng: obj.formatting) {
                if (rng.covers(i) && rng.capitalize) c = toupper(c);
            }
            s += c;
        }
        return os << s;
    }
private:
    string plain_text;
    vector<TextRange> formatting;
};

int main() {
    FormattedText ft("This is a brave new world");
    ft.capitalize(10, 15);
    cout << ft << endl;
}