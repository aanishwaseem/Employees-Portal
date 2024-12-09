#pragma once
#include "ILeaves_Team.h"
class Unpaid : public Leaves_Team {
private:
	static Unpaid* instance;

	Unpaid() {
	}

public:
	static Unpaid* getInstance() {
		if (instance == nullptr) {

			instance = new Unpaid();
		}
		return instance;
	}
	void postactions(IApplication* newapp) {
		InactivityManager* inactivityManager = InactivityManager::getInstance();
		inactivityManager->addInactive(newapp->getEmpId(), newapp->getDuration());
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
			approvers[ApproverNumber]->AddObserver(application, Unpaid::getInstance(), ApproverNumber);  // Start the approval process with the first approver

		}
	}
	string getType() {
		return "Unpaid";
	}
};
