#pragma once
#include "ILeaves_Team.h"
#include "AwardHoursAction.h"

class Official : public Leaves_Team {
private:
	static Official* instance;

	Official(vector<IApplicationObservable*> admins) {
		approvers = admins;
	}

public:
	static Official* getInstance(vector<IApplicationObservable*> admins) {
		if (!instance) {
			instance = new Official(admins);
		}
		return instance;
	}
	static Official* getInstance() {
		if (!instance) {
			return nullptr;
		}
		return instance;
	}
	void postactions(Record<IAttendenceEntity>* attenrecords, IApplication* newapp) {
		IPostAction* action = new AwardHoursAction(attenrecords, newapp, 8);
		action->postaction();
		delete action;
	}
	void ForwardApplicationFurther(Record<IAttendenceEntity>* attenRecords, IApplication* application, int ApproverNumber = 0, string status = "Pending") { // actually registering in observables (approval team)
		if (ApproverNumber >= approvers.size() || status == "Rejected") // // supervisor has denied the application so do not forward to Director
		{// application has gone through all the process now update the status
			application->setStatus(status);
			NotifyStatusChange(application);
			if (status == "Approved") {
				postactions(attenRecords, application);
			}
			applications.erase(std::find(applications.begin(), applications.end(), application));

		}
		else {
			approvers[ApproverNumber]->AddObserver(attenRecords, application, Official::getInstance(approvers), ApproverNumber);  // Start the approval process with the first approver

		}
	}
	string getType() {
		return "Official";
	}
};
