#pragma once
#include "ILeaves_Team.h"
class Employee {
private:
	int id;

	Record<IAttendenceEntity>* myAttendenceRecord;
	Record<IApplication>* myLeaveRecord;
public:

	Employee() {}
	Employee(int id, Record<IAttendenceEntity>* NewRecord, Record<IApplication>* NewLeaveRecord) : id(id) {
		myAttendenceRecord = NewRecord;
		myLeaveRecord = NewLeaveRecord;
		myAttendenceRecord->refresh();
		myLeaveRecord->refresh();
	}

	void generateAttendenceReport() {
		myAttendenceRecord->viewRecords();
	}
	void generateLeaveReport() {
		myLeaveRecord->viewRecords();

	}
	void generateLeaveSummary() {
		myLeaveRecord->generateSummary();
	}
	int getID() {
		return id;
	}
	bool operator==(const Employee& other) const {
		return this->id == other.id;
	}
	~Employee() {

	}
};
