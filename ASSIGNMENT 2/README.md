# Assignment 2 – C++ (CO2)

**Student name:** Kathirvelan S V  
**Register number:** 1972260077  
**Compiler:** g++ (record the exact version installed on your computer using `g++ --version`)  
**Compile command:** `g++ -std=c++17 -Wall -Wextra Qn.cpp -o Qn`  
**Run:** `./Qn` (Windows: `Qn.exe`)

## Folder structure
- `Task01` – call by value/reference/pointer, return by reference, default arguments
- `Task02` – return by reference, const reference, copy; dangling-reference example explained
- `Task03` – default arguments and function overloading
- `Task04` – class with transaction array
- `Task05` – static members, array of objects, bus tickets
- `Task06` – friend functions and inline discussion
- `Task07` – redesign: shifting history versus circular buffer

Each task folder contains its `.cpp` source and `output.txt` captured by compiling and running the program.

## Task notes
1. **Q1:** `a` is passed by value and does not change in the caller; `b` is a reference and `c` is accessed through a pointer, so both caller variables change. `bigger()` returns a reference to the larger argument, so assignment changes that original variable. `show()` uses default values for omitted arguments. The fee is 45000 after call-by-value, 40500 after a 10% reference concession, then 34425 after a 15% pointer concession.
2. **Q2:** Returning `int&` to an array element allows the caller to modify it. A `const int&` prevents modification through that reference; returning `int` gives a copy. Returning a reference to a local variable is invalid because the local object is destroyed when the function returns (dangling reference / undefined behaviour); the unsafe function is shown commented out and is not executed.
3. **Q3:** The ambiguous original overload set is avoided by using one `double` function with default arguments, an `int` overload for grams, and a `Parcel` overload. `charge(2)` selects the exact `int` overload; floating-point input selects the `double` overload.
4. **Q4:** The class keeps data private. When more than 10 transactions arrive, it shifts the 9 newer entries left and appends the latest transaction. Shifting costs O(k) per full-buffer insertion; a circular buffer or deque can add in O(1). Storage is O(10), or O(k) for capacity k.
5. **Q5:** `Token::next` and `BusTicket::issued` are static and each has one out-of-class definition. The first three tickets receive seats; the fourth is waiting. The static count persists across object creation and function calls for the lifetime of the program.
6. **Q6:** Friend functions can access private hour/minute fields and compare two objects symmetrically. Both functions run in O(1). `inline` is a request/ODR facility, not a guarantee that the compiler substitutes the function body. Optimisation settings can affect timing.
7. **Q7:** The original shifting design is retained as a baseline and compared with a fixed-size circular buffer. Both use O(k) memory. Insertion is O(k) for shifting when full and O(1) for circular indexing. Runtime numbers vary by compiler, optimisation and hardware; run the benchmark on your own computer and record your result.

## Important before submission
- The Q7 timing output is machine-dependent; execute it on your own system and use your own measured values in your report.
- The assignment asks for a prediction written **before** running each predict-output task. Write your prediction separately before execution; do not present the captured output as a pre-run prediction.
- Add screenshots of your own terminal executions to each task folder if required by your faculty.
- Commit each task separately to show progressive Git history.
