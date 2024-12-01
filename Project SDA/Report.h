#pragma once
#pragma once
using namespace std;
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include "Time.h"
#include <unordered_map>
#include <ctime>
#include "NotificationManager.h"


class Status;
// needed to make long leaves go to director after supervisor approval
class Employee;
class Main;



//class IAttendenceEntity {
//public:
//	virtual void display() = 0;
//	virtual int getEmpId() = 0;
//	virtual int getHours() = 0;
//	virtual string getFileObjectFormat() = 0;
//};

//class AttendenceEntity : public IAttendenceEntity {
//private:
//	int id;
//	int week;
//	int day;
//	int hours;
//
//public:
//	// Constructor
//	AttendenceEntity(int id, int w, int d, int h)
//		: id(id), week(w), day(d), hours(h) {}
//
//	
//	// Display the record
//	void display() {
//		cout << "ID: " << id << ", week: " << week << ", day: "<<day <<" hours: " << hours << endl;
//	}
//	string getFileObjectFormat(){
//		string format = to_string(id) + " " + to_string(week) + " " + to_string(day) + " " + to_string(hours);
//		return format;
//	}
//	
//
//	// Getters (if needed)
//	int getEmpId() { return id; }
//	int getHours() { return hours; }
////};
//template <typename T>
//class Record {
//public:
//	virtual void addRecord(T*) = 0;
//	virtual void viewRecords() = 0;
//	virtual void generateSummary(int = 0) = 0;
//	virtual void refresh() = 0;
//	virtual vector<T*>* getAllRecords() = 0;
//};
// Class to manage summary of attendence records, WeeklyHoursWorked resets after every week while the whole class reset after every month

//class ISummary : public ITimeObserver{
//private:
//	int* Percentage;
//	int* WeeklyHrsWorked;
//	int* MonthlyHrsWorked;
//	vector<IAttendenceEntity*>* records;
//public:
//	ISummary(int* p, int* w, int* m, vector<IAttendenceEntity*>* r) {
//		Percentage = p;
//		WeeklyHrsWorked = w;
//		MonthlyHrsWorked = m;
//		records = r;
//	}
//
//	void onWeekUpdate() override {
//		
//			*WeeklyHrsWorked = 0;
//	}
//
//	void onMonthUpdate() override {
//		
//		*WeeklyHrsWorked = 0;
//		*MonthlyHrsWorked = 0;
//		*Percentage = 0;
//	}
//	void onYearUpdate() override {
//	}
//};
//class Attendence : public Record<IAttendenceEntity> {
//private:
//	int id;
//	vector<IAttendenceEntity*> records; // Container to hold multiple records
//	int Percentage;	
//	int WeeklyHrsWorked;
//	int MonthlyHrsWorked;
//	ITimeObserver* summaryTimer;
//public:
//
//	Attendence() {
//
//	}
//	Attendence(int id) : id(id) {
//		summaryTimer = new ISummary(&Percentage,&WeeklyHrsWorked, &MonthlyHrsWorked, &records);
//		MyTime* timeobj = MyTime::getInstance();
//		timeobj->addObserver(summaryTimer);
//	}
//	vector<IAttendenceEntity*>* getAllRecords() {
//		return &records;
//	}
//	//Mark Attendence
//	void addRecord(IAttendenceEntity* attendenceObject) {
//
//		
//		// Add the new record to the vector
//		records.push_back(attendenceObject);
//
//		// Update the file with the new record
//		ofstream file(to_string(id) + ".txt", ios::app);  // Open file in append mode
//
//		if (!file.is_open()) {
//			cerr << "Unable to open attendance file for employee ID: " << id << endl;
//			return;
//		}
//		string format = attendenceObject->getFileObjectFormat();
//		file << format << endl; // impostor
//		file.close();
//		int hrsworked = attendenceObject->getHours();
//		addWeekHours(hrsworked, MyTime::Week);
//		cout << "Attendance marked for Employee ID " << id << " with " << hrsworked << " hours worked." << endl;
//	}
//
//	void addWeekHours(int hrsworked, int week) {
//		// if its simple attendence then 
//		if (week == MyTime::Week) {
//			MonthlyHrsWorked -= WeeklyHrsWorked;
//
//			WeeklyHrsWorked += hrsworked;  // Add the current day's hours to weekly total
//
//			if (WeeklyHrsWorked > 40) {
//				// If weekly hours exceed 40, cancel the update to MonthlyHrsWorked and reset WeeklyHrsWorked
//				WeeklyHrsWorked = 40;  // Cap weekly hours at 40
//			}
//			MonthlyHrsWorked += hrsworked;
//			// Calculate percentage again based on new hours worked
//			Percentage = MonthlyHrsWorked * 100 / 160;  // 160 is the monthly work hour limit (40 * 4)
//		}
//		else {
//		//awarding credit hours for leaves
//			MonthlyHrsWorked += hrsworked;
//		}
//	}
//	// Read file and populate attendance records
//	void refresh() { // method at check-in
//		records.clear();
//		string filename = to_string(id) + ".txt";
//		ifstream file(filename);
//
//		if (!file.is_open()) {
//			cerr << "No record found for the employee id: " << id << endl;
//			return;
//		}
//		// to tackle: what if someone leaved without approval then what about leavebalances?
//		int WeekIterate = 0;
//		int HoursWorkedInWeek = 0;
//		int MonthlyHrsWorked = 0;
//		int currentweekhours = 0;
//		string line;
//		while (getline(file, line)) {
//			
//			stringstream ss(line);
//			int id, week, day, hrsworked;
//
//			// Parse the line (assumes format: ID DATE STATUS)
//			ss >> id >> week >> day >> hrsworked;
//			if (week > WeekIterate) {
//				if (HoursWorkedInWeek >= 40) {
//					HoursWorkedInWeek = 40;
//				}
//				MonthlyHrsWorked += HoursWorkedInWeek;
//				HoursWorkedInWeek = 0;
//				WeekIterate++;
//			}
//			if (week == MyTime::Week) {
//				currentweekhours += hrsworked;
//			}
//			HoursWorkedInWeek += hrsworked;
//
//			// Add the parsed record to the vector
//			IAttendenceEntity* newentity = new AttendenceEntity(id, week, day, hrsworked);
//			records.emplace_back(newentity);
//
//		}
//		if (HoursWorkedInWeek >= 40) {
//			HoursWorkedInWeek = 40;
//		}
//		if (currentweekhours >= 40) {
//			currentweekhours = 40;
//		}
//		MonthlyHrsWorked += HoursWorkedInWeek;
//		updateSummary(MonthlyHrsWorked, currentweekhours);
//		file.close();
//	}
//
//	void updateSummary(int hrs, int weekhrs) {
//		MonthlyHrsWorked = hrs;
//		Percentage = MonthlyHrsWorked * 100 / 160; // 40*4 = 160 monthly hour limit
//		WeeklyHrsWorked = weekhrs;
//
//	}
//	// Display all attendance individual record
//	void viewRecords() {
//		cout << "-- My ID: "<< id << endl;
//		for (const auto& record : records) {
//			record->display();
//		}
//		display();
//	}
//	void generateSummary(int percentage = 80) {
//		if (Percentage < percentage) {
//			string filename = to_string(MyTime::Day) + "-" + to_string(MyTime::Month) + "-" + to_string(MyTime::Year) + ".txt";
//			ofstream outFile(filename, ios::app); // Open in append mode
//			if (outFile.is_open()) {
//				outFile << "-- Employee ID: " << id << endl;
//				outFile.close();
//				writeToFile(filename);
//			}
//			else {
//				cerr << "Error: Unable to open file: " << filename << endl;
//			}
//		}
//	}
//	void display() {
//		cout << "\tPercentage: " << Percentage << endl;
//		cout << "\tweekly Hours worked: " << WeeklyHrsWorked << endl;
//		cout << "\tMonthly Hours worked: " << MonthlyHrsWorked << endl;
//	}
//	void writeToFile(const string& filename) {
//		ofstream outFile(filename, ios::app); // Open in append mode
//		if (outFile.is_open()) {
//			outFile << "\tPercentage: " << Percentage << endl;
//			outFile << "\tWeekly Hours Worked: " << WeeklyHrsWorked << endl;
//			outFile << "\tMonthly Hours Worked: " << MonthlyHrsWorked << endl;
//			outFile.close();
//		}
//	}
//};

//class Guard {
//public:
//	void markAttendance(vector<Record<IAttendenceEntity>*> empReg, int eid, bool overtime = false) {
//		int hours = 8;
//		if (overtime)
//			hours = 10;
//		IAttendenceEntity* attendence = new AttendenceEntity(eid, MyTime::Week, MyTime::Day, hours);
//		empReg[eid-1]->addRecord(attendence);
//	};
//};
//
//class IApplicationObserver {
//public:
//	virtual void ForwardApplicationFurther(Record<IAttendenceEntity>* attenRecords, IApplication* application, int ApproverNumber = 0, string = "Pending") = 0;
//};
//class IApplicationObservable {
//public:
//	virtual void AddObserver(Record<IAttendenceEntity>* attenRecords, IApplication* application, IApplicationObserver*, int) = 0;
//	virtual void notifyApplicationChange(int, string) = 0;
//};
//class Status {
//	int appID;
//	string state;
//public:
//	Status(string stat, int id): appID(id) {
//		state = stat;
//	}
//	void setState(string stat, IApplication* app) {
//		state = stat;
//		updateLeaveStatus(app);
//	};
//	string getState() {
//		return state;
//	}
//	void updateLeaveStatus(IApplication* app) {
//		string eid = to_string(app->getEmpId());
//		string filename = eid + "LR.txt"; // File name based on employee ID
//		fstream file(filename, ios::in | ios::out);
//
//		if (!file.is_open()) {
//			cerr << "Unable to open leave record file for employee ID: " << app->getEmpId() << endl;
//			return;
//		}
//
//		string line;
//		stringstream buffer;
//		bool found = false;
//
//		// Read each line and check for the leave record ID
//		while (getline(file, line)) {
//			stringstream ss(line);
//			
//			int recordID;
//			// Parse the line
//			ss >> recordID;
//
//			// Check if this is the record to update
//			if (recordID == appID) {
//				found = true;
//
//				// Update the status
//				string format = app->getFileObjectFormat();
//				buffer << format << endl;
//			}
//			else {
//				// Write the unchanged record back to the buffer
//				buffer << recordID << line << endl;
//			}
//		}
//
//		file.close(); // Close the file before reopening for writing
//
//		if (!found) {
//			cerr << "No leave record found with ID: " << appID << endl;
//			return;
//		}
//
//		// Write the updated buffer back to the file
//		ofstream outFile(filename, ios::trunc);
//		if (!outFile.is_open()) {
//			cerr << "Unable to update leave record file for employee ID: " << eid << endl;
//			return;
//		}
//
//		outFile << buffer.str();
//		outFile.close();
//
//		cout << "Leave record with ID " << appID << " successfully updated." << endl;
//	}
//
//};

//class DefaultLeaveApplication: public IApplication {
//private:
//	int eid;
//	int id;
//	string leaveType;
//	string days;       // Number of leave days
//	Status status;
//	string date;
//	string to;
//	string from;
//	string reason;
//
//public:
//	// Constructor
//	DefaultLeaveApplication(int Eid, int ID, string leaveType, string days, string Date, string To, string From, string Reason, string state="Pending") : date(Date), to(To), from(From), reason(Reason), eid(Eid), id(ID), leaveType(leaveType), days(days), status(state, ID) { // when new app is created it gets pending
//	}
//
//	// Display the record
//	void display() {
//		cout << "Emp. ID: " << eid << ", Leave type: " << leaveType << ", days: " << days << ", from: " << from << ", to: "<< to << endl;
//	}
//	string getFileObjectFormat() {
//		string objformat = to_string(id) + " " + leaveType + " " + to_string(eid) + " " + (days) + " " + status.getState() + " " + (date) + " " + (to) + " " + (from) + " " + reason;
//		return objformat;
//	}
//	// Getters
//	string getApplicationType() { return leaveType; }
//	string getDate() { return date; }
//	string getToDate() { return from; }
//	int getEmpId() { return (eid); }
//	int getRecordId() { return (id); }
//	int getDuration() { return stoi(days); }
//	void setStatus(string newstate){
//		status.setState(newstate, &(*this));
//	} 
//	static IApplication* getObjectFromLine(const string& line) {
//		stringstream ss(line);
//		int eid, id;
//		string Type, days, status, date, to, from, reason;
//
//		ss >> id >> Type >> eid >> days >> status >> date >> to >> from >> reason;
//		return new DefaultLeaveApplication(eid, id, Type, days, date, to, from, reason, status);
//	}
//	string getStatus() {
//		return
//			status.getState();
//	}
//	void addRecordInFile() {
//
//		// Prepare the file to append the new record
//		string filename = to_string(eid) + "LR.txt"; // File is named after Employee ID
//		ofstream file(filename, ios::app); // Open file in append mode
//
//		if (!file.is_open()) {
//			cerr << "Error: Unable to open file for writing.\n";
//			return;
//		}
//
//		// Write the record to the file
//		string objFormat = getFileObjectFormat();
//		file << objFormat << endl;
//		file.close(); // Close the file
//		cout << "Leave record added successfully.\n";
//	}
//};
//
//class OfficialLeaveApp : public DefaultLeaveApplication {
//private:
//	string travelAllowance; // Amount allocated for travel
//	string purpose;         // Purpose of the leave
//	string destination;     // Destination for official duty
//
//public:
//	// Constructor
//	OfficialLeaveApp(int eid, int id, string leaveType, string days, string date, string to, string from, string reason,
//		string travelAllowance, string purpose, string destination, string state = "Pending")
//		: DefaultLeaveApplication(eid, id, leaveType, days, date, to, from, reason, state),
//		travelAllowance(travelAllowance), purpose(purpose), destination(destination) {}
//
//	// Overriding display to include additional data
//	void display() override {
//		DefaultLeaveApplication::display();
//		cout << "Travel Allowance: " << travelAllowance << ", Purpose: " << purpose
//			<< ", Destination: " << destination << endl;
//	}
//
//	// Overriding file format to include new fields
//	string getFileObjectFormat() override {
//		return DefaultLeaveApplication::getFileObjectFormat() + " " + travelAllowance + " " + purpose + " " + destination;
//	}
//
//	// Static method to create an OfficialLeave object from a file line
//	static OfficialLeaveApp* getObjectFromLine(const string& line, string type = "Official") {
//		stringstream ss(line);
//		int eid, id;
//		string Type, days, status, date, to, from, reason, travelAllowance, purpose, destination;
//
//		ss >> id >> Type >> eid >> days >> status >> date >> to >> from >> reason;
//		ss>>travelAllowance >> purpose >> destination;
//
//		return new OfficialLeaveApp(eid, id, Type, days, date, to, from, reason, travelAllowance, purpose, destination, status);
//	}
//};
//
//class UnpaidLeaveApp : public DefaultLeaveApplication {
//private:
//	string leaveAddress;   // Estimated financial loss for unpaid leave
//
//public:
//	// Constructor
//	UnpaidLeaveApp(int eid, int id, string leaveType, string days, string date, string to, string from, string reason,
//		string LeaveAddress, string state = "Pending")
//		: DefaultLeaveApplication(eid, id, leaveType, days, date, to, from, reason, state),
//		leaveAddress(LeaveAddress) {}
//
//	// Overriding display to include additional data
//	void display() override {
//		DefaultLeaveApplication::display();
//		cout << "leave Address: " << leaveAddress << endl;
//	}
//
//	// Overriding file format to include new fields
//	string getFileObjectFormat() override {
//		return DefaultLeaveApplication::getFileObjectFormat() + " " + leaveAddress;
//	}
//
//	// Static method to create an UnpaidLeave object from a file line
//	static UnpaidLeaveApp* getObjectFromLine(const string& line, string type = "Unpaid") {
//		stringstream ss(line);
//		int eid, id;
//		string Type, leaveAddress, days, status, date, to, from, reason, financialImpact, alternativeContact;
//
//		ss >> id >> Type >> eid >> days >> status >> date >> to >> from >> reason;
//		ss >> leaveAddress;
//
//		return new UnpaidLeaveApp(eid, id, Type, days, date, to, from, reason, leaveAddress, status);
//	}
//};



//class Leave : public Record<IApplication> { //needed ; on exit, save the records again
//private:
//	int id;                                  // Employee ID
//	vector<IApplication*> records;
//
//public:
//	// Constructor
//	Leave() {}
//	Leave(int ida) : id(ida) {
//	}
//	vector<IApplication*>* getAllRecords() {
//		return &records;
//	}
//	void addRecord(IApplication* app) {
//		records.push_back(app);
//		app->addRecordInFile();
//	}
//	void refresh() {
//		string filename = to_string(id) + "LR.txt"; // File name based on ID
//		ifstream file(filename);
//
//		if (!file.is_open()) {
//			cerr << "No leave record found for employee ID: " << id << endl;
//			return;
//		}
//
//		records.clear(); // Clear previous records
//		
//		string line;
//		while (getline(file, line)) {
//			IApplication* app = AppsGeneratingFactory(line);
//			if (app)
//				records.push_back(app);
//		}
//
//		file.close();
//	}
//	IApplication* AppsGeneratingFactory(const std::string& line) {
//		if (line.size() > 0) {
//			stringstream ss(line);
//			string leaveType;
//			// Parse the line
//			ss >> leaveType;
//
//			// Instantiate based on leaveType
//			if (leaveType == "Official") {
//				return OfficialLeaveApp::getObjectFromLine(line);
//			}
//			else if (leaveType == "Unpaid") {
//				return UnpaidLeaveApp::getObjectFromLine(line);
//			}
//			else {
//				return DefaultLeaveApplication::getObjectFromLine(line);
//			}
//		}
//		return nullptr;
//	}
//
//	// Display all individual leave records
//	void viewRecords() {
//		cout << "-- Leave Records for Employee ID: " << id << " --" << endl;
//		for (auto& record : records) {
//			record->display();
//		}
//	}
//	void generateSummary(int = 0) {
//		// Map to store leaveType and corresponding total leave days
//		unordered_map<string, int> leaveSummary;
//
//		// Iterate over all records and sum up the leave days for each leaveType
//		for (auto& record : records) {
//			if (stoi(record->getDate().substr(record->getDate().size() - 4)) == MyTime::Year) //fetch year from date format dd-mm-yyyy
//			{
//				string leaveType = record->getApplicationType(); // Get leaveType
//				int days = record->getDuration();               // Get leave duration
//
//				// Add to the corresponding leaveType in the map
//				leaveSummary[leaveType] += days;
//			}
//		}
//
//		// Display the summary
//		cout << "-- Leave Summary for Employee ID: " << id << " -- for the current year of " << MyTime::Year << endl;
//		
//		for (const auto& pair : leaveSummary) {
//			const string& leaveType = pair.first;  // Access the key (leaveType)
//			int totalDays = pair.second;          // Access the value (totalDays)
//
//			cout << "Leave Type: " << leaveType << ", Total Leaves: " << totalDays << endl;
//		}
//
//	}
//
//};


//int getYear(string date) {
//	// Assuming the date format is "DD-MM-YYYY"
//	return stoi(date.substr(date.size() - 4));
//}
//int getWeekNumberInMonth(const std::string& dateStr) {
//	// Parse the input string in dd-mm-yyyy format
//	int day, month, year;
//	char delimiter1, delimiter2;
//	std::istringstream dateStream(dateStr);
//	dateStream >> day >> delimiter1 >> month >> delimiter2 >> year;
//
//	if (delimiter1 != '-' || delimiter2 != '-' || dateStream.fail()) {
//		throw std::invalid_argument("Invalid date format. Expected format: dd-mm-yyyy");
//	}
//
//	// Create a tm struct for the first day of the given month
//	std::tm firstDayOfMonth = {};
//	firstDayOfMonth.tm_mday = 1;
//	firstDayOfMonth.tm_mon = month - 1; // tm_mon is 0-based
//	firstDayOfMonth.tm_year = year - 1900; // tm_year is years since 1900
//
//	// Normalize the first day of the month
//	std::mktime(&firstDayOfMonth);
//
//	// Get the weekday for the 1st day of the month (0 = Sunday, 1 = Monday, ..., 6 = Saturday)
//	int firstDayWeekday = (firstDayOfMonth.tm_wday == 0) ? 7 : firstDayOfMonth.tm_wday; // Adjust so Monday = 1, ..., Sunday = 7
//
//	// Calculate the week number
//	int adjustedDay = day + (firstDayWeekday - 1); // Account for days before the 1st of the month
//	int weekNumber = (adjustedDay - 1) / 7 + 1;
//
//	return weekNumber;
//}
//
//struct PendingWork{
//	int ApproverNumber;
//	IApplicationObserver* team;
//	IApplication* app;
//	Record<IAttendenceEntity>* attenRecords;
//	PendingWork(int AN, IApplicationObserver* Team, IApplication* appl, Record<IAttendenceEntity>* AttenRecords) {
//		ApproverNumber = AN;
//		team = Team;
//		app = appl;
//		attenRecords = AttenRecords;
//	}
//};
//class AdminObserverManager {
//	vector<PendingWork> appObservers;
//public:
//	void AddObserver(Record<IAttendenceEntity>* attenRecords, IApplication* record, IApplicationObserver* team, int AN) {
//		appObservers.push_back(PendingWork(AN, team, record, attenRecords));
//	}
//	vector<PendingWork> getMyPendingWork() const {
//		return appObservers;
//	}
//};
//
//class AdminNotifier {
//public:
//	void notifyApplicationChange(vector<PendingWork> appObservers, int index, string status) {
//		if (index < 0 || index >= appObservers.size()) return; // Validate index
//		appObservers[index].team->ForwardApplicationFurther(
//			appObservers[index].attenRecords,
//			appObservers[index].app,
//			++(appObservers[index].ApproverNumber),
//			status
//		);
//		appObservers.erase(appObservers.begin() + index);
//	}
//};
//
//
//class DisplayStrategy {
//public:
//	bool display(vector<PendingWork> pendingWork) {
//		cout << "Pending Work for Admin: " << endl;
//		int index = 0;
//		for (const auto& pending : pendingWork) {
//			cout << index++ << ". ";
//			pending.app->display();
//		}
//		if (index == 0)
//			return false;
//		return true;
//	}
//};
//
//
//
//class Admin : public IApplicationObservable {
//protected:
//	AdminObserverManager observerManager;
//	AdminNotifier notifier;
//	DisplayStrategy displayStrategy;
//public:
//	void AddObserver(Record<IAttendenceEntity>* attenRecords, IApplication* record, IApplicationObserver* team, int AN) {
//		observerManager.AddObserver(attenRecords, record, team, AN);
//	}
//	bool displayPendingWork() {
//		return displayStrategy.display(observerManager.getMyPendingWork());
//	}
//	void notifyApplicationChange(int index, string status) {
//		notifier.notifyApplicationChange(observerManager.getMyPendingWork(), index, status);
//	}
//	virtual int getID() = 0;
//};

//class Director : public Admin {
//	int id;
//public:
//
//	Director() {}
//	Director(int ida) { id = ida; }
//	int getID() { return id; }
//
//};
//class Supervisor : public Admin {
//	int id;
//public:
//	Supervisor(){}
//	Supervisor(int ida) { id = ida; }
//	int getID() { return id; }
//};
//class IPostAction {
//protected:
//	IApplication* latestApp;
//public:
//	IPostAction(IApplication* latest) {
//		latestApp = latest;
//	}
//	virtual void postaction() = 0;
//};
//
//class AwardHoursAction : public IPostAction {
//	Attendence* attenRecord;
//	int NoOfHours;
//public:
//	AwardHoursAction(Record<IAttendenceEntity>* record, IApplication* latest, int hours) : IPostAction(latest), NoOfHours(hours) {
//		attenRecord = static_cast<Attendence*>(record);
//	}
//	void postaction() {
//		attenRecord->addWeekHours(NoOfHours, getWeekNumberInMonth(latestApp->getToDate()));
//	}
//};
//class IValidator {
//public:
//	virtual bool validate() {
//		return true; // default
//	};
//
//};

//class CasualLeaveValidator : public IValidator {
//	vector<IApplication*>* LeavesHistory;
//	IApplication* latestApp;
//public:
//	CasualLeaveValidator(vector<IApplication*>* history, IApplication* latest) {
//		LeavesHistory = history;
//		latestApp = latest;
//	}
//	bool validate() override {
//		int leavescount = calculateEmpCasualLeavesCount(LeavesHistory);
//		if (leavescount < 15 && latestApp->getDuration() <= 4) { // "cannot avail more than 4 leaves at a time"
//			return true;
//		}
//		return false;
//	}
//	int calculateEmpCasualLeavesCount(vector<IApplication*>* records) {
//		// Initialize casualLeavesCount to 0
//		int casualLeavesCount = 0;
//		int currentYear = MyTime::Year;
//		// Traverse through each record (IApplication pointer)
//		for (auto& record : *records) {
//			// Check if the application type is "Casual" and the year matches the current year
//			if (record->getApplicationType() == "Casual" && getYear(record->getDate()) == currentYear) {
//				// Add the number of days to casualLeavesCount
//				casualLeavesCount += record->getDuration();
//			}
//		}
//
//		return casualLeavesCount;
//	}
//
//};

//class EarnedLeaveValidator : public IValidator {
//	vector<IApplication*>* LeavesHistory;
//	IApplication* latestApp;
//public:
//	EarnedLeaveValidator(vector<IApplication*>* history) {
//		LeavesHistory = history;
//	}
//	bool validate() override {
//		int leavescount = calculateEarnedLeavesCount(LeavesHistory);
//		if (leavescount < 21 && latestApp->getDuration() > 4) {
//			return true;
//		}
//		return false;
//	}
//	int calculateEarnedLeavesCount(vector<IApplication*>* records) {
//		// Initialize casualLeavesCount to 0
//		int casualLeavesCount = 0;
//		int currentYear = MyTime::Year;
//		// Traverse through each record (IApplication pointer)
//		for (auto& record : *records) {
//			// Check if the application type is "Casual" and the year matches the current year
//			if (record->getApplicationType() == "Earned" && getYear(record->getDate()) == currentYear) {
//				// Add the number of days to casualLeavesCount
//				casualLeavesCount += record->getDuration();
//			}
//		}
//
//		return casualLeavesCount;
//	}
//};

//class Leaves_Team : public IApplicationObserver {
//protected:
//	vector<IApplication*> applications;
//	vector<IApplicationObservable*> approvers;  // List of approvers for a team
//public:
//	virtual ~Leaves_Team() = default;
//
//	bool apply(IApplication* application, Record<IAttendenceEntity>* attenRecords, vector<IApplication*>* leaveRecords) { // template method for applying procedure f0r all teams
//		bool check = true;
//		check = validate(leaveRecords, application);
//		if (check) {
//			AddInApplicationVector(application); //Command pattern (submitting leave).
//			ForwardApplicationFurther(attenRecords, application); // send to first admin in hierachy
//			return true;
//		}
//		else {
//			NotifiyFailure(application->getEmpId());
//			return false;
//		}
//	}
//
//	virtual void ForwardApplicationFurther(Record<IAttendenceEntity>*, IApplication* application, int ApproverNumber = 0, string status = "Pending") = 0;
//	virtual string getType() = 0;
//	virtual bool validate(vector<IApplication*>* leaveRecords, IApplication* newapp) {
//		return true; // default validation is true;
//	};
//	void AddInApplicationVector(IApplication* application) {
//		applications.emplace_back(application);
//	}
//	void NotifyStatusChange(IApplication* application) {
//		INotificationManager* notificationManager = NotificationManager::getInstance();
//		string msg = "[" + getType() + " Leave] Your application (ID: " + to_string(application->getRecordId()) + ") has been " + application->getStatus() + "!";
//		notificationManager->addNotification(application->getEmpId(), msg);
//	}
//	void NotifiyFailure(int eid) {
//		INotificationManager* notificationManager = NotificationManager::getInstance();
//		string msg = "[" + getType() + " Leave] Your application could not be processed!";
//		notificationManager->addNotification(eid, msg);
//	}
//	void getApplications() {
//		cout << "displaying outstanding leave applications" << endl;
//		for (auto& app : applications) {
//			app->display();
//		}
//	}
//};

//
//class Casual : public Leaves_Team {
//private:
//	static Casual* instance;
//
//	Casual(vector<IApplicationObservable*> admin) {
//		approvers = admin;
//	}
//
//public:
//	static Casual* getInstance(vector<IApplicationObservable*> admins) {
//		if (!instance) {
//			instance = new Casual(admins);
//		}
//		return instance;
//	}
//	bool validate(vector<IApplication*>* leaveRecords, IApplication* newapp) override {
//		IValidator* validator = new CasualLeaveValidator(leaveRecords, newapp);
//		bool result = validator->validate();
//		delete validator;
//		return result;
//	}
//	void postactions(Record<IAttendenceEntity>* attenrecords, IApplication* newapp) {
//		IPostAction* action = new AwardHoursAction(attenrecords, newapp, newapp->getDuration()*8);
//		action->postaction();
//		delete action;
//	}
//	void ForwardApplicationFurther(Record<IAttendenceEntity>* attenRecords, IApplication* application, int ApproverNumber = 0, string status = "Pending") { // actually registering in observables (approval team)
//		if (ApproverNumber >= approvers.size() || status == "Rejected") // // supervisor has denied the application so do not forward to Director
//		{// application has gone through all the process now update the status
//			application->setStatus(status);
//			NotifyStatusChange(application);
//			if (status == "Approved") {
//				postactions(attenRecords, application);
//			}
//		}
//		else {
//			approvers[ApproverNumber]->AddObserver(attenRecords, application, Casual::getInstance(approvers), ApproverNumber);  // Start the approval process with the first approver
//
//		}
//	}
//	
//
//	
//	string getType() {
//		return "Casual";
//	}
//};
//Casual* Casual::instance = nullptr;




//class Official : public Leaves_Team {
//private:
//	static Official* instance;
//
//	Official(vector<IApplicationObservable*> admins) {
//		approvers = admins;
//	}
//
//public:
//	static Official* getInstance(vector<IApplicationObservable*> admins) {
//		if (!instance) {
//			instance = new Official(admins);
//		}
//		return instance;
//	}
//	void postactions(Record<IAttendenceEntity>* attenrecords, IApplication* newapp) {
//		IPostAction* action = new AwardHoursAction(attenrecords, newapp, 8);
//		action->postaction();
//		delete action;
//	}
//	void ForwardApplicationFurther(Record<IAttendenceEntity>* attenRecords, IApplication* application, int ApproverNumber = 0, string status = "Pending") { // actually registering in observables (approval team)
//		if (ApproverNumber >= approvers.size() || status == "Rejected") // // supervisor has denied the application so do not forward to Director
//		{// application has gone through all the process now update the status
//			application->setStatus(status);
//			NotifyStatusChange(application);
//			if (status == "Approved") {
//				postactions(attenRecords, application);
//			}
//		}
//		else {
//			approvers[ApproverNumber]->AddObserver(attenRecords, application, Official::getInstance(approvers), ApproverNumber);  // Start the approval process with the first approver
//
//		}
//	}
//	string getType() {
//		return "Official";
//	}
//};
//Official* Official::instance = nullptr;

//class Earned : public Leaves_Team {
//private:
//	static Earned* instance;
//
//	Earned(vector<IApplicationObservable*> admins) {
//		approvers = admins;
//	}
//
//public:
//	static Earned* getInstance(vector<IApplicationObservable*> admins) {
//		if (!instance) {
//			instance = new Earned(admins);
//		}
//		return instance;
//	}
//	bool validate(vector<IApplication*>* leaveRecords, IApplication* newapp) override {
//		IValidator* validator = new EarnedLeaveValidator(leaveRecords);
//		bool result = validator->validate();
//		delete validator;
//		return result;
//	}
//	void postactions(Record<IAttendenceEntity>* attenrecords, IApplication* newapp) {
//		IPostAction* action = new AwardHoursAction(attenrecords, newapp, newapp->getDuration() * 8);
//		action->postaction();
//		delete action;
//	}
//	void ForwardApplicationFurther(Record<IAttendenceEntity>* attenRecords, IApplication* application, int ApproverNumber = 0, string status = "Pending") { // actually registering in observables (approval team)
//		if (ApproverNumber >= approvers.size() || status == "Rejected") // // supervisor has denied the application so do not forward to Director
//		{// application has gone through all the process now update the status
//			application->setStatus(status);
//			NotifyStatusChange(application);
//			if (status == "Approved") {
//
//				postactions(attenRecords, application);
//			}
//		}
//		else {
//			approvers[ApproverNumber]->AddObserver(attenRecords, application, Earned::getInstance(approvers), ApproverNumber);  // Start the approval process with the first approver
//
//		}
//	}
//	string getType() {
//		return "Earned";
//	}
//};
//Earned* Earned::instance = nullptr;





//class Employee {
//private:
//	int id;
//
//	Record<IAttendenceEntity>* myAttendenceRecord;
//	Record<IApplication>* myLeaveRecord;
//public:
//
//	Employee() {}
//	Employee(int id, Record<IAttendenceEntity>* NewRecord, Record<IApplication>* NewLeaveRecord) : id(id) {
//		myAttendenceRecord = NewRecord;
//		myLeaveRecord = NewLeaveRecord;
//		myAttendenceRecord->refresh();
//		myLeaveRecord->refresh();
//	}
//
//	void generateAttendenceReport() {
//		myAttendenceRecord->viewRecords();
//	}
//	void generateLeaveReport() {
//		myLeaveRecord->viewRecords();
//
//	}
//	void generateLeaveSummary() {
//		myLeaveRecord->generateSummary();
//	}
//
//	void applyLeave(IApplication* app, Leaves_Team* team) {
//		// file the leave
//		bool success = team->apply(app, myAttendenceRecord, myLeaveRecord->getAllRecords());
//		if (success) {
//			myLeaveRecord->addRecord(app);
//		}
//
//	}
//	int getID() {
//		return id;
//	}
//	bool operator==(const Employee& other) const {
//		return this->id == other.id;
//	}
//	~Employee() {
//
//	}
////};
//struct inactive {
//	int id;
//	int days;
//};
//generate monthly reports
//class IMonthlyReports : public ITimeObserver {
//	vector<Record<IAttendenceEntity>*>* empAttendRegisters;
//	vector<inactive>* inactiveIDs;
//public:
//	IMonthlyReports(vector<Record<IAttendenceEntity>*>* reg, vector<inactive>* outreg) {
//		empAttendRegisters = reg;
//		inactiveIDs = outreg;
//	}
//	void onWeekUpdate() {};
//	void onMonthUpdate() {
//		GenerateMonthlyReports();
//		GenerateSalaries();
//	};
//	void onYearUpdate() {};
//	void GenerateMonthlyReports() {
//		for (auto& rgister : *empAttendRegisters) {
//			rgister->generateSummary(80);
//		}
//	}
//	void GenerateSalaries() {
//		cout << "[SYSTEM] Salary" << endl;
//		for (int i = 0; i < empAttendRegisters->size(); i++) {
//			if (isEmpInactive(i + 1)) {
//				cout << "- Emp # " << i + 1 << " is gone to some visit so no salary" << endl;
//			}
//			else {
//				cout << "- Awarded salary to Emp # " << i + 1 << "!" << endl;
//			}
//		} // impostor give only salaries to Emp having > 80 attendence
//	}
//	bool isEmpInactive(int id) {
//		for (int i = 0; i < inactiveIDs->size(); i++) {
//			if (id == (*inactiveIDs)[i].id) {//employee is inactive
//				return true;
//			}
//		}
//		return false;
//	}
//};
//class InactivityManager : public ITimeObserver {
//	vector<inactive>* inactiveIDs;
//public:
//	InactivityManager(vector<inactive>* reg) {
//		inactiveIDs = reg;
//	}
//	void onWeekUpdate() {
//		reduceInactiveDays(7);
//	};
//	void onMonthUpdate() {
//		reduceInactiveDays(30);
//	};
//	void onYearUpdate() {
//		reduceInactiveDays(365);
//	};
//	void reduceInactiveDays(int days) {
//		for (int i = 0; i < inactiveIDs->size(); ) { // Use a while loop to handle dynamic resizing
//			(*inactiveIDs)[i].days -= days; // Reduce days
//			if ((*inactiveIDs)[i].days <= 0) {
//				// Remove the inactive element if days <= 0
//				inactiveIDs->erase(inactiveIDs->begin() + i);
//				// Do not increment i, as the next element shifts into the current index
//			}
//			else {
//				++i; // Only increment if no element was erased
//			}
//		}
//	}
//};
//class Main {
//private:
//	static Main* main;
//	vector<Record<IAttendenceEntity>*> empAttendRegisters;
//	vector<Record<IApplication>*> empLeaveRecords;
//	vector<inactive> inactiveIDs;
//	Director director;
//	Supervisor supervisor;
//	Guard guard;
//	ITimeObserver* ReportsGenerator;
//	ITimeObserver* inactivityManager;
//	Main(){
//		ReportsGenerator = new IMonthlyReports(&empAttendRegisters, &inactiveIDs);
//		inactivityManager = new InactivityManager(&inactiveIDs);
//		MyTime* timeobj = MyTime::getInstance();
//		timeobj->addObserver(ReportsGenerator);
//		timeobj->addObserver(inactivityManager);
//	}
//public:
//	static Main* getMain() {
//		if (main == nullptr) {
//			main = new Main();
//
//		}
//		return main;
//	}
//	void markattendenceportal() {
//		int eid;
//		cout << "whose attendence you want to mark? " << endl;
//		cin >> eid;
//
//		guard.markAttendance(empAttendRegisters, eid);
//	}
//	Record<IAttendenceEntity>* generateEmployeeRecord(int id) { // Factory Method called when creating employees agr employe null krta ha register ko istrah to record ka track to gya, agr to record getter sy arha to koi b edit kr skta 
//		string filename = to_string(id) + ".txt";
//		ifstream file(filename);
//		MyTime* timeobj = MyTime::getInstance();
//		if (!file.is_open()) {
//			file.close();
//			//register new record to system
//			ofstream fw(filename);
//			fw.close();
//		}
//		Record<IAttendenceEntity>* newentity = new Attendence(id);
//		empAttendRegisters.push_back(newentity);
//		return empAttendRegisters.back();
//
//	}
//	void addEmpInactive(int id, int days) {
//		inactive newentity;
//		newentity.id = id; newentity.days = days;
//		inactiveIDs.push_back(newentity);
//	}
//	Record<IApplication>* createLeaveRecord(int id) { // Factory Method called when creating employees agr employe null krta ha register ko istrah to record ka track to gya, agr to record getter sy arha to koi b edit kr skta 
//		string filename = to_string(id) + "LR.txt";
//		ifstream file(filename);
//
//		if (!file.is_open()) {
//			file.close();
//			//register new record to system
//			ofstream fw(filename);
//			fw.close();
//		}
//		Record<IApplication>* newentity = new Leave(id);
//		empLeaveRecords.push_back(newentity);
//		return empLeaveRecords.back();
//
//	}
//
//	void getCollectiveReportConsole(int percentage) {
//		for (auto& rgister : empAttendRegisters) {
//			rgister->generateSummary(percentage);
//		}
//	}
//};
//
//
//
//Main* Main::main = nullptr;


//class UnpaidLeavePA : public IPostAction {
//	Main* system;
//public:
//	UnpaidLeavePA(IApplication* latest, Main* systeM) : IPostAction(latest) {
//		latestApp = latest;
//		system = systeM;
//	}
//	void postaction() {
//		system->addEmpInactive(latestApp->getEmpId(), latestApp->getDuration());
//	}
//};


//class Unpaid : public Leaves_Team {
//private:
//	static Unpaid* instance;
//
//	Unpaid(vector<IApplicationObservable*> admins) {
//		approvers = admins;
//	}
//
//public:
//	static Unpaid* getInstance(vector<IApplicationObservable*> admins) {
//		if (!instance) {
//			instance = new Unpaid(admins);
//		}
//		return instance;
//	}
//	void postactions(IApplication* newapp) {
//		IPostAction* action = new UnpaidLeavePA(newapp, Main::getMain());
//		action->postaction();
//		delete action;
//	}
//	void ForwardApplicationFurther(Record<IAttendenceEntity>* attenRecords, IApplication* application, int ApproverNumber = 0, string status = "Pending") { // actually registering in observables (approval team)
//		if (ApproverNumber >= approvers.size() || status == "Rejected") // // supervisor has denied the application so do not forward to Director
//		{// application has gone through all the process now update the status
//			application->setStatus(status);
//			NotifyStatusChange(application);
//			if (status == "Approved") {
//				postactions(application);
//			}
//		}
//		else {
//			approvers[ApproverNumber]->AddObserver(attenRecords, application, Unpaid::getInstance(approvers), ApproverNumber);  // Start the approval process with the first approver
//
//		}
//	}
//	string getType() {
//		return "Unpaid";
//	}
//};
//Unpaid* Unpaid::instance = nullptr;
