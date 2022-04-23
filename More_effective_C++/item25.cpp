#include <iostream>
#include <string>
#include <vector>

class NLComponent {
public:
    virtual void func1() = 0;
    virtual NLComponent* clone() const = 0;
    virtual ostream& print(ostream& s) const = 0;
};

class TextBlock: public NLComponent {
public: 
    void func1() {
        std::cout << "This is a TextBlock" << std::endl;
    }

    virtual TextBlock* clone() const {
        return new TextBlock(*this);
    }

    virtual ostream& print(ostream& s) const { return s;}
};

class Graphic: public NLComponent {
public: 
    void func1() {
        std::cout << "This is a Graphic" << std::endl;
    }  

    virtual Graphic* clone() const {
        return new Graphic(*this);
    }

    virtual ostream& print(ostream& s) const { return s;}
};

class NewsLetter {
public:
    NewsLetter(std::string str) {
        components.push_back(readComponent(str));
    }
private:
    std::vector<NLComponent*> components;

private:
    static NLComponent* readComponent(std::string str) {
        if (str == "TextBlock") return new TextBlock();
        else if (str == "Graphic") return new Graphic();
    }
};

inline 
ostream& operator<<(ostream& s, const NLComponent& c) {
    return c.print(s);
}

int main() {
    NewsLetter letter("TextBlock");
    return 0;
}