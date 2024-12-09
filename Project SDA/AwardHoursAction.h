#pragma once
#include "IPostAction.h"
#include "Time.h"
#include "AttendenceRecord.h"
#include "IApplyLeave.h"
#include "InactivityManager.h"
class AwardHoursAction : public IPostAction {
	int NoOfHours;
	int eid;
	string actiondate;		// Assuming the date format is "DD-MM-YYYY"

	int days;
public:
	AwardHoursAction(int Eid, string Actiondate, int Days, int hours) : eid(Eid), actiondate(Actiondate), days(Days), NoOfHours(hours) {
	}
	void postaction() {
		Record<IAttendenceEntity>* record = (*IApplyLeave::getInstance()->empAttendRegisters)[eid-1];
		IAttendenceEntity* proxy = new AttendenceEntity(eid, getWeekNumberInMonth(actiondate), getDayGivenMonth(actiondate), NoOfHours);
		// Assuming the date format is "DD-MM-YYYY"

		int day = getDay(actiondate);
		if (day >= MyTime::Day) {
			InactivityManager* inactives = InactivityManager::getInstance();
			inactives->addInactive(eid, days);
		}
		// Assuming the date format is "DD-MM-YYYY"

		record->addRecord(proxy);
	}

	int getDay(string date) {
		// Assuming the date format is "DD-MM-YYYY"
		return stoi(date.substr(0, 2)); // Extract the first two characters and convert to integer
	}

};