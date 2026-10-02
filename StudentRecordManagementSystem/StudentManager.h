/*
    StudentManager.h
    -----------------
    Module Owners: Member 1 (Add), Member 2 (Search/Display),
                    Member 3 (Update/Delete), Member 4 (Sort/Dashboard)

    This class manages the entire in-memory database of students using a
    vector<Student>, and provides all the DSA-focused operations:
    linear search, binary search, merge sort, and statistics for the
    dashboard.
*/

#ifndef STUDENTMANAGER_H
#define STUDENTMANAGER_H

#include <vector>
#include <string>
#include <functional>
#include "Student.h"
using namespace std;

class StudentManager {
private:
    vector<Student> students;     // the core data structure holding all records
    string dataFile;
    int nextIDNumber;             // used to auto-generate IDs like ST0001, ST0002, ...

    // Generates the next sequential Student ID.
    string generateNextID();

    // ---- Merge Sort (Member 4) ----
    // Generic merge sort that accepts a comparator, so it can sort by
    // ID, Name, or CGPA without duplicating the algorithm three times.
    void mergeSort(vector<Student>& arr, int left, int right,
                    const function<bool(const Student&, const Student&)>& comesBefore);
    void merge(vector<Student>& arr, int left, int mid, int right,
               const function<bool(const Student&, const Student&)>& comesBefore);

public:
    StudentManager(string filename = "students.txt");

    // ---- File Handling ----
    void loadRecords();
    void saveRecords();

    // ---- Member 1: Add ----
    void addStudent();

    // ---- Member 3: Update / Delete ----
    void updateStudent();
    void deleteStudent();

    // ---- Member 2: Search / Display ----
    void searchByID();      // Linear Search, O(n)
    void searchByName();    // Linear Search, O(n)
    void searchByDOB();     // Linear Search, O(n)
    void binarySearchByID(); // Binary Search, O(log n) -- requires sorted data first
    void displayAll();

    // ---- Member 4: Sort / Dashboard ----
    void sortMenu();
    void showDashboard();

    int getTotalStudents() const;
};

#endif
