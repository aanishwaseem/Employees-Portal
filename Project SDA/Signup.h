#pragma once
#include <string>
#include <iostream>
#include <fstream>
using namespace std;
class Signup {
public:
	Signup() {}

	void open()
	{
		system("cls");
		cout << "Sign Up:\n";
		employeeSignup();
		system("pause");
	}

	void employeeSignup()
	{
		string name;
		int id, password;
		getCredentials(id, password);
		cout << "Signed Up as Employee: " << id << endl;
	}


	void getCredentials(int& id, int& password)
	{
		cout << "Enter Id:";
		cin >> id;
		cout << "Enter Password:";
		cin >> password;

		string filename = "file" + to_string(id);
		ofstream file(filename);
		file << id << " " << password;
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
