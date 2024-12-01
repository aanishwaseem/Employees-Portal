#pragma once
#include "Admin.h"
#include "IPerson.h"
class Supervisor : public Admin, public Person {

public:
	Supervisor() {}
	Supervisor(int ID, string PASs) {
		id = ID;
		pass = PASs;
		type = "Supervisor";
	}

	string getType() {
		return type;
	}
	string getPass() {
		return pass;
	}
	Supervisor(int ida) { id = ida; }
	int getID() { return id; }
};