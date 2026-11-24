# C++ Programming – Assignment 5 (CO5)

**Student:** Kathirvelan S V  
**Register number:** 1972260077  
**Department:** CSE with AI  
**Course:** C++ Programming

## Folder structure

- `Task01/` – Runtime polymorphism and hospital billing hierarchy
- `Task02/` – Exception handling, claim approval and rethrowing
- `Task03/` – Virtual-function vs switch benchmark and status vs exception benchmark
- `Task04/` – Corporate payer extension and resource-safe ownership

## Requirements

A C++17 compiler such as GCC/g++.

## Compile and run

Run these commands from the repository root:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic Task01/billing.cpp -o task01
./task01

g++ -std=c++17 -Wall -Wextra -pedantic Task02/exceptions.cpp -o task02
./task02

g++ -std=c++17 -O2 -Wall -Wextra -pedantic Task03/benchmark.cpp -o task03
./task03

g++ -std=c++17 -Wall -Wextra -g -fsanitize=address -fno-omit-frame-pointer Task04/resource_safety.cpp -o task04_asan
./task04_asan
```

On Windows, run the generated `.exe` files instead of `./taskXX`.

## Expected outputs

See `expected_output.txt` in Task01, Task02 and Task04. Task03 reports measured
timings, which vary by computer and must be generated on the student's machine.

## Important submission notes

1. Compile and run every program yourself.
2. Save genuine terminal screenshots of your runs.
3. Record your own Task03 timing results; do not invent or copy measurements.
4. Save the actual AddressSanitizer output for Task04.
5. Add the GitHub repository URL to the assignment cover page.
6. Commit each task separately, for example:

```bash
git add Task01
git commit -m "Complete Task 01 polymorphic billing"

git add Task02
git commit -m "Complete Task 02 exception handling"

git add Task03
git commit -m "Add Task 03 performance benchmarks"

git add Task04
git commit -m "Complete Task 04 resource safety"
```

The PDF asks for a repository named `DSA01-1972260077-Assignment5`.
