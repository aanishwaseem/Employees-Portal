#pragma once
#include "IPerson.h"
#include "AttendenceRecord.h"
#include "Time.h"
class Guard : public Person {
private:
public:
	Guard(int ID, string PASs) {
		id = ID;
		pass = PASs;
		type = "Guard";
	}
	void markAttendance(vector<Record<IAttendenceEntity>*> empReg, int eid, bool overtime = false) {
		int hours = 8;
		if (overtime)
			hours = 10;
		IAttendenceEntity* attendence = new AttendenceEntity(eid, MyTime::Week, getDayInWeek(MyTime::Day), hours);
		empReg[eid - 1]->addRecord(attendence);
		cout << "Attendance marked for Employee ID " << id << " with " << hours << " hours worked." << endl;

	};
};