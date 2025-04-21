#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <limits>
#include <memory>
#include <vector>
#include "Customer.h"

using namespace std;

template<typename T> //Template used to ensure a valid input like int, double, etc.
bool getValidatedInput(const string& prompt, T& value) {
    cout << prompt;
    cin >> value;
    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return true; //This will return true if input is valid, otherwise false.
}

Customer* findCustomerByAccount(const vector<unique_ptr<Customer>>& customers, int accNum) { //Used to find customers through the account number.
    for (const auto& c : customers) {
        if (c->getAccountNumber() == accNum) {
            return c.get();
        }
    }
    return nullptr;
}//Pointer will return to matching customer if found. If account number is invalid or not found, it will return nullptr.

void manageCustomer(Customer* customer) { //Displays the submenu for a customer once they are selected.
    int subChoice;
    do {
        cout << "\nManage Customer:\n";
        cout << "1. Deposit Funds\n";
        cout << "2. Withdraw Funds\n";
        cout << "3. Show Info\n";
        cout << "4. Show Transaction History\n";
        cout << "5. Return to Main Menu\n";
        cout << "Enter your choice: ";
        cin >> subChoice;

        if (cin.fail()) { //Used to handle invalid menu choices.
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid choice.\n";
            continue;
        }

        cin.ignore();

        if (subChoice == 1) { //Option to deposit into an account.
            double amount;
            if (getValidatedInput("Enter deposit amount: ", amount)) {
                customer->deposit(amount);
            }
        } else if (subChoice == 2) { //Option to withdrawal from an account.
            double amount;
            if (getValidatedInput("Enter withdrawal amount: ", amount)) {
                customer->withdraw(amount);
            }
        } else if (subChoice == 3) { //Option to display customer info.
            customer->displayInfo();
        } else if (subChoice == 4) { //Option to show all transactions.
            customer->showTransactionHistory();
        }

    } while (subChoice != 5); //To ensure submenu will loop until option to return to main menu is chosen.
}

#endif
