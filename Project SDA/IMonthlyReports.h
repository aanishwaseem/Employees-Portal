#pragma once
#include "IRecord.h"
#include "IAttendenceEntity.h"
#include "InactivityManager.h"
#include <vector>
#include <iostream>

class IMonthlyReports : public ITimeObserver {
	vector<Record<IAttendenceEntity>*>* empAttendRegisters;
public:
	IMonthlyReports(vector<Record<IAttendenceEntity>*>* reg) {
		empAttendRegisters = reg;
	}
	void onDayUpdate(){}
	void onWeekUpdate() {};
	void onMonthUpdate() {
		GenerateMonthlyReports();
		GenerateSalaries();
	};
	void onYearUpdate() {};
	void GenerateMonthlyReports() {
		cout << endl << endl << "[SYSTEM] Monthly Reports" << endl;
		for (auto& rgister : *empAttendRegisters) {
			rgister->generateSummary(80);
		}
	}
	void GenerateSalaries() {
		cout << endl << endl << "[SYSTEM] Salary" << endl;
		for (int i = 0; i < empAttendRegisters->size(); i++) {
			if (InactivityManager::getInstance()->isInactive(i + 1) > 20) {
				cout << "- Emp # " << i + 1 << " is on unpaid leave so no salary" << endl;
			}
			else {
				cout << "- Awarded salary to Emp # " << i + 1 << "!" << endl;
			}
		}
	}
};
