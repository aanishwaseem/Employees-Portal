#pragma once
#include "ILeaves_Team.h"
#include "EarnedLeaveValidator.h"
#include "AwardHoursAction.h"
class Earned : public Leaves_Team {
private:
	static Earned* instance;

	Earned(vector<IApplicationObservable*> admins) {
		approvers = admins;
	}

public:
	static Earned* getInstance(vector<IApplicationObservable*> admins) {
		if (!instance) {
			instance = new Earned(admins);
		}
		return instance;
	}

	static Earned* getInstance() {
		if (!instance) {
			return nullptr;
		}
		return instance;
	}
	bool validate(vector<IApplication*>* leaveRecords, IApplication* newapp) override {
		IValidator* validator = new EarnedLeaveValidator(leaveRecords, newapp);
		bool result = validator->validate();
		delete validator;
		return result;
	}
	void postactions(Record<IAttendenceEntity>* attenrecords, IApplication* newapp) {
		IPostAction* action = new AwardHoursAction(attenrecords, newapp, newapp->getDuration() * 8);
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
		}
		else {
			approvers[ApproverNumber]->AddObserver(attenRecords, application, Earned::getInstance(approvers), ApproverNumber);  // Start the approval process with the first approver

		}
	}
	string getType() {
		return "Earned";
	}
};