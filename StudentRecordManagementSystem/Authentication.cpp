/*
    Authentication.cpp
    -------------------
    Module Owner: Member 5
    Implements Authentication.h
*/

#include "Authentication.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

Authentication::Authentication(string filename) {
    this->filename = filename;
    this->loggedInUser = "";
    createDefaultCredentialsIfMissing();
}

// If login.txt does not exist (first run), create it with a default
// admin account so the application can be used immediately.
void Authentication::createDefaultCredentialsIfMissing() {
    ifstream checkFile(filename);
    if (!checkFile) {
        ofstream newFile(filename);
        newFile << "admin|admin123" << endl;
        newFile.close();
        cout << "[Setup] Default admin account created -> username: admin, password: admin123\n";
    }
    checkFile.close();
}

// Reads username|password pairs from login.txt and checks them against
// what the user types in. Time Complexity: O(n) where n = number of
// stored credentials (in this project, typically just one admin account).
bool Authentication::login() {
    string enteredUser, enteredPass;

    cout << "\n========== ADMIN LOGIN ==========\n";
    cout << "Username : ";
    cin >> enteredUser;
    cout << "Password : ";
    cin >> enteredPass;

    ifstream file(filename);
    if (!file) {
        cout << "Error: login.txt not found.\n";
        return false;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string storedUser, storedPass;
        getline(ss, storedUser, '|');
        getline(ss, storedPass, '|');

        if (storedUser == enteredUser && storedPass == enteredPass) {
            loggedInUser = storedUser;
            cout << "\nLogin successful. Welcome, " << loggedInUser << "!\n";
            return true;
        }
    }

    cout << "\nInvalid username or password.\n";
    return false;
}

void Authentication::logout() {
    cout << "\nUser '" << loggedInUser << "' logged out successfully.\n";
    loggedInUser = "";
}
