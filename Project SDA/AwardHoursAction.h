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
		
		IAttendenceEntity* proxy = new AttendenceEntity(latestApp->getEmpId(), getWeekNumberInMonth(latestApp->getToDate()), getDayGivenMonth(latestApp->getToDate()), NoOfHours);

		attenRecord->addRecord(proxy);
	}
};