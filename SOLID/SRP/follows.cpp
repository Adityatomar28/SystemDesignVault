// =============================================================================
// SINGLE RESPONSIBILITY PRINCIPLE — the FIX.
// One class per responsibility. Order just MODELS the order; persistence,
// formatting and emailing each get their own class. Each now has exactly ONE
// reason to change, and each can be tested or swapped independently.
//
// Build & run:
//   g++ -std=c++14 follows-srp.cpp -o follows
//   ./follows
// =============================================================================
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Line {
  string item;
  double price;
  int qty;
};

// (1) Pure domain model — changes only if the business meaning of an order
// does.
class Order {
private:
  vector<Line> lines;

public:
  void addLine(Line l) { this->lines.push_back(l); }
  double total() {
    double t = 0;
    for (int i = 0; i < (int)this->lines.size(); i++) {
      t += this->lines[i].price * this->lines[i].qty;
    }
    return t;
  }
  vector<Line> &getLines() { return this->lines; }
};

// (2) Persistence — changes only if the storage mechanism changes.
class OrderRepository {
public:
  void save(Order &o) {
    cout << "  [DB] saved order, total = Rs " << (long)o.total() << endl;
  }
};

// (3) Presentation — changes only if the receipt layout changes.
class ReceiptFormatter {
public:
  string format(Order &o) {
    string s = "  RECEIPT\n";
    vector<Line> &lines = o.getLines();
    for (int i = 0; i < (int)lines.size(); i++) {
      s += "    " + lines[i].item + " x" + to_string(lines[i].qty) + "\n";
    }
    s += "    TOTAL: Rs " + to_string((long)o.total());
    return s;
  }
};

// (4) Notification — changes only if the delivery channel changes.
class EmailService {
public:
  void send(string body) { cout << "  [EMAIL] sending:\n" << body << endl; }
};

int main() {
  Order *order = new Order();
  order->addLine({"USB-C Cable", 299, 2});
  order->addLine({"Phone Case", 499, 1});

  OrderRepository *repo = new OrderRepository();
  ReceiptFormatter *formatter = new ReceiptFormatter();
  EmailService *email = new EmailService();

  repo->save(*order); // each collaborator does exactly ONE job
  email->send(
      formatter->format(*order)); // and can be swapped / tested on its own

  delete order;
  delete repo;
  delete formatter;
  delete email;
  return 0;
}