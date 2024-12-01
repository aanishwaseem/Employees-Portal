#pragma once
#include "Signup.h"
#include "Director.h"
#include "Supervisor.h"

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
			cout << "Log In As:\n"
				<< "1.Guard\n"
				<< "2.Employee\n"
				<< "3.Admin\n"
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
		string password;		int type = 0;

		cout << "Admin Type:\n"
			<< "1.Director\n"
			<< "2.Supervisor\n"
			<< endl;
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
		while (type < 0 || type > 3) {

			cin >> type;
			if (type < 0 || type > 3)
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
	void loadUsers(const string& filename, const string& type) {
		ifstream file(filename);
		if (file.is_open()) {
			int id;
			string pass;
			while (file >> id >> pass) {
				if (type == "Employee") {
					users.push_back(new Employee(id, pass));
				}
				else if (type == "Director") {
					users.push_back(new Director(id, pass));
				}
				else if (type == "Supervisor") {
					users.push_back(new Supervisor(id, pass));
				}
				else if (type == "Guard") {
					users.push_back(new Guard(id, pass));
				}
			}
			file.close();
		}
	}

	MainPortal() {
		loadUsers("employees.txt", "Employee");
		loadUsers("admins.txt", "Director");
		loadUsers("admins.txt", "Supervisor");
		loadUsers("guards.txt", "Guard");
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
		signup.open();
	}

};


