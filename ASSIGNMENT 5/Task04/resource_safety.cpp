#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Bill {
protected:
    std::string patient;
public:
    explicit Bill(std::string p) : patient(std::move(p)) {}
    virtual ~Bill() = default;
    virtual double total() const = 0;
    virtual std::string type() const = 0;
    void print() const {
        std::cout << type() << " " << patient << ": " << total() << '\n';
    }
};

class OutPatient : public Bill {
    double consultation, tests;
public:
    OutPatient(std::string p, double c, double t)
        : Bill(std::move(p)), consultation(c), tests(t) {}
    double total() const override { return consultation + tests; }
    std::string type() const override { return "OP"; }
};

class PharmacyBill : public Bill {
    double medicineCost;
public:
    PharmacyBill(std::string p, double cost)
        : Bill(std::move(p)), medicineCost(cost) {}
    double total() const override { return medicineCost; }
    std::string type() const override { return "Pharmacy"; }
};

class Payer {
public:
    virtual ~Payer() = default;
    virtual double amountCovered(const Bill& bill) const = 0;
};

class CorporatePayer : public Payer {
    double discountPercent;
public:
    explicit CorporatePayer(double discount) : discountPercent(discount) {}
    double amountCovered(const Bill& bill) const override {
        return bill.total() * (1.0 - discountPercent / 100.0);
    }
};

int main() {
    std::vector<std::unique_ptr<Bill>> bills;
    bills.push_back(std::make_unique<OutPatient>("Ravi", 500, 1200));
    bills.push_back(std::make_unique<PharmacyBill>("Meena", 2500));

    CorporatePayer payer(10.0);
    for (const auto& bill : bills) {
        bill->print();
        std::cout << "Corporate covered: "
                  << payer.amountCovered(*bill) << '\n';
    }
    return 0;
}
