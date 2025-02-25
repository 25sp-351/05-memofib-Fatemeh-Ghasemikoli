# **Testing Memoized Fibonacci Program**

##  Overview**
This document outlines the test cases and expected behaviors of the **memoized Fibonacci program** that utilizes **GMP (`mpz_t`)** for large number computations and a **cache system (`libmemoize.so`)** for memoization.
The program will run 200 test with max fibunacci index of 150.
---

## **Functional Testing**
### **Run Without Memoization**
#### **Command:**
```bash
./main
```
#### **Expected Behavior:**
- Every Fibonacci number is computed **from scratch**.
- No cache statistics should be displayed.
- The program should correctly print Fibonacci numbers for the generated test cases.
- Example output:
  ```
  Starting test 5: 83
  Completed test 5: 83rd Fibonacci number is 99194853094755497
  ```

---

### ** Run With Memoization**
#### **Command:**
```bash
./main libmemoize.so
```
#### **Expected Behavior:**
- **The cache system should be used**, reducing redundant calculations.
- Fibonacci numbers should still be correctly computed.
- The program should print **cache statistics** at the end:
  ```
  Cache Statistics:
  requests   (1)   74
  hits       (2)   45
  misses     (3)   29
  size       (5)  151
  ```
- Hits should increase over multiple runs as more values get cached.

---

## ** Edge Cases & Special Cases**

### ** Edge Cases (Small Values)**
| Test Case | Expected Output |
|-----------|----------------|
| `Fibonacci(0)`  | `0` |
| `Fibonacci(1)`  | `1` |
| `Fibonacci(2)`  | `1` |
| `Fibonacci(3)`  | `2` |

#### **Command:**
```bash
./main libmemoize.so
```
#### **Expected Behavior:**
- Program should correctly compute small Fibonacci numbers.
- Small values should quickly be cached.

---

### ** Edge Cases (Large Values)**
| Test Case | Expected Output |
|-----------|----------------|
| `Fibonacci(50)`  | `12586269025` |
| `Fibonacci(75)`  | `2111485077978050` |
| `Fibonacci(100)` | `354224848179261915075` |

#### **Command:**
```bash
./main libmemoize.so
```
#### **Expected Behavior:**
- Large Fibonacci numbers should be **computed correctly using GMP**.
- If previously computed, they should be retrieved from the cache.


---

The program is correctly **storing, retrieving, and using cached Fibonacci numbers**, ensuring efficiency in repeated computations.



