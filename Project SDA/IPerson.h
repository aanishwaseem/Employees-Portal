#pragma once
#include <iostream>
using namespace std;
class Person {
protected:
	int id;
	string pass;
	string type;
public:
	virtual int getID() = 0;
	virtual string getPass() = 0;
	virtual string getType() = 0;
};