# Student Record Management System

A menu-driven **C++ console application** built for our Data Structures and Algorithms (DSA) course project. It manages student records for a small college — adding, updating, deleting, searching, sorting, and tracking semester-wise academic performance.

This README explains **what** the project does, **how** it's built, and **why** each decision was made — so every team member can understand the whole system, not just their own module.

---

## 1. What This Project Is

A single C++ program that:
- Logs in an admin using a username/password stored in a file
- Stores student records (personal info + semester-wise GPA history) in memory using a `vector`
- Lets the admin add, update, delete, search, sort, and view students through a text menu
- Saves everything to a plain text file (`students.txt`) so data isn't lost when the program closes

It is **not** a website, app, or database system — it's a terminal program, on purpose (see [Section 6](#6-why-no-database-web-tech-or-gui)).

---

## 2. Why We Built It This Way

Our assignment asked for a project that:
- Demonstrates real DSA concepts (searching, sorting, OOP, file handling)
- Is realistic for 5 students to build and explain in a semester
- Doesn't look over-engineered or "enterprise" — just solid, understandable code

So every choice below was made to keep things **simple and explainable in a viva**, not to add extra features for their own sake.

---

## 3. How the Program Flows (Big Picture)

```
Start program
    ↓
Login (username/password checked against login.txt)
    ↓
Load all existing students from students.txt into memory (a vector)
    ↓
Show menu → user picks an option → that operation runs → save changes to file
    ↓
(repeat menu until Logout or Exit)
    ↓
Logout → back to login screen        OR        Exit → program closes
```

Every single time you add, update, delete, or sort, the program **immediately re-saves `students.txt`**, so you never lose changes even if the program crashes.

---

## 4. Project Files — What Each One Does and Why It's Separate

We split the project into files by **responsibility**, not by feature, so each person can own one clean piece.

| File | Purpose | Why it's separate |
|---|---|---|
| `Student.h` / `Student.cpp` | Defines a single student: their info + semester GPA history + CGPA calculation | This is the core data unit — every other file uses it |
| `StudentManager.h` / `StudentManager.cpp` | Holds the list (`vector`) of ALL students and every operation: add, update, delete, search, sort, dashboard | This is the "brain" of the app — all the DSA logic lives here |
| `Authentication.h` / `Authentication.cpp` | Handles login/logout using `login.txt` | Keeps login logic out of the main menu code |
| `FileManager.h` / `FileManager.cpp` | Reads/writes `students.txt` using `fstream` | Isolates all raw file I/O in one place, so if we ever change the file format, we only edit here |
| `main.cpp` | Shows the menu, reads user choice, calls the right function | Just "wiring" — no business logic of its own |
| `students.txt` | Auto-created. Stores all student records, one per line | Plain text = easy to open, inspect, and debug |
| `login.txt` | Auto-created. Stores `username\|password` | Kept separate from student data intentionally |

**Why split into so many small files instead of one big file?**
This is called **modular design**. It means:
1. Five people can work on different files at the same time without editing the same lines and creating merge conflicts.
2. Each person can explain *their* file confidently in the viva without needing to understand every other file in depth.
3. If something breaks, it's easier to find — e.g., a save issue is almost certainly in `FileManager.cpp`, not `Student.cpp`.

---

## 5. How Each Feature Works (and Why)

### 5.1 Login System (`Authentication.cpp`)
- On first run, if `login.txt` doesn't exist, the program creates it automatically with `admin` / `admin123`.
- Login reads every line of `login.txt` and checks if the entered username/password matches — this is itself a small **linear search** through the credentials.
- No encryption is used. **Why:** the assignment focuses on DSA, not security, and encryption would add complexity that isn't the point of this project.

### 5.2 Adding a Student (`StudentManager::addStudent`)
- Collects all personal details, then generates a new ID automatically: `ST0001`, `ST0002`, etc.
- **Why zero-padded IDs (`ST0001` not `ST1`)?** So that sorting IDs alphabetically also sorts them in the correct numeric order — this matters later for Binary Search.
- The student is added to the `vector` with `push_back()`, then the whole list is saved to file.

### 5.3 Updating / Deleting (`StudentManager.cpp`)
- Both first use a **linear search** to locate the student by ID (loop through the vector, checking each ID).
- Update lets you leave a field blank to keep its current value.
- Delete uses `vector::erase()` to remove that one student, then re-saves the file.

### 5.4 Searching — the core DSA concept (`StudentManager.cpp`)
We implement **two different search algorithms** so we can compare them:

**Linear Search (primary method — used for ID, Name, and Date of Birth searches)**
- Checks each student one by one from the start until it finds a match (or reaches the end).
- Time Complexity: **O(n)** — in the worst case, checks every single record.
- Works on data in *any* order — no sorting needed first.
- **Why it's our main method:** it's simple, always correct, and doesn't require the data to be sorted, which fits a small dataset like a class of students.

**Binary Search (optional/bonus — used for ID search only)**
- Repeatedly checks the *middle* element and eliminates half the remaining records each time.
- Time Complexity: **O(log n)** — dramatically faster than linear search as data grows.
- **Requires the data to be sorted first** — this is why, before running, the program automatically sorts all students by ID using Merge Sort.
- **Why it needs sorted data:** binary search's speed comes from *skipping* half the data each step, based on comparing to the middle value. That comparison only makes sense — and only guarantees you haven't skipped past the answer — if everything is arranged in order.

### 5.5 Sorting (`StudentManager::mergeSort` / `merge`)
We implemented **Merge Sort**, and can sort students by ID, Name, or CGPA.

**How it works:**
1. **Divide:** split the list in half, repeatedly, until each piece has just 1 element (which is automatically "sorted").
2. **Conquer/Merge:** combine pairs of sorted pieces back together in order, over and over, until the whole list is one sorted list.

**Why Merge Sort specifically (and not Bubble Sort or Quick Sort)?**
- Time Complexity: **O(n log n)** in the *best, average, and worst* case — it's consistently fast, unlike Bubble Sort (O(n²)) or Quick Sort (which can degrade to O(n²) on unlucky input).
- Space Complexity: **O(n)** — it needs extra temporary arrays during merging, which is a fair tradeoff for guaranteed speed.
- It's **stable**: if two students have the same CGPA, they stay in their original relative order — useful for a real gradebook.

### 5.6 Dashboard (`StudentManager::showDashboard`)
- One pass through the vector (**O(n)**) to compute Total Students, Highest CGPA, Lowest CGPA, and Average CGPA.
- Kept intentionally simple — no charts or graphics, just numbers, since this is a console app.

### 5.7 File Handling (`FileManager.cpp`)
- `students.txt`: each student = **one line of text**, fields separated by `|`, and semester records inside that line separated by `;` and `,`. Example:
  ```
  ST0001|Rahul Kumar|Male|12-05-2004|Suresh Kumar|Anita Kumar|CSE|B.Tech|5|9876543210|rahul@example.com|92.50|1,8.20,80.50,A;2,8.45,82.00,A
  ```
- **Why this format?** It's human-readable (you can open the `.txt` file and understand it directly), doesn't need any external library, and is simple to parse with `fstream` + `stringstream` — exactly the tools taught in a DSA/file-handling course.
- Records auto-load on startup and auto-save after every change, as required.

---

## 6. Why No Database, Web Tech, or GUI

This was a deliberate scope decision, not a limitation:
- **No database (SQL, MongoDB, etc.):** the assignment is about DSA fundamentals — vectors and file handling are the "database" here.
- **No website (HTML/CSS/JS):** would require an entirely different skill set (frontend + backend + hosting) unrelated to DSA, and risks looking AI-generated / disconnected from what 5 students could realistically build and explain in one semester.
- **No GUI library:** console menus keep the code simple, portable (compiles anywhere with `g++`), and focused on logic rather than interface design.

If a course-related "extra" is wanted later, a good option is styling the *terminal output itself* (colors, boxes) — not switching platforms entirely.

---

## 7. How to Compile and Run

```bash
g++ -std=c++14 -o srms main.cpp Student.cpp StudentManager.cpp Authentication.cpp FileManager.cpp
```

Then run:
- Windows: `.\srms.exe`
- Mac/Linux: `./srms`

First login: username `admin`, password `admin123` (auto-created in `login.txt` on first run).

---

## 8. Team Module Ownership

| Member | Owns | Files |
|---|---|---|
| Member 1 | Student class design, Add Student, vector setup | `Student.h/.cpp`, part of `StudentManager.cpp` (addStudent) |
| Member 2 | Search & Display | part of `StudentManager.cpp` (search functions, displayAll) |
| Member 3 | Update & Delete | part of `StudentManager.cpp` (updateStudent, deleteStudent) |
| Member 4 | Sorting & Dashboard & complexity analysis | part of `StudentManager.cpp` (mergeSort, merge, sortMenu, showDashboard) |
| Member 5 | Authentication, File Handling, Menu Integration | `Authentication.h/.cpp`, `FileManager.h/.cpp`, `main.cpp` |

Even though everyone's core logic lives in `StudentManager.cpp`, each function is clearly separated and commented with its owner's area — so during the viva, each person can scroll straight to and explain their own functions.

---

## 9. Quick Concept Cheat Sheet (for viva prep)

| Concept | Where it's used | Complexity |
|---|---|---|
| Classes / OOP | `Student`, `StudentManager`, `Authentication`, `FileManager` | — |
| `vector` | Storing all students, and each student's semester history | — |
| Linear Search | Search by ID / Name / DOB | O(n) |
| Binary Search | Search by ID (after sorting) | O(log n) |
| Merge Sort | Sort by ID / Name / CGPA | O(n log n) time, O(n) space |
| File Handling (`fstream`) | Loading/saving `students.txt` and `login.txt` | O(n) |

---

## 10. Data Files Are Auto-Managed — Don't Edit Them by Hand

`students.txt` and `login.txt` are generated and updated automatically by the program. You *can* open them in a text editor to check what's stored, but avoid hand-editing them — a small typo in the `|` or `;` formatting will break parsing when the program loads.
