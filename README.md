### PERSONAL FINANCE TRACKER

A simple yet powerful **Console-Based Personal Finance Tracker** built in C++
It tracks income, expenses, and in the future a budgets tracker - all in one using the terminal!

-------------------------------------------------------------------------

## OVERVIEW

A **fully functional finance tracker** that allows users to:

- Add transactions (income or expenses)
- View all transactions
- Search by categories, description, or date
- Save/Load transactions data from "data.txt" file
- See total balance at a glance

All built entirely with C++ with OOP, FileIO

-------------------------------------------------------------------------

## FEATURES

| Feature | Description |
| ------- | ----------- |
| **Add Transactions** | Record income or expense with amount, category, description, and date |
| **View Transactions** | List all saved transactions with formatted output |
| **Search Transactions** | Filter transactions by category, description, or date |
| **Save & Load** | Data presists across sessions using text file storage |

-------------------------------------------------------------------------

## TECHNOLOGIES USED

- **C++17+**
- **Standard Template Library (STL)**: `vector`, `string`, `fstream`, `iostream`
- **Object-Oriented Programming (OOP)**: Structured around a `Transaction` class
- **File Storage**: Plain `.txt` file for presistent data

-------------------------------------------------------------------------

## GETTING STARTED

# PREREQUISITES

- **C++ Compiler**: `g++`, `clang++`, `MSVC`
- **Terminal or command-line access**

# BUILD INSTRUCTIONS

To compile and run the app:

`g++ src/main.cpp include/finance.hpp include/finance.cpp -o finance`
`./finance`
