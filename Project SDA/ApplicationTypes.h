#pragma once
#include "IApplication.h"
#include "EarnedTeam.h"
#include "CasualTeam.h"
#include "IApplyLeave.h"
#include <iostream>
#include <fstream>
#include <sstream>
class Status {
	int appID;
	string state;
public:
	Status(){}
	Status(string stat, int id): appID(id) {
		state = stat;
	}
	void setState(string stat, int eid, string format) {
		state = stat;
		updateLeaveStatus(eid, format);
	};
	string getState() {
		return state;
	}
	void updateLeaveStatus(int Eid, string format) {
		string eid = to_string(Eid);
		string filename = eid + "LR.txt"; // File name based on employee ID
		fstream file(filename, ios::in | ios::out);
		string line;
		stringstream buffer;
		bool found = false;
	
		// Read each line and check for the leave record ID
		while (getline(file, line)) {
			stringstream ss(line);
				
			int recordID;
			// Parse the line
			ss >> recordID;
	
			// Check if this is the record to update
			if (recordID == appID) {
				found = true;
	
				// Update the status
				buffer << format << endl;
			}
			else {
				// Write the unchanged record back to the buffer
				buffer << line << endl;
			}
		}
	
		file.close(); // Close the file before reopening for writing
	
		if (!found) {
			cerr << "No leave record found with ID: " << appID << endl;
			return;
		}
	
		// Write the updated buffer back to the file
		ofstream outFile(filename, ios::trunc);
	
		outFile << buffer.str();
		outFile.close();
	
		cout << "Leave record with ID " << appID << " successfully updated." << endl;
	}
	
};


class Approved : public Status {
public:
	Approved() {}
};
class Pending : public Status {
public:
	Pending() {}
};
class Rejected : public Status {
public:
	Rejected() {}
};

class DefaultLeaveApplication : public IApplication {
protected:
	int eid;
	int id;
	string leaveType;
	string days;       // Number of leave days
	Status status;
	string date;
	string to;
	string from;
	string reason;
	
	virtual string getFileObjectFormat() {
		string objformat = to_string(id) + " " + leaveType + " " + to_string(eid) + " " + (days)+" " + status.getState() + " " + (date)+" " + (to)+" " + (from)+" " + reason;
		return objformat;
	}
public:
	// Constructor
	DefaultLeaveApplication(int Eid, int ID, string leaveType, string days, string Date, string To, string From, string Reason, string state = "Pending") : date(Date), to(To), from(From), reason(Reason), eid(Eid), id(ID), leaveType(leaveType), days(days), status(state, ID) { // when new app is created it gets pending
	}

	// Display the record
	void display() {
		cout << "Emp. ID: " << eid << ", Leave type: " << leaveType << ",  status: " << status.getState() << ", days: " << days << ", from: " << from << ", to: " << to << endl;
	}
	// Getters
	string getApplicationType() { return leaveType; }
	string getToDate() { return to; }
	int getEmpId() { return (eid); }
	int getDuration() { return stoi(days); }
	void setStatus(string newstate) {
		status.setState(newstate, eid, getFileObjectFormat());
	}
	static IApplication* getObjectFromLine(const string& line) {
		stringstream ss(line);
		int eid, id;
		string Type, days, status, date, to, from, reason;

		ss >> id >> Type >> eid >> days >> status >> date >> to >> from >> reason;
		IApplication* app = new DefaultLeaveApplication(eid, id, Type, days, date, to, from, reason, status);

		return app;
	}
	void processPendingApplication(){
		IApplyLeave* system = IApplyLeave::getInstance();
		if (leaveType == "Casual")
			system->applyLeave(eid, this, Casual::getInstance(), false);
		else
			system->applyLeave(eid, this, Earned::getInstance(), false);
	}
	string getStatus() {
		return
			status.getState();
	}
	void addRecordInFile() {

		// Prepare the file to append the new record
		string filename = to_string(eid) + "LR.txt"; // File is named after Employee ID
		ofstream file(filename, ios::app); // Open file in append mode

		// Write the record to the file
		string objFormat = getFileObjectFormat();
		file << objFormat << endl;
		file.close(); // Close the file
	}
};



