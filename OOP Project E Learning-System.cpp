#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib> // For system()
using namespace std;

// Forward declarations
class Course;
class Instructor;
class Student;

class Course {
    string title;
public:
    Course(string t) : title(t) {}
    string getTitle() { return title; }
};

class Material {
private:
    string label; // Changed from pdfName to label (e.g., lec 1, lec 2)
    string courseTitle;
    string content; // Text-based content for console display
public:
    Material(string lbl = "", string course = "", string cont = "") : label(lbl), courseTitle(course), content(cont) {}
    string getLabel() { return label; } // Changed from getPdfName to getLabel
    string getCourseTitle() { return courseTitle; }
    string getContent() { return content; }
};

class Quiz {
private:
    string label; // Changed from pdfName to label
    string courseTitle;
    string deadline; // Added deadline in dd/mm/yyyy format
public:
    Quiz(string lbl = "", string course = "", string dl = "") : label(lbl), courseTitle(course), deadline(dl) {}
    string getLabel() { return label; } // Changed from getPdfName to getLabel
    string getCourseTitle() { return courseTitle; }
    string getDeadline() { return deadline; }
};

class Assignment {
private:
    string label; // Changed from pdfName to label
    string courseTitle;
    string deadline; // Added deadline in dd/mm/yyyy format
public:
    Assignment(string lbl = "", string course = "", string dl = "") : label(lbl), courseTitle(course), deadline(dl) {}
    string getLabel() { return label; } // Changed from getPdfName to getLabel
    string getCourseTitle() { return courseTitle; }
    string getDeadline() { return deadline; }
};

class User {
protected:
    string name, email, password;
public:
    User(string n, string e, string p) : name(n), email(e), password(p) {}
    virtual void displayDashboard() = 0;
    string getEmail() { return email; }
    string getPassword() { return password; }
    string getName() { return name; }
    virtual ~User() {}
};

class Instructor : public User {
private:
    vector<Material> materials;
    vector<Quiz> quizzes;
    vector<Assignment> assignments;
public:
    Instructor(string n, string e, string p) : User(n, e, p) {}
    void displayDashboard() override {
        cout << "Welcome Instructor: " << name << endl;
        cout << "\nCurrent Materials:\n";
        for (auto& material : materials) {
            cout << "- Label: " << material.getLabel() << " (Course: " << material.getCourseTitle() << ")\n";
        }
        cout << "\nCurrent Quizzes:\n";
        for (auto& quiz : quizzes) {
            cout << "- Label: " << quiz.getLabel() << " (Course: " << quiz.getCourseTitle() << ", Deadline: " << quiz.getDeadline() << ")\n";
        }
        cout << "\nCurrent Assignments:\n";
        for (auto& assignment : assignments) {
            cout << "- Label: " << assignment.getLabel() << " (Course: " << assignment.getCourseTitle() << ", Deadline: " << assignment.getDeadline() << ")\n";
        }
    }

    void addMaterial(string label, string courseTitle, string content) {
        materials.push_back(Material(label, courseTitle, content));
        cout << "Material '" << label << "' added for course: " << courseTitle << endl;
        saveToFile();
    }

    void addQuiz(string label, string courseTitle, string deadline) {
        quizzes.push_back(Quiz(label, courseTitle, deadline));
        cout << "Quiz '" << label << "' added for course: " << courseTitle << " with deadline: " << deadline << endl;
        saveToFile();
    }

    void addAssignment(string label, string courseTitle, string deadline) {
        assignments.push_back(Assignment(label, courseTitle, deadline));
        cout << "Assignment '" << label << "' added for course: " << courseTitle << " with deadline: " << deadline << endl;
        saveToFile();
    }

    void viewSubmissions() { cout << "Viewing submissions.\n"; }
    void markStudents(Student*[], int, int) { cout << "Marked students.\n"; }

    void saveToFile() {
        ofstream file(email + "_instructor_data.txt");
        file << name << "\n" << email << "\n" << password << "\n";
        for (auto& material : materials) {
            file << "Material:" << material.getLabel() << ":" << material.getCourseTitle() << ":" << material.getContent() << "\n";
        }
        for (auto& quiz : quizzes) {
            file << "Quiz:" << quiz.getLabel() << ":" << quiz.getCourseTitle() << ":" << quiz.getDeadline() << "\n";
        }
        for (auto& assignment : assignments) {
            file << "Assignment:" << assignment.getLabel() << ":" << assignment.getCourseTitle() << ":" << assignment.getDeadline() << "\n";
        }
        file.close();
    }

    void loadFromFile() {
        ifstream file(email + "_instructor_data.txt");
        if (!file.is_open()) return;

        string line, temp;
        getline(file, temp); // Read name
        getline(file, temp); // Read email
        getline(file, temp); // Read password
        materials.clear();
        quizzes.clear();
        assignments.clear();

        while (getline(file, line)) {
            if (line.find("Material:") == 0) {
                size_t pos1 = line.find(":", 9);
                size_t pos2 = line.find(":", pos1 + 1);
                string label = line.substr(9, pos1 - 9);
                string courseTitle = line.substr(pos1 + 1, pos2 - pos1 - 1);
                string content = line.substr(pos2 + 1);
                materials.push_back(Material(label, courseTitle, content));
            } else if (line.find("Quiz:") == 0) {
                size_t pos1 = line.find(":", 5);
                size_t pos2 = line.find(":", pos1 + 1);
                string label = line.substr(5, pos1 - 5);
                string courseTitle = line.substr(pos1 + 1, pos2 - pos1 - 1);
                string deadline = line.substr(pos2 + 1);
                quizzes.push_back(Quiz(label, courseTitle, deadline));
            } else if (line.find("Assignment:") == 0) {
                size_t pos1 = line.find(":", 11);
                size_t pos2 = line.find(":", pos1 + 1);
                string label = line.substr(11, pos1 - 11);
                string courseTitle = line.substr(pos1 + 1, pos2 - pos1 - 1);
                string deadline = line.substr(pos2 + 1);
                assignments.push_back(Assignment(label, courseTitle, deadline));
            }
        }
        file.close();
    }

    vector<Material> getMaterials() { return materials; }
};

class Student : public User {
private:
    vector<Course*> enrolledCourses;
public:
    Student(string n, string e, string p) : User(n, e, p) {}
    void displayDashboard() override {
        cout << "Welcome Student: " << name << endl;
        cout << "\nEnrolled Courses:\n";
        for (auto& course : enrolledCourses) {
            cout << "- " << course->getTitle() << endl;
        }
    }

    void enrollInCourse(Course* c) {
        enrolledCourses.push_back(c);
        cout << "Enrolled in: " << c->getTitle() << endl;
        saveToFile();
    }

    void takeQuiz(Course* c) { cout << "Quiz taken for: " << c->getTitle() << endl; }

    void submitAssignment(Course* c, int idx, string sub) {
        cout << "Submitted assignment #" << idx + 1 << " for: " << c->getTitle() << endl;
    }

    void viewPerformance() { cout << "Viewing performance.\n"; }

    void viewMaterials(Instructor* instructors[], int instructorCount) {
        cout << "\nAvailable Courses:\n";
        for (size_t i = 0; i < enrolledCourses.size(); ++i) {
            cout << i + 1 << ". " << enrolledCourses[i]->getTitle() << endl;
        }
        int courseChoice;
        cout << "Choose a course to view materials: ";
        while (!(cin >> courseChoice) || courseChoice < 1 || courseChoice > (int)enrolledCourses.size()) {
            cout << "Invalid choice. Try again: ";
            cin.clear();
            cin.ignore(10000, '\n');
        }
        string courseTitle = enrolledCourses[courseChoice - 1]->getTitle();

        cout << "\n1. Open Label in Console\n2. View Content in Console\nEnter: ";
        int viewChoice;
        while (!(cin >> viewChoice) || viewChoice < 1 || viewChoice > 2) {
            cout << "Invalid choice. Try again: ";
            cin.clear();
            cin.ignore(10000, '\n');
        }

        for (int i = 0; i < instructorCount; i++) {
            vector<Material> materials = instructors[i]->getMaterials();
            for (auto& material : materials) {
                if (material.getCourseTitle() == courseTitle) {
                    if (viewChoice == 1) {
                        cout << "\nMaterial Label for " << courseTitle << ":\n";
                        cout << "Label: " << material.getLabel() << "\n";
                    } else {
                        cout << "\nMaterial for " << courseTitle << ":\n";
                        cout << "Label: " << material.getLabel() << "\n";
                        cout << "Content: " << (material.getContent().empty() ? "No text content available." : material.getContent()) << "\n";
                    }
                }
            }
        }
    }

    void saveToFile() {
        ofstream file(email + "_student_data.txt");
        file << name << "\n" << email << "\n" << password << "\n";
        for (auto& course : enrolledCourses) {
            file << "Course:" << course->getTitle() << "\n";
        }
        file.close();
    }

    void loadFromFile(Course* courses[], int courseCount) {
        ifstream file(email + "_student_data.txt");
        if (!file.is_open()) return;

        string line, temp;
        getline(file, temp); // Read name
        getline(file, temp); // Read email
        getline(file, temp); // Read password
        enrolledCourses.clear();

        while (getline(file, line)) {
            if (line.find("Course:") == 0) {
                string courseTitle = line.substr(7);
                for (int i = 0; i < courseCount; i++) {
                    if (courses[i]->getTitle() == courseTitle) {
                        enrolledCourses.push_back(courses[i]);
                        break;
                    }
                }
            }
        }
        file.close();
    }
};

class LMS {
    Instructor* instructors[10];
    int instructorCount = 0;
    Student* students[10];
    int studentCount = 0;
    Course* courses[10];
    int courseCount = 0;
public:
    void registerInstructor(string name, string email, string pass) {
        instructors[instructorCount++] = new Instructor(name, email, pass);
        cout << "Instructor registered.\n";
    }

    void registerStudent(string name, string email, string pass) {
        students[studentCount++] = new Student(name, email, pass);
        cout << "Student registered.\n";
    }

    Instructor* loginInstructor(string email, string pass) {
        for (int i = 0; i < instructorCount; i++) {
            if (instructors[i]->getEmail() == email && instructors[i]->getPassword() == pass) {
                instructors[i]->loadFromFile();
                return instructors[i];
            }
        }
        return nullptr;
    }

    Student* loginStudent(string email, string pass) {
        for (int i = 0; i < studentCount; i++) {
            if (students[i]->getEmail() == email && students[i]->getPassword() == pass) {
                students[i]->loadFromFile(courses, courseCount);
                return students[i];
            }
        }
        return nullptr;
    }

    void addCourse(Instructor* inst) {
        string title;
        cout << "Enter course title: ";
        cin.ignore(); getline(cin, title);
        courses[courseCount++] = new Course(title);
        cout << "Course added.\n";
    }

    Course** getCourses() { return courses; }
    int getCourseCount() { return courseCount; }
    Student** getStudents() { return students; }
    int getStudentCount() { return studentCount; }
    Instructor** getInstructors() { return instructors; }
    int getInstructorCount() { return instructorCount; }

    ~LMS() {
        for (int i = 0; i < instructorCount; i++) delete instructors[i];
        for (int i = 0; i < studentCount; i++) delete students[i];
        for (int i = 0; i < courseCount; i++) delete courses[i];
    }
};

int main() {
    LMS lms;
    int choice;
    while (true) {
        cout << "\n1. Register Instructor\n2. Login Instructor\n3. Register Student\n4. Login Student\n5. Exit\nEnter: ";
        while (!(cin >> choice)) {
            cout << "Invalid input. Enter 1-5: ";
            cin.clear();
            cin.ignore(10000, '\n');
        }

        if (choice == 1) {
            string name, email, pass;
            cout << "Enter name: "; cin.ignore(); getline(cin, name);
            cout << "Enter email: "; getline(cin, email);
            cout << "Enter password: "; getline(cin, pass);
            lms.registerInstructor(name, email, pass);
        } else if (choice == 2) {
            string email, pass;
            cout << "Enter email: "; cin.ignore(); getline(cin, email);
            cout << "Enter password: "; getline(cin, pass);
            Instructor* inst = lms.loginInstructor(email, pass);
            if (inst != nullptr) {
                inst->displayDashboard();
                int ichoice;
                while (true) {
                    cout << "\n1. Create Course\n2. Add Material\n3. Add Quiz\n4. Add Assignment\n5. View Submissions\n6. Mark Students\n7. Logout\nEnter: ";
                    while (!(cin >> ichoice)) {
                        cout << "Invalid input. Try again: ";
                        cin.clear();
                        cin.ignore(10000, '\n');
                    }
                    if (ichoice == 1) {
                        lms.addCourse(inst);
                    } else if (ichoice == 2) {
                        string label, courseTitle, content;
                        cout << "Enter material label (e.g., lec 1): ";
                        cin.ignore(); getline(cin, label);
                        cout << "Enter course title: ";
                        getline(cin, courseTitle);
                        cout << "Enter material content (or press Enter to skip): ";
                        getline(cin, content);
                        inst->addMaterial(label, courseTitle, content);
                    } else if (ichoice == 3) {
                        string label, courseTitle, deadline;
                        cout << "Enter quiz label (e.g., quiz 1): ";
                        cin.ignore(); getline(cin, label);
                        cout << "Enter course title: ";
                        getline(cin, courseTitle);
                        cout << "Enter deadline (dd/mm/yyyy): ";
                        getline(cin, deadline);
                        inst->addQuiz(label, courseTitle, deadline);
                    } else if (ichoice == 4) {
                        string label, courseTitle, deadline;
                        cout << "Enter assignment label (e.g., assn 1): ";
                        cin.ignore(); getline(cin, label);
                        cout << "Enter course title: ";
                        getline(cin, courseTitle);
                        cout << "Enter deadline (dd/mm/yyyy): ";
                        getline(cin, deadline);
                        inst->addAssignment(label, courseTitle, deadline);
                    } else if (ichoice == 5) {
                        inst->viewSubmissions();
                    } else if (ichoice == 6) {
                        inst->markStudents(lms.getStudents(), lms.getStudentCount(), 50);
                    } else if (ichoice == 7) {
                        break;
                    } else {
                        cout << "Invalid option.\n";
                    }
                }
            } else {
                cout << "Invalid login.\n";
            }
        } else if (choice == 3) {
            string name, email, pass;
            cout << "Enter name: "; cin.ignore(); getline(cin, name);
            cout << "Enter email: "; getline(cin, email);
            cout << "Enter password: "; getline(cin, pass);
            lms.registerStudent(name, email, pass);
        } else if (choice == 4) {
            string email, pass;
            cout << "Enter email: "; cin.ignore(); getline(cin, email);
            cout << "Enter password: "; getline(cin, pass);
            Student* stu = lms.loginStudent(email, pass);
            if (stu != nullptr) {
                stu->displayDashboard();
                int schoice;
                while (true) {
                    cout << "\n1. Enroll in Course\n2. Take Quiz\n3. Submit Assignment\n4. View Performance\n5. View Course Materials\n6. Logout\nEnter: ";
                    while (!(cin >> schoice)) {
                        cout << "Invalid input. Try again: ";
                        cin.clear();
                        cin.ignore(10000, '\n');
                    }
                    if (schoice == 1) {
                        Course** courses = lms.getCourses();
                        int count = lms.getCourseCount();
                        if (count == 0) {
                            cout << "No courses available.\n";
                        } else {
                            for (int i = 0; i < count; ++i)
                                cout << i + 1 << ". " << courses[i]->getTitle() << endl;
                            int ch;
                            cout << "Choose course: ";
                            while (!(cin >> ch) || ch < 1 || ch > count) {
                                cout << "Invalid choice. Try again: ";
                                cin.clear();
                                cin.ignore(10000, '\n');
                            }
                            stu->enrollInCourse(courses[ch - 1]);
                        }
                    } else if (schoice == 2) {
                        Course** courses = lms.getCourses();
                        int count = lms.getCourseCount();
                        if (count == 0) {
                            cout << "No courses available.\n";
                        } else {
                            for (int i = 0; i < count; ++i)
                                cout << i + 1 << ". " << courses[i]->getTitle() << endl;
                            int ch;
                            cout << "Choose course: ";
                            while (!(cin >> ch) || ch < 1 || ch > count) {
                                cout << "Invalid choice. Try again: ";
                                cin.clear();
                                cin.ignore(10000, '\n');
                            }
                            stu->takeQuiz(courses[ch - 1]);
                        }
                    } else if (schoice == 3) {
                        Course** courses = lms.getCourses();
                        int count = lms.getCourseCount();
                        if (count == 0) {
                            cout << "No courses available.\n";
                        } else {
                            for (int i = 0; i < count; ++i)
                                cout << i + 1 << ". " << courses[i]->getTitle() << endl;
                            int ch;
                            cout << "Choose course: ";
                            while (!(cin >> ch) || ch < 1 || ch > count) {
                                cout << "Invalid choice. Try again: ";
                                cin.clear();
                                cin.ignore(10000, '\n');
                            }
                            int idx;
                            cout << "Enter assignment index: ";
                            cin >> idx;
                            string sub;
                            cout << "Enter submission text: ";
                            cin.ignore();
                            getline(cin, sub);
                            stu->submitAssignment(courses[ch - 1], idx - 1, sub);
                        }
                    } else if (schoice == 4) {
                        stu->viewPerformance();
                    } else if (schoice == 5) {
                        stu->viewMaterials(lms.getInstructors(), lms.getInstructorCount());
                    } else if (schoice == 6) {
                        break;
                    } else {
                        cout << "Invalid option.\n";
                    }
                }
            } else {
                cout << "Invalid login.\n";
            }
        } else if (choice == 5) {
            cout << "Goodbye!\n";
            break;
        } else {
            cout << "Invalid option.\n";
        }
    }
    return 0;
}
