#include <iostream>
#include <stdexcept>
#include <string>

class Book {
    std::string title;
    int copies;
public:
    Book() : Book("", 0) {}
    explicit Book(const std::string& t) : Book(t, 0) {}
    Book(const std::string& t, int c) : title(t), copies(c) {
        if (c < 0) throw std::invalid_argument("Copies cannot be negative");
    }

    void print() const {
        std::cout << "Title: " << (title.empty() ? "(empty)" : title)
                  << ", Copies: " << copies << '\n';
    }
};

int main() {
    Book empty;
    Book donated("C++ Basics");
    Book full("Data Structures", 5);

    empty.print();
    donated.print();
    full.print();

    try {
        Book invalid("Invalid Book", -2);
    } catch (const std::invalid_argument& e) {
        std::cout << e.what() << '\n';
    }
}
