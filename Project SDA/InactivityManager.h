#pragma once
#include "Time.h"
#include <vector>
using namespace std;
class InactivityManager : public ITimeObserver {
	vector<inactive> inactiveIDs;
	static InactivityManager* main;
	InactivityManager() {
	}
public:

	
	static InactivityManager* getInstance() {
		if (main == nullptr) {
			main = new InactivityManager();

		}
		return main;
	}
	void addInactive(int id, int days) {
		inactive entity;
		entity.id = id; entity.days = days;
		inactiveIDs.push_back(entity);
	}
	vector<inactive>* getIDs() {
		return &inactiveIDs;
	}
	void onWeekUpdate() {
		reduceInactiveDays(7);
	};
	void onMonthUpdate() {
		reduceInactiveDays(30);
	};
	void onYearUpdate() {
		reduceInactiveDays(365);
	};
	void reduceInactiveDays(int days) {
		for (int i = 0; i < inactiveIDs.size(); ) { // Use a while loop to handle dynamic resizing
			(inactiveIDs)[i].days -= days; // Reduce days
			if ((inactiveIDs)[i].days <= 0) {
				// Remove the inactive element if days <= 0
				inactiveIDs.erase(inactiveIDs.begin() + i);
				// Do not increment i, as the next element shifts into the current index
			}
			else {
				++i; // Only increment if no element was erased
			}
		}
	}
};
