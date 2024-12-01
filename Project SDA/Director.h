#pragma once
#include "Admin.h"
class Director : public Admin {
	int id;
public:

	Director() {}
	Director(int ida) { id = ida; }
	int getID() { return id; }

};
