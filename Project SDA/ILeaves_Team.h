#pragma once
#include "IApplicationObserve.h"
#include "Time.h"
#include "NotificationManager.h"
#include <vector>
class Leaves_Team : public IApplicationObserver {
protected:
	vector<IApplication*> applications;
	vector<IApplicationObservable*> approvers;  // List of approvers for a team
public:
	virtual ~Leaves_Team() = default;
	void addAdmin(IApplicationObservable* admin) {
		approvers.push_back(admin);
	}
	bool apply(IApplication* application, vector<IApplication*>* leaveRecords) { // template method for applying procedure f0r all teams
		bool check = true;
		check = validate(leaveRecords, application);
		if (check) {
			AddInApplicationVector(application); //Command pattern (submitting leave).
			ForwardApplicationFurther(application); // send to first admin in hierachy
			return true;
		}
		else {
			NotifiyFailure(application->getEmpId());
			return false;
		}
	}

	virtual void ForwardApplicationFurther(IApplication* application, int ApproverNumber = 0, string status = "Pending") = 0;
	virtual string getType() = 0;
	virtual bool validate(vector<IApplication*>* leaveRecords, IApplication* newapp) {
		return true; // default validation is true;
	};
	void AddInApplicationVector(IApplication* application) {
		applications.emplace_back(application);
	}
	void NotifyStatusChange(IApplication* application) {
		INotificationManager* notificationManager = NotificationManager::getInstance();
		string msg = "[" + getType() + " Leave] Your application has been " + application->getStatus() + "!";
		notificationManager->addNotification(application->getEmpId(), msg);
	}
	void NotifiyFailure(int eid) {
		INotificationManager* notificationManager = NotificationManager::getInstance();
		string msg = "[" + getType() + " Leave] Your application could not be processed!";
		notificationManager->addNotification(eid, msg);
	}
	void getApplications() {
		cout << "displaying outstanding leave applications" << endl;
		for (auto& app : applications) {
			app->display();
		}
	}

	int calculateEmpLeavesCount(vector<IApplication*>* records) {
		// Initialize casualLeavesCount to 0
		int casualLeavesCount = 0;
		int currentYear = MyTime::Year;
		// Traverse through each record (IApplication pointer)
		for (auto& record : *records) {
			// Check if the application type is "Casual" and the year matches the current year
			if (record->getApplicationType() == getType() && getYear(record->getToDate()) == currentYear) {
				// Add the number of days to casualLeavesCount
				casualLeavesCount += record->getDuration();
			}
		}

		return casualLeavesCount;
	}
};
