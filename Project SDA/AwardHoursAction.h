#pragma once
#include "IPostAction.h"
#include "Time.h"
#include "AttendenceRecord.h"
class AwardHoursAction : public IPostAction {
	Attendence* attenRecord;
	int NoOfHours;
public:
	AwardHoursAction(Record<IAttendenceEntity>* record, IApplication* latest, int hours) : IPostAction(latest), NoOfHours(hours) {
		attenRecord = static_cast<Attendence*>(record);
	}
	void postaction() {
		attenRecord->addWeekHours(NoOfHours, getWeekNumberInMonth(latestApp->getToDate()));
	}
};