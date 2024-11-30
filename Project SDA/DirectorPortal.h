#pragma once
#include "Report.h"


class DirectorPortal {
private:
	Main* mainInstance; // Reference to Main instance
	Admin* director;  // Reference to Director class or an instance to handle approval logic

public:
	DirectorPortal(Admin* supervisord) {
		director = supervisord;
	}

	// Method to display and select applications for status change
	void DirectorPortalDisplay() {
		Main* main = Main::getMain();  // Get the main instance
		int count = director->displayPendingWork();

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
			cout << "Enter decision (1: Approved, 2: Rejected, 3: pending): ";
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
			director->notifyApplicationChange(appChoice, status);
		}
		else
		{
			cout << "no pending works for you @director, enjoy" << endl;
		}
		// Step 6: Notify the corresponding employee
	}


};