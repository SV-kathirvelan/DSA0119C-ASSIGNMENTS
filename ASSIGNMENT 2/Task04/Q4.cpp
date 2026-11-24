#include <iostream>
#include <string>
using namespace std;

class SavingsAccount {
private:
    string accountNumber;
    double balance;
    string transactions[10];
    int count;

    void addTransaction(const string& item) {
        if (count < 10) {
            transactions[count++] = item;
        } else {
            // Keep the newest 10: shift left, then insert at the end.
            for (int i = 1; i < 10; ++i) transactions[i - 1] = transactions[i];
            transactions[9] = item;
        }
    }

public:
    SavingsAccount(const string& number, double opening)
        : accountNumber(number), balance(opening), count(0) {}

    void deposit(double amount);
    void withdraw(double amount);
    void miniStatement() const;
};

void SavingsAccount::deposit(double amount) {
    if (amount > 0) {
        balance += amount;
        addTransaction("Deposit: +" + to_string(amount));
    }
}
void SavingsAccount::withdraw(double amount) {
    if (amount > 0 && amount <= balance) {
        balance -= amount;
        addTransaction("Withdraw: -" + to_string(amount));
    }
}
void SavingsAccount::miniStatement() const {
    cout << "Account: " << accountNumber << '\n';
    cout << "Balance: " << balance << '\n';
    cout << "Last transactions (" << count << "):\n";
    for (int i = 0; i < count; ++i) cout << transactions[i] << '\n';
}

int main() {
    SavingsAccount account("SB1001", 1000);
    for (int i = 1; i <= 12; ++i) account.deposit(i * 100);
    account.withdraw(250);
    account.miniStatement();
}