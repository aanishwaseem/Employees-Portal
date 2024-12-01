#pragma once
#include "InactivityManager.h"
#include "IMonthlyReports.h"
#include "LeavesRecord.h"
#include "AttendenceRecord.h"
#include <fstream>
#include "GuardPortal.h"
#include "IApplyLeave.h"
class Main {
private:
	static Main* main;
	vector<Record<IApplication>*> empLeaveRecords;
	ITimeObserver* ReportsGenerator;
	ITimeObserver* inactivityManager;
	IApplyLeave* IApply;
	Main() {
		inactivityManager = InactivityManager::getInstance();
		IApply = IApplyLeave::Instantiate(&empLeaveRecords, &empAttendRegisters);
		ReportsGenerator = new IMonthlyReports(&empAttendRegisters, static_cast<InactivityManager*>(inactivityManager)->getIDs());
		MyTime* timeobj = MyTime::getInstance();
		timeobj->addObserver(ReportsGenerator);
		timeobj->addObserver(inactivityManager);
	}
public:
	vector<Record<IAttendenceEntity>*> empAttendRegisters;

	static Main* getMain() {
		if (main == nullptr) {
			main = new Main();

		}
		return main;
	}
	Record<IAttendenceEntity>* generateEmployeeRecord(int id) { // Factory Method called when creating employees agr employe null krta ha register ko istrah to record ka track to gya, agr to record getter sy arha to koi b edit kr skta 
		string filename = to_string(id) + ".txt";
		ifstream file(filename);
		MyTime* timeobj = MyTime::getInstance();
		if (!file.is_open()) {
			file.close();
			//register new record to system
			ofstream fw(filename);
			fw.close();
		}
		Record<IAttendenceEntity>* newentity = new Attendence(id);
		empAttendRegisters.push_back(newentity);
		return empAttendRegisters.back();

	}
	Record<IApplication>* createLeaveRecord(int id) { // Factory Method called when creating employees agr employe null krta ha register ko istrah to record ka track to gya, agr to record getter sy arha to koi b edit kr skta 
		string filename = to_string(id) + "LR.txt";
		ifstream file(filename);

		if (!file.is_open()) {
			file.close();
			//register new record to system
			ofstream fw(filename);
			fw.close();
		}
		Record<IApplication>* newentity = new Leave(id);
		empLeaveRecords.push_back(newentity);
		return empLeaveRecords.back();

	}

};



