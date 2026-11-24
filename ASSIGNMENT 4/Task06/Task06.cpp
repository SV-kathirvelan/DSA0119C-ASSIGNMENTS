#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
using namespace std;

class Vehicle {
protected:
    string registration;
    double dailyRate;
    bool rented = false;
public:
    Vehicle(string reg, double rate) : registration(move(reg)), dailyRate(rate) {}
    virtual double charge(int days) const = 0;
    virtual void list() const = 0;
    virtual string type() const = 0;
    virtual ~Vehicle() = default;
    bool rent() {
        if (rented) return false;
        rented = true;
        return true;
    }
    double returnVehicle(int daysLate = 0) {
        rented = false;
        return charge(1) * daysLate * 0.25; // late fee: 25% of one daily charge/day
    }
    bool isRented() const { return rented; }
};

class Bicycle : public Vehicle {
public:
    using Vehicle::Vehicle;
    double charge(int days) const override { return dailyRate * days; }
    string type() const override { return "Bicycle"; }
    void list() const override { cout << type() << " " << registration << " rate=" << dailyRate << '\n'; }
};

class MotorVehicle : public Vehicle {
protected:
    double fuelOrCharge;
    string serviceDue;
    MotorVehicle(string reg, double rate, double level, string due)
        : Vehicle(move(reg), rate), fuelOrCharge(level), serviceDue(move(due)) {}
};

class TwoWheeler : public MotorVehicle {
public:
    TwoWheeler(string r, double rate, double fuel, string due)
        : MotorVehicle(move(r), rate, fuel, move(due)) {}
    double charge(int days) const override { return dailyRate * days; }
    string type() const override { return "TwoWheeler"; }
    void list() const override { cout << type() << " " << registration << " fuel=" << fuelOrCharge << " service=" << serviceDue << '\n'; }
};

class Car : public MotorVehicle {
protected:
    int seats;
public:
    Car(string r, double rate, double fuel, string due, int s)
        : MotorVehicle(move(r), rate, fuel, move(due)), seats(s) {}
    double charge(int days) const override { return dailyRate * days; }
    string type() const override { return "Car"; }
    void list() const override { cout << type() << " " << registration << " seats=" << seats << " fuel=" << fuelOrCharge << '\n'; }
};

class ElectricCar : public Car {
    double batteryKWh;
    double kWhRate;
public:
    ElectricCar(string r, double rate, double battery, string due, int seats, double unitRate)
        : Car(move(r), rate, 100, move(due), seats), batteryKWh(battery), kWhRate(unitRate) {}
    double charge(int days) const override {
        return dailyRate * days + batteryKWh * kWhRate * 0.10;
    }
    string type() const override { return "ElectricCar"; }
    void list() const override { cout << type() << " " << registration << " seats=" << seats << " battery=" << batteryKWh << " kWh\n"; }
};

// New type added without modifying the existing hierarchy.
class ElectricBus : public Vehicle {
    int seats;
    double batteryKWh;
public:
    ElectricBus(string r, double rate, int s, double battery)
        : Vehicle(move(r), rate), seats(s), batteryKWh(battery) {}
    double charge(int days) const override { return dailyRate * days + batteryKWh * 0.05; }
    string type() const override { return "ElectricBus"; }
    void list() const override { cout << type() << " " << registration << " seats=" << seats << " battery=" << batteryKWh << " kWh\n"; }
};

int main() {
    vector<unique_ptr<Vehicle>> fleet;
    fleet.push_back(make_unique<Bicycle>("B-101", 100));
    fleet.push_back(make_unique<TwoWheeler>("T-202", 250, 75, "2026-12-01"));
    fleet.push_back(make_unique<Car>("C-303", 1200, 60, "2026-12-10", 5));
    fleet.push_back(make_unique<ElectricCar>("E-404", 1500, 60, "2026-12-15", 5, 8));
    fleet.push_back(make_unique<ElectricBus>("EB-505", 3000, 30, 300));

    cout << fixed << setprecision(2);
    for (const auto& v : fleet) v->list();

    for (auto& v : fleet) {
        if (v->rent()) cout << v->type() << " rented; charge for 2 days = " << v->charge(2) << '\n';
    }
    cout << "Late return fee for " << fleet[2]->type() << " (2 days late) = "
         << fleet[2]->returnVehicle(2) << '\n';
}
