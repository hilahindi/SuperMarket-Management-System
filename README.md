# SuperMarket Management System in C

A robust and modular management system for a supermarket, featuring advanced memory management, generic data structures, and bit-level file optimization. This project was developed as a comprehensive systems programming assignment.

## 🛠 Key Technical Features
* **Bit-Level Compression**: Implemented high-efficiency data persistence by compressing product data into specific bit-fields (e.g., 10-bit product counts, 6-bit name lengths) to significantly reduce the file system footprint.
* **Generic Data Structures**: Developed a generic linked list implementation using `void*` and function pointers to manage a modular and sorted shopping cart system.
* **Structural Inheritance**: Simulated object-oriented concepts like inheritance and polymorphism using C structures (e.g., `ClubMember` inheriting from `Customer`) with specialized discount logic.
* **Advanced Memory Management**: Managed complex dynamic heap allocations for arrays and strings, ensuring exact memory sizing and strict prevention of memory leaks.
* **Binary & Text File I/O**: Implemented dual-layer storage using Binary files for inventory and Text files for customer data.

## 💻 Environment & Build
* **IDE**: Optimized for **Visual Studio**.
* **Language**: C .

### Execution Instructions
The program utilizes command-line arguments to toggle between different operating modes:
1.  **Standard Mode**: Run with arguments `0 <filename>` to use standard binary file handling.
2.  **Compressed Mode**: Run with arguments `1 <filename>` to activate the bit-compression engine.

## ⌨️ Menu Features
1. **Inventory Management**: Add, update, and print supermarket products.
2. **Customer Registration**: Manage regular customers and Club Members with loyalty tracking.
3. **Shopping Process**: Real-time cart management with automated inventory updates and payment/cancellation logic.
4. **Search & Sort**: High-performance product lookup using `qsort` and `bsearch` on multiple fields.

---
*Developed as part of the Systems Programming Course.*
