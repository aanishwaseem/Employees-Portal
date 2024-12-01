#pragma once
#include "IRecord.h"
#include "IAttendenceEntity.h"
#include <vector>
#include <iostream>
#include "Time.h"

class IMonthlyReports : public ITimeObserver {
	vector<Record<IAttendenceEntity>*>* empAttendRegisters;
	vector<inactive>* inactiveIDs;
public:
	IMonthlyReports(vector<Record<IAttendenceEntity>*>* reg, vector<inactive>* outreg) {
		empAttendRegisters = reg;
		inactiveIDs = outreg;
	}
	void onWeekUpdate() {};
	void onMonthUpdate() {
		GenerateMonthlyReports();
		GenerateSalaries();
	};
	void onYearUpdate() {};
	void GenerateMonthlyReports() {
		for (auto& rgister : *empAttendRegisters) {
			rgister->generateSummary(80);
		}
	}
	void GenerateSalaries() {
		cout << "[SYSTEM] Salary" << endl;
		for (int i = 0; i < empAttendRegisters->size(); i++) {
			if (isEmpInactive(i + 1)) {
				cout << "- Emp # " << i + 1 << " is gone to some visit so no salary" << endl;
			}
			else {
				cout << "- Awarded salary to Emp # " << i + 1 << "!" << endl;
			}
		} // impostor give only salaries to Emp having > 80 attendence
	}
	bool isEmpInactive(int id) {
		for (int i = 0; i < inactiveIDs->size(); i++) {
			if (id == (*inactiveIDs)[i].id) {//employee is inactive
				return true;
			}
		}
		return false;
	}
};
