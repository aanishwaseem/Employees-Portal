#pragma once
#include "ILeaves_Team.h"
#include "IPerson.h"
class Employee : public Person {
private:
	Record<IAttendenceEntity>* myAttendenceRecord;
	Record<IApplication>* myLeaveRecord;
public:
	int inactive = 0;

	Employee() {}
	Employee(int ID, string PASs) {
		id = ID;
		pass = PASs;
		type = "Employee";
	}

	string getType() {
		return type;
	}
	void initializeEmployee(int Inactive, Record<IAttendenceEntity>* NewRecord, Record<IApplication>* NewLeaveRecord){
		myAttendenceRecord = NewRecord;
		myLeaveRecord = NewLeaveRecord;
		myAttendenceRecord->refresh();
		myLeaveRecord->refresh();
		inactive = Inactive;
	}
	string getPass() {
		return pass;
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
	int getNextLeaveID() {
		return myLeaveRecord->getAllRecords()->size()+1;
	}
	bool operator==(const Employee& other) const {
		return this->id == other.id;
	}
	~Employee() {

	}
};
