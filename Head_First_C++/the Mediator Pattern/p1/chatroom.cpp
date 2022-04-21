
#include "chatroom.h"

void Person::say(const string& message) const {
    room->broadcast(name, message);
}

void Person::pm(const string& who, const string& message) const {
    room->message(name, who, message);
}

void Person::receive(const string& origin, const string& message) {
    string s{origin + ": \"" + message + "\""};
    cout << "[" << name << "'s chat session] " << s << "\n";
    chat_log.emplace_back(s);
}

int main()
{
    ChatRoom room;
    Person john{"john"};
    Person jane{"jane"};
    room.join(&john);
    room.join(&jane);
    john.say("hi room");
    jane.say("oh, hey john");

    Person simon("simon");
    room.join(&simon);
    simon.say("hi everyone!");

    jane.pm("simon", "glad you could join us, simon");
    return 0;
}