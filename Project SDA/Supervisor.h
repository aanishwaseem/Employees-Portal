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

};