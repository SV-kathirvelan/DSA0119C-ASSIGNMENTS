#include <iostream>
using namespace std;

struct Person {
    string name;
    int id;
    string phone;
    Person(string n = "Unknown", int i = 0, string p = "NA")
        : name(n), id(i), phone(p) {
        cout << "Person ";
    }
    virtual ~Person() = default;
};

struct Student : virtual public Person {
    string programme;
    double cgpa;
    Student(string n = "Unknown", int i = 0, string p = "NA",
            string pr = "NA", double c = 0)
        : Person(n, i, p), programme(pr), cgpa(c) {
        cout << "Student ";
    }
};

struct Staff : virtual public Person {
    string department;
    double salary;
    Staff(string n = "Unknown", int i = 0, string p = "NA",
          string d = "NA", double s = 0)
        : Person(n, i, p), department(d), salary(s) {
        cout << "Staff ";
    }
};

struct TeachingAssistant : public Student, public Staff {
    TeachingAssistant(string n, int i, string p, string pr, double c,
                      string d, double s)
        : Person(n, i, p), Student(n, i, p, pr, c),
          Staff(n, i, p, d, s) {
        cout << "TA ";
    }
};

struct ResearchScholar : public Student {
    double fellowship;
    ResearchScholar(string n, int i, string p, string pr, double c, double f)
        : Person(n, i, p), Student(n, i, p, pr, c), fellowship(f) {}
};

int main() {
    TeachingAssistant ta("Kavya", 11, "9876543210", "CSE", 8.7, "AI", 30000);
    cout << "\n" << ta.name << " " << ta.programme << " "
         << ta.department << '\n';
}
