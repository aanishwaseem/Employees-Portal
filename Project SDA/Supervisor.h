#pragma once
#include "Admin.h"
class Supervisor : public Admin {
	int id;
public:
	Supervisor() {}
	Supervisor(int ida) { id = ida; }
	int getID() { return id; }
};