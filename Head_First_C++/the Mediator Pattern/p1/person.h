#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <sstream>
#include <memory>
#include "stdint.h"

using namespace std;

struct ChatRoom;

struct Person 
{
    string name;
    ChatRoom* room = nullptr;
    vector<string> chat_log;
    Person(const string& name): name{name} { }
    void receive(const string& origin, const string& message);
    void say(const string& message) const;
    void pm(const string& who, const string& message) const;
};
