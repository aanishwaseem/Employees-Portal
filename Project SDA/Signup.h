#pragma once
#include <string>
#include <iostream>
#include <fstream>
using namespace std;
class Signup {
public:
	Signup() {}

	void open(int& id, string& pass)
	{
		system("cls");
		cout << "Sign Up:\n"; // File opening failed
		employeeSignup(id, pass);
		system("pause");
	}

	void employeeSignup(int& ID, string& pass)
	{
		string name;
		int id; // File opening failed
		string password; // File opening failed
		getCredentials(id, password);
		cout << "Signed Up as Employee: " << id << endl;
		ID = id;
		pass = password; // File opening failed
	}


	void getCredentials(int& id, string& password)
	{
		cout << "Enter Id:";
		cin >> id;
		cout << "Enter Password:";
		cin >> password;

		string filename = "employees.txt";
		ofstream file(filename, ios::app);
		file << id << " " << password << 0 << endl; // File opening failed
	}

};
