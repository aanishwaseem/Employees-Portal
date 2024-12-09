#pragma once
#include "ILeaves_Team.h"
#include "AwardHoursAction.h"

class Official : public Leaves_Team {
private:
	static Official* instance;

	Official() {
	}

public:
	static Official* getInstance() {
		if (instance == nullptr) {

			instance = new Official();
		}
		return instance;
	}
	void postactions(IApplication* newapp) {
		IPostAction* action = new AwardHoursAction(newapp->getEmpId(), newapp->getToDate(), newapp->getDuration(), 8);
		action->postaction();
		delete action;
	}
	void ForwardApplicationFurther(IApplication* application, int ApproverNumber = 0, string status = "Pending") { // actually registering in observables (approval team)
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
			approvers[ApproverNumber]->AddObserver(application, Official::getInstance(), ApproverNumber);  // Start the approval process with the first approver

		}
	}
	string getType() {
		return "Official";
	}
};
