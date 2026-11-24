#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <memory>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

struct BillData {
    int type;       // 0=OP, 1=IP, 2=DayCare, 3=Emergency
    double a, b;
    int days;
};

double switchTotal(const BillData& x) {
    switch (x.type) {
        case 0: return x.a + x.b;
        case 1: return x.days * x.a + x.b;
        case 2: return x.a;
        case 3: return x.a + x.b;
        default: throw std::runtime_error("Unknown bill type");
    }
}

class Bill {
public:
    virtual ~Bill() = default;
    virtual double total() const = 0;
};

class OP final : public Bill {
    double a, b;
public:
    OP(double x, double y) : a(x), b(y) {}
    double total() const override { return a + b; }
};
class IP final : public Bill {
    int days;
    double rate, surgery;
public:
    IP(int d, double r, double s) : days(d), rate(r), surgery(s) {}
    double total() const override { return days * rate + surgery; }
};
class DC final : public Bill {
    double price;
public:
    explicit DC(double p) : price(p) {}
    double total() const override { return price; }
};
class EM final : public Bill {
    double base, time;
public:
    EM(double b, double t) : base(b), time(t) {}
    double total() const override { return base + time; }
};

using Clock = std::chrono::steady_clock;

template<class Function>
double timeMilliseconds(Function fn) {
    auto start = Clock::now();
    volatile double result = fn(); // Keep the accumulated result observable.
    auto stop = Clock::now();
    (void)result;
    return std::chrono::duration<double, std::milli>(stop - start).count();
}

void runBillBenchmark(std::size_t n) {
    std::vector<BillData> data;
    std::vector<std::unique_ptr<Bill>> virtualBills;
    data.reserve(n);
    virtualBills.reserve(n);

    for (std::size_t i = 0; i < n; ++i) {
        int type = static_cast<int>(i % 4);
        double a = 100.0 + static_cast<double>(i % 1000);
        double b = 50.0 + static_cast<double>(i % 300);
        int days = 1 + static_cast<int>(i % 10);
        data.push_back({type, a, b, days});

        if (type == 0) virtualBills.push_back(std::make_unique<OP>(a, b));
        else if (type == 1) virtualBills.push_back(std::make_unique<IP>(days, a, b));
        else if (type == 2) virtualBills.push_back(std::make_unique<DC>(a));
        else virtualBills.push_back(std::make_unique<EM>(a, b));
    }

    double virtualSum = 0, switchSum = 0;
    double virtualMs = timeMilliseconds([&] {
        double sum = 0;
        for (const auto& bill : virtualBills) sum += bill->total();
        virtualSum = sum;
        return sum;
    });
    double switchMs = timeMilliseconds([&] {
        double sum = 0;
        for (const auto& bill : data) sum += switchTotal(bill);
        switchSum = sum;
        return sum;
    });

    if (std::abs(virtualSum - switchSum) > 0.001)
        throw std::runtime_error("Virtual and switch totals do not match");

    std::cout << "Bills: " << n
              << " | total: " << virtualSum
              << " | virtual: " << virtualMs << " ms"
              << " | switch: " << switchMs << " ms\n";
}

void runExceptionBenchmark(int invalidPercent) {
    constexpr std::size_t N = 1000000;
    std::vector<double> values(N, 100.0);
    const std::size_t invalidCount = N * invalidPercent / 100;
    for (std::size_t i = 0; i < invalidCount; ++i) values[i] = -1.0;

    volatile std::size_t accepted = 0;
    auto statusMs = timeMilliseconds([&] {
        std::size_t count = 0;
        for (double value : values) {
            if (value >= 0) ++count;
        }
        accepted = count;
        return static_cast<double>(count);
    });

    auto exceptionMs = timeMilliseconds([&] {
        std::size_t count = 0;
        for (double value : values) {
            try {
                if (value < 0) throw std::invalid_argument("negative claim");
                ++count;
            } catch (const std::invalid_argument&) {
                // Rejected claim.
            }
        }
        accepted = count;
        return static_cast<double>(count);
    });

    std::cout << "Invalid claims: " << invalidPercent
              << "% | status: " << statusMs
              << " ms | exceptions: " << exceptionMs << " ms\n";
}

int main() {
    try {
        std::cout << "VIRTUAL FUNCTIONS VS SWITCH\n";
        runBillBenchmark(1000);
        runBillBenchmark(1000000);
        runBillBenchmark(10000000);

        std::cout << "\nSTATUS VS EXCEPTIONS (1,000,000 claims)\n";
        runExceptionBenchmark(0);
        runExceptionBenchmark(1);
        runExceptionBenchmark(50);
    } catch (const std::exception& e) {
        std::cerr << "Benchmark error: " << e.what() << '\n';
        return 1;
    }
    return 0;
}
