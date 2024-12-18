#pragma once
#include "ApplicationTypes.h"
#include "UnpaidTeam.h"

class UnpaidLeaveApp : public DefaultLeaveApplication {
private:
	string leaveAddress;   // Estimated financial loss for unpaid leave
	// Overriding file format to include new fields
	string getFileObjectFormat() override {
		return DefaultLeaveApplication::getFileObjectFormat() + " " + leaveAddress;
	}

public:
	// Constructor
	UnpaidLeaveApp(int eid, int id, string leaveType, string days, string date, string to, string from, string reason,
		string LeaveAddress, string state = "Pending")
		: DefaultLeaveApplication(eid, id, leaveType, days, date, to, from, reason, state),
		leaveAddress(LeaveAddress) {}

	// Overriding display to include additional data
	void display() override {
		DefaultLeaveApplication::display();
		cout << "leave Address: " << leaveAddress << endl;
	}

	
	// Static method to create an UnpaidLeave object from a file line
	static UnpaidLeaveApp* getObjectFromLine(const string& line, string type = "Unpaid") {
		stringstream ss(line);
		int eid, id;
		string Type, leaveAddress, days, status, date, to, from, reason, financialImpact, alternativeContact;

		ss >> id >> Type >> eid >> days >> status >> date >> to >> from >> reason;
		ss >> leaveAddress;

		UnpaidLeaveApp* app = new UnpaidLeaveApp(eid, id, Type, days, date, to, from, reason, leaveAddress, status);
		return app;
	}
	void processPendingApplication() override {
		IApplyLeave* system = IApplyLeave::getInstance();
		system->applyLeave(eid, this, Unpaid::getInstance(), false);
	}
};
