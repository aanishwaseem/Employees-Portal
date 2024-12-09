#pragma once
#include "Signup.h"
#include "Director.h"
#include "Supervisor.h"
#include "Main.h"
#include "GuardPortal.h"
#include "SupervisorPortal.h"
#include "DirectorPortal.h"
#include "EmployeePortal.h"
#include <iostream>
using namespace std;
class SystemPortal {

	static Person* findUserById(int id, vector<Person*>& users, string& type) {
		for (Person* user : users) {
			if (user->getID() == id &&
				(user->getType() == "Supervisor" || user->getType() == "Director")) {
				type = user->getType();
				return user;
			}
		}
		return nullptr;
	}
	static void manageApproversMenu(vector<Person*>& users) {
		int leaveTypeChoice;
		cout << "Manage Approvers:" << endl;
		cout << "1. Casual Leave" << endl;
		cout << "2. Earned Leave" << endl;
		cout << "3. Official Leave" << endl;
		cout << "4. Unpaid Leave" << endl;
		cout << "Enter your choice: ";
		cin >> leaveTypeChoice;

		Leaves_Team* leave = nullptr; // Pointer to the selected leave type
		switch (leaveTypeChoice) {
		case 1: leave = Casual::getInstance(); break;
		case 2: leave = Earned::getInstance(); break;
		case 3: leave = Official::getInstance(); break;
		case 4: leave = Unpaid::getInstance(); break;
		default:
			cout << "Invalid choice! Returning to main menu." << endl;
			return;
		}

		cout << "Listing all admins (Directors and Supervisors):" << endl;
		for (Person* user : users) {
			if (user->getType() == "Supervisor" || user->getType() == "Director") {
				cout << "ID: " << user->getID() << ", Type: " << user->getType() << endl;
			}
		}

		int id;
		cout << "Enter the ID of the user to add as an approver for this leave: ";
		cin >> id;
		string type;
		Person* approver = findUserById(id, users,type);
		if (approver) {
			if (type == "Supervisor")
				leave->addAdmin(static_cast<Supervisor*>(approver));
			else
				leave->addAdmin(static_cast<Director*>(approver));

			cout << "New approver added for this leave." << endl;
		}
		else {
			cout << "Invalid ID! No user found with the given ID." << endl;
		}

		leave = Casual::getInstance();
		leave = leave;
		leave = leave;
	}
public:
	static void systemPortal(vector<Person*>& users) {
		int choice;
		InactivityManager* inactives = InactivityManager::getInstance(); // Assuming a Singleton class
		MyTime* time = MyTime::getInstance(); // Assuming MyTime is a Singleton class

		do {
			cout << endl;
			cout << "--------------------------------------------" << endl;
			cout << "           System Portal Menu               " << endl;
			cout << "--------------------------------------------" << endl;
			cout << "1. Pass Time" << endl;
			cout << "2. Get Inactive Employees" << endl;
			cout << "3. Manage Approvers" << endl;

			cout << "4. Exit" << endl;

			cout << "Enter your choice: ";
			cin >> choice;

			switch (choice) {
			case 1: {
				int timeChoice;
				cout << "Pass Time Options:" << endl;
				cout << "1. Pass a Day" << endl; // reduces inactives days
				cout << "2. Pass a Week" << endl;  // reduces inactives days
				cout << "3. Pass a Month" << endl; //generating report
				cout << "4. Pass a Year" << endl; // reduces inactives days
				cout << "Enter your choice: ";
				cin >> timeChoice;

				switch (timeChoice) {
				case 1:
					time->incrementDay(); // reduces inactives days
					cout << "A day has passed." << endl;
					break;
				case 2:
					time->notifyWeekUpdate(); // reduces inactives days
					cout << "A week has passed." << endl;
					break;
				case 3:
					time->notifyMonthUpdate(); 
					cout << "--------------------------------------------" << endl;
					cout << "Generated report and given salaries. Since a new month has started, we are clearing all employees' monthly hours and percentages." << endl;
					cout << "--------------------------------------------" << endl;
					break;
				case 4:
					time->notifyYearUpdate(); // reduces inactives days
					cout << "A year has passed." << endl;
					break;
				default:
					cout << "Invalid option! Please choose again." << endl;
				}
				break;
			}
			case 2:
				inactives->displayInactives(); // Assuming MyTime is a Singleton class
				break;
			case 3:

				manageApproversMenu(users);
				break;
			case 4:
				cout << "Exiting System Portal..." << endl;
				break;
			default:
				cout << "Invalid choice! Please try again." << endl;
			}
		} while (choice != 3);
	}

};
class Login {
private:
	vector<Person*> users;
public:
	Login(vector<Person*> userS) { users = userS; }

	void open()
	{
		bool open = true;
		while (open) {
			system("cls");
			cout << "Current time: " << MyTime::Day <<"-"<<MyTime::Month <<"-"<<MyTime::Year << endl;
			cout << "Current Week: " << MyTime::Week << endl;
			cout << "Log In As:\n"
				<< "1. Guard\n"
				<< "2. Employee\n"
				<< "3.Admin\n"
				<< "4.System\n"
				<< "0.Go Back\n"
				<< endl;
			int type = -1;
			getType(type);
			if (type == 0)
			{
				return;
			}
			if (type == 1) // guard
			{
				open = guardLogin();
			}
			else if (type == 2) // employee
			{
				open = employeeLogin();
			}
			else if (type == 4) // employee
			{
				SystemPortal::systemPortal(users);
				open = false;
			}
			else // Admin
			{
				open = adminLogin();
			}
			if (open) {
				cerr << "login unsuccessful\n";
				system("pause");
			}
		}
	}

	bool guardLogin()
	{
		string name;
		int id;
		string password;		getCredentials(id, password);
		Person* user = verifyLogin("Guard", id, password);
		if (user)
		{
			cout << "Logged in as Guard: " << id << endl;
			GuardPortal* gP = new GuardPortal(static_cast<Guard*>(user));
			gP->markattendenceportal();
		}
		else
		{
			return true;
		}
	}


	bool employeeLogin()
	{
		string name;
		int id;
		string password;
		getCredentials(id, password);
		Person* user = verifyLogin("Employee", id, password);

		if (user)
		{
			Employee* emp = static_cast<Employee*>(user);
			cout << "Logged in as Employee: " << id << endl;
			EmployeePortal* eP = new EmployeePortal(emp);
			InactivityManager* inactives = InactivityManager::getInstance();
			emp->setInactive(inactives->isInactive(emp->getID()));
			eP->EmployeeMenu();
		}
		else
		{
			return true;
		}
	}

	bool adminLogin()
	{
		string name;
		int id;
		string password;		int type;

		cout << "Admin Type:\n"
			<< "1.Director\n"
			<< "2.Supervisor\n"
			<< endl;
		cin >> type;
		getType(type);

		if (type == 1) // Director
		{
			getCredentials(id, password);
			Person* user = verifyLogin("Director", id, password);
			if (user)
			{
				cout << "Logged in as Director: " << id << endl;
				DirectorPortal* dP = new DirectorPortal(static_cast<Director*>(user));
				dP->DirectorPortalDisplay();
			}
			else
			{
				return true;
			}
		}
		else if (type == 2) // Supervisor
		{
			getCredentials(id, password);
			Person* user = verifyLogin("Supervisor", id, password);
			if (user) {
				cout << "Logged in as Supervisor: " << id << endl;
				SupervisorPortal* sP = new SupervisorPortal(static_cast<Supervisor*>(user));
				sP->SupervisorPortalDisplay();
			}
			else
			{
				return true;
			}

		}
	}

	void getCredentials(int& id, string& password)
	{
		cout << "Enter Id:";
		cin >> id;
		cout << "Enter Password:";
		cin >> password;

	}

	void getType(int& type)
	{
		while (type < 0 || type > 4) {

			cin >> type;
			if (type < 0 || type > 4)
				cout << "Enter valid input\n";
		}
	}

	Person* verifyLogin(const string& type, int& id, string& password) {
		for (auto& user : users) {
			if (user->getType() == type && user->getID() == id && user->getPass() == password) {
				return user; // Return the matching Person pointer
			}
		}
		return nullptr; // Return nullptr if no match is found
	}
};

class MainConfiguration {
	static void loadAdmins(const string& filename, vector<Person*>& users) {
		ifstream file(filename);
		if (file.is_open()) {
			int id;
			string pass, type;
			bool admin;
			while (file >> id >> pass >> type >> admin) {
				if (type == "Director") {
					Director* d = new Director(id, pass);
					users.push_back(d);
					if (admin) {
						(Unpaid::getInstance())->addAdmin(d);
						(Earned::getInstance())->addAdmin(d);
					}
				}
				if (type == "Supervisor") {
					Supervisor* s = new Supervisor(id, pass);
					users.push_back(s);
					if (admin) {
						(Casual::getInstance())->addAdmin(s);
						(Official::getInstance())->addAdmin(s);

						(Unpaid::getInstance())->addAdmin(s);
						(Earned::getInstance())->addAdmin(s);
					}

				}

			}


			file.close();
		}
	}
	static void loadUsers(const string& filename, const string& type, vector<Person*>& users) {
		ifstream file(filename);
		if (file.is_open()) {
			int id, inactives;
			string pass;
			while (file >> id >> pass) {
				if (type == "Employee") {
					file >> inactives;
					Employee* emp = new Employee;
					emp = emp->initializeEmployee(id, pass);
					users.push_back(emp);
				}
				else if (type == "Guard") {
					users.push_back(new Guard(id, pass));
				}

			}
			file.close();
		}
	}
public:
	MainConfiguration() {

	}
	static void configure(vector<Person*>& users) {
		//load users
		loadAdmins("admins.txt", users);
		loadUsers("guards.txt", "Guard", users);
		loadUsers("employees.txt", "Employee", users);

		//load notifications
		NotificationManager* notes = NotificationManager::getInstance();
		notes->loadNotificationsFromFile();
	}
};



class MainPortal {
private:
	static MainPortal* main;

	vector<Person*> users;

	

	MainPortal() {
		MainConfiguration::configure(users);
	}
public:
	static MainPortal* getMain() {
		if (main == nullptr) {
			main = new MainPortal();

		}
		return main;
	}
	void start() {
		int input;
		input = -1;
		while (input) {
			input = -1;
			system("cls");
			cout << "Welcome to the Leave & Attendance Managemet System!" << endl;
			cout << "--Menu--\n"
				<< "0.Exit\n"
				<< "1.Log In\n"
				<< "2.Sign Up\n"
				<< endl;
			while (input < 0 || input > 2) {
				cin >> input;
				if (input == 1)
					logIn();
				else if (input == 2)
					signUp();
				else if (input == 0)
					cout << "Program Terminated!\n";
				else
					cout << "Enter valid input\n";
			}
			if (!input)
			{
				system("pause");
			}
		}
		cout << "Thanks For Using Attendance Management System" << endl;
	}
	void logIn() {
		system("cls");
		cout << "Log In Page\n";
		Login login(users);
		login.open();
	}
	void signUp() {
		system("cls");
		cout << "Sign Up Page\n";
		Signup signup;
		int id;
		string pass;
		signup.open(id, pass);
		Employee* emp = new Employee;
		emp=emp->initializeEmployee(id, pass);
		users.push_back(emp);
	}

};


