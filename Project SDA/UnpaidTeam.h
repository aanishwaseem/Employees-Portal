#pragma once
#include "ILeaves_Team.h"
#include "UnpaidPostAction.h"
class Unpaid : public Leaves_Team {
private:
	static Unpaid* instance;

	Unpaid(vector<IApplicationObservable*> admins) {
		approvers = admins;
	}

public:
	static Unpaid* getInstance(vector<IApplicationObservable*> admins) {
		if (!instance) {
			instance = new Unpaid(admins);
		}
		return instance;
	}
	static Unpaid* getInstance() {
		if (!instance) {
			return nullptr;
		}
		return instance;
	}
	void postactions(IApplication* newapp) {
		IPostAction* action = new UnpaidLeavePA(newapp);
		action->postaction();
		delete action;
	}

	void ForwardApplicationFurther(Record<IAttendenceEntity>* attenRecords, IApplication* application, int ApproverNumber = 0, string status = "Pending") { // actually registering in observables (approval team)
		if (ApproverNumber >= approvers.size() || status == "Rejected") // // supervisor has denied the application so do not forward to Director
		{// application has gone through all the process now update the status
			application->setStatus(status);
			NotifyStatusChange(application);
			if (status == "Approved") {
				postactions(application);
			}
			applications.erase(std::find(applications.begin(), applications.end(), application));

		}
		else {
			approvers[ApproverNumber]->AddObserver(attenRecords, application, Unpaid::getInstance(approvers), ApproverNumber);  // Start the approval process with the first approver

		}
	}
	string getType() {
		return "Unpaid";
	}
};
