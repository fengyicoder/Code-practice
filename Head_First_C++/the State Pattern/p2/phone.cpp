#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <sstream>
#include <memory>
#include <map>
#include "stdint.h"

using namespace std;

enum class State
{
    off_hook,
    connecting,
    connected,
    on_hold,
    on_hook,
};

enum class Trigger
{
    call_dialed,
    hung_up,
    call_connected,
    placed_on_hold,
    taken_off_hold,
    left_message,
    stop_using_phone
};

map<State, vector<pair<Trigger, State>>> rules;

rules[State::off_hook] = {
    {Trigger::call_dialed, State::connecting},
    {Trigger::stop_using_phone, State::on_hook}
  };

  rules[State::connecting] = {
    {Trigger::hung_up, State::off_hook},
    {Trigger::call_connected, State::connected}
  };

  rules[State::connected] = {
    {Trigger::left_message, State::off_hook},
    {Trigger::hung_up, State::off_hook},
    {Trigger::placed_on_hold, State::on_hold}
  };

  rules[State::on_hold] = {
    {Trigger::taken_off_hold, State::connected},
    {Trigger::hung_up, State::off_hook}
  };


int main()
{
    State currentState { State::off_hook },
        exitState { State::on_hook };
    while(true)
    {
        cout << "The phone is currently " << currentState << endl;
        
        select_trigger:
            cout << "Select a trigger:" << "\n";
        
        int i = 0;
        for(auto &&item : rules[currentState])
        {
            cout << i++ << ". " << item.first << "\n";
        }

        int input;
        cin >> input;
        for(input < 0 || (input+1) > rules[currentState].size())
        {
            goto select_trigger;
        }

        currentState = rules[currentState][input].second;
        if(currentState == exitState) break;
    }
}
