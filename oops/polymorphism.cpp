// =============================================================================
// POLYMORPHISM — "many forms". The SAME call behaves correctly for the actual
// object behind it. There are two flavours, both shown below:
//
//   1. STATIC (compile-time): function overloading & templates. The compiler
//      picks the correct version from the argument types. No runtime cost.
//   2. DYNAMIC (run-time): virtual functions. The correct override is chosen
//      at run time from the REAL object sitting behind a base-class pointer.
//      This is the kind that powers almost every design pattern in this course.
//
// Real-world analogy: you press "send" in your phone. Email, SMS and Push all
// understand "send", but each performs it in its own way. Your code just says
// send(); the right behaviour happens based on which channel object you hold.
//
// Build & run:
//   g++ -std=c++14 polymorphism.cpp -o polymorphism
//   ./polymorphism
// =============================================================================
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// ---------- STATIC polymorphism: same name, compiler chooses by type ---------
string asLabel(int n) { return "int(" + to_string(n) + ")"; }
string asLabel(double d) { return "double(" + to_string(d) + ")"; }
string asLabel(const string &s) { return "string(\"" + s + "\")"; }

// A function template is also static polymorphism: ONE source, the compiler
// stamps out a concrete version for each type you actually use it with.
template <typename T> T maxOf(T a, T b) { return (a > b) ? a : b; }

// ---------- DYNAMIC polymorphism: virtual dispatch at run time ---------------
// The parent class defines the COMMON contract. send() is abstract (= 0): every
// channel MUST provide it, but each one does it differently.
class Notifier {
public:
  virtual void
  send(const string &msg) = 0; // abstract -> forces each child to override
  virtual ~Notifier() {}       // virtual dtor -> safe delete via a base*
};

class EmailNotifier : public Notifier {
public:
  void send(const string &msg) { cout << "  [EMAIL] " << msg << endl; }
};

class SmsNotifier : public Notifier {
public:
  void send(const string &msg) { cout << "  [SMS]   " << msg << endl; }
};

class PushNotifier : public Notifier {
public:
  void send(const string &msg) { cout << "  [PUSH]  " << msg << endl; }
};

// Written ONCE against the base type. It works for every current AND future
// Notifier subclass without changing a line here — the power of dynamic
// polymorphism (and the Open/Closed Principle you will meet in section 05).
void broadcast(const vector<Notifier *> &channels, const string &msg) {
  for (int i = 0; i < (int)channels.size(); i++) {
    channels[i]->send(msg); // resolved at RUN TIME to the real type
  }
}

int main() {
  cout << "Static polymorphism (overloading):" << endl;
  cout << "  " << asLabel(42) << endl;
  cout << "  " << asLabel(3.14) << endl;
  cout << "  " << asLabel(string("hello")) << endl;
  cout << "  maxOf(3, 9)     = " << maxOf(3, 9) << endl;
  cout << "  maxOf(2.5, 1.5) = " << maxOf(2.5, 1.5) << endl;
  cout << endl;

  cout << "Dynamic polymorphism (virtual dispatch):" << endl;
  vector<Notifier *> channels;
  channels.push_back(new EmailNotifier());
  channels.push_back(new SmsNotifier());
  channels.push_back(new PushNotifier());

  broadcast(channels, "Your OTP is 4827");

  // We used raw `new`, so we must `delete`. The virtual destructor guarantees
  // the correct subclass destructor runs even through a base-class pointer.
  for (int i = 0; i < (int)channels.size(); i++) {
    delete channels[i];
  }
  return 0;
}