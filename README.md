# Array-Based Library Management System

A minimalist, memory-bound console application for managing library books, users, and transactions. This project was built to demonstrate a deep understanding of core programming fundamentals by adhering to a strict set of technical constraints.

## 🚀 Overview

This system simulates a relational database and transaction engine using **only** fundamental C operations. It completely avoids advanced features like `structs`, pointers, dynamic memory allocation, and file I/O, relying entirely on parallel arrays and linear search algorithms to manage state.

## 🛠️ Technical Constraints & Architecture

* **Language:** C
* **Allowed Features:** 1D/2D Arrays, `for`/`while` loops, `if-else` conditionals, and `<time.h>`.
* **Database Schema (Parallel Arrays):** Data relationships are maintained by aligning indices across multiple arrays. For example, a book's ID at `book_ids[5]` directly corresponds to its title at `book_titles[5]` and its stock count at `book_stock[5]`.
* **Real-Time Tracking:** Uses the standard `<time.h>` library (`time_t`, `difftime()`, and `ctime()`) to generate accurate 14-day checkout deadlines and calculate real-time overdue fines upon return.

## ✨ Features

1. **Add a New Book:** Register a book ID, title, and initial stock into the system.
2. **Register a User:** Add a new user ID and name to the database.
3. **Display Catalog:** View all books currently tracked by the system and their available stock.
4. **Search by ID:** Instantly look up a specific book's details using its unique ID.
5. **Issue a Book (Checkout):** Validates user and book availability, decrements stock, and generates a precise 14-day due date timestamp.
6. **Return a Book:** Clears the active transaction, replenishes stock, and automatically calculates overdue fines based on the exact time elapsed since the due date.
7. **User Dashboard:** Enter a user ID to view a list of all currently borrowed books and their human-readable due dates.

## 💻 Compilation & Usage

Since this project relies purely on standard C libraries, it can be compiled on any machine with a C compiler (like GCC or Clang) without external dependencies.
