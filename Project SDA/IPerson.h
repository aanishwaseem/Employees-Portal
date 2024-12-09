#pragma once
#include <iostream>
using namespace std;
class Person {
protected:
	int id;
	string pass;
	string type;
public:
	int getID() { return id; };
	string getPass() {
		return pass;
	};
	string getType() {
		return type;
	};
};