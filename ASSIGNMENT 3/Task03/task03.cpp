#include <iostream>
#include <algorithm>

class Event {
    int rows, seats;
    char** map;
    int booked = 0;

public:
    Event(int r, int s)
        : rows(std::max(0, r)), seats(std::max(0, s)), map(nullptr) {
        if (rows > 0 && seats > 0) {
            map = new char*[rows];
            int made = 0;
            try {
                for (; made < rows; ++made) {
                    map[made] = new char[seats];
                    std::fill(map[made], map[made] + seats, 'O');
                }
            } catch (...) {
                while (made > 0) delete[] map[--made];
                delete[] map;
                map = nullptr;
                throw;
            }
        }
        std::cout << "Event created: " << rows << " x " << seats << '\n';
    }

    bool book(int r, int s) {
        if (!map || r < 0 || r >= rows || s < 0 || s >= seats ||
            map[r][s] == 'X')
            return false;

        map[r][s] = 'X';
        ++booked;
        return true;
    }

    bool cancel(int r, int s) {
        if (!map || r < 0 || r >= rows || s < 0 || s >= seats ||
            map[r][s] == 'O')
            return false;

        map[r][s] = 'O';
        --booked;
        return true;
    }

    ~Event() {
        if (map) {
            for (int i = 0; i < rows; ++i)
                delete[] map[i];
            delete[] map;
        }
        std::cout << "Event closed; booked seats at closing: "
                  << booked << '\n';
    }
};

int main() {
    Event local(2, 3);
    local.book(0, 1);

    {
        Event blockEvent(1, 2);
        blockEvent.book(0, 0);
    }

    Event* heapEvent = new Event(2, 2);
    heapEvent->book(1, 1);
    delete heapEvent;

    Event zero(0, 5);
}
