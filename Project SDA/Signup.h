#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include "Employee.h"
using namespace std;
class Signup {
public:
	Signup() {}

	Employee* open()
	{
		system("cls");
		cout << "Sign Up:\n";
		Employee* newe = employeeSignup();
		system("pause");
		return newe;
	}

	Employee* employeeSignup()
	{
		string name;
		int id;
		string password;
		getCredentials(id, password);
		cout << "Signed Up as Employee: " << id << endl;
		return new Employee(id, password);
	}


	void getCredentials(int& id, string& password)
	{
		cout << "Enter Id:";
		cin >> id;
		cout << "Enter Password:";
		cin >> password;

		string filename = "employees.txt";
		ofstream file(filename, ios::app);
		file << id << " " << password << endl;
	}

	void getType(int& type)
	{
		while (type < 0 || type > 3) {

			cin >> type;
			if (type < 0 || type > 3)
				cout << "Enter valid input\n";
		}
	}

};
