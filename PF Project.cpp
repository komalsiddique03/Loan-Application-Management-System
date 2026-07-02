#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <windows.h>
using namespace std;

// -------------------- Function Declarations --------------------
void eligibility_check();                // Check if user is eligible for loan
void new_application(int& count);        // Add a new loan application
string eligibility(short age, int salary, int loan_amount); // Determine eligibility
void view_Applications();                // Display all applications
void search_application();               // Search application by ID
void statistics();                       // Display statistics of applications
void delete_application();               // Delete application by ID
void update_application();               // Update existing application
void setColor(int color);                // Change console text color
void loadApplications();                 // Load applications from file into memory
void resizeApps();                       // Resize dynamic array when capacity is full

// -------------------- Structure to store loan application --------------------
struct application {
    int id;               // Application ID
    string name;          // Applicant name (single word)
    short age;            // Applicant age
    int salary;           // Applicant salary
    string interest_rate; // Interest rate for selected loan category
    int loan_amount;      // Requested loan amount
    int payable;          // Total payable amount (loan + interest)
    string status;        // Loan eligibility status ("eligible" / "not_eligible")
};

// -------------------- Global Variables --------------------
application* apps;      // Dynamic array to store applications
int appCount = 0;       // Current number of applications
int capacity = 2;       // Initial capacity of dynamic array

// -------------------- Function to resize dynamic array --------------------
void resizeApps() {
    capacity *= 2;  // Double the capacity
    application* temp = new application[capacity];
    for (int i = 0; i < appCount; i++)
        temp[i] = apps[i];   // Copy old data to new array
    delete[] apps;            // Free old memory
    apps = temp;              // Point apps to new array
}

// -------------------- Load applications from file --------------------
void loadApplications() {
    ifstream fin("record.txt");
    if (!fin) return; // File doesn't exist yet

    appCount = 0;
    while (fin >> apps[appCount].id
        >> apps[appCount].name
        >> apps[appCount].age
        >> apps[appCount].salary
        >> apps[appCount].interest_rate
        >> apps[appCount].loan_amount
        >> apps[appCount].payable
        >> apps[appCount].status) {
        appCount++;
        if (appCount >= capacity)
            resizeApps(); // Resize if array is full
    }
    fin.close();
}

// -------------------- Get next unique ID --------------------
int load_last_id() {
    if (appCount == 0) return 1;
    return apps[appCount - 1].id + 1;
}

// -------------------- Main Program --------------------
int main() {
    // Initialize dynamic array
    apps = new application[capacity];

    // Welcome message with color
    setColor(11);
    cout << "------------------ Welcome to Loan Application Management System ----------------------\n";
    setColor(7);

    string password;
    int attempts = 0;
    bool loggedIn = false;

    // -------------------- Password login --------------------
    while (attempts < 3) {
        cout << "Enter password: ";
        cin >> password;

        if (password == "1234") {
            loggedIn = true;
            setColor(10);
            cout << "Login successful!\n";
            setColor(7);
            MessageBox(0, L"Login Successful", L"Loan System", MB_OK);
            Sleep(800);
            system("cls");  // Clear console after successful login
            break;
        }
        else {
            Beep(600, 300); // Error beep
            setColor(12);
            cout << "Incorrect password! Try again...\n";
            setColor(7);
            MessageBox(0, L"Wrong Password", L"Error", MB_ICONERROR);
            attempts++;
        }
    }

    // Exit program if maximum login attempts reached
    if (!loggedIn) {
        Beep(500, 400); // Warning beep
        MessageBox(0, L"Maximum login attempts reached", L"Access Denied", MB_ICONERROR);
        setColor(12);
        cout << "Sorry! You have reached maximum login attempts.\n";
        setColor(7);
        delete[] apps; // Free memory before exit
        return 0;
    }

    loadApplications();          // Load existing applications
    int count = load_last_id();  // Initialize next application ID

    int choice;
    do {
        // -------------------- Display menu --------------------
        setColor(11);
        cout << "\n---------- MENU ----------\n";
        setColor(14);
        cout << "1. Add Loan Application\n";
        cout << "2. Check Eligibility\n";
        cout << "3. View All Applications\n";
        cout << "4. Search an Application\n";
        cout << "5. Update an Application\n";
        cout << "6. Delete an Application\n";
        cout << "7. View Statistics\n";
        cout << "8. Exit\n";
        setColor(7);

        cin >> choice;

        // -------------------- Menu actions --------------------
        switch (choice) {
        case 1: new_application(count); break;
        case 2: eligibility_check(); break;
        case 3: view_Applications(); break;
        case 4: search_application(); break;
        case 5: update_application(); break;
        case 6: delete_application(); break;
        case 7: statistics(); break;
        case 8:
            setColor(14);
            cout << "Exiting system...\n";
            setColor(7);
            break;
        default:
            Beep(600, 300); // Error beep
            setColor(12);
            cout << "Invalid input! Please enter a number from 1-8.\n";
            setColor(7);
        }

        // -------------------- Pause before returning to menu --------------------
        if (choice != 8) {
            cout << "\nPress Enter to return to menu...";
            cin.ignore();
            cin.get();
            system("cls"); // Clear console for neat menu display
        }

    } while (choice != 8);

    delete[] apps; // Free dynamic memory
    return 0;
}

// -------------------- Check loan eligibility --------------------
void eligibility_check() {
    int salary, loan_amount;
    short age;

    cout << "Enter salary: ";
    cin >> salary;
    cout << "Enter age: ";
    cin >> age;
    cout << "Enter loan amount: ";
    cin >> loan_amount;

    if (salary < 50000) setColor(12), cout << "Not eligible: Salary must be at least 50000\n";
    else if (age < 18 || age > 60) setColor(12), cout << "Not eligible: Age must be between 18 and 60\n";
    else if (loan_amount > 1000000) setColor(12), cout << "Not eligible: Loan limit exceeded\n";
    else setColor(10), cout << "You are eligible for the loan\n";
    setColor(7);
}

// -------------------- Add new application --------------------
void new_application(int& count) {
    if (appCount >= capacity)
        resizeApps(); // Resize dynamic array if needed

    application newApp;
    newApp.id = count;

    short category;
    float rate;

    cout << "Enter Name: ";
    cin >> newApp.name;
    cout << "Enter Age: ";
    cin >> newApp.age;
    cout << "Enter Salary: ";
    cin >> newApp.salary;

    do {
        cout << "Loan Category:\n1. Personal\n2. Home\n3. Education\n4. Business\n";
        cin >> category;
    } while (category < 1 || category > 4);

    switch (category) {
    case 1: rate = 12; newApp.interest_rate = "12%"; break;
    case 2: rate = 8; newApp.interest_rate = "8%"; break;
    case 3: rate = 5; newApp.interest_rate = "5%"; break;
    case 4: rate = 10; newApp.interest_rate = "10%"; break;
    }

    cout << "Enter Loan Amount: ";
    cin >> newApp.loan_amount;
    newApp.payable = newApp.loan_amount + (newApp.loan_amount * rate / 100);
    newApp.status = eligibility(newApp.age, newApp.salary, newApp.loan_amount);

    if (newApp.status == "eligible") setColor(10); // Green for eligible
    else setColor(12);                             // Red for not eligible
    cout << "You are " << newApp.status << " for the loan.\n";
    setColor(7);

    // Save to memory and file
    apps[appCount] = newApp;
    appCount++;

    ofstream fout("record.txt", ios::app);
    fout << left
        << setw(5) << newApp.id
        << setw(15) << newApp.name
        << setw(6) << newApp.age
        << setw(12) << newApp.salary
        << setw(15) << newApp.interest_rate
        << setw(15) << newApp.loan_amount
        << setw(15) << newApp.payable
        << setw(15) << newApp.status << endl;
    fout.close();

    Beep(800, 200); // Success beep
    MessageBox(0, L"Application Added Successfully", L"Success", MB_OK);
    count++;
}

// -------------------- Determine eligibility --------------------
string eligibility(short age, int salary, int loan_amount) {
    if (salary >= 50000 && age >= 18 && age <= 60 && loan_amount <= 1000000)
        return "eligible";
    return "not_eligible";
}

// -------------------- Display all applications --------------------
void view_Applications() {
    if (appCount == 0) {
        Beep(600, 300);
        cout << "No applications found!\n";
        return;
    }

    setColor(11); // Cyan headers
    cout << left
        << setw(5) << "ID"
        << setw(15) << "Name"
        << setw(6) << "Age"
        << setw(12) << "Salary"
        << setw(15) << "Interest"
        << setw(15) << "LoanAmount"
        << setw(15) << "Payable"
        << setw(15) << "Status" << endl;
    setColor(7);

    for (int i = 0; i < appCount; i++) {
        cout << left
            << setw(5) << apps[i].id
            << setw(15) << apps[i].name
            << setw(6) << apps[i].age
            << setw(12) << apps[i].salary
            << setw(15) << apps[i].interest_rate
            << setw(15) << apps[i].loan_amount
            << setw(15) << apps[i].payable;
        if (apps[i].status == "eligible") setColor(10);
        else setColor(12);
        cout << setw(15) << apps[i].status << endl;
        setColor(7);
    }
}

// -------------------- Search application by ID --------------------
void search_application() {
    int search_id;
    cout << "Enter Application ID: ";
    cin >> search_id;
    for (int i = 0; i < appCount; i++) {
        if (apps[i].id == search_id) {
            MessageBox(0, L"Application Found", L"Success", MB_OK);
            setColor(10);
            cout << "\nApplication Found!\n";
            setColor(7);

            cout << "ID: " << apps[i].id
                << "\nName: " << apps[i].name
                << "\nAge: " << apps[i].age
                << "\nSalary: " << apps[i].salary
                << "\nInterest: " << apps[i].interest_rate
                << "\nLoan Amount: " << apps[i].loan_amount
                << "\nPayable: " << apps[i].payable;
            if (apps[i].status == "eligible") setColor(10);
            else setColor(12);
            cout << "\nStatus: " << apps[i].status << endl;
            setColor(7);
            return;
        }
    }
    Beep(600, 300);
    MessageBox(0, L"Application Not Found", L"Error", MB_ICONERROR);
    setColor(12);
    cout << "Application not found!\n";
    setColor(7);
}

// -------------------- Display statistics --------------------
void statistics() {
    if (appCount == 0) {
        cout << "No applications found!\n";
        return;
    }

    int eligibleCount = 0;
    for (int i = 0; i < appCount; i++)
        if (apps[i].status == "eligible") eligibleCount++;

    setColor(11);
    cout << "\n--- Statistics ---\n";
    setColor(7);
    setColor(12);
    cout << "Total applications: " << appCount << endl;
    cout << "Eligible: " << eligibleCount << endl;
    cout << "Not Eligible: " << (appCount - eligibleCount) << endl;
    setColor(7);
}

// -------------------- Update application --------------------
void update_application() {
    if (appCount == 0) {
        cout << "No applications found!\n";
        return;
    }

    int id;
    cout << "Enter Application ID to update: ";
    cin >> id;

    for (int i = 0; i < appCount; i++) {
        if (apps[i].id == id) {
            setColor(11);
            cout << "Updating application of " << apps[i].name << "\n";
            setColor(7);

            cout << "Enter new Name: ";
            cin >> apps[i].name;
            cout << "Enter new Age: ";
            cin >> apps[i].age;
            cout << "Enter new Salary: ";
            cin >> apps[i].salary;

            short category;
            float rate;
            do {
                cout << "Loan Category:\n1. Personal\n2. Home\n3. Education\n4. Business\n";
                cin >> category;
            } while (category < 1 || category > 4);

            switch (category) {
            case 1: rate = 12; apps[i].interest_rate = "12%"; break;
            case 2: rate = 8; apps[i].interest_rate = "8%"; break;
            case 3: rate = 5; apps[i].interest_rate = "5%"; break;
            case 4: rate = 10; apps[i].interest_rate = "10%"; break;
            }

            cout << "Enter new Loan Amount: ";
            cin >> apps[i].loan_amount;
            apps[i].payable = apps[i].loan_amount + (apps[i].loan_amount * rate / 100);
            apps[i].status = eligibility(apps[i].age, apps[i].salary, apps[i].loan_amount);

            // Rewrite all applications to file
            ofstream fout("record.txt");
            for (int j = 0; j < appCount; j++) {
                fout << left
                    << setw(5) << apps[j].id
                    << setw(15) << apps[j].name
                    << setw(6) << apps[j].age
                    << setw(12) << apps[j].salary
                    << setw(15) << apps[j].interest_rate
                    << setw(15) << apps[j].loan_amount
                    << setw(15) << apps[j].payable
                    << setw(15) << apps[j].status << endl;
            }
            fout.close();

            Beep(800, 200);
            MessageBox(0, L"Application Updated Successfully", L"Success", MB_OK);
            return;
        }
    }

    Beep(600, 300);
    cout << "Application not found!\n";
}

// -------------------- Delete application --------------------
void delete_application() {
    if (appCount == 0) {
        cout << "No applications found!\n";
        return;
    }

    int id;
    cout << "Enter Application ID to delete: ";
    cin >> id;

    for (int i = 0; i < appCount; i++) {
        if (apps[i].id == id) {
            for (int j = i; j < appCount - 1; j++)
                apps[j] = apps[j + 1]; // Shift left
            appCount--;

            // Rewrite all applications to file
            ofstream fout("record.txt");
            for (int j = 0; j < appCount; j++) {
                fout << left
                    << setw(5) << apps[j].id
                    << setw(15) << apps[j].name
                    << setw(6) << apps[j].age
                    << setw(12) << apps[j].salary
                    << setw(15) << apps[j].interest_rate
                    << setw(15) << apps[j].loan_amount
                    << setw(15) << apps[j].payable
                    << setw(15) << apps[j].status << endl;
            }
            fout.close();

            Beep(800, 200);
            MessageBox(0, L"Application Deleted Successfully", L"Success", MB_OK);
            return;
        }
    }

    Beep(600, 300);
    cout << "Application not found!\n";
}

// -------------------- Set console text color --------------------
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

