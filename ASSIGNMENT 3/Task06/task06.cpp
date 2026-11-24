#include <cstring>
#include <iostream>

class Text {
    char* data;

public:
    explicit Text(const char* s = "") {
        std::size_t n = std::strlen(s);
        data = new char[n + 1];
        std::memcpy(data, s, n + 1);
    }

    Text(const Text& x) {
        std::size_t n = std::strlen(x.data);
        data = new char[n + 1];
        std::memcpy(data, x.data, n + 1);
    }

    Text& operator=(const Text& x) {
        if (this != &x) {
            std::size_t n = std::strlen(x.data);
            char* fresh = new char[n + 1];
            std::memcpy(fresh, x.data, n + 1);

            delete[] data;
            data = fresh;
        }
        return *this;
    }

    ~Text() {
        delete[] data;
    }

    friend Text operator+(const Text& a, const Text& b) {
        std::size_t na = std::strlen(a.data);
        std::size_t nb = std::strlen(b.data);

        char* temp = new char[na + nb + 1];
        std::memcpy(temp, a.data, na);
        std::memcpy(temp + na, b.data, nb + 1);

        Text result(temp);
        delete[] temp;
        return result;
    }

    bool operator==(const Text& x) const {
        return std::strcmp(data, x.data) == 0;
    }

    bool operator!=(const Text& x) const {
        return !(*this == x);
    }

    friend std::ostream& operator<<(std::ostream& os, const Text& x) {
        return os << x.data;
    }
};

int main() {
    Text a("Hello "), b("World"), empty;
    Text c = a + b;
    Text longMessage("This is a longer message for testing.");

    a = a;

    std::cout << c << '\n'
              << (empty == Text("")) << '\n'
              << longMessage << '\n';
}
