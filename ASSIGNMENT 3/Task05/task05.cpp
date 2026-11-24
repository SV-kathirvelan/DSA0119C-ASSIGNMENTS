#include <iostream>
#include <stdexcept>
#include <string>

class Time {
    int seconds;

public:
    explicit Time(int s = 0) : seconds(s) {}

    Time& operator++() {
        ++seconds;
        return *this;
    }

    Time operator++(int) {
        Time old = *this;
        ++seconds;
        return old;
    }

    bool operator==(const Time& x) const {
        return seconds == x.seconds;
    }

    bool operator<(const Time& x) const {
        return seconds < x.seconds;
    }

    int value() const {
        return seconds;
    }
};

class Playlist {
    std::string* tracks;
    int size;

public:
    explicit Playlist(int n)
        : tracks(n > 0 ? new std::string[n] : nullptr),
          size(n > 0 ? n : 0) {}

    ~Playlist() {
        delete[] tracks;
    }

    std::string& operator[](int i) {
        if (i < 0 || i >= size)
            throw std::out_of_range("Playlist index out of range");
        return tracks[i];
    }

    const std::string& operator[](int i) const {
        if (i < 0 || i >= size)
            throw std::out_of_range("Playlist index out of range");
        return tracks[i];
    }
};

int main() {
    Time t(10);
    Time a = t++;
    Time b = ++t;

    std::cout << a.value() << ' '
              << b.value() << ' '
              << t.value() << '\n';

    Playlist p(2);
    p[0] = "Track One";
    p[1] = "Track Two";

    std::cout << p[1] << '\n';
}
