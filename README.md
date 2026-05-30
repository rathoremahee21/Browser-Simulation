# Browser Simulation

A console-based browser engine simulation built in C++ to understand how modern browsers work internally.

Instead of focusing only on front-end design, this project explores browser architecture and system design concepts such as request processing, browser rendering flow, caching, DOM parsing, history management, and tab handling using Object-Oriented Programming (OOP).

The project simulates how a browser processes a request — from URL input to rendering output — while modelling browser components through modular and layered system design.

---

## Why This Project?

Modern browsers such as Chrome and Firefox are complex systems containing multiple interacting layers such as:

* Browser Engine
* Rendering Pipeline
* Cache Management
* Page Parsing
* History Navigation
* Tab Management
* Request Processing

This project was created to better understand these internal workflows through hands-on implementation using C++ and OOP principles.

Rather than building only a browser-like interface, the goal was to simulate the internal behaviour and architecture of a browser.

---

## Browser Request Pipeline

The following diagram represents the browser workflow explored in this project:

```text
User enters URL
        │
        ▼
 Request Processing
        │
        ▼
 Browser Engine
        │
        ▼
 Cache Lookup
    ┌──────────────┐
    │              │
    ▼              ▼
 Cache Hit     Cache Miss
    │              │
    │         Fetch from
    │        Page Database
    └──────┬───────┘
           ▼
      DOM Parsing
           │
           ▼
   HTML Rendering Layer
           │
           ▼
      WebPage Factory
           │
           ▼
     Render Final Page
```

---

## System Architecture

```text
main.cpp
   │
   ▼
Browser Engine
   │
   ├── UserManager
   │      └── Authentication + File Handling
   │
   ├── RequestPipeline
   │      ├── Cache (LRU-style simulation)
   │      └── PageDatabase
   │
   ├── DOMParser
   ├── HTMLRenderer
   │
   ├── WebPage Factory
   │      ├── NewsPage
   │      ├── SportsPage
   │      ├── ShopPage
   │      ├── CalendarPage
   │      ├── QuotesPage
   │      └── GamePage
   │
   └── Tab & History Management
          ├── Circular Doubly Linked List
          └── Doubly Linked List
```

---

## Key Features

* Console-based browser simulation
* Browser request pipeline modelling
* Cache hit / cache miss mechanism
* Browser rendering flow simulation
* Multi-tab browser support
* Back and forward history navigation
* User authentication (signup/login using file handling)
* DOM parsing and rendering simulation
* Interactive browser pages (News, Shop, Sports, Quotes, Calendar, Game)
* Themed console output system

---

## Browser Concepts Explored

This project focuses on understanding how browsers work internally, including:

* Browser Engine workflow
* Rendering pipeline
* Cache systems
* Request processing lifecycle
* DOM parsing and rendering
* Browser history management
* Multi-tab architecture
* Layered software architecture

---

## OOP Concepts Implemented

| Concept        | Implementation                             |
| -------------- | ------------------------------------------ |
| Abstraction    | `WebPage` abstract class                   |
| Inheritance    | `NewsPage`, `SportsPage`, `ShopPage`, etc. |
| Polymorphism   | Runtime rendering using `render()`         |
| Encapsulation  | Cache, HistoryManager, UserManager         |
| Templates      | `Tab<T>` implementation                    |
| Factory Method | Dynamic page creation                      |
| Composition    | Browser built using multiple subsystems    |

---

## Data Structures Used

* `std::map` → Page database and cache lookup
* Doubly Linked List → Browser history navigation
* Circular Doubly Linked List → Multi-tab management

---

## Technologies Used

* C++17
* Object-Oriented Programming (OOP)
* File Handling
* Data Structures
* HTML/CSS/JavaScript (front-end simulation)

---

## Project Goals

The objective of this project was to:

* Understand browser internals through implementation
* Apply Object-Oriented Programming to a real-world system
* Explore layered architecture and modular software design
* Simulate browser request and rendering behaviour

---

## How to Run

### Clone the repository

```bash
git clone https://github.com/rathoremahee21/Browser-Simulation.git
```

### Run the project

1. Open the project in VS Code or CodeBlocks
2. Compile using a C++17 compiler (`g++`)
3. Run the executable

Example:

```bash
g++ main.cpp -o browser
./browser
```

---

## Current Limitations

* No real internet access (predefined pages only)
* Windows-targeted implementation
* Passwords stored in plaintext (for academic simulation)
* No CSS/JS rendering engine implementation
* No multimedia support

---

## Future Improvements

* Real HTTP request handling
* Better browser engine simulation
* Secure authentication with hashing
* Cross-platform compatibility
* GUI-based browser version
* Enhanced rendering pipeline simulation

---

## Learning Outcomes

Through this project, I gained practical exposure to:

* Browser architecture fundamentals
* OOP-based system design
* Data structures in real applications
* Modular software engineering
* Cache and request lifecycle concepts

---

## Author
**Anushka Lal**
**Mahee Rathore**


GitHub: rathoremahee21
