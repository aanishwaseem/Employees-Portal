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
};
