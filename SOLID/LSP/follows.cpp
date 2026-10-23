// =============================================================================
// LISKOV SUBSTITUTION PRINCIPLE — the FIX.
// Split the hierarchy by CAPABILITY. Only accounts that truly support
// withdrawal implement a Withdrawable interface, and payBill() asks for exactly
// that. Handing it a FixedDeposit becomes a COMPILE-TIME error, not a crash.
//
// Build & run:
//   g++ -std=c++14 follows-lsp.cpp -o follows
//   ./follows
// =============================================================================
#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

// Base = what EVERY account can honestly do.
class Account {
protected:
  double balance;

public:
  Account() { this->balance = 0; }
  void deposit(double a) { this->balance += a; }
  double getBalance() { return this->balance; }
  virtual ~Account() {}
};

// A separate capability. Only accounts that can really do this implement it.
class Withdrawable {
public:
  virtual void withdraw(double amount) = 0;
  virtual ~Withdrawable() {}
};

class SavingsAccount : public Account, public Withdrawable {
public:
  void withdraw(double amount) {
    if (amount > this->balance)
      throw runtime_error("insufficient funds");
    this->balance -= amount;
  }
};

// An FD IS-A Account, but it is simply NOT Withdrawable — and the type says so.
class FixedDepositAccount : public Account {
private:
  int maturityMonths;

public:
  FixedDepositAccount(int months) { this->maturityMonths = months; }
  int getMaturityMonths() { return this->maturityMonths; }
};

// This function asks for exactly the capability it needs. It can NEVER be
// handed something that can't withdraw — the mistake is caught at compile time.
void payBill(Withdrawable *source, double amt) { source->withdraw(amt); }

int main() {
  SavingsAccount *sav = new SavingsAccount();
  sav->deposit(5000);
  payBill(sav, 1200); // fine — Savings IS Withdrawable
  cout << "Savings balance after bill: Rs " << sav->getBalance() << endl;

  FixedDepositAccount *fd = new FixedDepositAccount(12);
  fd->deposit(100000);
  cout << "FD balance: Rs " << fd->getBalance() << " (matures in "
       << fd->getMaturityMonths() << " months)" << endl;
  // payBill(fd, 100);  // <-- would NOT compile: FixedDepositAccount isn't
  // Withdrawable
  cout << "Compiler refuses payBill(fd, ...): the LSP trap is gone." << endl;

  delete sav;
  delete fd;
  return 0;
}