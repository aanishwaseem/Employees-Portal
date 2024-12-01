#pragma once
#include "IRecord.h"
#include "AttendenceEntity.h"
#include "Time.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
class ISummary : public ITimeObserver {
private:
	int* Percentage;
	int* WeeklyHrsWorked;
	int* MonthlyHrsWorked;
	vector<IAttendenceEntity*>* records;
public:
	ISummary(int* p, int* w, int* m, vector<IAttendenceEntity*>* r) {
		Percentage = p;
		WeeklyHrsWorked = w;
		MonthlyHrsWorked = m;
		records = r;
	}

	void onWeekUpdate() override {

		*WeeklyHrsWorked = 0;
	}

	void onMonthUpdate() override {

		*WeeklyHrsWorked = 0;
		*MonthlyHrsWorked = 0;
		*Percentage = 0;
	}
	void onYearUpdate() override {
	}
};

class Attendence : public Record<IAttendenceEntity> {
private:
	int id;
	int Percentage;
	int WeeklyHrsWorked;
	int MonthlyHrsWorked;
	ITimeObserver* summaryTimer;
	vector<IAttendenceEntity*> records;
public:

	Attendence() {

	}
	Attendence(int id) : id(id) {
		summaryTimer = new ISummary(&Percentage, &WeeklyHrsWorked, &MonthlyHrsWorked, &records);
		MyTime* timeobj = MyTime::getInstance();
		timeobj->addObserver(summaryTimer);
	}

	vector<IAttendenceEntity*>* getAllRecords() {
		return &records;
	}
	//Mark Attendence
	void addRecord(IAttendenceEntity* attendenceObject) {


		// Add the new record to the vector
		records.push_back(attendenceObject);

		// Update the file with the new record
		ofstream file(to_string(id) + ".txt", ios::app);  // Open file in append mode

		if (!file.is_open()) {
			cerr << "Unable to open attendance file for employee ID: " << id << endl;
			return;
		}
		string format = attendenceObject->getFileObjectFormat();
		file << format << endl; // impostor
		file.close();
		int hrsworked = attendenceObject->getHours();
		addWeekHours(hrsworked, MyTime::Week);
		cout << "Attendance marked for Employee ID " << id << " with " << hrsworked << " hours worked." << endl;
	}

	void addWeekHours(int hrsworked, int week) {
		// if its simple attendence then 
		if (week == MyTime::Week) {
			MonthlyHrsWorked -= WeeklyHrsWorked;

			WeeklyHrsWorked += hrsworked;  // Add the current day's hours to weekly total

			if (WeeklyHrsWorked > 40) {
				// If weekly hours exceed 40, cancel the update to MonthlyHrsWorked and reset WeeklyHrsWorked
				WeeklyHrsWorked = 40;  // Cap weekly hours at 40
			}
			MonthlyHrsWorked += hrsworked;
			// Calculate percentage again based on new hours worked
			Percentage = MonthlyHrsWorked * 100 / 160;  // 160 is the monthly work hour limit (40 * 4)
		}
		else {
			//awarding credit hours for leaves
			MonthlyHrsWorked += hrsworked;
		}
	}
	// Read file and populate attendance records
	void refresh() { // method at check-in
		records.clear();
		string filename = to_string(id) + ".txt";
		ifstream file(filename);

		if (!file.is_open()) {
			cerr << "No record found for the employee id: " << id << endl;
			return;
		}
		// to tackle: what if someone leaved without approval then what about leavebalances?
		int WeekIterate = 0;
		int HoursWorkedInWeek = 0;
		int MonthlyHrsWorked = 0;
		int currentweekhours = 0;
		string line;
		while (getline(file, line)) {

			stringstream ss(line);
			int id, week, day, hrsworked;

			// Parse the line (assumes format: ID DATE STATUS)
			ss >> id >> week >> day >> hrsworked;
			if (week > WeekIterate) {
				if (HoursWorkedInWeek >= 40) {
					HoursWorkedInWeek = 40;
				}
				MonthlyHrsWorked += HoursWorkedInWeek;
				HoursWorkedInWeek = 0;
				WeekIterate++;
			}
			if (week == MyTime::Week) {
				currentweekhours += hrsworked;
			}
			HoursWorkedInWeek += hrsworked;

			// Add the parsed record to the vector
			IAttendenceEntity* newentity = new AttendenceEntity(id, week, day, hrsworked);
			records.emplace_back(newentity);

		}
		if (HoursWorkedInWeek >= 40) {
			HoursWorkedInWeek = 40;
		}
		if (currentweekhours >= 40) {
			currentweekhours = 40;
		}
		MonthlyHrsWorked += HoursWorkedInWeek;
		updateSummary(MonthlyHrsWorked, currentweekhours);
		file.close();
	}

	void updateSummary(int hrs, int weekhrs) {
		MonthlyHrsWorked = hrs;
		Percentage = MonthlyHrsWorked * 100 / 160; // 40*4 = 160 monthly hour limit
		WeeklyHrsWorked = weekhrs;

	}
	// Display all attendance individual record
	void viewRecords() {
		cout << "-- My ID: " << id << endl;
		for (const auto& record : records) {
			record->display();
		}
		display();
	}
	void generateSummary(int percentage = 80) {
		if (Percentage < percentage) {
			string filename = to_string(MyTime::Day) + "-" + to_string(MyTime::Month) + "-" + to_string(MyTime::Year) + ".txt";
			ofstream outFile(filename, ios::app); // Open in append mode
			if (outFile.is_open()) {
				outFile << "-- Employee ID: " << id << endl;
				outFile.close();
				writeToFile(filename);
			}
			else {
				cerr << "Error: Unable to open file: " << filename << endl;
			}
		}
	}
	void display() {
		cout << "\tPercentage: " << Percentage << endl;
		cout << "\tweekly Hours worked: " << WeeklyHrsWorked << endl;
		cout << "\tMonthly Hours worked: " << MonthlyHrsWorked << endl;
	}
	void writeToFile(const string& filename) {
		ofstream outFile(filename, ios::app); // Open in append mode
		if (outFile.is_open()) {
			outFile << "\tPercentage: " << Percentage << endl;
			outFile << "\tWeekly Hours Worked: " << WeeklyHrsWorked << endl;
			outFile << "\tMonthly Hours Worked: " << MonthlyHrsWorked << endl;
			outFile.close();
		}
	}
};
