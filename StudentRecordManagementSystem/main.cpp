/*
    main.cpp
    --------
    Module Owner: Member 5 (Menu Integration)

    Entry point of the Student Record Management System.
    Ties together Authentication, StudentManager, and the console menu.

    Project    : Student Record Management System
    Course     : Data Structures and Algorithms (DSA)
    Team Size  : 5 members
*/

#include <iostream>
#include "Authentication.h"
#include "StudentManager.h"
using namespace std;

void printMainMenu() {
    cout << "\n=====================================\n";
    cout << "   STUDENT RECORD MANAGEMENT SYSTEM\n";
    cout << "=====================================\n";
    cout << "1. Add Student\n";
    cout << "2. Update Student\n";
    cout << "3. Delete Student\n";
    cout << "4. Search Student\n";
    cout << "5. Display All Students\n";
    cout << "6. Sort Students\n";
    cout << "7. Dashboard\n";
    cout << "8. Save Records\n";
    cout << "9. Logout\n";
    cout << "0. Exit\n";
    cout << "=====================================\n";
    cout << "Enter your choice: ";
}

void printSearchMenu() {
    cout << "\n----- Search Student -----\n";
    cout << "1. Search by Student ID (Linear Search)\n";
    cout << "2. Search by Name (Linear Search)\n";
    cout << "3. Search by Date of Birth (Linear Search)\n";
    cout << "4. Search by Student ID (Binary Search)\n";
    cout << "Enter choice: ";
}

int main() {
    cout << "Welcome to the Student Record Management System\n";

    Authentication auth;
    StudentManager manager;
    bool programRunning = true;

    while (programRunning) {
        // Require login before any operation can be performed
        if (!auth.login()) {
            cout << "\nTry again? (y/n): ";
            char retry;
            cin >> retry;
            if (retry != 'y' && retry != 'Y') {
                programRunning = false;
            }
            continue;
        }

        manager.loadRecords();   // automatically load records after login

        bool sessionActive = true;
        while (sessionActive) {
            printMainMenu();
            int choice;
            cin >> choice;

            switch (choice) {
                case 1:
                    manager.addStudent();
                    break;
                case 2:
                    manager.updateStudent();
                    break;
                case 3:
                    manager.deleteStudent();
                    break;
                case 4: {
                    printSearchMenu();
                    int searchChoice;
                    cin >> searchChoice;
                    if (searchChoice == 1) manager.searchByID();
                    else if (searchChoice == 2) manager.searchByName();
                    else if (searchChoice == 3) manager.searchByDOB();
                    else if (searchChoice == 4) manager.binarySearchByID();
                    else cout << "Invalid choice.\n";
                    break;
                }
                case 5:
                    manager.displayAll();
                    break;
                case 6:
                    manager.sortMenu();
                    break;
                case 7:
                    manager.showDashboard();
                    break;
                case 8:
                    manager.saveRecords();
                    break;
                case 9:
                    auth.logout();
                    sessionActive = false;   // return to login screen
                    break;
                case 0:
                    manager.saveRecords();
                    cout << "\nThank you for using the Student Record Management System. Goodbye!\n";
                    sessionActive = false;
                    programRunning = false;
                    break;
                default:
                    cout << "Invalid choice. Please try again.\n";
            }
        }
    }

    return 0;
}
