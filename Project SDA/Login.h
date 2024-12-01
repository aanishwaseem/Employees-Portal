//#pragma once
//#include "GuardPortal.h"
//#include "SupervisorPortal.h"
//#include "DirectorPortal.h"
//#include "EmployeePortal.h"
//#include "Director.h"
//#include "Supervisor.h"
//
//class Login {
//private:
//	vector<Person*> users;
//public:
//	Login(vector<Person*> userS) { users = userS; }
//
//	void open()
//	{
//		bool open = true;
//		while (open) {
//			system("cls");
//			cout << "Log In As:\n"
//				<< "1.Guard\n"
//				<< "2.Employee\n"
//				<< "3.Admin\n"
//				<< "0.Go Back\n"
//				<< endl;
//			int type = -1;
//			getType(type);
//			if (type == 0)
//			{
//				return;
//			}
//			if (type == 1) // guard
//			{
//				open = guardLogin();
//			}
//			else if (type == 2) // employee
//			{
//				open = employeeLogin();
//			}
//			else // Admin
//			{
//				open = adminLogin();
//			}
//			if (open) {
//				cerr << "login unsuccessful\n";
//				system("pause");
//			}
//		}
//	}
//
//	bool guardLogin()
//	{
//		string name;
//		int id, password;
//		getCredentials(id, password);
//		Person* user = verifyLogin("Guard", id, password);
//		if (user)
//		{
//			cout << "Logged in as Guard: " << id << endl;
//			GuardPortal* gP = new GuardPortal(dynamic_cast<Guard*>(user));
//			gP->markattendenceportal();
//		}
//		else
//		{
//			return true;
//		}
//	}
//
//	bool employeeLogin()
//	{
//		string name;
//		int id, password;
//		getCredentials(id, password);
//		Person* user = verifyLogin("Employee", id, password);
//
//		if (user)
//		{
//			cout << "Logged in as Employee: " << id << endl;
//			EmployeePortal* eP = new EmployeePortal(dynamic_cast<Employee*>(user));
//			eP->EmployeeMenu();
//		}
//		else
//		{
//			return true;
//		}
//	}
//
//	bool adminLogin()
//	{
//		string name;
//		int id, password;
//		int type = 0;
//
//		cout << "Admin Type:\n"
//			<< "1.Director\n"
//			<< "2.Supervisor\n"
//			<< endl;
//		getType(type);
//
//		if (type == 1) // Director
//		{
//			getCredentials(id, password);
//			Person* user = verifyLogin("Director", id, password);
//			if (user)
//			{
//				cout << "Logged in as Director: " << id << endl;
//				DirectorPortal* dP = new DirectorPortal(dynamic_cast<Director*>(user));
//				dP->DirectorPortalDisplay();
//			}
//			else
//			{
//				return true;
//			}
//		}
//		else if (type == 2) // Supervisor
//		{
//			getCredentials(id, password);
//			Person* user = verifyLogin("Supervisor", id, password);
//			if (user) {
//				cout << "Logged in as Supervisor: " << id << endl;
//				SupervisorPortal* sP = new SupervisorPortal(dynamic_cast<Supervisor*>(user));
//				sP->SupervisorPortalDisplay();
//			}
//			else
//			{
//				return true;
//			}
//
//		}
//	}
//
//	void getCredentials(int& id, int& password)
//	{
//		cout << "Enter Id:";
//		cin >> id;
//		cout << "Enter Password:";
//		cin >> password;
//
//	}
//
//	void getType(int& type)
//	{
//		while (type < 0 || type > 3) {
//
//			cin >> type;
//			if (type < 0 || type > 3)
//				cout << "Enter valid input\n";
//		}
//	}
//
//	Person* verifyLogin(const string& type, int& id, int& password) {
//		string passwordStr = to_string(password); // Convert password to string for comparison.
//		for (auto& user : users) {
//			if (user->getType() == type && user->getID() == id && user->getPass() == passwordStr) {
//				return user; // Return the matching Person pointer
//			}
//		}
//		return nullptr; // Return nullptr if no match is found
//	}
//};
