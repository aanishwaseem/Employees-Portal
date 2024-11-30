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


class Status;
// needed to make long leaves go to director after supervisor approval
class Employee;
class Main;


class IApplication {
public:
	virtual void display() = 0;
	virtual int getEmpId() = 0;
	virtual void setStatus(string) = 0;
	virtual void addRecordInFile() = 0;
	virtual string getApplicationType() = 0;
	virtual string getDate() = 0;
	virtual string getToDate() = 0;
	virtual int getDuration() = 0;
	virtual string getStatus() = 0;
	virtual string getFileObjectFormat() = 0;
};
class IAttendenceEntity {
public:
	virtual void display() = 0;
	virtual int getEmpId() = 0;
	virtual int getHours() = 0;
	virtual string getFileObjectFormat() = 0;
};

class AttendanceRecord : public IAttendenceEntity {
private:
	int id;
	int week;
	int day;
	int hours;

public:
	// Constructor
	AttendanceRecord(int id, int w, int d, int h)
		: id(id), week(w), day(d), hours(h) {}

	
	// Display the record
	void display() {
		cout << "ID: " << id << ", week: " << week << ", day: "<<day <<" hours: " << hours << endl;
	}
	string getFileObjectFormat(){
		string format = to_string(id) + " " + to_string(week) + " " + to_string(day) + " " + to_string(hours);
		return format;
	}
	

	// Getters (if needed)
	int getEmpId() { return id; }
	int getHours() { return hours; }
};
template <typename T>
class Record {
public:
	virtual void addRecord(T*) = 0;
	virtual void viewRecords() = 0;
	virtual void generateSummary() = 0;
	virtual void refresh() = 0;
	virtual vector<T*>* getAllRecords() = 0;
};
// Class to manage summary of attendence records, WeeklyHoursWorked resets after every week while the whole class reset after every month

class ISummary : public ITimeObserver{
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
	vector<IAttendenceEntity*> records; // Container to hold multiple records
	int Percentage;	
	int WeeklyHrsWorked;
	int MonthlyHrsWorked;
	ITimeObserver* summaryTimer;
public:

	Attendence() {

	}
	Attendence(int id) : id(id) {
		summaryTimer = new ISummary(&Percentage,&WeeklyHrsWorked, &MonthlyHrsWorked, &records);
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
		file << format << endl;
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
			IAttendenceEntity* newentity = new AttendanceRecord(id, week, day, hrsworked);
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
		cout << "-- My ID: "<< id << endl;
		for (const auto& record : records) {
			record->display();
		}
		display();
	}
	void generateSummary() {
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

class Guard {
public:
	void markAttendance(vector<Record<IAttendenceEntity>*> empReg, int eid, bool overtime = false) {
		int hours = 8;
		if (overtime)
			hours = 10;
		IAttendenceEntity* attendence = new AttendanceRecord(eid, MyTime::Week, MyTime::Day, hours);
		empReg[eid-1]->addRecord(attendence);
	};
};

class IApplicationObserver {
public:
	virtual void ForwardApplicationFurther(IApplication* application, int ApproverNumber = 0, string = "Pending") = 0;
};
class IApplicationObservable {
public:
	virtual void AddObserver(IApplication* application, IApplicationObserver*, int) = 0;
	virtual void notifyApplicationChange(int, string) = 0;
};
class Status {
	int appID;
	string state;
public:
	Status(string stat, int id): appID(id) {
		state = stat;
	}
	void setState(string stat, IApplication* app) {
		state = stat;
		updateLeaveStatus(app);
	};
	string getState() {
		return state;
	}
	void updateLeaveStatus(IApplication* app) {
		string eid = to_string(app->getEmpId());
		string filename = eid + "LR.txt"; // File name based on employee ID
		fstream file(filename, ios::in | ios::out);

		if (!file.is_open()) {
			cerr << "Unable to open leave record file for employee ID: " << app->getEmpId() << endl;
			return;
		}

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
				string format = app->getFileObjectFormat();
				buffer << format << endl;
			}
			else {
				// Write the unchanged record back to the buffer
				buffer << recordID << line << endl;
			}
		}

		file.close(); // Close the file before reopening for writing

		if (!found) {
			cerr << "No leave record found with ID: " << appID << endl;
			return;
		}

		// Write the updated buffer back to the file
		ofstream outFile(filename, ios::trunc);
		if (!outFile.is_open()) {
			cerr << "Unable to update leave record file for employee ID: " << eid << endl;
			return;
		}

		outFile << buffer.str();
		outFile.close();

		cout << "Leave record with ID " << appID << " successfully updated." << endl;
	}

};
class ILeaveRecordAdapter {
public:
	virtual ~ILeaveRecordAdapter() = default;

	// Method to map input data to LeaveRecord fields
	virtual vector<string> getFields() const = 0;
};
class LeaveRecord: public IApplication {
private:
	int NoOfFields = 7;
	int eid;
	int id;
	string leaveType;  // 0 for casual leave, 1 for earned leave
	string days;       // Number of leave days
	Status status;
	string date;
	string to;
	string from;
	string reason;

public:
	// Constructor
	LeaveRecord(int Eid, int ID, string leaveType, string days, string Date, string To, string From, string Reason, string state="Pending") : date(Date), to(To), from(From), reason(Reason), eid(Eid), id(ID), leaveType(leaveType), days(days), status(state, ID) { // when new app is created it gets pending
	}

	// Display the record
	void display() {
		cout << "Emp. ID: " << eid << ", Leave type: " << leaveType << ", days: " << days << ", from: " << from << ", to: "<< to << endl;
	}
	string getFileObjectFormat() {
		string objformat = to_string(id) + " " + leaveType + " " + (days) + " " + status.getState() + " " + (date) + " " + (to) + " " + (from) + " " + reason;
		return objformat;
	}
	// Getters
	string getApplicationType() { return leaveType; }
	string getDate() { return date; }
	string getToDate() { return from; }
	int getEmpId() { return (eid); }
	int getDuration() { return stoi(days); }
	void setStatus(string newstate){
		status.setState(newstate, &(*this));
	} 

	string getStatus() {
		return
			status.getState();
	}
	void addRecordInFile() {

		// Prepare the file to append the new record
		string filename = to_string(eid) + "LR.txt"; // File is named after Employee ID
		ofstream file(filename, ios::app); // Open file in append mode

		if (!file.is_open()) {
			cerr << "Error: Unable to open file for writing.\n";
			return;
		}

		// Write the record to the file
		string objFormat = getFileObjectFormat();
		file << objFormat << endl;
		file.close(); // Close the file
		cout << "Leave record added successfully.\n";
	}
};

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
			stringstream ss(line);
			int recordid;
			string status, days, leaveType, reason, date, to, from;
			// Parse the line (assumes format: LeaveType Days)
			ss >> recordid >> leaveType >> days >> status >> date >> to >> from >> reason;
			// Add to records
			records.emplace_back(new LeaveRecord(id, recordid, leaveType, days, date, to, from, reason, status));

		}

		file.close();
	}

	// Display all individual leave records
	void viewRecords() {
		cout << "-- Leave Records for Employee ID: " << id << " --" << endl;
		for (auto& record : records) {
			record->display();
		}
	}
	void generateSummary() {
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


int getYear(string date) {
	// Assuming the date format is "DD-MM-YYYY"
	return stoi(date.substr(date.size() - 4));
}
struct PendingWork{
	int ApproverNumber;
	IApplicationObserver* team;
	IApplication* app;
	PendingWork(int AN, IApplicationObserver* Team, IApplication* appl) {
		ApproverNumber = AN;
		team = Team;
		app = appl;
	}
};
class AdminObserverManager {
	vector<PendingWork> appObservers;
public:
	void AddObserver(IApplication* record, IApplicationObserver* team, int AN) {
		appObservers.push_back(PendingWork(AN, team, record));
	}
	vector<PendingWork> getMyPendingWork() const {
		return appObservers;
	}
};

class AdminNotifier {
public:
	void notifyApplicationChange(vector<PendingWork> appObservers, int index, string status) {
		if (index < 0 || index >= appObservers.size()) return; // Validate index
		appObservers[index].team->ForwardApplicationFurther(
			appObservers[index].app,
			++(appObservers[index].ApproverNumber),
			status
		);
		appObservers.erase(appObservers.begin() + index);
	}
};


class DisplayStrategy {
public:
	bool display(vector<PendingWork> pendingWork) {
		cout << "Pending Work for Admin: " << endl;
		int index = 0;
		for (const auto& pending : pendingWork) {
			cout << index++ << ". ";
			pending.app->display();
		}
		if (index == 0)
			return false;
		return true;
	}
};



class Admin : public IApplicationObservable {
protected:
	AdminObserverManager observerManager;
	AdminNotifier notifier;
	DisplayStrategy displayStrategy;
public:
	void AddObserver(IApplication* record, IApplicationObserver* team, int AN) {
		observerManager.AddObserver(record, team, AN);
	}
	bool displayPendingWork() {
		return displayStrategy.display(observerManager.getMyPendingWork());
	}
	void notifyApplicationChange(int index, string status) {
		notifier.notifyApplicationChange(observerManager.getMyPendingWork(), index, status);
	}
	virtual int getID() = 0;
};

class Director : public Admin {
	int id;
public:

	Director() {}
	Director(int ida) { id = ida; }
	int getID() { return id; }

};
class Supervisor : public Admin {
	int id;
public:
	Supervisor(){}
	Supervisor(int ida) { id = ida; }
	int getID() { return id; }
};
class IPostAction {
public:
	virtual void postaction() = 0;

};

class OfficialLeavePC : public IPostAction {
	Attendence* attenRecord;
	IApplication* latestApp;

public:
	OfficialLeavePC(Record<IAttendenceEntity>* record, IApplication* latest) {
		attenRecord = static_cast<Attendence*>(record);
		latestApp = latest;
	}
	void postaction() {
		attenRecord->addWeekHours(8, getYear(latestApp->getToDate()));	
	}
};

class IValidator {
public:
	virtual bool validate() {
		return true; // default
	};

};

class CasualLeaveValidator : public IValidator {
	vector<IApplication*>* LeavesHistory;
	IApplication* latestApp;
public:
	CasualLeaveValidator(vector<IApplication*>* history, IApplication* latest) {
		LeavesHistory = history;
		latestApp = latest;
	}
	bool validate() override {
		int leavescount = calculateEmpCasualLeavesCount(LeavesHistory);
		if (leavescount < 14 && latestApp->getDuration() <= 4) { // "cannot avail more than 4 leaves at a time"
			return true;
		}
		return false;
	}
	int calculateEmpCasualLeavesCount(vector<IApplication*>* records) {
		// Initialize casualLeavesCount to 0
		int casualLeavesCount = 0;
		int currentYear = MyTime::Year;
		// Traverse through each record (IApplication pointer)
		for (auto& record : *records) {
			// Check if the application type is "Casual" and the year matches the current year
			if (record->getApplicationType() == "Casual" && getYear(record->getDate()) == currentYear) {
				// Add the number of days to casualLeavesCount
				casualLeavesCount += record->getDuration();
			}
		}

		return casualLeavesCount;
	}

};

class EarnedLeaveValidator : public IValidator {
	vector<IApplication*>* LeavesHistory;
	IApplication* latestApp;
public:
	EarnedLeaveValidator(vector<IApplication*>* history) {
		LeavesHistory = history;
	}
	bool validate() override {
		int leavescount = calculateEarnedLeavesCount(LeavesHistory);
		if (leavescount < 14) {
			return true;
		}
		return false;
	}
	int calculateEarnedLeavesCount(vector<IApplication*>* records) {
		// Initialize casualLeavesCount to 0
		int casualLeavesCount = 0;
		int currentYear = MyTime::Year;
		// Traverse through each record (IApplication pointer)
		for (auto& record : *records) {
			// Check if the application type is "Casual" and the year matches the current year
			if (record->getApplicationType() == "Earned" && getYear(record->getDate()) == currentYear) {
				// Add the number of days to casualLeavesCount
				casualLeavesCount += record->getDuration();
			}
		}

		return casualLeavesCount;
	}

	int getYear(string date) const {
		// Assuming the date format is "DD-MM-YYYY"
		return stoi(date.substr(date.size() - 4));
	}
};

class Leaves_Team : public IApplicationObserver {
protected:
	vector<IApplication*> applications;
	vector<IApplicationObservable*> approvers;  // List of approvers for a team
	IValidator* validator;
	IPostAction* action;
public:
	virtual ~Leaves_Team() = default;

	bool apply(IApplication* application) { // template method for applying procedure f0r all teams
		bool check = true;
		if (validator) {
			check = validator->validate();
		}
		if (check) {
			AddInApplicationVector(application); //Command pattern (submitting leave).
			ForwardApplicationFurther(application); // send to first admin in hierachy
			
			return true;
		}
		else {
			cout << "You cannot eligibile to apply for the leave" << endl;
			return false;
		}
	}//updateOnApplicationReceived
	virtual void ForwardApplicationFurther(IApplication* application, int ApproverNumber = 0, string status = "Pending") = 0;
	//virtual void PostActions(Employee) = 0; // post actions like awarding credit hrs, deducting balance etc
	virtual string getType() = 0;
	void AddInApplicationVector(IApplication* application) {
		applications.emplace_back(application);
	}
	//};
	//void NotifyStatusChangeToApplicants(int employeeID, LeaveRecord application) {
	//	//imposter
	//}
	void getApplications() {
		cout << "displaying outstanding leave applications" << endl;
		for (auto& app : applications) {
			app->display();
		}
	}
};


class Casual : public Leaves_Team {
private:
	static Casual* instance;

	Casual(vector<IApplicationObservable*> admin, IValidator* vdtr) {
		approvers = admin;
		validator = vdtr;
		action = nullptr;
	}

public:
	static Casual* getInstance(vector<IApplicationObservable*> admins, IValidator* vdtr) {
		if (!instance) {
			instance = new Casual(admins, vdtr);
		}
		return instance;
	}
	void ForwardApplicationFurther(IApplication* application, int ApproverNumber = 0, string status = "Pending") { // actually registering in observables (approval team)
		if (ApproverNumber >= approvers.size() || status == "Rejected") // // supervisor has denied the application so do not forward to Director
		{// application has gone through all the process now update the status
			application->setStatus(status);
			cout << "applicaion is finally :" << application->getStatus() << endl;
		}
		else {
			cout << "[Casual Team] We have forward application further. total No. Of Approvals Required: " << approvers.size() << endl;
			approvers[ApproverNumber]->AddObserver(application, Casual::getInstance(approvers, validator), ApproverNumber);  // Start the approval process with the first approver

		}
	}
	

	
	string getType() {
		return "Casual";
	}
};
Casual* Casual::instance = nullptr;




class Official : public Leaves_Team {
private:
	static Official* instance;

	Official(vector<IApplicationObservable*> admins, IPostAction* Action) {
		approvers = admins;
		validator = nullptr; //default validator which is always true
		action = Action;
	}

public:
	static Official* getInstance(vector<IApplicationObservable*> admins, IPostAction* Action) {
		if (!instance) {
			instance = new Official(admins, Action);
		}
		return instance;
	}
	void ForwardApplicationFurther(IApplication* application, int ApproverNumber = 0, string status = "Pending") { // actually registering in observables (approval team)
		if (ApproverNumber >= approvers.size() || status == "Denied") // // supervisor has denied the application so do not forward to Director
		{// application has gone through all the process now update the status
			application->setStatus(status);
			cout << "applicaion is finally :" << application->getStatus() << endl;
			if (action) {
				action->postaction();
				cout << "we have awarded 8 credit hours to the applicant" << endl;
			}
		}
		else {
			cout << "[Official Team] We have forward application further. total No. Of Approvals Required: " << approvers.size() << endl;
			approvers[ApproverNumber]->AddObserver(application, Official::getInstance(approvers, action), ApproverNumber);  // Start the approval process with the first approver

		}
	}
	//void PostActions(Employee emp) {
	//	//award credits in attendence
	//}
	string getType() {
		return "Official";
	}
};
Official* Official::instance = nullptr;


class Unpaid : public Leaves_Team {
private:
	static Unpaid* instance;

	Unpaid(vector<IApplicationObservable*> admins, IPostAction* Action) {
		approvers = admins;
		validator = nullptr;// default validator which is always true
		action = Action;
	}

public:
	static Unpaid* getInstance(vector<IApplicationObservable*> admins, IPostAction* Action) {
		if (!instance) {
			instance = new Unpaid(admins, Action);
		}
		return instance;
	}
	void ForwardApplicationFurther(IApplication* application, int ApproverNumber = 0, string status = "Pending") { // actually registering in observables (approval team)
		if (ApproverNumber >= approvers.size() || status == "Denied") // // supervisor has denied the application so do not forward to Director
		{// application has gone through all the process now update the status
			application->setStatus(status);
			cout << "applicaion is finally :" << application->getStatus() << endl;
			if (action) {
				action->postaction();
			}
		}
		else {
			cout << "[Unpaid Team] We have forward application further. total No. Of Approvals Required: " << approvers.size() << endl;
			approvers[ApproverNumber]->AddObserver(application, Unpaid::getInstance(approvers, action), ApproverNumber);  // Start the approval process with the first approver

		}
	}
	//void PostActions(Employee emp) {
	//	//no salary
	//}
	
	string getType() {
		return "Unpaid";
	}
};
Unpaid* Unpaid::instance = nullptr;

class Earned : public Leaves_Team {
private:
	static Earned* instance;

	Earned(vector<IApplicationObservable*> admins, IValidator* vdtr) {
		approvers = admins;
		validator = vdtr;
		action = nullptr;
	}

public:
	static Earned* getInstance(vector<IApplicationObservable*> admins, IValidator* vdtr) {
		if (!instance) {
			instance = new Earned(admins, vdtr);
		}
		return instance;
	}
	//void PostActions(Employee emp) {
	//	//subtract 1 from leave balance
	//}
	
	void ForwardApplicationFurther(IApplication* application, int ApproverNumber = 0, string status = "Pending") { // actually registering in observables (approval team)
		if (ApproverNumber >= approvers.size() || status == "Denied") // // supervisor has denied the application so do not forward to Director
		{// application has gone through all the process now update the status
			application->setStatus(status);
			cout << "applicaion is finally :" << application->getStatus() << endl;
		}
		else {
			cout << "[Earned Team] We have forward application further. total No. Of Approvals Required: " << approvers.size() << endl;
			approvers[ApproverNumber]->AddObserver(application, Earned::getInstance(approvers,validator), ApproverNumber);  // Start the approval process with the first approver

		}
	}
	string getType() {
		return "Earned";
	}
};
Earned* Earned::instance = nullptr;





class Employee {
private:
	int id;
public:
	Record<IAttendenceEntity>* myAttendenceRecord;
	Record<IApplication>* myLeaveRecord;

	Employee() {}
	Employee(int id, Record<IAttendenceEntity>* NewRecord, Record<IApplication>* NewLeaveRecord) : id(id) {
		myAttendenceRecord = NewRecord;
		myLeaveRecord = NewLeaveRecord;
		myAttendenceRecord->refresh();
		myLeaveRecord->refresh();
	}

	void generateAttendenceReport() {
		myAttendenceRecord->viewRecords();
	}
	void generateLeaveReport() {
		myLeaveRecord->viewRecords();

	}
	void generateLeaveSummary() {
		myLeaveRecord->generateSummary();
	}

	void applyLeave(IApplication* app, Leaves_Team* team) {
		// file the leave
		bool success = team->apply(app);
		if (success) {
			myLeaveRecord->addRecord(app);
		}

	}
	int getID() {
		return id;
	}
	bool operator==(const Employee& other) const {
		return this->id == other.id;
	}
	~Employee() {

	}
};
struct inactive {
	int id;
	int days;
};
//generate monthly reports
class IMonthlyReports : public ITimeObserver {
	vector<Record<IAttendenceEntity>*>* empAttendRegisters;
	vector<inactive>* inactiveIDs;
public:
	IMonthlyReports(vector<Record<IAttendenceEntity>*>* reg, vector<inactive>* outreg) {
		empAttendRegisters = reg;
		inactiveIDs = outreg;
	}
	void onWeekUpdate() {};
	void onMonthUpdate() {
		GenerateMonthlyReports();
		GenerateSalaries();
	};
	void onYearUpdate() {};
	void GenerateMonthlyReports() {
		for (auto& rgister : *empAttendRegisters) {
			rgister->generateSummary();
		}
	}
	void GenerateSalaries() {
		cout << "[SYSTEM] Salary" << endl;
		for (int i = 0; i < empAttendRegisters->size(); i++) {
			if (isEmpInactive(i + 1)) {
				cout << "- Emp # " << i + 1 << " is gone to some visit so no salary" << endl;
			}
			else {
				cout << "- Awarded salary to Emp # " << i + 1 << "!" << endl;
			}
		} // impostor give only salaries to Emp having > 80 attendence
	}
	bool isEmpInactive(int id) {
		for (int i = 0; i < inactiveIDs->size(); i++) {
			if (id == (*inactiveIDs)[i].id) {//employee is inactive
				return true;
			}
		}
		return false;
	}
};
class InactivityManager : public ITimeObserver {
	vector<inactive>* inactiveIDs;
public:
	InactivityManager(vector<inactive>* reg) {
		inactiveIDs = reg;
	}
	void onWeekUpdate() {
		reduceInactiveDays(7);
	};
	void onMonthUpdate() {
		reduceInactiveDays(30);
	};
	void onYearUpdate() {
		reduceInactiveDays(365);
	};
	void reduceInactiveDays(int days) {
		for (int i = 0; i < inactiveIDs->size(); i++) {
			(*inactiveIDs)[i].days -= days;
			if ((*inactiveIDs)[i].days <= 0) {
				// imposter remove from inactive
			}
		}
	}
};
class Main {
private:
	static Main* main;
	vector<Record<IAttendenceEntity>*> empAttendRegisters;
	vector<Record<IApplication>*> empLeaveRecords;
	vector<inactive> inactiveIDs;
	Director director;
	Supervisor supervisor;
	Guard guard;
	ITimeObserver* ReportsGenerator;
	ITimeObserver* inactivityManager;
	Main(){
		ReportsGenerator = new IMonthlyReports(&empAttendRegisters, &inactiveIDs);
		inactivityManager = new InactivityManager(&inactiveIDs);
		MyTime* timeobj = MyTime::getInstance();
		timeobj->addObserver(ReportsGenerator);
		timeobj->addObserver(inactivityManager);
	}
public:
	static Main* getMain() {
		if (main == nullptr) {
			main = new Main();

		}
		return main;
	}
	void markattendenceportal() {
		int eid;
		cout << "whose attendence you want to mark? " << endl;
		cin >> eid;

		guard.markAttendance(empAttendRegisters, eid);
	}
	Record<IAttendenceEntity>* generateEmployeeRecord(int id) { // Factory Method called when creating employees agr employe null krta ha register ko istrah to record ka track to gya, agr to record getter sy arha to koi b edit kr skta 
		string filename = to_string(id) + ".txt";
		ifstream file(filename);
		MyTime* timeobj = MyTime::getInstance();
		if (!file.is_open()) {
			file.close();
			//register new record to system
			ofstream fw(filename);
			fw.close();
		}
		Record<IAttendenceEntity>* newentity = new Attendence(id);
		empAttendRegisters.push_back(newentity);
		return empAttendRegisters.back();

	}
	void addEmpInactive(int id, int days) {
		inactive newentity;
		newentity.id = id; newentity.days = days;
		inactiveIDs.push_back(newentity);
	}
	Record<IApplication>* createLeaveRecord(int id) { // Factory Method called when creating employees agr employe null krta ha register ko istrah to record ka track to gya, agr to record getter sy arha to koi b edit kr skta 
		string filename = to_string(id) + "LR.txt";
		ifstream file(filename);

		if (!file.is_open()) {
			file.close();
			//register new record to system
			ofstream fw(filename);
			fw.close();
		}
		Record<IApplication>* newentity = new Leave(id);
		empLeaveRecords.push_back(newentity);
		return empLeaveRecords.back();

	}

	void getCollectiveReportConsole() {
		for (auto& rgister : empAttendRegisters) {
			rgister->generateSummary();
		}
	}
};



Main* Main::main = nullptr;


class UnpaidLeavePC : public IPostAction {
	IApplication* latestApp;

public:
	UnpaidLeavePC(IApplication* latest) {
		latestApp = latest;
	}
	void postaction() {
		Main* system = Main::getMain();
		system->addEmpInactive(latestApp->getEmpId(), latestApp->getDuration());
	}
};
