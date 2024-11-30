#include "Report.h"
#include "DirectorPortal.h"
#include <iostream>
using namespace std;

class SupervisorPortal {
private:
	Main* mainInstance; // Reference to Main instance
	Admin* supervisor;  // Reference to Director class or an instance to handle approval logic

public:
	SupervisorPortal(Admin* supervisord) {
		supervisor = supervisord;
	}

	// Method to display and select applications for status change
	void SupervisorPortalDisplay() {
		Main* main = Main::getMain();  // Get the main instance
		int count = supervisor->displayPendingWork();
		
		if (count) {
			// Step 4: Supervisor selects an application
			int appChoice;
			cout << "Select an application to approve/reject (enter number): ";
			cin >> appChoice;

			if (appChoice < 0) {
				cout << "Invalid choice.\n";
				return;
			}

			// Step 5: Update application status
			cout << "Enter decision (1: Approved, 2: Rejected): ";
			int statusChoice;
			cin >> statusChoice;
			string status;
			if (statusChoice == 1) {
				status = "Approved";
			}
			else if (statusChoice == 2) {
				status = "Rejected";
			}
			else {
				cout << "Invalid status choice.\n";
				return;
			}
			supervisor->notifyApplicationChange(appChoice, status);
		}
		else {
			cout << "no pending works , enjoy!" << endl;
		}
		// Step 6: Notify the corresponding employee
	}


};

int main() {
	Supervisor* super = new Supervisor(50);
	Director* director = new Director(69);
	vector<IApplicationObservable*> ShortLeaveApprovers;
	vector<IApplicationObservable*> LongLeaveApprovers;
	ShortLeaveApprovers.push_back(super);
	//director is also an admin in case of long leaves
	LongLeaveApprovers.push_back(super);
	LongLeaveApprovers.push_back(director);
	
	//MAKE THE EMPLOYEES ID WITH SEQUENCE ONLY
    Main* system = Main::getMain();
	cout << "Creating 2 employees" << endl << endl;
    Employee emp(1, system->generateEmployeeRecord(1), system->createLeaveRecord(1)); //employee id starts with 1
    Employee emp2(2, system->generateEmployeeRecord(2), system->createLeaveRecord(2));

	cout << "[Guard Portal]" << endl;
	//system->markattendenceportal();

	cout << "[INDIVIDUAL REPORT] Generating Employee 2's Attendence Details" << endl;
    emp2.generateAttendenceReport();
	cout << endl << endl;
	
	cout << "[Employee 1] Going to apply Casual Leave: " << endl;
	IApplication* newapp = new LeaveRecord(emp.getID(), 9, "Casual", "2", "2-2-2024", "2-2-2024", "2-2-2024", "noreason");
	IValidator* validator = new CasualLeaveValidator(emp.myLeaveRecord->getAllRecords(),newapp);
    emp.applyLeave(newapp, Casual::getInstance(ShortLeaveApprovers, validator));
	cout << endl;
	cout << "[Employee 2] Going to apply Official Leave: " << endl;
	IApplication* newapp2 = new LeaveRecord(emp2.getID(), 1, "Official", "2", "2-2-2024", "2-2-2024", "2-2-2024", "noreason");
	
	IPostAction* action = new OfficialLeavePC(emp2.myAttendenceRecord, newapp2);
	emp2.applyLeave(newapp2, Official::getInstance(ShortLeaveApprovers, action));
	cout << endl << endl;

    cout << "[INDIVIDUAL REPORT] Gnerating Employee 1's Leave reports" << endl;
    emp.generateLeaveReport();
	cout << endl << endl;

	cout << "[LEAVE BALANCES] Gnerating Employee 1's Leave balances" << endl;
	emp.generateLeaveSummary();
	cout << endl << endl;


	SupervisorPortal admin(super);
	cout << "[Supervisor Portal]" << endl;
	admin.SupervisorPortalDisplay();
	cout << endl << endl;
	DirectorPortal admin2(director);
	cout << "[Director Portal]" << endl;
	admin2.DirectorPortalDisplay();

	
	cout << endl;
	cout << "--------------------------------------------" << endl;
	cout << "--------------------------------------------" << endl;
	cout << "--------------------------------------------" << endl;
	cout << "-- we have generated a file of monthly attendence report:  " << endl;
	MyTime* time = MyTime::getInstance();
	time->notifyMonthUpdate();
	cout << "PS: Since new month has started, we are clearing all employees monthly hours and percentages " << endl;
	cout << "--------------------------------------------" << endl;

    return 0;
}


