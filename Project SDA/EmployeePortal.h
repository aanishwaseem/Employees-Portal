#pragma once
#include "Employee.h"
#include "IApplyLeave.h"
class EmployeePortal {
	Employee* emp;
	int inactive = 0;
private:
public:
	EmployeePortal(Employee* employee) {
		emp = employee;
	}
	void EmployeeMenu()
	{
		//EmployeeMenu
		bool open = true;
		while (open) {
			system("cls");
			cout << "Employee Menu:\n";
				// Check inactivity status
				if (emp->inactive > 0) {
					cout << "Your status: You are on a leave for another " << emp->inactive << " days.\n";
				}
				else {
					cout << "Your status: You are not on any leave.\n";
				} 
			
			cout << "1.Check Attendance Details\n"
				<< "2.Check Leave Details\n"
				<< "3.Apply Leave\n"
				<< "4.Check Notifications\n"
				<< "0.Go Back\n"
				<< endl;
			int type = -1;
			getType(type);
			if (type == 0)
			{
				return;
			}
			if (type == 1)
			{
				emp->generateAttendenceReport();
				//call Attendance Details Function by emp id
			}
			else if (type == 2)
			{
				emp->generateLeaveSummary();
				//call Leave Details Function by emp id

			}
			else if (type == 3)
			{
				cout << "Enter Leave Type:\n"
					<< "1.Casual\n"
					<< "2.Earned\n"
					<< "3.Official\n"
					<< "4.Unpaid\n"
					<< "0.Go Back\n"
					<< endl;
				int option = -1;
				getType(option);
				string e;
				getline(cin,e);
				cin.ignore(numeric_limits<streamsize>::max(), '\n');

				if (option == 1)
				{
					cout << " Going to apply Casual Leave : " << endl;
					string days;
					cout << "Enter Number of Days: ";
					getline(cin, days);
					string date;
					cout << "Enter Date of Application(dd-mm-yyyy): ";
					getline(cin, date);
					string from;
					cout << "Enter Starting Date(dd-mm-yyyy): ";
					getline(cin, from);
					string to;
					cout << "Enter Ending Date(dd-mm-yyyy): ";
					getline(cin, to);
					string reason;
					getline(cin, reason);
					IApplication* newapp = new DefaultLeaveApplication(emp->getID(), emp->getNextLeaveID(), "Casual", days, date, to, from, reason);
					IApplyLeave* ApplyManager = IApplyLeave::getInstance();
					ApplyManager->applyLeave(emp->getID(), newapp, Casual::getInstance());
				}
				else if (option == 2)
				{
					cout << " Going to apply Earned Leave : " << endl;
					string days;
					cout << "Enter Number of Days: ";
					getline(cin, days);
					string date;
					cout << "Enter Date of Application(dd-mm-yyyy): ";
					getline(cin, date);
					string from;
					cout << "Enter Starting Date(dd-mm-yyyy): ";
					getline(cin, from);
					string to;
					cout << "Enter Ending Date(dd-mm-yyyy): ";
					getline(cin, to);
					string reason;
					getline(cin, reason);
					IApplication* newapp = new DefaultLeaveApplication(emp->getID(), emp->getNextLeaveID(), "Earned", days, date, to, from, reason);
					IApplyLeave* ApplyManager = IApplyLeave::getInstance();
					ApplyManager->applyLeave(emp->getID(), newapp, Earned::getInstance());
					cout << "Leave Applied!" << endl;
				}
				else if (option == 3)
				{
					cout <<" Going to apply Official Leave : " << endl;
					string days;
					cout << "Enter Number of Days: ";
					getline(cin, days);
					string date;
					cout << "Enter Date of Application(dd-mm-yyyy): ";
					getline(cin, date);
					string from;
					cout << "Enter Starting Date(dd-mm-yyyy): ";
					getline(cin, from);
					string to;
					cout << "Enter Ending Date(dd-mm-yyyy): ";
					getline(cin, to);
					string reason;
					cout << "Enter Reason of Leave: ";
					getline(cin, reason);
					string travelAllowance; // Amount allocated for travel
					cout << "Enter Amount Allocated for travelling: ";
					getline(cin, travelAllowance);
					///////////////////////string purpose;         // Purpose of the leave
					string destination;     // Destination for official duty
					cout << "Enter Destination: ";
					getline(cin, destination);
					string purpose;
					cout << "Enter Main Purpose: ";
					getline(cin, purpose);

					IApplication* newapp = new OfficialLeaveApp(emp->getID(), emp->getNextLeaveID(), "Official", days, date, to, from, reason, travelAllowance, purpose, destination);
					IApplyLeave* ApplyManager = IApplyLeave::getInstance();
					ApplyManager->applyLeave(emp->getID(), newapp, Official::getInstance());
					cout << "Leave Applied!" << endl;
				}
				else if (option == 4)
				{
					cout <<" Going to apply Unpaid Leave : " << endl;
					string days;
					cout << "Enter Number of Days: ";
					getline(cin, days);
					string date;
					cout << "Enter Date of Application(dd-mm-yyyy): ";
					getline(cin, date);
					string from;
					cout << "Enter Starting Date(dd-mm-yyyy): ";
					getline(cin, from);
					string to;
					cout << "Enter Ending Date(dd-mm-yyyy): ";
					getline(cin, to);
					string reason;
					cout << "Enter Reason of Leave: ";
					getline(cin, reason);
					string leaveadd; // Amount allocated for travel
					cout << "Enter Leave address: ";
					getline(cin, leaveadd);
					
					IApplication* newapp = new UnpaidLeaveApp(emp->getID(), emp->getNextLeaveID(), "Unpaid", days, date, to, from, reason, leaveadd);
					IApplyLeave* ApplyManager = IApplyLeave::getInstance();
					ApplyManager->applyLeave(emp->getID(), newapp, Unpaid::getInstance());
					cout << "Leave Applied!" << endl;
				}
			}
			else if (type == 4)
			{
				NotificationManager* notes = NotificationManager::getInstance();
				notes->getNotificationsForEmployee(emp->getID());
				//call Notifications Function by emp 
			}
			system("pause");
		}
	}
	void getType(int& type)
	{
		while (type < 0 || type > 4) {

			cin >> type;
			if (type < 0 || type > 4)
				cout << "Enter valid input\n";
		}
	}
};
//
//class Status {
//	int appID;
//	string state;
//	Status* status;
//public:
//	Status() {}
//	Status(string stat, int id) : appID(id) {
//		//state = stat;
//		if (stat == "Pending")
//		{
//			status = new Pending;
//		}
//		else if (stat == "Approved")
//		{
//			status = new Approved;
//		}
//		else
//		{
//			status = new Rejected;
//		}
//	}
//	//~Status() { delete status; }
//	void setState(string stat, IApplication* app) {
//		//state = stat;
//		if (stat == "Pending")
//		{
//			delete status;
//			status = new Pending;
//		}
//		else if (stat == "Approved")
//		{
//			delete status;
//			status = new Approved;
//		}
//		else
//		{
//			delete status;
//			status = new Rejected;
//		}
//		updateLeaveStatus(app);
//	};
//	string getState() {
//		return state;
//	}
//	void updateLeaveStatus(IApplication* app) {
//		string eid = to_string(app->getEmpId());
//		string filename = eid + "LR.txt"; // File name based on employee ID
//		fstream file(filename, ios::in | ios::out);
//
//		if (!file.is_open()) {
//			cerr << "Unable to open leave record file for employee ID: " << app->getEmpId() << endl;
//			return;
//		}
//
//		string line;
//		stringstream buffer;
//		bool found = false;
//
//		// Read each line and check for the leave record ID
//		while (getline(file, line)) {
//			stringstream ss(line);
//
//			int recordID;
//			// Parse the line
//			ss >> recordID;
//
//			// Check if this is the record to update
//			if (recordID == appID) {
//				found = true;
//
//				// Update the status
//				string format = app->getFileObjectFormat();
//				buffer << format << endl;
//			}
//			else {
//				// Write the unchanged record back to the buffer
//				buffer << recordID << line << endl;
//			}
//		}
//
//		file.close(); // Close the file before reopening for writing
//
//		if (!found) {
//			cerr << "No leave record found with ID: " << appID << endl;
//			return;
//		}
//
//		// Write the updated buffer back to the file
//		ofstream outFile(filename, ios::trunc);
//		if (!outFile.is_open()) {
//			cerr << "Unable to update leave record file for employee ID: " << eid << endl;
//			return;
//		}
//
//		outFile << buffer.str();
//		outFile.close();
//
//		cout << "Leave record with ID " << appID << " successfully updated." << endl;
//	}
//
//};
//class Approved : public Status {
//public:
//	Approved() {}
//};
//class Pending : public Status {
//public:
//	Pending() {}
//};
//class Rejected : public Status {
//public:
//	Rejected() {}
//};