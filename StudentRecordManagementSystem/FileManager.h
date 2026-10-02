/*
    FileManager.h
    -------------
    Module Owner: Member 5

    Handles all reading/writing of the student database file (students.txt).
    Kept as a separate module so file-handling logic (fstream) is isolated
    from the business logic in StudentManager.
*/

#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <vector>
#include <string>
#include "Student.h"
using namespace std;

class FileManager {
public:
    // Loads all students from the given file into the vector.
    // Time Complexity: O(n) where n = number of records in the file.
    static void loadStudents(vector<Student>& students, const string& filename);

    // Overwrites the file with the current contents of the vector.
    // Time Complexity: O(n) where n = number of students in memory.
    static void saveStudents(const vector<Student>& students, const string& filename);
};

#endif
