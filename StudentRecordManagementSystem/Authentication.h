/*
    Authentication.h
    ----------------
    Module Owner: Member 5

    Provides a simple, file-based admin login system.
    Credentials are stored as plain text in login.txt in the format:
        username|password

    This is intentionally kept simple (no encryption/hashing) since the
    focus of this project is Data Structures and Algorithms, not security.
*/

#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H

#include <string>
using namespace std;

class Authentication {
private:
    string filename;
    string loggedInUser;

    void createDefaultCredentialsIfMissing();

public:
    Authentication(string filename = "login.txt");

    bool login();     // returns true if login succeeds
    void logout();
};

#endif
