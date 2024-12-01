#pragma once
#include "IValidator.h"
#include "IApplication.h"
#include "Time.h"
class EarnedLeaveValidator : public IValidator {
	vector<IApplication*>* LeavesHistory;
	IApplication* latestApp;
public:
	EarnedLeaveValidator(vector<IApplication*>* history, IApplication* latest) {
		LeavesHistory = history;
		latestApp = latest;
	}
	bool validate() override {
		int leavescount = calculateEarnedLeavesCount(LeavesHistory);
		if (leavescount < 21 && latestApp->getDuration() > 4) {
			return true;
		}
		return false;
	}
	int calculateEarnedLeavesCount(vector<IApplication*>* records) {
		// Initialize casualLeavesCount to 0
		int casualLeavesCount = 0;
		int currentYear = MyTime::Year;
		// Traverse through each record (IApplication pointer)
		for (auto& record : *records) {
			// Check if the application type is "Casual" and the year matches the current year
			if (record->getApplicationType() == "Earned" && getYear(record->getDate()) == currentYear) {
				// Add the number of days to casualLeavesCount
				casualLeavesCount += record->getDuration();
			}
		}

		return casualLeavesCount;
	}
};
