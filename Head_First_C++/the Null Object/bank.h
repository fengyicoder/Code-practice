#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <sstream>
#include <memory>
#include "stdint.h"

using namespace std;

struct Logger
{
    virtual ~Logger() = default;
    virtual void info(const string& s) = 0;
    virtual void warn(const string& s) = 0;
};

struct BankAccount
{
    shared_ptr<Logger> log;
    string name;
    int balance = 0;
    BankAccount(const shared_ptr<Logger>& logger, const string& name, int balance):
        log{logger}, name{name}, balance{balance} {}
    
    void BankAccount::deposit(int amount)
    {
        balance += amount;
        log->info(("Deposited $" + lexical_cast<string>(amount)
                + " to " + name + ", balance is now $" 
                + lexical_cast<string>(balance));
    }
};

struct ConsoleLogger: Logger
{
    void info(const string& s) override
    {
        cout << "INFO: " << s << endl;
    }

    void warn(const string& s) override
    {
        cout << "WARNNING!!!" << s << endl;
    }
};

struct NullLogger: Logger
{
    void info(const string& s) override {}
    void warn(const string& s) override {}
};

struct OptionalLogger: Logger
{
    shared_ptr<Logger> impl;
    // static shared_ptr<Logger> no_logging;
    Logger(const shared_ptr<Logger>& logger): impl{logger} { }
    virtual void info(const string& s) override
    {
        if(impl) impl->info(s);
    }
};

struct BankAccount2
{
    shared_ptr<Logger> log;
    static shared_ptr<Logger> no_logging;
    string name;
    int balance = 0;
    BankAccount(const string& name, int balance, const shared_ptr<Logger>& logger=no_logging):
        log{make_shared<OptionalLogger>(logger)}, name{name}, balance{balance} {}
    
    void BankAccount::deposit(int amount)
    {
        balance += amount;
        log->info(("Deposited $" + lexical_cast<string>(amount)
                + " to " + name + ", balance is now $" 
                + lexical_cast<string>(balance));
    }
};

shared_ptr<Logger> BankAccount2::no_logging{};

