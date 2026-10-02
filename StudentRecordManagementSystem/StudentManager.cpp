/*
    StudentManager.cpp
    -------------------
    Implements StudentManager.h
*/

#include "StudentManager.h"
#include "FileManager.h"
#include <iostream>
#include <iomanip>
#include <limits>
using namespace std;

StudentManager::StudentManager(string filename) {
    dataFile = filename;
    nextIDNumber = 1;
}

// ---------------- File Handling ----------------

void StudentManager::loadRecords() {
    FileManager::loadStudents(students, dataFile);

    // Recalculate nextIDNumber based on the highest existing ID,
    // so new students continue the sequence correctly after reload.
    for (size_t i = 0; i < students.size(); i++) {
        string id = students[i].getStudentID();      // format: ST0001
        if (id.size() > 2) {
            int num = stoi(id.substr(2));
            if (num >= nextIDNumber) nextIDNumber = num + 1;
        }
    }
    cout << "[Info] " << students.size() << " student record(s) loaded from " << dataFile << ".\n";
}

void StudentManager::saveRecords() {
    FileManager::saveStudents(students, dataFile);
    cout << "[Info] Records saved to " << dataFile << ".\n";
}

// Generates IDs like ST0001, ST0002, ... using zero-padding so that
// alphabetical sorting of IDs also matches numerical order (this is
// what allows Binary Search to work correctly after sorting by ID).
string StudentManager::generateNextID() {
    stringstream ss;
    ss << "ST" << setfill('0') << setw(4) << nextIDNumber;
    nextIDNumber++;
    return ss.str();
}

// ---------------- Add Student (Member 1) ----------------

void StudentManager::addStudent() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string name, gender, dob, father, mother, dept, course, phone, email;
    int sem;
    double attendance;

    cout << "\n----- Add New Student -----\n";
    cout << "Full Name          : "; getline(cin, name);
    cout << "Gender             : "; getline(cin, gender);
    cout << "Date of Birth (DD-MM-YYYY): "; getline(cin, dob);
    cout << "Father's Name      : "; getline(cin, father);
    cout << "Mother's Name      : "; getline(cin, mother);
    cout << "Department         : "; getline(cin, dept);
    cout << "Course             : "; getline(cin, course);
    cout << "Current Semester   : "; cin >> sem;
    cout << "Phone Number       : "; cin >> phone;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Email              : "; getline(cin, email);
    cout << "Attendance %       : "; cin >> attendance;

    string newID = generateNextID();
    Student newStudent(newID, name, gender, dob, father, mother, dept, course, sem, phone, email, attendance);

    // Ask whether to enter semester-wise GPA history now.
    cout << "\nDo you want to enter semester-wise GPA history now? (y/n): ";
    char choice;
    cin >> choice;
    if (choice == 'y' || choice == 'Y') {
        for (int s = 1; s <= sem; s++) {
            SemesterRecord rec;
            rec.semesterNumber = s;
            cout << "  Semester " << s << " GPA        : "; cin >> rec.gpa;
            cout << "  Semester " << s << " Percentage : "; cin >> rec.percentage;
            cout << "  Semester " << s << " Grade      : "; cin >> rec.grade;
            newStudent.addSemesterRecord(rec);
        }
    }

    students.push_back(newStudent);   // vector push_back -> amortized O(1)
    saveRecords();                    // save after every modification (as required)

    cout << "\nStudent added successfully with ID: " << newID << endl;
}

// ---------------- Update Student (Member 3) ----------------

void StudentManager::updateStudent() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string id;
    cout << "\nEnter Student ID to update: ";
    getline(cin, id);

    // Linear search to locate the record. O(n)
    int index = -1;
    for (size_t i = 0; i < students.size(); i++) {
        if (students[i].getStudentID() == id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        cout << "Student ID not found.\n";
        return;
    }

    Student& s = students[index];
    cout << "Leave blank and press Enter to keep the current value.\n";

    cout << "Full Name [" << s.getFullName() << "]: ";
    string temp;
    getline(cin, temp);
    if (!temp.empty()) s.setFullName(temp);

    cout << "Department [" << s.getDepartment() << "]: ";
    getline(cin, temp);
    if (!temp.empty()) s.setDepartment(temp);

    cout << "Current Semester [" << s.getCurrentSemester() << "]: ";
    getline(cin, temp);
    if (!temp.empty()) s.setCurrentSemester(stoi(temp));

    cout << "Phone Number [" << s.getPhoneNumber() << "]: ";
    getline(cin, temp);
    if (!temp.empty()) s.setPhoneNumber(temp);

    cout << "Email [" << s.getEmail() << "]: ";
    getline(cin, temp);
    if (!temp.empty()) s.setEmail(temp);

    cout << "Attendance % [" << s.getAttendance() << "]: ";
    getline(cin, temp);
    if (!temp.empty()) s.setAttendance(stod(temp));

    cout << "Add a new semester GPA record? (y/n): ";
    char choice;
    cin >> choice;
    if (choice == 'y' || choice == 'Y') {
        SemesterRecord rec;
        cout << "  Semester Number : "; cin >> rec.semesterNumber;
        cout << "  GPA             : "; cin >> rec.gpa;
        cout << "  Percentage      : "; cin >> rec.percentage;
        cout << "  Grade           : "; cin >> rec.grade;
        s.addSemesterRecord(rec);
    }

    saveRecords();
    cout << "Student record updated successfully.\n";
}

// ---------------- Delete Student (Member 3) ----------------

void StudentManager::deleteStudent() {
    string id;
    cout << "\nEnter Student ID to delete: ";
    cin >> id;

    // Linear search to locate the record. O(n)
    int index = -1;
    for (size_t i = 0; i < students.size(); i++) {
        if (students[i].getStudentID() == id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        cout << "Student ID not found.\n";
        return;
    }

    cout << "Are you sure you want to delete " << students[index].getFullName() << "? (y/n): ";
    char confirm;
    cin >> confirm;
    if (confirm == 'y' || confirm == 'Y') {
        students.erase(students.begin() + index);   // vector erase -> O(n) shift
        saveRecords();
        cout << "Student record deleted successfully.\n";
    } else {
        cout << "Deletion cancelled.\n";
    }
}


void StudentManager::searchByID() {
    string id;
    cout << "\nEnter Student ID: ";
    cin >> id;

    for (size_t i = 0; i < students.size(); i++) {
        if (students[i].getStudentID() == id) {
            students[i].displayDetailed();
            return;
        }
    }
    cout << "No student found with ID " << id << ".\n";
}

void StudentManager::searchByName() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string name;
    cout << "\nEnter Student Name: ";
    getline(cin, name);

    bool found = false;
    for (size_t i = 0; i < students.size(); i++) {
        if (students[i].getFullName() == name) {
            students[i].displayDetailed();
            found = true;
        }
    }
    if (!found) cout << "No student found with name \"" << name << "\".\n";
}

void StudentManager::searchByDOB() {
    string dob;
    cout << "\nEnter Date of Birth (DD-MM-YYYY): ";
    cin >> dob;

    bool found = false;
    for (size_t i = 0; i < students.size(); i++) {
        if (students[i].getDOB() == dob) {
            students[i].displayDetailed();
            found = true;
        }
    }
    if (!found) cout << "No student found with DOB " << dob << ".\n";
}

// Binary Search 

void StudentManager::binarySearchByID() {
    if (students.empty()) {
        cout << "No records to search.\n";
        return;
    }

    // Step 1: sort by ID first (Binary Search precondition)
    mergeSort(students, 0, students.size() - 1,
              [](const Student& a, const Student& b) { return a.getStudentID() < b.getStudentID(); });

    string id;
    cout << "\nEnter Student ID: ";
    cin >> id;

    int low = 0, high = students.size() - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        string midID = students[mid].getStudentID();

        if (midID == id) {
            students[mid].displayDetailed();
            return;
        } else if (midID < id) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    cout << "No student found with ID " << id << ".\n";
}

// ---------------- Display All (Member 2) ----------------

void StudentManager::displayAll() {
    if (students.empty()) {
        cout << "\nNo student records found.\n";
        return;
    }

    cout << "\n" << left << setw(10) << "ID" << setw(22) << "Name"
         << setw(10) << "Dept" << setw(6) << "Sem" << setw(10) << "CGPA" << endl;
    cout << string(58, '-') << endl;

    for (size_t i = 0; i < students.size(); i++) {
        students[i].displayShort();
    }
    cout << "\nTotal Students: " << students.size() << endl;
}

// ---------------- Sorting (Member 4) ----------------
// MERGE SORT was chosen over other algorithms (like Bubble/Selection Sort)
// because it guarantees O(n log n) performance even in the worst case,
// which matters as the student database grows. It works by:
//   1. Dividing the vector into two halves recursively until each part
//      has only one element (which is trivially "sorted").
//   2. Merging the sorted halves back together in the correct order.
//
// Time Complexity : O(n log n) in best, average, and worst case.
// Space Complexity: O(n) extra space is used for the temporary arrays
//                    created during the merge step.
// It is also a STABLE sort, meaning records that compare equal keep
// their original relative order -- useful when, for example, two
// students share the same CGPA.

void StudentManager::merge(vector<Student>& arr, int left, int mid, int right,
                            const function<bool(const Student&, const Student&)>& comesBefore) {
    vector<Student> leftPart(arr.begin() + left, arr.begin() + mid + 1);
    vector<Student> rightPart(arr.begin() + mid + 1, arr.begin() + right + 1);

    size_t i = 0, j = 0;
    int k = left;

    while (i < leftPart.size() && j < rightPart.size()) {
        // "!comesBefore(rightPart[j], leftPart[i])" means leftPart[i] is NOT
        // greater than rightPart[j], i.e. left <= right. Picking left in
        // this case (instead of only on strict less-than) keeps the sort
        // stable -- equal elements retain their original relative order.
        if (!comesBefore(rightPart[j], leftPart[i])) {
            arr[k++] = leftPart[i++];
        } else {
            arr[k++] = rightPart[j++];
        }
    }
    while (i < leftPart.size()) arr[k++] = leftPart[i++];
    while (j < rightPart.size()) arr[k++] = rightPart[j++];
}

void StudentManager::mergeSort(vector<Student>& arr, int left, int right,
                                const function<bool(const Student&, const Student&)>& comesBefore) {
    if (left >= right) return;   // base case: 0 or 1 element is already sorted

    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid, comesBefore);
    mergeSort(arr, mid + 1, right, comesBefore);
    merge(arr, left, mid, right, comesBefore);
}

void StudentManager::sortMenu() {
    if (students.empty()) {
        cout << "No records to sort.\n";
        return;
    }

    cout << "\n----- Sort Students -----\n";
    cout << "1. Sort by Student ID\n";
    cout << "2. Sort by Name\n";
    cout << "3. Sort by CGPA (Highest first)\n";
    cout << "Enter choice: ";
    int choice;
    cin >> choice;

    switch (choice) {
        case 1:
            mergeSort(students, 0, students.size() - 1,
                      [](const Student& a, const Student& b) { return a.getStudentID() < b.getStudentID(); });
            cout << "Sorted by Student ID.\n";
            break;
        case 2:
            mergeSort(students, 0, students.size() - 1,
                      [](const Student& a, const Student& b) { return a.getFullName() < b.getFullName(); });
            cout << "Sorted by Name.\n";
            break;
        case 3:
            mergeSort(students, 0, students.size() - 1,
                      [](const Student& a, const Student& b) { return a.calculateCGPA() > b.calculateCGPA(); });
            cout << "Sorted by CGPA (highest first).\n";
            break;
        default:
            cout << "Invalid choice.\n";
            return;
    }

    saveRecords();
    displayAll();
}

// ---------------- Dashboard (Member 4) ----------------
// Time Complexity: O(n), one pass over the vector to compute the stats.

void StudentManager::showDashboard() {
    if (students.empty()) {
        cout << "\nNo data available for dashboard.\n";
        return;
    }

    double highest = students[0].calculateCGPA();
    double lowest = students[0].calculateCGPA();
    double total = 0.0;

    for (size_t i = 0; i < students.size(); i++) {
        double cgpa = students[i].calculateCGPA();
        total += cgpa;
        if (cgpa > highest) highest = cgpa;
        if (cgpa < lowest) lowest = cgpa;
    }

    double average = total / students.size();

    cout << "\n============ DASHBOARD ============\n";
    cout << "Total Students   : " << students.size() << endl;
    cout << fixed << setprecision(2);
    cout << "Highest CGPA     : " << highest << endl;
    cout << "Lowest CGPA      : " << lowest << endl;
    cout << "Average CGPA     : " << average << endl;
    cout << "====================================\n";
}

int StudentManager::getTotalStudents() const {
    return students.size();
}
