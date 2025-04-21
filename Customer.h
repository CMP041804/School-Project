#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>
#include <string>
#include <memory>
#include "Transaction.h"

using namespace std;

struct AccountInfo { //Struct to hold account related data like account number and balance.
    int accountNumber;
    double balance;
};

class Customer { //Class for Normal Customers. Provides data like name, a pointer to account info via dynamic memory*, and transaction history.
protected:
    string name;
    AccountInfo* account;
    vector<Transaction> transactions;

public: //Initializes customer details and logs first, or initial, deposits.
    Customer(string name, int accNum, double bal) {
        this->name = name;
        account = new AccountInfo{accNum, bal};
        logTransaction("Initial Deposit", bal);
    }

    virtual ~Customer() { //This is applied to allow proper cleanup of derived classes.
        delete account; //*And cleanup dynamically allocated memory.
    }

    virtual void displayInfo() const { //Displays customer info which can be overridden by derived classes if needed.
        cout << "Customer Name: " << name << endl;
        cout << "Account Number: " << account->accountNumber << endl;
        cout << fixed << setprecision(2);
        cout << "Balance: $" << account->balance << endl;
    }

    virtual void saveToFile(ofstream& out) const { //Option to save customer info to file.
        out << "Customer " << name << " " << account->accountNumber << " " << account->balance << endl;
    }

    void deposit(double amount) { //Option to deposit money into account.
        account->balance += amount;
        logTransaction("Deposit", amount);
        cout << "Deposited $" << amount << ". New balance: $" << account->balance << endl;
    }

    virtual bool withdraw(double amount) { //Option to withdrawal money from account. This will fail if insufficient funds are in the account.
        if (account->balance >= amount) {
            account->balance -= amount;
            logTransaction("Withdrawal", amount);
            cout << "Withdrew $" << amount << ". New balance: $" << account->balance << endl;
            return true;
        } else {
            cout << "Insufficient balance.\n";
            return false;
        }
    }

    int getAccountNumber() const { //Used to get account number, important for searching.
        return account->accountNumber;
    }

    virtual void applyInterest() { //Used to apply interest to Premium Customers, does not do anything for Normal Customers.
        // Regular customers don't earn interest
    }

    void showTransactionHistory() const { //Used to show full transaction history of selected customer.
        cout << "Transaction History for " << name << ":\n";
        for (const auto& t : transactions) {
            cout << t.timestamp << " - " << t.type << ": $" << fixed << setprecision(2) << t.amount << endl;
        }
    }

protected: //A helper function to aid in recording a transaction into history.
    void logTransaction(const string& type, double amount) {
        transactions.push_back({type, amount, currentDateTime()});
    }
};

class PremiumCustomer : public Customer { //Derived Premium Customer Class. Effectively the same as Normal Customer Class,
    double interestRate;

public: //Overrides used to initialize interest rates, log transactions, and save premium customer data.
    PremiumCustomer(string name, int accNum, double bal, double interest)
        : Customer(name, accNum, bal), interestRate(interest) {}

    void applyInterest() override {
        double interest = account->balance * interestRate;
        account->balance += interest;
        logTransaction("Interest", interest);
        cout << "Applied interest of $" << interest << " to account " << account->accountNumber << endl;
    }

    void displayInfo() const override {
        cout << "[Premium Customer]" << endl;
        Customer::displayInfo();
        cout << "Interest Rate: " << interestRate * 100 << "%" << endl;
    }

    void saveToFile(ofstream& out) const override {
        out << "PremiumCustomer " << name << " " << account->accountNumber << " "
            << account->balance << " " << interestRate << endl;
    }
};

#endif
