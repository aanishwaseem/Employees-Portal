#pragma once
#include "OfficialTeam.h"
#include "ApplicationTypes.h"

class OfficialLeaveApp : public DefaultLeaveApplication {
private:
	string travelAllowance; // Amount allocated for travel
	string purpose;         // Purpose of the leave
	string destination;     // Destination for official duty

public:
	// Constructor
	OfficialLeaveApp(int eid, int id, string leaveType, string days, string date, string to, string from, string reason,
		string travelAllowance, string purpose, string destination, string state = "Pending")
		: DefaultLeaveApplication(eid, id, leaveType, days, date, to, from, reason, state),
		travelAllowance(travelAllowance), purpose(purpose), destination(destination) {}

	// Overriding display to include additional data
	void display() override {
		DefaultLeaveApplication::display();
		cout << "Travel Allowance: " << travelAllowance << ", Purpose: " << purpose
			<< ", Destination: " << destination << endl;
	}

	// Overriding file format to include new fields
	string getFileObjectFormat() override {
		return DefaultLeaveApplication::getFileObjectFormat() + " " + travelAllowance + " " + purpose + " " + destination;
	}

	// Static method to create an OfficialLeave object from a file line
	static OfficialLeaveApp* getObjectFromLine(const string& line, string type = "Official") {
		stringstream ss(line);
		int eid, id;
		string Type, days, status, date, to, from, reason, travelAllowance, purpose, destination;

		ss >> id >> Type >> eid >> days >> status >> date >> to >> from >> reason;
		ss >> travelAllowance >> purpose >> destination;

		OfficialLeaveApp* app = new OfficialLeaveApp(eid, id, Type, days, date, to, from, reason, travelAllowance, purpose, destination, status);
		if (stoi(days) > 0 && status == "Approved") {
			IApplyLeave* system = IApplyLeave::getInstance();
			
			AwardHoursAction action((*system->empAttendRegisters)[eid-1], app, 8);
			action.postaction();
		}
		if (status == "Pending") {
			IApplyLeave* system = IApplyLeave::getInstance();
			system->applyLeave(eid, app, Official::getInstance());
		}
		return app;
	}
};
