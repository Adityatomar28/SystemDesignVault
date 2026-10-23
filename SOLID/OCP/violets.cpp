// =============================================================================
// OPEN/CLOSED PRINCIPLE — the VIOLATION.
// "Software entities should be OPEN for extension, CLOSED for modification."
//
// Here, finalPrice() is a growing if/else over coupon strings. Every NEW coupon
// type forces an edit to this already-tested function — re-risking every branch
// that was working (exactly the "ripple bug" from section 02).
//
// Build & run:
//   g++ -std=c++14 violates-ocp.cpp -o violates
//   ./violates
// =============================================================================
#include <iostream>
#include <string>

using namespace std;

double finalPrice(string coupon, double cart) {
  if (coupon == "FLAT100")
    return cart - 100;
  else if (coupon == "PERCENT10")
    return cart * 0.90;
  else if (coupon == "FESTIVE")
    return cart * 0.80;
  // Next quarter: add "BANK_OFFER", "FIRST_ORDER", "BOGO" ... this function
  // grows forever and every edit risks the existing branches.
  return cart;
}

int main() {
  double cart = 1000;
  cout << "Cart = Rs " << cart << endl;
  cout << "  FLAT100   ->  Rs " << finalPrice("FLAT100", cart) << endl;
  cout << "  PERCENT10 ->  Rs " << finalPrice("PERCENT10", cart) << endl;
  cout << "  FESTIVE   ->  Rs " << finalPrice("FESTIVE", cart) << endl;
  cout << "Every new coupon type means editing finalPrice() again." << endl;
  return 0;
}