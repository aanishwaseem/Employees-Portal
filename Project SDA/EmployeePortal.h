#pragma once
#include "Employee.h"
#include "IApplyLeave.h"
class EmployeePortal {
    Employee* emp;
    int inactive = 0;

public:
    EmployeePortal(Employee* employee) : emp(employee) {} // dependency injection

    void EmployeeMenu() {
        bool open = true;
        while (open) {
            system("cls");
            displayMenu();

            int type = -1;
            cin >> type;

            switch (type) {
            case 0:
                open = false;
                break;
            case 1:
                emp->generateAttendenceReport(); // generate report
                break;
            case 2:
                emp->generateLeaveSummary();// generate report
                break;
            case 3:
                handleLeaveApplication();// generate report
                break;
            case 4:
                NotificationManager::getInstance()->getNotificationsForEmployee(emp->getID()); //get notiifications
                break;
            default:
                cout << "Invalid option. Try again.\n";
            }
            if (open) {
                system("pause");
            }
        }
    }

private:
    void displayMenu() { // display menu
        cout << "Employee Menu:\n";
        if (emp->inactive > 0) {
            cout << "Your status: You are on a leave for another " << emp->inactive << " days.\n";
        }
        else {
            cout << "Your status: You are not on any leave.\n";
        }

        cout << "1. Check Attendance Details\n"
            << "2. Check Leave Details\n"
            << "3. Apply Leave\n"
            << "4. Check Notifications\n"
            << "0. Go Back\n\n";
    }


    void handleLeaveApplication() { // different applications for dif leave types
        cout << "Enter Leave Type:\n"
            << "1. Casual\n"
            << "2. Earned\n"
            << "3. Official\n"
            << "4. Unpaid\n"
            << "0. Go Back\n";
        int option = -1;
        cin >> option;

        switch (option) {
        case 0:
            return;
        case 1:
            applyLeave("Casual");
            break;
        case 2:
            applyLeave("Earned");
            break;
        case 3:
            applyOfficialLeave();
            break;
        case 4:
            applyUnpaidLeave();
            break;
        default:
            cout << "Invalid leave type.\n";
            break;
        }
    }

    void applyLeave(const string& leaveType) {
        string days, date, from, to, reason;
        gatherLeaveDetails(days, date, from, to, reason);

        IApplication* newapp = new DefaultLeaveApplication(
            emp->getID(), emp->getNextLeaveID(), leaveType, days, date, to, from, reason);
        cout << "Leave application forwarded to system!\n"; //system subbmited leave application
        if (leaveType == "Casual")
            IApplyLeave::getInstance()->applyLeave(emp->getID(), newapp, Casual::getInstance());  // application forwarded
        else
            IApplyLeave::getInstance()->applyLeave(emp->getID(), newapp, Earned::getInstance());  // application forwarded

    }

    void applyOfficialLeave() {
        string days, date, from, to, reason, travelAllowance, destination, purpose;
        gatherLeaveDetails(days, date, from, to, reason);

        cout << "Enter Amount Allocated for travelling: ";
        getline(cin, travelAllowance);
        cout << "Enter Destination: ";
        getline(cin, destination);
        cout << "Enter Main Purpose: (no space)";
        getline(cin, purpose);

        IApplication* newapp = new OfficialLeaveApp(
            emp->getID(), emp->getNextLeaveID(), "Official", days, date, to, from, reason,
            travelAllowance, purpose, destination);
        cout << "Leave application forwarded to system!\n"; //system subbmited leave application

        IApplyLeave::getInstance()->applyLeave(emp->getID(), newapp, Official::getInstance()); // application forwarded
    }

    void applyUnpaidLeave() {
        string days, date, from, to, reason, leaveAddress;
        gatherLeaveDetails(days, date, from, to, reason);

        cout << "Enter Leave address: ";
        getline(cin, leaveAddress);

        IApplication* newapp = new UnpaidLeaveApp(
            emp->getID(), emp->getNextLeaveID(), "Unpaid", days, date, to, from, reason, leaveAddress);
        cout << "Leave application forwarded to system!\n"; //system subbmited leave application

        IApplyLeave::getInstance()->applyLeave(emp->getID(), newapp, Unpaid::getInstance()); // application forwarded
    }

    void gatherLeaveDetails(string& days, string& date, string& from, string& to, string& reason) {
        string e;
        cout << "press any key to proceed "; // to remove spaces

        getline(cin, e);
        cin.ignore(numeric_limits<streamsize>::max(), '\n');


        cout << "Enter Number of Days: ";
        getline(cin, days);
        cout << "Enter Date of Application (dd-mm-yyyy): ";
        getline(cin, date);
        cout << "Enter Starting Date (dd-mm-yyyy): ";
        getline(cin, from);
        cout << "Enter Ending Date (dd-mm-yyyy): ";
        getline(cin, to);
        cout << "Enter Reason of Leave: (no space)";
        getline(cin, reason); // basic leave details
    }
};