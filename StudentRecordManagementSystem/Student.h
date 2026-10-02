/*
    Student.h
    ---------
    Module Owner: Member 1

    This file defines the Student class, which is the core data unit of the
    Student Record Management System. It demonstrates:
        - Object-Oriented Programming (class with private data + public interface)
        - Encapsulation (private attributes accessed via getters/setters)
        - Use of vector to store semester-wise academic history
*/

#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include <vector>
using namespace std;

// A simple structure to hold one semester's academic performance.
// Kept separate from Student so that academic history can grow
// dynamically each semester without changing the Student class.
struct SemesterRecord {
    int semesterNumber;
    double gpa;
    double percentage;
    string grade;
};

class Student {
private:
    string studentID;              // Auto-generated, e.g. ST0001
    string fullName;
    string gender;
    string dob;                    // Date of Birth, format: DD-MM-YYYY
    string fatherName;
    string motherName;
    string department;
    string course;
    int currentSemester;
    string phoneNumber;
    string email;
    double attendancePercentage;

    vector<SemesterRecord> academicHistory;   // vector: dynamic semester-wise records

public:
    // Constructors
    Student();
    Student(string id, string name, string gender, string dob, string father, string mother,
            string dept, string course, int sem, string phone, string email, double attendance);

    // ---------- Getters ----------
    string getStudentID() const;
    string getFullName() const;
    string getGender() const;
    string getDOB() const;
    string getFatherName() const;
    string getMotherName() const;
    string getDepartment() const;
    string getCourse() const;
    int getCurrentSemester() const;
    string getPhoneNumber() const;
    string getEmail() const;
    double getAttendance() const;
    vector<SemesterRecord>& getAcademicHistory();   // returned by reference so history can be edited

    // ---------- Setters (used by Update Module) ----------
    void setStudentID(string id);
    void setFullName(string name);
    void setGender(string g);
    void setDOB(string d);
    void setFatherName(string f);
    void setMotherName(string m);
    void setDepartment(string d);
    void setCourse(string c);
    void setCurrentSemester(int sem);
    void setPhoneNumber(string p);
    void setEmail(string e);
    void setAttendance(double a);

    // ---------- Academic Record handling ----------
    void addSemesterRecord(const SemesterRecord& record);
    double calculateCGPA() const;   // O(n) where n = number of semesters recorded

    // ---------- Display ----------
    void displayShort() const;      // one-line summary, used in "Display All"
    void displayDetailed() const;   // full record with academic history, used in "Search"

    // ---------- File Handling helpers ----------
    // Converts a Student object into a single line of text for storage.
    string toFileString() const;
    // Rebuilds a Student object from a single stored line of text.
    static Student fromFileString(const string& line);
};

#endif
