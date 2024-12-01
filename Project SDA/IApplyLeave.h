#pragma once

#include "IApplication.h"
#include "IRecord.h"
#include "IAttendenceEntity.h"
#include "ILeaves_Team.h"

class IApplyLeave {
	static IApplyLeave* main;
	vector<Record<IApplication>*>* empLeaveRecords;
	IApplyLeave(vector<Record<IApplication>*>* EmpLeaveRecords, vector<Record<IAttendenceEntity>*>* EmpAttendRegisters) {
		empLeaveRecords = EmpLeaveRecords;
		empAttendRegisters = EmpAttendRegisters;
	}
public:
	vector<Record<IAttendenceEntity>*>* empAttendRegisters;
	void applyLeave(int eid, IApplication* app, Leaves_Team* team, bool WriteToFalse = true) {
		// file the leave
		bool success = team->apply(app, (*empAttendRegisters)[eid - 1], ((*empLeaveRecords)[eid - 1])->getAllRecords());
		if (success && WriteToFalse) {
			(*empLeaveRecords)[eid - 1]->addRecord(app);
		}

	}

	static IApplyLeave* Instantiate(vector<Record<IApplication>*>* EmpLeaveRecords,vector<Record<IAttendenceEntity>*>* EmpAttendRegisters) {
		if (main == nullptr) {
			main = new IApplyLeave(EmpLeaveRecords, EmpAttendRegisters);

		}
		return main;
	}
	static IApplyLeave* getInstance() {
		if (main == nullptr) {
			return nullptr;
		}
		return main;
	}
};

