#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <sstream>
#include "stdint.h"

using namespace std;

struct BankAccount
{
    int balance = 0;
    int overdraft_limit = -500;

    void deposit(int amount) {
        balance += amount;
        cout << "deposited " << amount << ", balance is now " << balance << "\n";
    }

    bool withdraw(int amount) {
        if (balance - amount >= overdraft_limit) {
            balance -= amount;
            cout << "withdraw " << amount << ", balance is now " << balance << "\n";
            return true;
        }
        return false;
    }
};

struct Command
{
    virtual void call() = 0;
    virtual void undo() = 0;
};

struct BankAccountCommand: Command
{
    BankAccount& account;
    enum Action{ deposit, withdraw} action;
    int amount;
    bool withdrawal_succeeded;
    BankAccountCommand(BankAccount& account, const Action
    action, const int amount): account(account), action(action), 
    amount(amount), withdrawal_succeeded(false) {}

    void call() override
    {
        switch(action) {
            case deposit:
                account.deposit(amount);
                break;
            case withdraw:
                withdrawal_succeeded = account.withdraw(amount);
                break;
        }
    }

    void undo() override
    {
        switch(action)
        {
            case withdraw:
                if(withdrawal_succeeded) account.deposit(amount);
                break;
            case deposit:
                account.withdraw(amount);
                break;
        }
    }
};

struct Command2
{
    bool succeeded;
    virtual void call() = 0;
    virtual void undo() = 0;
};

struct BankAccountCommand2: Command2
{
    BankAccount& account;
    enum Action{ deposit, withdraw} action;
    int amount;
    BankAccountCommand2(BankAccount& account, const Action
    action, const int amount): account(account), action(action), 
    amount(amount) {
        succeeded = false;
    }

    void call() override
    {
        switch(action) {
            case deposit:
                account.deposit(amount);
                succeeded = true;
                break;
            case withdraw:
                succeeded = account.withdraw(amount);
                break;
        }
    }

    void undo() override
    {
        if (!succeeded) return;
        switch(action)
        {
            case withdraw:
                if(succeeded) account.deposit(amount);
                break;
            case deposit:
                account.withdraw(amount);
                break;
        }
    }
};

struct CompositeBankAccountCommand: vector<BankAccountCommand2>, Command2
{
    CompositeBankAccountCommand(const initializer_list
    <value_type>& items): vector<BankAccountCommand2>(items) {}

    void call() override
    {
        for (auto& cmd : *this) cmd.call();
    }

    void undo() override
    {
        for (auto it = rbegin(); it != rend(); ++it) it->undo();
    }
};

struct DependentCompositeCommand : CompositeBankAccountCommand
{
  explicit DependentCompositeCommand(
    const initializer_list<value_type>& _Ilist)
    : CompositeBankAccountCommand{ _Ilist } {}

  void call() override
  {
    bool ok = true;
    for (auto& cmd : *this)
    {
      if (ok)
      {
        cmd.call();
        ok = cmd.succeeded;
      }
      else
      {
        cmd.succeeded = false;
      }
    }
  }
};

int main() {
    BankAccount ba; 
    Command* cmd = new BankAccountCommand(ba, BankAccountCommand::Action::deposit, 100);
    cmd->call();
    return 0;
}