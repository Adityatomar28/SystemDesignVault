// =============================================================================
// ABSTRACTION — exposing *what* an object can do (its essential interface) while
// hiding *how* it does it. Callers program to the abstraction, not the details.
//
// Domain: a checkout that accepts different payment methods. The checkout knows
// only "a PaymentMethod can pay(amount)". It has no idea about card networks,
// UPI handshakes, or wallet balances — those details live behind the interface.
//
// Build & run:
//   g++ -std=c++14 abstraction.cpp -o abstraction
//   ./abstraction
// =============================================================================
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// The abstraction: the essential capability, with ZERO implementation detail.
class PaymentMethod {
public:
    virtual bool pay(double amount) = 0;   // pure virtual => abstract: the WHAT, not the HOW
    virtual string name() = 0;
    virtual ~PaymentMethod() {}
};

// Each concrete type hides its own messy "how" behind the same clean "what".
class CreditCard : public PaymentMethod {
private:
    string last4;
public:
    CreditCard(string last4) {
        this->last4 = last4;
    }
    bool pay(double amount) {
        // (hidden detail) tokenize, contact the card network, handle 3-D Secure...
        cout << "  [CreditCard ****" << this->last4 << "] authorizing Rs " << amount << endl;
        return true;
    }
    string name() { return "Credit Card"; }
};

class UpiPayment : public PaymentMethod {
private:
    string vpa;
public:
    UpiPayment(string vpa) {
        this->vpa = vpa;
    }
    bool pay(double amount) {
        // (hidden detail) build a collect request, poll the PSP, await the PIN...
        cout << "  [UPI " << this->vpa << "] collecting Rs " << amount << endl;
        return true;
    }
    string name() { return "UPI"; }
};

// The client depends ONLY on the abstraction. Add a Wallet tomorrow and this
// function never changes — that is the payoff of abstraction.
void checkout(PaymentMethod* method, double amount) {
    cout << "Paying via " << method->name() << "..." << endl;
    if (method->pay(amount)) {
        cout << "  -> success" << endl;
    }
}

int main() {
    vector<PaymentMethod*> methods;
    methods.push_back(new CreditCard("4242"));
    methods.push_back(new UpiPayment("aarav@okbank"));

    for (int i = 0; i < (int)methods.size(); i++) {
        checkout(methods[i], 799.0);
    }

    for (int i = 0; i < (int)methods.size(); i++) {
        delete methods[i];
    }
    return 0;
}