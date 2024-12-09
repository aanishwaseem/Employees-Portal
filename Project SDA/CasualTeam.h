#pragma once
#include "ILeaves_Team.h"
#include "AwardHoursAction.h"
class Casual : public Leaves_Team {
private:
	static Casual* instance;

	Casual() {
	}

public:
	static Casual* getInstance() {
		if (instance == nullptr) {
			instance = new Casual();
		}
		return instance;
	}
	bool validate(vector<IApplication*>* leaveRecords, IApplication* newapp) override {
		int leavescount = calculateEmpLeavesCount(leaveRecords);
		if (leavescount < 15 && newapp->getDuration() <= 4) { // "cannot avail more than 4 leaves at a time"
			return true;
		}
		cout << "You are not eligible to apply" << endl;

		return false;
	}

	void postactions(IApplication* newapp) {
		IPostAction* action = new AwardHoursAction(newapp->getEmpId(), newapp->getToDate(), newapp->getDuration(), newapp->getDuration() * 8);
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
			approvers[ApproverNumber]->AddObserver(application, Casual::getInstance(), ApproverNumber);  // Start the approval process with the first approver

		}
	}



	string getType() {
		return "Casual";
	}
};
