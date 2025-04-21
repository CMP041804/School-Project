#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>

#include "Customer.h"
#include "Transaction.h"
#include "Utils.h"

using namespace std;

int main() { //Vector used to store all customer objects. This uses pointers.
    vector<unique_ptr<Customer>> customers;
    int choice;

    do { // This will display the main menu options and its loops.
        cout << "\nBanking System Menu:\n";
        cout << "1. Add Regular Customer\n";
        cout << "2. Add Premium Customer\n";
        cout << "3. Display All Customers\n";
        cout << "4. Save to File\n";
        cout << "5. Manage Existing Customer\n";
        cout << "6. Apply Interest to All Premium Customers\n";
        cout << "7. Remove Customer\n";
        cout << "8. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (cin.fail() || choice < 1 || choice > 8) { //Input validation check. Will also handle invalid inputs.
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid choice. Please enter a number from 1 to 8.\n";
            continue;
        }

        cin.ignore();

        if (choice == 1 || choice == 2) { //Option to add either a Regular Customer or Premium Customer.
            string name;
            int accNum;
            double bal;

            cout << "Enter name: ";
            getline(cin, name);

            if (!getValidatedInput("Enter account number: ", accNum)) continue;
            if (!getValidatedInput("Enter balance: ", bal)) continue;

            if (choice == 1) {
                customers.push_back(make_unique<Customer>(name, accNum, bal));
            } else {
                double rate;
                if (!getValidatedInput("Enter interest rate (e.g., 0.05 for 5%): ", rate)) continue; //For Premium Customers, the option to include interest is applied.
                auto pc = make_unique<PremiumCustomer>(name, accNum, bal, rate);
                pc->applyInterest();  // Optional: Apply once at creation
                customers.push_back(move(pc));
            }

        } else if (choice == 3) { //Option to display all customers within the database.
            if (customers.empty()) {
                cout << "No customers to display.\n"; //Handles scenario in which no customers are in the database, in which there is nothing to display.
            } else {
                for (const auto& c : customers) {
                    c->displayInfo();
                    cout << "------------------\n";
                }
            }

        } else if (choice == 4) { //Option to save all customer data to a file, customer.txt.
            ofstream outFile("customers.txt");
            if (!outFile) {
                cout << "Error opening file.\n";
            } else {
                for (const auto& c : customers) {
                    c->saveToFile(outFile);
                }
                cout << "Data saved to 'customers.txt'\n";
            }

        } else if (choice == 5) { //Option to manage an existing customer via inputting their banking account number.
            int accNum;
            if (!getValidatedInput("Enter account number to manage: ", accNum)) continue;

            Customer* cust = findCustomerByAccount(customers, accNum);
            if (!cust) {
                cout << "Customer not found.\n"; //Handles case in which banking number is invalid or no account associated with inputted number.
            } else {
                manageCustomer(cust);
            }

        } else if (choice == 6) { //Option to apply interest to all Premium Customers.
            for (auto& c : customers) {
                c->applyInterest();
            }
            cout << "Interest applied to all premium customers.\n";

        } else if (choice == 7) { //Option to fully remove an account via banking account number.
            int accNum;
            if (!getValidatedInput("Enter account number to remove: ", accNum)) {
                cout << "Invalid input.\n"; //Handles scenario where number is invalid or no account associated with inputted number.
                continue;
            }

            auto it = remove_if(customers.begin(), customers.end(), //Code to remove a customer once banking account is found.
                [accNum](const unique_ptr<Customer>& c) {
                    return c->getAccountNumber() == accNum;
                });

            if (it != customers.end()) { //If found, program will erase customer from the vector.
                customers.erase(it, customers.end());
                cout << "Customer removed successfully.\n";
            } else {
                cout << "Customer with account number " << accNum << " not found.\n";
            }
        }

    } while (choice != 8); //Option to exit program.

    cout << "Exiting program. Goodbye!\n";
    return 0;
}
