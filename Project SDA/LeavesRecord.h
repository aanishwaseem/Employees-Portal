#pragma once
#include "IRecord.h"
#include "UnpaidLeaveApp.h"
#include "OfficialLeaveApp.h"
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
using namespace std;
class Leave : public Record<IApplication> { //needed ; on exit, save the records again
private:
	int id;                                  // Employee ID
	vector<IApplication*> records;

public:
	// Constructor
	Leave() {}
	Leave(int ida) : id(ida) {
	}
	vector<IApplication*>* getAllRecords() {
		return &records;
	}
	void addRecord(IApplication* app) {
		records.push_back(app);
		app->addRecordInFile();
	}
	void refresh() {
		string filename = to_string(id) + "LR.txt"; // File name based on ID
		ifstream file(filename);

		if (!file.is_open()) {
			cerr << "No leave record found for employee ID: " << id << endl;
			return;
		}

		records.clear(); // Clear previous records

		string line;
		while (getline(file, line)) {
			IApplication* app = AppsGeneratingFactory(line);
			if (app)
				records.push_back(app);
		}

		file.close();
	}
	IApplication* AppsGeneratingFactory(const std::string& line) {
		if (line.size() > 0) {
			stringstream ss(line);
			int id;
			string leaveType;
			// Parse the line
			ss >> id >> leaveType;

			// Instantiate based on leaveType
			if (leaveType == "Official") {
				return OfficialLeaveApp::getObjectFromLine(line);
			}
			else if (leaveType == "Unpaid") {
				return UnpaidLeaveApp::getObjectFromLine(line);
			}
			else {
				return DefaultLeaveApplication::getObjectFromLine(line);
			}
		}
		return nullptr;
	}

	// Display all individual leave records
	void viewRecords() {
		cout << "-- Leave Records for Employee ID: " << id << " --" << endl;
		for (auto& record : records) {
			record->display();
		}
	}
	void generateSummary(int = 0) {
		// Map to store leaveType and corresponding total leave days
		unordered_map<string, int> leaveSummary;

		// Iterate over all records and sum up the leave days for each leaveType
		for (auto& record : records) {
			if (stoi(record->getDate().substr(record->getDate().size() - 4)) == MyTime::Year) //fetch year from date format dd-mm-yyyy
			{
				string leaveType = record->getApplicationType(); // Get leaveType
				int days = record->getDuration();               // Get leave duration

				// Add to the corresponding leaveType in the map
				leaveSummary[leaveType] += days;
			}
		}

		// Display the summary
		cout << "-- Leave Summary for Employee ID: " << id << " -- for the current year of " << MyTime::Year << endl;

		for (const auto& pair : leaveSummary) {
			const string& leaveType = pair.first;  // Access the key (leaveType)
			int totalDays = pair.second;          // Access the value (totalDays)

			cout << "Leave Type: " << leaveType << ", Total Leaves: " << totalDays << endl;
		}

	}

};
