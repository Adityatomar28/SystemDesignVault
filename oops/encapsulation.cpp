// =============================================================================
// ENCAPSULATION — bundling data with the methods that guard it, and hiding the
// internals behind a controlled interface so the object's rules ("invariants")
// can never be violated.
//
// Domain: a bank account. The ONE rule we must never break: balance >= 0.
// Because `balancePaise` is private, the ONLY way to change it is through
// methods that enforce that rule — no outside code can corrupt the state.
//
// Build & run:
//   g++ -std=c++14 encapsulation.cpp -o encapsulation
//   ./encapsulation
// =============================================================================
#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

class BankAccount {
private:
  string owner;
  long long
      balancePaise; // stored in paise (integer) to avoid float drift on money

public:
  BankAccount(string owner, long long openingPaise) {
    this->owner = owner;
    this->balancePaise = 0;
    // Route even the opening balance through deposit() so the rule and its
    // validation live in exactly ONE place (don't-repeat-yourself).
    if (openingPaise > 0)
      this->deposit(openingPaise);
  }

  // --- Commands: the only ways to change state, each guarding the invariant.
  void deposit(long long paise) {
    if (paise <= 0)
      throw invalid_argument("deposit must be positive");
    this->balancePaise += paise;
  }

  void withdraw(long long paise) {
    if (paise <= 0)
      throw invalid_argument("withdrawal must be positive");
    if (paise > this->balancePaise)
      throw runtime_error("insufficient funds");
    this->balancePaise -= paise; // the only place the balance ever decreases
  }

  // --- Queries: read-only views. We expose rupees, not the raw field.
  double balanceRupees() { return this->balancePaise / 100.0; }
  string getOwner() { return this->owner; }
};

int main() {
  BankAccount *acc = new BankAccount("Aarav", 500000); // Rs 5000.00
  cout << acc->getOwner() << "'s balance: Rs " << acc->balanceRupees() << endl;

  acc->deposit(150000);
  acc->withdraw(200000);
  cout << "After +1500, -2000:      Rs " << acc->balanceRupees() << endl;

  // The invariant is enforced no matter how the object is misused:
  try {
    acc->withdraw(99999900); // way more than we have
  } catch (exception &e) {
    cout << "Blocked bad withdraw:    " << e.what() << endl;
  }

  cout << "Final balance (still >=0): Rs " << acc->balanceRupees() << endl;
  delete acc;
  return 0;
}