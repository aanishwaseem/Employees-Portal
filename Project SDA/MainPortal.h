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
				<< "1.Guard\n"
				<< "2.Employee\n"
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
				systemPortal();
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
			GuardPortal* gP = new GuardPortal(dynamic_cast<Guard*>(user));
			gP->markattendenceportal();
		}
		else
		{
			return true;
		}
	}
	void systemPortal() {
		int choice;
		InactivityManager* inactives = InactivityManager::getInstance();
		MyTime* time = MyTime::getInstance(); // Assuming MyTime is a Singleton class

		do {
			cout << endl;
			cout << "--------------------------------------------" << endl;
			cout << "           System Portal Menu               " << endl;
			cout << "--------------------------------------------" << endl;
			cout << "1. Pass Time" << endl;
			cout << "2. Get Inactive Employees" << endl;
			cout << "3. Exit" << endl;

			cout << "Enter your choice: ";
			cin >> choice;

			switch (choice) {
			case 1: {
				int timeChoice;
				cout << "Pass Time Options:" << endl;
				cout << "1. Pass a Day" << endl;
				cout << "2. Pass a Week" << endl;
				cout << "3. Pass a Month" << endl;
				cout << "4. Pass a Year" << endl;
				cout << "Enter your choice: ";
				cin >> timeChoice;

				switch (timeChoice) {
				case 1:
					time->incrementDay();
					cout << "A day has passed." << endl;
					break;
				case 2:
					time->notifyWeekUpdate();
					cout << "A week has passed." << endl;
					break;
				case 3:
					time->notifyMonthUpdate();
					cout << "--------------------------------------------" << endl;
					cout << "PS: Generated report and given salaries. Since a new month has started, we are clearing all employees' monthly hours and percentages." << endl;
					cout << "--------------------------------------------" << endl;
					break;
				case 4:
					time->notifyYearUpdate();
					cout << "A year has passed." << endl;
					break;
				default:
					cout << "Invalid option! Please choose again." << endl;
				}
				break;
			}
			case 2:
				inactives->displayInactives();
				break;
			case 3:
				cout << "Exiting System Portal..." << endl;
				break;
			default:
				cout << "Invalid choice! Please try again." << endl;
			}
		} while (choice != 3);
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
			cout << "Logged in as Employee: " << id << endl;
			EmployeePortal* eP = new EmployeePortal(dynamic_cast<Employee*>(user));
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
				DirectorPortal* dP = new DirectorPortal(dynamic_cast<Director*>(user));
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
				SupervisorPortal* sP = new SupervisorPortal(dynamic_cast<Supervisor*>(user));
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

class MainPortal {
private:
	static MainPortal* main;

	vector<Person*> users;
	void loadAdmins(const string& filename, const string& type, vector<IApplicationObservable*>& shortapprovers, vector<IApplicationObservable*>& longapprovers) {
		ifstream file(filename);
		if (file.is_open()) {
			int id;
			string pass;
			while (file >> id >> pass) {
				if (type == "Director") {
					Director* d = new Director(id, pass);
					users.push_back(d);
					longapprovers.push_back(d);
				}
				if (type == "Supervisor") {
					Supervisor* s = new Supervisor(id, pass);
					users.push_back(s);
					shortapprovers.push_back(s);
					longapprovers.push_back(s);

				}

			}

			
			NotificationManager* notes = NotificationManager::getInstance();
			notes->loadNotificationsFromFile();
			file.close();
		}
	}
		void loadUsers(const string & filename, const string & type) {
			ifstream file(filename);
			if (file.is_open()) {
				int id;
				string pass;
				while (file >> id >> pass) {
					if (type == "Employee") {

						Main* system = Main::getMain();
						Employee* emp = new Employee(id, pass);
						InactivityManager* inactives = InactivityManager::getInstance();
						emp->initializeEmployee(inactives->isInactive(emp->getID()), system->generateEmployeeRecord(emp->getID()), system->createLeaveRecord(emp->getID()));

						users.push_back(emp);
					}
					else if (type == "Guard") {
						users.push_back(new Guard(id, pass));
					}

				}
				file.close();
			}
		}
	

	MainPortal() {
		vector<IApplicationObservable*> ShortLeaveApprovers;
		vector<IApplicationObservable*> LongLeaveApprovers;
		loadAdmins("admins.txt", "Supervisor", ShortLeaveApprovers, LongLeaveApprovers);

		loadAdmins("admins.txt", "Director", ShortLeaveApprovers, LongLeaveApprovers);
		Casual::getInstance(ShortLeaveApprovers);
			Official::getInstance(ShortLeaveApprovers);
			Earned::getInstance(LongLeaveApprovers);
			Unpaid::getInstance(LongLeaveApprovers);

		loadUsers("guards.txt", "Guard");
		loadUsers("employees.txt", "Employee");

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
		Employee* emp = signup.open();
		Main* system = Main::getMain();
		InactivityManager* inactives = InactivityManager::getInstance();
		emp->initializeEmployee(0, system->generateEmployeeRecord(emp->getID()), system->createLeaveRecord(emp->getID()));

		users.push_back(emp);
	}

};


