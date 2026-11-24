# Assignment 4 – Written Solutions and Sample Outputs

> Sample output is based on the programs in this repository. Compiler diagnostics and sanitizer reports can vary by compiler/platform. Run each program locally and capture your own screenshots as required by the assignment.

## Task 1 – Class design review

### 1.1 Deficiencies
1. `name`, `id`, `basic` and some other fields are duplicated.
2. Data members are public, so invalid values can be assigned directly.
3. `name`/`nm` and `id`/`code` use inconsistent names.
4. `da` is declared but never used.
5. `0.42` is a magic number with no named policy constant.
6. A new category requires duplicated code and more maintenance.
7. The unrelated classes cannot be processed uniformly through one `Staff*` collection.
8. `GuestFaculty` has a different pay basis (hours × rate), so the common base should store only genuinely common information.

### 1.2 Hierarchy / UML
```text
                 Staff (abstract)
          name, id, basic [protected]
                 + pay() = 0
                 /      |       \
                /       |        \
        Professor   LabAssistant  GuestFaculty
        research       (none)      hours, rate
```
`pay()` is virtual and overridden in every concrete class. Public inheritance models an is-a relationship. Data is protected/private rather than public.

### 1.3 Design defence (under 100 words)
The base class stores common staff information and defines the polymorphic `pay()` interface. Each derived class implements only its own payment rule. A new category can be introduced by deriving from `Staff` and overriding `pay()`. Existing code that processes `Staff` pointers does not need a change, so the design is easier to extend and reduces duplicated data and logic.

### Alternative: one class plus a type/switch
A single class would need fields for every category (research, hours, hourly rate, etc.) and a category code. `pay()` would switch on that code. Both approaches take O(1) time per pay calculation. The single class can waste per-object space because irrelevant fields remain allocated. Adding a category changes the class fields, constructor/validation logic, switch, and possibly callers/tests. With polymorphism, a new derived class can be added without editing existing derived classes or the processing loop.

### Sample output
```text
Anita (101): 79000
Bala (102): 42600
Charan (103): 32000
```

## Task 2 – From requirement to hierarchy

### 2.1 Relationships
- `Student is-a Person`: public single inheritance.
- `Staff is-a Person`: public single inheritance.
- `TeachingAssistant is-a Student` and `is-a Staff`: public multiple inheritance; together with the shared Person base this is hybrid inheritance.
- `ResearchScholar is-a Student`: public multilevel inheritance.
- `Person has-a Address` and `Student has-a Course` would be composition/member-class relationships if those details are included; they are not inheritance.

### 2.2 Diamond problem
Without virtual inheritance, `TeachingAssistant` contains one `Person` through `Student` and another through `Staff`. This duplicates the base state and makes expressions such as `ta.name` ambiguous. Declaring both intermediate bases as `virtual public Person` gives the most-derived `TeachingAssistant` one shared `Person` subobject. The most-derived constructor initializes that virtual base.

### 2.3 Class declarations
The declarations are implemented in `Task02/Task02.cpp`. `Person` is inherited publicly by `Student` and `Staff`; `TeachingAssistant` inherits publicly from both; `ResearchScholar` inherits publicly from `Student`. Public inheritance preserves the public interface of an is-a relationship. A `TeachingAssistant` constructor explicitly initializes `Person`.

### 2.4 Access table (Person member → visibility in Student)
| Person member | public inheritance | protected inheritance | private inheritance |
|---|---|---|---|
| public | public | protected | private |
| protected | protected | protected | private |
| private | not directly accessible | not directly accessible | not directly accessible |

### Predict-the-output question
For the exact short program in the PDF:
```text
Person Student Staff TA
```
The virtual base `Person` is constructed once, before the non-virtual direct bases. Then `Student`, `Staff`, and finally `TeachingAssistant` are constructed. If `virtual` is removed from both intermediate bases, two `Person` subobjects are constructed:
```text
Person Student Person Staff TA
```
The unqualified `ta.name` then becomes ambiguous if used. The compiler generally does not issue an error merely for constructing the object; ambiguity is diagnosed when the ambiguous member is accessed.

### Composition alternative
If `TeachingAssistant` contains `Student student;` and `Staff staff;`, it has-a Student and has-a Staff rather than being substitutable for them. Since each member object contains its own Person base, there are normally two Person subobjects. Access is explicit (`ta.student.name` or `ta.staff.name`) and not ambiguous, but state can become inconsistent and a TA cannot be passed as a `Student*` or `Staff*`. `sizeof` is implementation-dependent; measure it with `sizeof(TeachingAssistant)` on the target compiler.

### Sample output from Task02.cpp
```text
Person Student Staff TA
Kavya CSE AI
```

## Task 3 – Constructors and member classes

### 3.1 Base constructor arguments
A derived constructor passes base values in its initializer list, e.g. `Student(...) : Person(name, id, phone), ...`. In virtual inheritance, only the most-derived class is responsible for initializing the virtual base. When a `Student` is constructed alone, `Student` initializes `Person`; when a `TeachingAssistant` is constructed, `TeachingAssistant` initializes the shared `Person`.

### 3.2 Member classes
`Person` can contain `Address address;` and `Student` can contain `Course course;`. Initialize them in the member-initializer list, e.g. `Person(...): name(n), address(city,pin) {}` and `Student(...): Person(...), course(code,title) {}`. Members are constructed in declaration order, not the order written in the initializer list.

### 3.3 Construction/destruction rule
Construction: virtual bases, direct non-virtual bases in declaration order, data members in declaration order, then the constructor body. Destruction is the reverse: derived destructor body, members in reverse declaration order, non-virtual bases in reverse order, then virtual bases.

### Predict-the-output question (Car)
```text
Vehicle Engine Car | ~Car ~Engine ~Vehicle
```
`Vehicle` is the base and is constructed first. `Engine e` is a member and is constructed next. Then the `Car` constructor body runs. On leaving the block, destruction reverses that order.

### Complete Code TODO 1
The implemented constructor is:
```cpp
Student(const std::string& n, int a, long long r,
        const std::string& c, int p)
    : Person(n, a), regNo(r), home(c, p) {}
```
Expected output:
```text
192311001 Kavya (19) Salem-636001
```
If initializer-list entries are written in an order different from member declaration order, the compiler still initializes in declaration order and `-Wall -Wextra` can warn with `-Wreorder`.

### Pointer alternative for Address
A raw pointer version would allocate with `new Address(...)`, store `Address*`, delete it in the destructor, and implement a deep-copy constructor/assignment operator to avoid two objects owning the same pointer. Allocation adds O(1) work and a pointer-sized field; the Address object is separately allocated. Prefer `std::unique_ptr<Address>` for automatic ownership, or a direct member object when optional/dynamic lifetime is unnecessary.

### Sample output from Task03.cpp
```text
Vehicle Engine Car | ~Car ~Engine ~Vehicle
192311001 Kavya (19) Salem-636001
```

## Task 4 – Pointers to derived classes

### 4.1 Defects
- `rate()` is non-virtual, so `Account*` calls `Account::rate()` (3.5), even when pointing to `SeniorAccount`.
- Deleting a derived object through `Account*` when the base destructor is non-virtual is undefined behaviour; the derived destructor may not run, leaking `history`.
- `Account copy = SeniorAccount();` slices the derived object: only the Account base subobject is copied, so derived state and dynamic type are lost.

### 4.2 / 4.3 Repair
Make the polymorphic operation virtual and override it in derived classes. Give a polymorphic base a virtual destructor. Store polymorphic objects through pointers/references, preferably `std::unique_ptr<Account>` in a vector. Use `override` to catch signature mistakes. The code in `Task04.cpp` demonstrates this repair and slicing.

An AddressSanitizer run can be attempted with:
```bash
g++ -std=c++17 -Wall -Wextra -fsanitize=address -fno-omit-frame-pointer Task04/Task04.cpp -o task04
./task04
```
Exact sanitizer output depends on the compiler/runtime. The repaired program should not report the original deletion-through-base leak.

### 4.4 Prevention rules
1. A base class intended for polymorphic deletion should have a virtual destructor.
2. Use `override` on derived virtual functions and use pointers/references (or smart pointers) for runtime polymorphism rather than storing derived objects by base value.

### Predict-the-output question
For the PDF's exact program:
```text
3.5 4 3.5
```
`p->rate()` is statically bound because `rate()` is non-virtual, so it calls `Account::rate()`. `p->vrate()` is virtual and dynamically dispatches to `SeniorAccount::vrate()`. `copy.vrate()` calls the base implementation because `copy` is a sliced standalone `Account` object.

### Smart pointer alternative / complexity
A `vector<unique_ptr<Account>>` processes n accounts in O(n) total time; each virtual call is O(1). The vector stores one smart pointer per object, while each allocated derived object stores its own fields and typically a vptr. Smart pointers prevent forgotten manual `delete` and avoid slicing when used polymorphically. They do not automatically fix a non-virtual function (dispatch remains static), nor do they prevent a programmer from explicitly making a sliced base copy.

### Sample output from Task04.cpp
```text
3.5 4 3.5
3.5
4
SeniorAccount destroyed
SeniorAccount destroyed
```

## Task 5 – Pointers to objects and this

### Four ways
- Direct: `Cart c; c.add(...)`.
- Pointer to existing object: `Cart* p = &c; p->total()`.
- Reference: `Cart& r = c; r.total()`.
- Dynamic: `Cart* p = new Cart; ...; delete p;` (prefer smart pointers in production).

`this` is the pointer to the current object inside a non-static member function. `*this` is that object as an lvalue reference. Returning `Cart&` enables chaining without copying. `this->discount` explicitly selects the data member when the parameter has the same name.

### Complete Code TODOs
```cpp
Cart& add(const std::string& item, double price) {
    if (n < 10) {
        items[n] = item;
        prices[n] = price;
        ++n;
    }
    return *this;
}

Cart& setDiscount(double discount) {
    this->discount = discount;
    return *this;
}
```
Expected output:
```text
324.00
```
Returning `Cart` by value would return a copy. Chained calls would operate on temporary copies after the first call, so changes would not necessarily affect the original cart; it also incurs copying (or move) overhead.

### Highest-total pointer array
Store `Cart* carts[]`, scan with indexing or pointer arithmetic, and keep the index/pointer with the largest `total()`. This takes O(n) time and O(1) extra search space. With `vector<Cart>`, the objects are stored directly and there is no separate allocation per object; however, vector reallocation can invalidate pointers/references/iterators into its elements. A vector of pointers stores pointer values and keeps separately allocated objects at stable addresses (subject to ownership/lifetime management).

### Sample output from Task05.cpp
```text
Dynamic cart total: 40
324.00
Highest cart total: 324.00
```

## Task 6 – Vehicle rental fleet

### Stage 1: testable requirements
1. Every vehicle has a registration number and daily rate.
2. The fleet can list every vehicle through a common interface.
3. A vehicle can be rented only if it is not already rented.
4. Returning a vehicle changes its state to available.
5. Each concrete vehicle calculates its charge using its own rule.
6. Motor vehicles store fuel/charge level and service due date; cars store seating capacity; electric cars store battery capacity and use an electricity-unit rate.
7. A new vehicle type can be added without editing existing vehicle classes or the fleet-processing loop.

### Stage 2: hierarchy
```text
                 Vehicle (abstract)
       registration, dailyRate, rented
       charge(days)=0, list()=0, type()=0
             /         |          \
        Bicycle    MotorVehicle    ElectricBus
                   /      |   \
             TwoWheeler  Car
                           |
                       ElectricCar
```
`MotorVehicle` is an abstract intermediate class in this sample. Inheritance is public/is-a. A registration or service-date helper could be a member class if a richer date/address type were required.

### Stage 3: decisions
Common identity, rate and rented state belong in `Vehicle`. `charge`, `list`, and `type` are virtual; `charge` is pure virtual. The fleet is `vector<unique_ptr<Vehicle>>`, which provides base-pointer polymorphism and automatic cleanup. The task specifically mentions an array of base pointers; the vector is a safer dynamically sized equivalent.

### Stage 4: pseudocode
```text
RENT(vehicle)
    IF vehicle is already rented
        RETURN failure
    SET rented = true
    RETURN success

RETURN_VEHICLE(vehicle, lateDays)
    SET rented = false
    lateFee = vehicle.charge(1) * lateDays * lateFeeRate
    RETURN lateFee
```

### Stage 5: demonstration
`Task06.cpp` creates a bicycle, two-wheeler, car, electric car and a newly added electric bus. All are listed and rented using the same base interface. The electric bus is added by creating a new derived class and adding one object to the fleet; the existing classes and loop remain unchanged.

The sample program uses a clearly stated illustrative policy: late fee is 25% of one day's charge per late day. The electric-car and electric-bus formulas are illustrative assumptions because the assignment does not specify exact rates/formulas for electricity or late returns. Replace them if your faculty provides a different rule.

### Alternative: one Vehicle class with a type code
`charge()` would switch on a type code and use fields for all vehicle categories. Both a switch and virtual dispatch are O(1) per charge; processing n vehicles is O(n). A single class wastes space on fields unused by many vehicle types, while polymorphic objects carry a vptr (implementation detail). Adding a new type requires editing the switch and the shared class, whereas the hierarchy adds a new derived class and leaves existing code unchanged.

### Sample output from Task06.cpp
```text
Bicycle B-101 rate=100
TwoWheeler T-202 fuel=75 service=2026-12-01
Car C-303 seats=5 fuel=60
ElectricCar E-404 seats=5 battery=60 kWh
ElectricBus EB-505 seats=30 battery=300 kWh
Bicycle rented; charge for 2 days = 200.00
TwoWheeler rented; charge for 2 days = 500.00
Car rented; charge for 2 days = 2400.00
ElectricCar rented; charge for 2 days = 3048.00
ElectricBus rented; charge for 2 days = 6015.00
Late return fee for Car (2 days late) = 600.00
```

## Important submission note
The assignment asks you to predict certain outputs before executing the programs and submit screenshots and sanitizer evidence. Do not claim that a prediction was made before running if it was not. Compile and run the supplied files on your own g++ installation, record its version, and add genuine terminal screenshots to your GitHub repository.
