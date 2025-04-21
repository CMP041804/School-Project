#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <ctime>

using namespace std;

struct Transaction { //Created to operate banking transactions like deposits, withdrawals, and interest.
    string type;
    double amount;
    string timestamp;
};

//Used to get current date/time as string. This is to timestamp when transactions occur.
inline string currentDateTime() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char buf[30];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", ltm);
    return string(buf);
}

#endif
