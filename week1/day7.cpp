#include <iostream>
#include <string>

class Account {
public:
    Account(const std::string& owner, double balance)
        : owner(owner), balance(balance) {}

    void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            std::cout << "Deposited: " << amount << std::endl;
        } else {
            std::cout << "Invalid deposit amount" << std::endl;
        }
    }

    void withdraw(double amount) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            std::cout << "Withdrew: " << amount << std::endl;
        } else {
            std::cout << "Invalid withdrawal amount" << std::endl;
        }
    }

    void display() const {
        std::cout << "Owner: " << owner << ", Balance: " << balance << std::endl;
    }

protected:
    std::string owner;
    double balance;
};

class SavingsAccount : public Account {
public:
    SavingsAccount(const std::string& owner, double balance, double interestRate)
        : Account(owner, balance), interestRate(interestRate) {}

    void addInterest() {
        double interest = balance * interestRate / 100;
        deposit(interest);
        std::cout << "Interest added: " << interest << std::endl;
    }

private:
    double interestRate;
};

int main() {
    Account acc("John Doe", 1000.0);
    acc.deposit(500.0);
    acc.withdraw(200.0);
    acc.display();

    SavingsAccount savAcc("Jane Doe", 2000.0, 5.0);
    savAcc.deposit(1000.0);
    savAcc.addInterest();
    savAcc.display();

    return 0;
}