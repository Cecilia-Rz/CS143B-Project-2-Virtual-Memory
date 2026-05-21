# CS143B-Project-2-Virtual-Memory-Management
## Author
Cecilia Rodriguez

---

# Compilation

Compile the program using:

```bash
g++ -std=c++17 main.cpp VMManager.cpp -o vm
```

---

# Execution
Execution requires the files to be stated in the following order:

```bash
./vm "init text file" "input text file" "output text file"
```
Examples Below

## Without Demand Paging

Run:

```bash
./vm init-no-dp.txt input-no-dp.txt output-no-dp.txt
```

---

## With Demand Paging

Run:

```bash
./vm init-dp.txt input-dp.txt output-dp.txt
```

---

# Files Included

- `main.cpp`
- `VMManager.cpp`
- `VMManager.h`

---

# Output

The program writes translated physical addresses to:
- `output-no-dp.txt`
- or `output-dp.txt`

Invalid virtual addresses are written as:

```text
-1
```
