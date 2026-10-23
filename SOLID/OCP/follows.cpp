// =============================================================================
// OPEN/CLOSED PRINCIPLE — the FIX.
// A DiscountPolicy interface (this is the Strategy pattern). A new coupon = a
// new class. finalPrice() is CLOSED for modification, OPEN for extension.
//
// Build & run:
//   g++ -std=c++14 follows-ocp.cpp -o follows
//   ./follows
// =============================================================================
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class DiscountPolicy {
public:
  virtual double apply(double cart) = 0;
  virtual string name() = 0;
  virtual ~DiscountPolicy() {}
};

class FlatOff : public DiscountPolicy {
private:
  double amt;

public:
  FlatOff(double amt) { this->amt = amt; }
  double apply(double cart) { return cart > this->amt ? cart - this->amt : 0; }
  string name() { return "Flat Rs " + to_string((int)this->amt); }
};

class PercentOff : public DiscountPolicy {
private:
  double pct;

public:
  PercentOff(double pct) { this->pct = pct; }
  double apply(double cart) { return cart * (1.0 - this->pct / 100.0); }
  string name() { return to_string((int)this->pct) + "% off"; }
};

// NEW requirement next quarter? Just add this class. Nothing above changes.
class CappedPercentOff : public DiscountPolicy {
private:
  double pct;
  double cap;

public:
  CappedPercentOff(double pct, double cap) {
    this->pct = pct;
    this->cap = cap;
  }
  double apply(double cart) {
    double off = cart * this->pct / 100.0;
    if (off > this->cap)
      off = this->cap;
    return cart - off;
  }
  string name() {
    return to_string((int)this->pct) + "% off (max Rs " +
           to_string((int)this->cap) + ")";
  }
};

// CLOSED for modification, OPEN for extension via new policies.
double finalPrice(DiscountPolicy *policy, double cart) {
  return policy->apply(cart);
}

int main() {
  double cart = 1000;
  vector<DiscountPolicy *> coupons;
  coupons.push_back(new FlatOff(100));
  coupons.push_back(new PercentOff(10));
  coupons.push_back(new CappedPercentOff(20, 150));

  cout << "Cart = Rs " << cart << endl;
  for (int i = 0; i < (int)coupons.size(); i++) {
    cout << "  " << coupons[i]->name() << "  ->  Rs "
         << finalPrice(coupons[i], cart) << endl;
  }

  for (int i = 0; i < (int)coupons.size(); i++) {
    delete coupons[i];
  }
  return 0;
}