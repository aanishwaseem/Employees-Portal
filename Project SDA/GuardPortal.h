#pragma once
#include "Guard.h"
#include "IApplyLeave.h"

class GuardPortal {
private:
	Guard* guard;  // Reference to Director class or an instance to handle approval logic
public:
	GuardPortal(Guard* Guard) {
		guard = Guard;

	}
	void markattendenceportal() {
		int eid;
		cout << "whose attendence you want to mark? " << endl;
		cin >> eid;
		guard->markAttendance(*IApplyLeave::getInstance()->empAttendRegisters, eid);
	}
};
