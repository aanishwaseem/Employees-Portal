#pragma once
#include "IValidator.h"
#include "IApplication.h"
#include "Time.h"
#include <vector>
class CasualLeaveValidator : public IValidator {
	vector<IApplication*>* LeavesHistory;
	IApplication* latestApp;
public:
	CasualLeaveValidator(vector<IApplication*>* history, IApplication* latest) {
		LeavesHistory = history;
		latestApp = latest;
	}
	bool validate() override {
		int leavescount = calculateEmpCasualLeavesCount(LeavesHistory);
		if (leavescount < 15 && latestApp->getDuration() <= 4) { // "cannot avail more than 4 leaves at a time"
			return true;
		}
		return false;
	}
	int calculateEmpCasualLeavesCount(vector<IApplication*>* records) {
		// Initialize casualLeavesCount to 0
		int casualLeavesCount = 0;
		int currentYear = MyTime::Year;
		// Traverse through each record (IApplication pointer)
		for (auto& record : *records) {
			// Check if the application type is "Casual" and the year matches the current year
			if (record->getApplicationType() == "Casual" && getYear(record->getDate()) == currentYear) {
				// Add the number of days to casualLeavesCount
				casualLeavesCount += record->getDuration();
			}
		}

		return casualLeavesCount;
	}

};
