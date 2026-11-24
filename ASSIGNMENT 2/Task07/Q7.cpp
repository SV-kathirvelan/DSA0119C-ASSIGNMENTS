#include <iostream>
#include <chrono>
#include <vector>
using namespace std;
using Clock = chrono::steady_clock;

class ShiftingHistory {
    static const int K = 10;
    int data[K]{};
    int count = 0;
public:
    void add(int value) {
        if (count < K) data[count++] = value;
        else {
            for (int i = 1; i < K; ++i) data[i - 1] = data[i];
            data[K - 1] = value;
        }
    }
    int latest() const { return count ? data[count - 1] : -1; }
};

class CircularHistory {
    static const int K = 10;
    int data[K]{};
    int next = 0;
    int count = 0;
public:
    void add(int value) {
        data[next] = value;
        next = (next + 1) % K;
        if (count < K) ++count;
    }
    int latest() const {
        return count ? data[(next + K - 1) % K] : -1;
    }
};

int main() {
    const int N = 1000000;
    ShiftingHistory shifting;
    CircularHistory circular;

    auto start1 = Clock::now();
    for (int i = 0; i < N; ++i) shifting.add(i);
    auto end1 = Clock::now();

    auto start2 = Clock::now();
    for (int i = 0; i < N; ++i) circular.add(i);
    auto end2 = Clock::now();

    auto shiftUs = chrono::duration_cast<chrono::microseconds>(end1 - start1).count();
    auto ringUs = chrono::duration_cast<chrono::microseconds>(end2 - start2).count();

    cout << "Operations per design: " << N << '\n';
    cout << "Latest value (shifting): " << shifting.latest() << '\n';
    cout << "Latest value (circular): " << circular.latest() << '\n';
    cout << "Shifting time (microseconds): " << shiftUs << '\n';
    cout << "Circular time (microseconds): " << ringUs << '\n';
    cout << "Both designs store at most 10 integers.\n";
}