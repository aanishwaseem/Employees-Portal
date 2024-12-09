#pragma once
#include "ILeaves_Team.h"
#include "AwardHoursAction.h"
class Earned : public Leaves_Team {
private:
	static Earned* instance;

	Earned() {
	}

public:
	static Earned* getInstance() {
		if (instance == nullptr) {
			instance = new Earned();
		}
		return instance;
	}
	bool validate(vector<IApplication*>* leaveRecords, IApplication* newapp) override {
		int leavescount = calculateEmpLeavesCount(leaveRecords);
		if (leavescount < 21 && newapp->getDuration() > 4) {

			return true;
		}
		cout << "You cannot apply days should be greater than 4 or you have availed all limits" << endl;

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
			approvers[ApproverNumber]->AddObserver(application, Earned::getInstance(), ApproverNumber);  // Start the approval process with the first approver

		}
	}
	string getType() {
		return "Earned";
	}
};