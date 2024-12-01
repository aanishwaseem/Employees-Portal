#pragma once
#include "Admin.h"
#include "IPerson.h"
class Director : public Admin, public Person {

public:

	Director() {}
	Director(int ID, string PASs) {
		id = ID;
		pass = PASs;
		type = "Director";
	}

	string getType() {
		return type;
	}
	string getPass() {
		return pass;
	}
	Director(int ida) { id = ida; }
	int getID() { return id; }

};
