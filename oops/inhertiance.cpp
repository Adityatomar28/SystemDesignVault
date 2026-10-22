// =============================================================================
// INHERITANCE — an "is-a" relationship that lets a subclass reuse and specialize
// a base class. Use it for genuine specialization, NOT just to share code
// (prefer composition for code reuse — see SOLID, section 05).
//
// Domain: employees in a company. Every Employee shares identity + the idea of
// monthly pay, but each role computes pay differently and may add its own data.
//
// Build & run:
//   g++ -std=c++14 inheritance.cpp -o inheritance
//   ./inheritance
// =============================================================================
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Employee {
protected:
    // protected => visible to subclasses, hidden from the outside world.
    string name;
    int id;
public:
    Employee(string name, int id) {
        this->name = name;
        this->id = id;
    }
    // Shared behaviour, inherited as-is by every subclass.
    string getName() { return this->name; }
    int getId() { return this->id; }
    // Specialized behaviour: each role MUST override these.
    virtual double monthlySalary() = 0;
    virtual string role() = 0;
    virtual ~Employee() {}
};

// "A FullTimeEmployee IS-A Employee" — paid a fixed monthly figure.
class FullTimeEmployee : public Employee {
private:
    double monthly;
public:
    FullTimeEmployee(string name, int id, double monthly) : Employee(name, id) {
        this->monthly = monthly;
    }
    double monthlySalary() { return this->monthly; }
    string role() { return "Full-time"; }
};

// "A Contractor IS-A Employee" — paid by the hour.
class Contractor : public Employee {
private:
    double rate;
    int hours;
public:
    Contractor(string name, int id, double hourlyRate, int hours) : Employee(name, id) {
        this->rate = hourlyRate;
        this->hours = hours;
    }
    double monthlySalary() { return this->rate * this->hours; }
    string role() { return "Contractor"; }
};

// A Manager reuses FullTimeEmployee and EXTENDS it with a team bonus.
class Manager : public FullTimeEmployee {
private:
    double bonus;
public:
    Manager(string name, int id, double monthly, double bonus)
        : FullTimeEmployee(name, id, monthly) {
        this->bonus = bonus;
    }
    double monthlySalary() {
        return FullTimeEmployee::monthlySalary() + this->bonus;   // call the base, then add
    }
    string role() { return "Manager"; }
};

int main() {
    vector<Employee*> staff;
    staff.push_back(new FullTimeEmployee("Aarav", 1, 90000));
    staff.push_back(new Contractor("Bhavna", 2, 1200, 160));
    staff.push_back(new Manager("Chetan", 3, 120000, 30000));

    double payroll = 0;
    for (int i = 0; i < (int)staff.size(); i++) {
        cout << staff[i]->getId() << "  " << staff[i]->getName()
             << "  (" << staff[i]->role() << ")  ->  Rs " << staff[i]->monthlySalary() << endl;
        payroll += staff[i]->monthlySalary();
    }
    cout << "Total monthly payroll: Rs " << payroll << endl;

    for (int i = 0; i < (int)staff.size(); i++) {
        delete staff[i];
    }
    return 0;
}