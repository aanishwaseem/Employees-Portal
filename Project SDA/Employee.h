#pragma once
#include "ILeaves_Team.h"
#include "IPerson.h"
#include "Main.h"
class Employee : public Person {
private:
	Record<IAttendenceEntity>* myAttendenceRecord;
	Record<IApplication>* myLeaveRecord;
public:
	int inactive = 0;

	Employee() {}

	Employee* initializeEmployee(int ID, string PASs){ // factory method
		InactivityManager* inactives = InactivityManager::getInstance();
		Main* system = Main::getMain();
		Employee* newemp = new Employee;
		newemp->id = ID;
		newemp->pass = PASs;
		Record<IAttendenceEntity>* NewRecord = system->generateEmployeeRecord(newemp->id);
		Record<IApplication>* NewLeaveRecord = system->createLeaveRecord(newemp->id);
		newemp->myAttendenceRecord = NewRecord;
		newemp->myLeaveRecord = NewLeaveRecord;
		newemp->myAttendenceRecord->refresh();
		newemp->myLeaveRecord->refresh();
		int Inactive = inactives->isInactive(newemp->id);
		newemp->inactive = Inactive;
		
		newemp->type = "Employee";
		return newemp;
	}
	void setInactive(int days) { // setting inactive emp
		inactive = days;
	}
	void generateAttendenceReport() { // generating emp report
		myAttendenceRecord->viewRecords();
	}
	void generateLeaveReport() { // leave report
		myLeaveRecord->viewRecords();

	}
	void generateLeaveSummary() { //leave summary
		myLeaveRecord->generateSummary();
	}
	int getNextLeaveID() {
		return myLeaveRecord->getAllRecords()->size()+1;
	}
	~Employee() {

	}
};
