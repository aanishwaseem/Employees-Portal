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
		cout << "is it overtime? 1 for yes, 0 for no" << endl;
		bool overtime;
		cin >> overtime;
		guard->markAttendance(*IApplyLeave::getInstance()->empAttendRegisters, eid, overtime);
	}
};
