#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>
#include <cctype>
#include <iomanip>

using namespace std;

class Date {
public:
    int day;
    int month;
    int year;

public:
    Date(int day, int month, int year) : day(day), month(month), year(year) {}

    friend ostream& operator<<(ostream& os, const Date& date) {
        os << setfill('0') << setw(2) << date.day << "-"
           << setfill('0') << setw(2) << date.month << "-"
           << setfill('0') << setw(4) << date.year;
        return os;
    }
};

class InputValidator {
public:
    virtual bool isValid(const string& input) const = 0;
    virtual ~InputValidator() {}
};

class AlphabeticValidator : public InputValidator {
public:
    virtual bool isValid(const string& input) const override {
        return all_of(input.begin(), input.end(), [](char c) {
            return isalpha(c) || isspace(c);
        });
    }
};

class NumericValidator : public InputValidator {
public:
    virtual bool isValid(const string& input) const override {
        return all_of(input.begin(), input.end(), [](char c) {
            return isdigit(c);
        });
    }
};

class Task {
private:
    string title;
    string description;
    Date dueDate;

public:
    Task(const string& title, const string& description, const Date& dueDate)
        : title(title), description(description), dueDate(dueDate) {}

    const string& getTitle() const {
        return title;
    }

    const string& getDescription() const {
        return description;
    }

    void display() const {
        cout << "Title: " << title << "\nDescription: " << description
             << "\nDue Date: " << dueDate << "\n\n";
    }

    const Date& getDueDate() const {
        return dueDate;
    }

    bool validateTitle(const InputValidator& validator) const {
        return validator.isValid(title);
    }

    bool validateDescription(const InputValidator& validator) const {
        return validator.isValid(description);
    }
};

class TaskManager{
private:
    vector<Task> tasks;

public:
    const vector<Task>& getTasks() const {
        return tasks;
    }

    bool addTask(const Task& task) {
        tasks.push_back(task);
        return true; // Indicate success, you can modify this as needed
    }

    void displayTasks() const {
        for (const auto& task : tasks) {
            task.display();
        }
    }

    void deleteTask(const string& title) {
        tasks.erase(remove_if(tasks.begin(), tasks.end(), [&](const Task& task) {
            return task.getTitle() == title;
        }), tasks.end());
    }

    // Sorting by due date and if same then alphabetically
    void sortTasks() {
        sort(tasks.begin(), tasks.end(), [](const Task& a, const Task& b) {
            if (a.getDueDate().year != b.getDueDate().year) {
                return a.getDueDate().year < b.getDueDate().year;
            } else if (a.getDueDate().month != b.getDueDate().month) {
                return a.getDueDate().month < b.getDueDate().month;
            } else {
                return a.getTitle() < b.getTitle();
            }
        });
    }
};

int getValidIntInput() {
    int value;
    while (true) {
        cout << "Enter a valid integer: ";
        cin >> value;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. ";
        } else {
            break;
        }
    }
    return value;
}

bool isDateValid(const Date& date) {
    return (date.day >= 1 && date.day <= 31) &&
           (date.month >= 1 && date.month <= 12) &&
           (date.year >= 2023);  // Assuming the year should be 2023 or later
}

bool isTaskUnique(const TaskManager& taskManager, const string& title) {
    for (const auto& task : taskManager.getTasks()) {
        if (task.getTitle() == title) {
            return false;
        }
    }
    return true;
}

bool getInputAndValidate(const string& prompt, string& input, const InputValidator& validator) {
    cout << prompt;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Clear the newline character left in the buffer
    getline(cin, input);

    while (!validator.isValid(input)) {
        cout << "Invalid input. Please enter a valid value: ";
        getline(cin, input);
    }

    return true;
}

int main() {
    TaskManager taskManager;
    int T;
    cout<<"Enter the no of tasks: "<<endl;
    cin>>T;
    for (int i = 0; i < T; ++i) {
        string title, description;
        int day, month, year;

        InputValidator* alphabeticValidator = new AlphabeticValidator();
        InputValidator* numericValidator = new NumericValidator();

        cout << "Enter title for task #" << (i + 1) << ": ";
        cin.ignore();  // Clear the newline character left in the buffer
        getline(cin, title);

        cout << "Enter description for task #" << (i + 1) << ": ";
        getline(cin, description);

        cout << "Enter due date (day month year format) for task #" << (i + 1) << ":\n";
        cout << "Day: ";
        cin >> day;

        cout << "Month: ";
        cin >> month;

        cout << "Year: ";
        cin >> year;

        if (isDateValid(Date(day, month, year))) {
            taskManager.addTask(Task(title, description, Date(day, month, year)));

            // Save the new task to the file
            ofstream outputFile("tasks.txt", ios::app);
            outputFile << title << " " << description << " " << day << " " << month << " " << year << "\n";
            outputFile.close();
        } else {
            cout << "Failed to add task #" << (i + 1) << ". Please check your input.\n";
            // Skip the newline character in the buffer
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        delete alphabeticValidator;
        delete numericValidator;
    }    

    int option;
    do {
        cout << "\nChoose an option:\n";
        cout << "1. Add a task\n";
        cout << "2. Display tasks\n";
        cout << "3. Delete a task\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin>>option;
        cin.ignore();
        // option = getValidIntInput();

        switch (option) {
        case 1: {
            string title, description;
            int day, month, year;

            InputValidator* alphabeticValidator = new AlphabeticValidator();
            InputValidator* numericValidator = new NumericValidator();

            cout << "Enter title for the new task: ";
            getline(cin, title);

            cout << "Enter description for the new task: ";
            getline(cin, description);

            cout << "Enter due date (day month year format) for the new task:\n";

            cout << "Day: ";
            cin >> day;

            cout << "Month: ";
            cin >> month;

            cout << "Year: ";
            cin >> year;

            if (isDateValid(Date(day, month, year))) {
                taskManager.addTask(Task(title, description, Date(day, month, year)));


                // Save the new task to the file
                ofstream outputFile("tasks.txt", ios::app);
                outputFile << title << " " << description << " " << day << " " << month << " " << year << "\n";
                outputFile.close();
            } else {
                cout << "Failed to add the task. Please check your input.\n";
            }

            delete alphabeticValidator;
            delete numericValidator;
            break;
        }
            case 2:
                taskManager.sortTasks();
                taskManager.displayTasks();
                break;
            case 3: {
                string title;
                cout << "Enter the title of the task to delete: ";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                getline(cin, title);
                taskManager.deleteTask(title);
                break;
            }
            case 4:
                cout << "Exiting the program.\n";
                break;
            default:
                cout << "Invalid choice. Please enter a valid option.\n";
                break;
    }
    } while (option != 4);

    return 0;
}
