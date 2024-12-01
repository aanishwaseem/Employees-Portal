#pragma once
#include "Admin.h"
#include "AttendenceEntity.h"
#include "Time.h"
class Guard {
public:
	void markAttendance(vector<Record<IAttendenceEntity>*> empReg, int eid, bool overtime = false) {
		int hours = 8;
		if (overtime)
			hours = 10;
		IAttendenceEntity* attendence = new AttendenceEntity(eid, MyTime::Week, MyTime::Day, hours);
		empReg[eid - 1]->addRecord(attendence);
	};
};