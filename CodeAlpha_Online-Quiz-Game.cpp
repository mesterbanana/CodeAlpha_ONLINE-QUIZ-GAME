/*
MIT License

Copyright (c) 2024 CodeAlpha

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
using namespace std;

const string RESET = "\033[0m";
const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";

class Management;
class User {
private:
    vector<string> usernames;
    vector<string> ids;
    vector<string> user_answers;
    vector<string> quizzes_taken;
    string username;
    string id;

public:
    friend class Management;
    friend void quiz_code();
    void username_login() {
        cout << "Enter the Username: ";
        cin >> username;
    }

    void id_login() {
        cout << "Enter the id: ";
        cin >> id;
        cout << endl;
    }

    [[nodiscard]] bool student_check() const {
        for (size_t i = 0; i < usernames.size(); i++) {
            if (username == usernames[i] && id == ids[i]) {
                return true;
            }
        }
        return false;
    }
};

class Management {
private:
    string adminname;
    string password;
    vector<string> quiz_sessions;
    vector<string> quiz_session_passwords;
    vector<string> quiz_subjects;
    vector<string> quiz_instructors;
    vector<string> questions;
    vector<string> right_answers;

public:
    void read_all_files(User &obj) {
        string line;
        string username, id;
        ifstream file("Users.txt");
        if (!file.is_open()) {
            cerr << "Unable to open file: " << "Users.txt" << endl;
            return;
        }
        while (getline(file, line)) {
            if (line.find("Username: ") != string::npos) {
                username = line.substr(line.find(':') + 2);
            }
            else if (line.find("ID: ") != string::npos) {
                id = line.substr(line.find(':') + 2);
            }
            else if (line.empty()) {
                if (!username.empty() && !id.empty()) {
                    obj.usernames.push_back(username);
                    obj.ids.push_back(id);
                }
            }
        }
        file.close();

        ifstream file1("Session_ids.txt");
        if (!file1.is_open()) {
            cerr << "Unable to open file: " << "Session_ids.txt" << endl;
            return;
        }

        string line1;
        string session, password, instructor, subject;

        while (getline(file1, line1)) {
            if (line1.find("Quiz Session: ") != string::npos) {
                session = line1.substr(line1.find(':') + 2);
            }
            else if (line1.find("Password: ") != string::npos) {
                password = line1.substr(line1.find(':') + 2);
            }
            else if (line1.find("Instructor: ") != string::npos) {
                instructor = line1.substr(line1.find(':') + 2);
            }
            else if (line1.find("Subject: ") != string::npos) {
                subject = line1.substr(line1.find(':') + 2);
            }
            else if (line1.empty()) {
                if (!session.empty() && !password.empty() && !instructor.empty() && !subject.empty()) {
                    quiz_sessions.push_back(session);
                    quiz_session_passwords.push_back(password);
                    quiz_instructors.push_back(instructor);
                    quiz_subjects.push_back(subject);
                    session.clear();
                    password.clear();
                    instructor.clear();
                    subject.clear();
                }
            }
        }
        if (!session.empty() && !password.empty() && !instructor.empty() && !subject.empty()) {
            quiz_sessions.push_back(session);
            quiz_session_passwords.push_back(password);
            quiz_instructors.push_back(instructor);
            quiz_subjects.push_back(subject);
        }

        file1.close();
    }

    void admin_login() {
        cout << "Enter the admin name: ";
        cin >> adminname;
        cout << "Enter the Password: ";
        cin >> password;
        cout << endl;
    }

    [[nodiscard]] bool admin_check() const {
        return adminname == "admin" && password == "123456789";
    }

    static void add_user_access(User &obj) {
        string usernamelogin, identification;
        cout << "Enter the Username: ";
        cin >> usernamelogin;
        cout << "Enter the id: ";
        cin >> identification;
        cout << endl;
        obj.usernames.push_back(usernamelogin);
        obj.ids.push_back(identification);
        ofstream file("Users.txt");
        if (!file.is_open()) {
            cerr << "Unable to open file: " << "Users.txt" << endl;
            return;
        }
        file << "Username: " << usernamelogin << "\n"
             << "ID: " << identification << "\n"
             << endl;

        file.close();
    }

    static void show_users_access(User const &obj) {
        for (size_t i = 0; i < obj.usernames.size(); i++) {
            cout << "Username: " << obj.usernames[i] << ", ID: " << obj.ids[i] << endl;
        }
        cout << endl;
    }

    static void remove_user_access(User &obj) {
        string remove_user;
        cout << "Enter username to be removed: ";
        cin >> remove_user;
        cout << endl;
        for (int i = 0; i < obj.usernames.size(); i++) {
            if (remove_user == obj.usernames[i]) {
                obj.usernames.erase(obj.usernames.begin() + i);
                obj.ids.erase(obj.ids.begin() + i);
                cout << "User " << remove_user << " removed successfully!\n" << endl;
                ofstream file("Users.txt");
                if (!file.is_open()) {
                    cerr << "Unable to open file: " << "Users.txt" << endl;
                    return;
                }
                for (int j = 0; j < obj.usernames.size(); j++) {
                    file << "Username: " << obj.usernames[j] << "\n"
                         << "ID: " << obj.ids[j] << "\n"
                         << endl;
                }
                file.close();
                return;
            }
        }
        cout << "User " << remove_user <<" not found!\n" << endl;;
    }

    void create_sessions() {
        string subj, inst;
        cin.ignore();
        cout << "Enter the subject of the quiz: ";
        getline(cin, subj);
        cout << "Enter the instructor of the quiz: ";
        getline(cin, inst);
        cout << endl;
        const string characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
        const string pass_ch = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%/";
        random_device rd;
        mt19937 generator(rd());
        uniform_int_distribution<> distribution(0, characters.size() - 1);
        uniform_int_distribution<> distribution1(0, pass_ch.size() - 1);
        string joinCode;
        string passkey;
        for (int i = 0; i < 6; ++i) {
            joinCode += characters[distribution(generator)];
        }
        for (int i = 0; i < 10; ++i) {
            passkey += pass_ch[distribution1(generator)];
        }
        cout << "The quiz session: " << joinCode << endl;
        cout << "Password: " << passkey << endl;
        cout << endl;
        quiz_sessions.push_back(joinCode);
        quiz_session_passwords.push_back(passkey);
        quiz_instructors.push_back(inst);
        quiz_subjects.push_back(subj);
        ofstream file1("Session_ids.txt");
        if (!file1.is_open()) {
            cerr << "Unable to open file: " << "Session_ids.txt" << endl;
            return;
        }
        for (int j = 0; j < quiz_sessions.size(); j++) {
            file1 << "Quiz Session: " << quiz_sessions[j] << "\n"
                 << "Password: " << quiz_session_passwords[j] << "\n"
                 << "Instructor: " << quiz_instructors[j] << "\n"
                 << "Subject: " << quiz_subjects[j] << "\n"
                 << endl;
        }
        file1.close();
    }

    void show_quizzes_sessions() const {
        for (size_t i = 0; i < quiz_sessions.size(); i++) {
            cout << "Session id: " << quiz_sessions[i] << ", Password: " << quiz_session_passwords[i] << ", Subject: " << quiz_subjects[i]
                 << ", Instructor: " << quiz_instructors[i] << endl;
        }
        cout << endl;
    }

    void delete_session() {
        string session_id;
        cout << "Enter the session id to be erased: ";
        cin >> session_id;
        cout << endl;
        for (int i = 0; i < quiz_sessions.size(); ++i) {
            if (quiz_sessions[i] == session_id) {
                quiz_sessions.erase(quiz_sessions.begin() + i);
                quiz_session_passwords.erase(quiz_session_passwords.begin() + i);
                quiz_subjects.erase(quiz_subjects.begin() + i);
                quiz_instructors.erase(quiz_instructors.begin() + i);
                cout << "Quiz session " << session_id << " has been removed successfully!\n" << endl;
                ofstream file1("Session_ids.txt");
                if (!file1.is_open()) {
                    cerr << "Unable to open file: " << "Session_ids.txt" << endl;
                    return;
                }
                for (int j = 0; j < quiz_sessions.size(); j++) {
                    file1 << "Quiz Session: " << quiz_sessions[j] << "\n"
                         << "Password: " << quiz_session_passwords[j] << "\n"
                         << "Instructor: " << quiz_instructors[j] << "\n"
                         << "Subject: " << quiz_subjects[j] << "\n"
                         << endl;
                }
                file1.close();
                return;
            }
        }
        cout << "Quiz session " << session_id << " not found!\n" << endl;
    }

    void loadQuestions(User &OBJ) {
        string session_id;
        string pass;
        cout << "Enter the quiz session code: ";
        cin >> session_id;
        cout << "Enter the session password: ";
        cin >> pass;
        cout << endl;

        bool session_found = false;
        for (int i = 0; i < quiz_sessions.size(); i++) {
            if (session_id == quiz_sessions[i] && pass == quiz_session_passwords[i]) {
                session_found = true;
                break;
            }
        }

        if (!session_found) {
            cout << "Wrong Password or session code. Please try again!" << endl;
            return;
        }

        for (int i = 0; i < OBJ.quizzes_taken.size(); i++) {
            if (session_id == OBJ.quizzes_taken[i]) {
                cout << "This quiz has been taken before!" << "\n" << endl;
                return;
            }
        }

        // Load the questions
        string location = "Quizzes/" + session_id + "/" + session_id + "_ques.txt";
        ifstream file(location);
        if (!file.is_open()) {
            cerr << "Quiz session " << session_id + " not found!" << "\n" << endl;
            return;
        }

        string line;
        while (getline(file, line)) {
            if (line.find('Q') == 0) {
                string question = line;
                for (int i = 0; i < 5; i++) {
                    if (getline(file, line)) {
                        question += "\n" + line;
                    }
                }
                questions.push_back(question);
            }
        }
        file.close();

        string location1 = "Quizzes/" + session_id + "/" + session_id + "_ans.txt";
        ifstream file1(location1);
        if (!file1.is_open()) {
            cerr << "Quiz session " << session_id + " not found!" << "\n" << endl;
            return;
        }

        string line1;
        while (getline(file1, line1)) {
            if (!line1.empty() && line1 != "---------------------") {
                right_answers.push_back(line1);
            }
        }
        file1.close();

        cout << "Starting the quiz......." << "\n" << endl;
        string ans;
        cin.ignore();
        for (int i = 0; i < questions.size(); i++) {
            cout << YELLOW << questions[i];
            cout << "Choose the correct answer (a,b,c,d): ";
            getline(cin, ans);
            cout << RESET << endl;
            while (ans != "a" && ans != "b" && ans != "c" && ans != "d") {
                cout << YELLOW << "Invalid answer. Please choose a, b, c, or d." << endl;
                cout << "Choose the correct answer (a,b,c,d): ";
                getline(cin, ans);
                cout << RESET << endl;
            }
            cout << "\n" << endl;
            OBJ.user_answers.push_back(ans);
        }

        OBJ.quizzes_taken.push_back(session_id);
        cout << "\n" << "Quiz has finished, wait for grading........" << "\n" << endl;

        int score = 0;
        for (int i = 0; i < questions.size(); i++) {
            if (OBJ.user_answers[i] == right_answers[i]) {
                score++;
            }
        }
        cout << "\n" << "Your score is: " << score << " out of " << questions.size() << "\n" << endl;

        for (int i = 0; i < right_answers.size(); i++) {
            if (OBJ.user_answers[i] == right_answers[i]) {
                cout << GREEN << "Your answer for Q" << i + 1 << ": "  << OBJ.user_answers[i] << RESET << endl;
                cout << GREEN << "The correct answer for Q" << i + 1 << ": "  << right_answers[i] << RESET << endl;
                cout << endl;
            } else {
                cout << RED << "Your answer for Q" << i + 1 << ": "  << OBJ.user_answers[i] << RESET << endl;
                cout << RED << "The correct answer for Q" << i + 1 << ": "  << right_answers[i] << RESET << endl;
                cout << endl;
            }
        }
    }


};

void vmenu() {
    cout << YELLOW << "=================================" << endl;
    cout << "|          Main Menu            |" << endl;
    cout << "=================================" << endl;
    cout << "| 1. Admin log-in               |" << endl;
    cout << "| 2. User log-in                |" << endl;
    cout << "| 3. Exit the program           |" << endl;
    cout << "=================================" << RESET << endl;
}

void adminmenu() {
    cout << YELLOW << "=================================" << endl;
    cout << "|        Admin Menu             |" << endl;
    cout << "=================================" << endl;
    cout << "| 1. Add user access            |" << endl;
    cout << "| 2. Show all users             |" << endl;
    cout << "| 3. Remove user access         |" << endl;
    cout << "| 4. Start new quiz session     |" << endl;
    cout << "| 5. Show all quizzes sessions  |" << endl;
    cout << "| 6. End current quiz session   |" << endl;
    cout << "| 7. Return to the main menu    |" << endl;
    cout << "=================================" << RESET << endl;
}

void usermenu() {
    cout << YELLOW << "=================================" << endl;
    cout << "|         User Menu             |" << endl;
    cout << "=================================" << endl;
    cout << "| 1. Take quiz                  |" << endl;
    cout << "| 2. Return to the main menu    |" << endl;
    cout << "=================================" << RESET << endl;

}

int main() {
    User user;
    Management adminstaff;
    adminstaff.read_all_files(user);

    while (true) {
        string choice;
        vmenu();
        cout << "Please enter your option: ";
        cin >> choice;
        cout << endl;

        if (choice == "1") {
            adminstaff.admin_login();
            if (adminstaff.admin_check()) {
                cout << "Accessing to admin menu......." << endl;
                cout << endl;
                while (true) {
                    adminmenu();
                    cout << "Please enter your option: ";
                    cin >> choice;
                    cout << endl;
                    if (choice == "1") {
                        Management::add_user_access(user);
                    }
                    else if (choice == "2") {
                        Management::show_users_access(user);
                    }
                    else if (choice == "3") {
                        Management::remove_user_access(user);
                    }
                    else if (choice == "4") {
                        adminstaff.create_sessions();
                    }
                    else if (choice == "5") {
                        adminstaff.show_quizzes_sessions();
                    }
                    else if (choice == "6") {
                        adminstaff.delete_session();
                    }
                    else if (choice == "7") {
                        cout << "Returning to the main menu.......\n" << endl;
                        break;
                    }
                    else {
                        cout << "Please enter a valid option!" << endl << endl;
                    }
                }
            }
            else {
                cout << "Permission is denied!" << endl << endl;
            }
        }
        else if (choice == "2") {
            user.username_login();
            user.id_login();
            if (user.student_check()) {
                cout << "Accessing to user menu......." << endl;
                cout << endl;
                while (true) {
                    usermenu();
                    cout << "Please enter your option: ";
                    cin >> choice;
                    cout << endl;
                    if (choice == "1") {
                        adminstaff.loadQuestions(user);
                    }
                    else if (choice == "2") {
                        cout << "Returning to the main menu.......\n" << endl;
                        break;
                    }
                    else {
                        cout << "Please enter a valid option!" << endl << endl;
                    }
                }
            }
            else {
                cout << "Permission is denied!" << endl << endl;
            }
        }
        else if (choice == "3") {
            cout << YELLOW << "Exiting......." << RESET << endl;
            return 0;
        }
        else {
            cout << "Please enter a valid option!\n" << endl;
        }
    }
}
