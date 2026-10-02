/*
    Student.cpp
    -----------
    Module Owner: Member 1
    Implements Student.h
*/

#include "Student.h"
#include <iostream>
#include <sstream>
#include <iomanip>
using namespace std;

// Small local helper to split a string by a delimiter character.
// Used only for file storage (serializing/deserializing academic history).
static vector<string> splitLocal(const string& text, char delimiter) {
    vector<string> tokens;
    stringstream ss(text);
    string token;
    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

// ---------------- Constructors ----------------

Student::Student() {
    studentID = "";
    fullName = "";
    gender = "";
    dob = "";
    fatherName = "";
    motherName = "";
    department = "";
    course = "";
    currentSemester = 1;
    phoneNumber = "";
    email = "";
    attendancePercentage = 0.0;
}

Student::Student(string id, string name, string gender, string dob, string father, string mother,
                  string dept, string course, int sem, string phone, string email, double attendance) {
    this->studentID = id;
    this->fullName = name;
    this->gender = gender;
    this->dob = dob;
    this->fatherName = father;
    this->motherName = mother;
    this->department = dept;
    this->course = course;
    this->currentSemester = sem;
    this->phoneNumber = phone;
    this->email = email;
    this->attendancePercentage = attendance;
}

// ---------------- Getters ----------------

string Student::getStudentID() const { return studentID; }
string Student::getFullName() const { return fullName; }
string Student::getGender() const { return gender; }
string Student::getDOB() const { return dob; }
string Student::getFatherName() const { return fatherName; }
string Student::getMotherName() const { return motherName; }
string Student::getDepartment() const { return department; }
string Student::getCourse() const { return course; }
int Student::getCurrentSemester() const { return currentSemester; }
string Student::getPhoneNumber() const { return phoneNumber; }
string Student::getEmail() const { return email; }
double Student::getAttendance() const { return attendancePercentage; }
vector<SemesterRecord>& Student::getAcademicHistory() { return academicHistory; }

// ---------------- Setters ----------------

void Student::setStudentID(string id) { studentID = id; }
void Student::setFullName(string name) { fullName = name; }
void Student::setGender(string g) { gender = g; }
void Student::setDOB(string d) { dob = d; }
void Student::setFatherName(string f) { fatherName = f; }
void Student::setMotherName(string m) { motherName = m; }
void Student::setDepartment(string d) { department = d; }
void Student::setCourse(string c) { course = c; }
void Student::setCurrentSemester(int sem) { currentSemester = sem; }
void Student::setPhoneNumber(string p) { phoneNumber = p; }
void Student::setEmail(string e) { email = e; }
void Student::setAttendance(double a) { attendancePercentage = a; }

// ---------------- Academic Record ----------------

void Student::addSemesterRecord(const SemesterRecord& record) {
    academicHistory.push_back(record);   // vector push_back -> amortized O(1)
}

// Time Complexity: O(n), where n = number of semesters recorded so far.
// We simply average the GPA of every stored semester.
double Student::calculateCGPA() const {
    if (academicHistory.empty()) return 0.0;

    double total = 0.0;
    for (size_t i = 0; i < academicHistory.size(); i++) {
        total += academicHistory[i].gpa;
    }
    return total / academicHistory.size();
}

// ---------------- Display ----------------

void Student::displayShort() const {
    cout << left << setw(10) << studentID
         << setw(22) << fullName
         << setw(10) << department
         << setw(6) << currentSemester
         << setw(10) << fixed << setprecision(2) << calculateCGPA()
         << endl;
}

void Student::displayDetailed() const {
    cout << "\n----------------------------------------\n";
    cout << "Student Details\n";
    cout << "----------------------------------------\n";
    cout << "Student ID       : " << studentID << endl;
    cout << "Name             : " << fullName << endl;
    cout << "Gender           : " << gender << endl;
    cout << "Date of Birth    : " << dob << endl;
    cout << "Father's Name    : " << fatherName << endl;
    cout << "Mother's Name    : " << motherName << endl;
    cout << "Department       : " << department << endl;
    cout << "Course           : " << course << endl;
    cout << "Current Semester : " << currentSemester << endl;
    cout << "Phone Number     : " << phoneNumber << endl;
    cout << "Email            : " << email << endl;
    cout << "Attendance %     : " << fixed << setprecision(2) << attendancePercentage << endl;

    cout << "\nAcademic History\n";
    if (academicHistory.empty()) {
        cout << "  No semester records available yet.\n";
    } else {
        for (size_t i = 0; i < academicHistory.size(); i++) {
            cout << "  Semester " << academicHistory[i].semesterNumber
                 << " -> GPA " << fixed << setprecision(2) << academicHistory[i].gpa
                 << " | Percentage " << academicHistory[i].percentage
                 << " | Grade " << academicHistory[i].grade << endl;
        }
    }
    cout << "\nOverall CGPA -> " << fixed << setprecision(2) << calculateCGPA() << endl;
    cout << "----------------------------------------\n";
}

// ---------------- File Handling ----------------
// A student is stored as ONE line of text using '|' to separate main fields.
// The last field holds the academic history, where each semester record is
// separated by ';' and the four values inside a record are separated by ','.
//
// Example line:
// ST0001|Rahul Kumar|Male|12-05-2004|Suresh Kumar|Anita Kumar|CSE|B.Tech|5|9876543210|rahul@example.com|92.50|1,8.20,80.50,A;2,8.45,82.00,A

string Student::toFileString() const {
    stringstream ss;
    ss << studentID << "|" << fullName << "|" << gender << "|" << dob << "|"
       << fatherName << "|" << motherName << "|" << department << "|" << course << "|"
       << currentSemester << "|" << phoneNumber << "|" << email << "|" << attendancePercentage << "|";

    for (size_t i = 0; i < academicHistory.size(); i++) {
        ss << academicHistory[i].semesterNumber << ","
           << academicHistory[i].gpa << ","
           << academicHistory[i].percentage << ","
           << academicHistory[i].grade;
        if (i != academicHistory.size() - 1) ss << ";";
    }
    return ss.str();
}

Student Student::fromFileString(const string& line) {
    vector<string> fields = splitLocal(line, '|');
    Student s;

    // Guard against malformed/blank lines
    if (fields.size() < 12) return s;

    s.setStudentID(fields[0]);
    s.setFullName(fields[1]);
    s.setGender(fields[2]);
    s.setDOB(fields[3]);
    s.setFatherName(fields[4]);
    s.setMotherName(fields[5]);
    s.setDepartment(fields[6]);
    s.setCourse(fields[7]);
    s.setCurrentSemester(stoi(fields[8]));
    s.setPhoneNumber(fields[9]);
    s.setEmail(fields[10]);
    s.setAttendance(stod(fields[11]));

    // Field 12 (if present) holds the semester history
    if (fields.size() >= 13 && !fields[12].empty()) {
        vector<string> semesterEntries = splitLocal(fields[12], ';');
        for (size_t i = 0; i < semesterEntries.size(); i++) {
            vector<string> parts = splitLocal(semesterEntries[i], ',');
            if (parts.size() == 4) {
                SemesterRecord rec;
                rec.semesterNumber = stoi(parts[0]);
                rec.gpa = stod(parts[1]);
                rec.percentage = stod(parts[2]);
                rec.grade = parts[3];
                s.addSemesterRecord(rec);
            }
        }
    }
    return s;
}
