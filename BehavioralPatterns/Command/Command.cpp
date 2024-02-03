#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <algorithm>

class BankAccount
{
public:
    int balance = 0;
    int overdraft_limit = -500;

    void deposit(int amount)
    {
        balance += amount;
        std::cout << "deposited " << amount << ", balance now " <<
            balance << std::endl;
    }

    bool withdraw(int amount)
    {
        if (balance - amount >= overdraft_limit)
        {
            balance -= amount;
            std::cout << "withdrew " << amount << ", balance now " <<
                balance << std::endl;
            return true;
        }
        return false;
    }
};

class Command
{
public:
    bool succeeded;
    virtual void call() = 0;
    virtual void undo() = 0;
};

class BankAccountCommand : Command
{
    BankAccount& account;
    int amount;

public:
    enum Action { deposit, withdraw } action;

    BankAccountCommand(BankAccount& account, const Action action,
        const int amount)
        : account(account),
        action(action), amount(amount)
    {
        succeeded = false;
    }

    void call() override
    {
        switch (action)
        {
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

        switch (action)
        {
        case withdraw:
            if (succeeded)
                account.deposit(amount);
            break;
        case deposit:
            account.withdraw(amount);
            break;
        }
    }
};

class CompositeBankAccountCommand : std::vector<BankAccountCommand>, Command
{
public:
    CompositeBankAccountCommand(const std::initializer_list<value_type>& _Ilist)
        : std::vector<BankAccountCommand>(_Ilist)
    {
    }

    void call() override
    {
        for (auto& cmd : *this)
            cmd.call();
    }

    void undo() override
    {
        for (auto it = rbegin(); it != rend(); ++it)
            it->undo();
    }
};

int main()
{
    BankAccount ba;
    /*vector<BankAccountCommand> commands{*/
    CompositeBankAccountCommand commands{
      BankAccountCommand{ ba, BankAccountCommand::deposit, 100 },
      BankAccountCommand{ ba, BankAccountCommand::withdraw, 200 }
    };

    std::cout << ba.balance << std::endl;

    commands.call();

    std::cout << ba.balance << std::endl;

    commands.undo();

    std::cout << ba.balance << std::endl;

    return 0;
}
