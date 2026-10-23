// =============================================================================
// LISKOV SUBSTITUTION PRINCIPLE — the VIOLATION.
// "Subtypes must be substitutable for their base type." Any code that works
// with the base must keep working when handed a subclass — no surprises.
//
// Here every Account can withdraw(). A FixedDeposit "is-a" Account but CAN'T be
// withdrawn before maturity, so it overrides withdraw() to throw. Now any
// function taking an Account& is a runtime landmine.
//
// Build & run:
//   g++ -std=c++14 violates-lsp.cpp -o violates
//   ./violates
// =============================================================================
#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

class Account {
protected:
  double balance;

public:
  Account() { this->balance = 0; }
  virtual void deposit(double a) { this->balance += a; }
  virtual void withdraw(double a) {
    this->balance -= a;
  } // base PROMISES this works
  double getBalance() { return this->balance; }
  virtual ~Account() {}
};

class FixedDeposit : public Account {
public:
  void withdraw(double a) {
    // Breaks the promise the base made. A function written against Account&
    // that calls withdraw() will crash when handed a FixedDeposit.
    throw logic_error("cannot withdraw from an FD before maturity");
  }
};

// Looks correct against the base type, but is a runtime landmine:
void payBill(Account *acc, double amt) { acc->withdraw(amt); }

int main() {
  Account *sav = new Account();
  sav->deposit(5000);
  payBill(sav, 1200); // fine for a normal account
  cout << "Savings balance after bill: Rs " << sav->getBalance() << endl;

  Account *fd = new FixedDeposit();
  fd->deposit(100000);
  cout << "Now paying a bill from the FD (typed as Account)..." << endl;
  try {
    payBill(fd, 100); // compiles, then BOOM at runtime
  } catch (exception &e) {
    cout << "  CRASH: " << e.what() << endl;
  }

  delete sav;
  delete fd;
  return 0;
}