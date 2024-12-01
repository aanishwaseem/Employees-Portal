#pragma once

#include "MainPortal.h"
#include <iostream>
using namespace std;
int main() {
	Main* system = Main::getMain();
	MainPortal* main = MainPortal::getMain();
	main->start();
	return 0;
}

//int main() {
//	//registering teams
//
//	NotificationManager* notes = NotificationManager::getInstance();
//	notes->loadNotificationsFromFile();
//	Supervisor* super = new Supervisor(50);
//	Director* director = new Director(69);
//	vector<IApplicationObservable*> ShortLeaveApprovers;
//	vector<IApplicationObservable*> LongLeaveApprovers;
//	ShortLeaveApprovers.push_back(super);
//	//director is also an admin in case of long leaves
//	LongLeaveApprovers.push_back(super);
//	LongLeaveApprovers.push_back(director);
//	Main* system = Main::getMain();
//	Casual::getInstance(ShortLeaveApprovers);
//	Official::getInstance(ShortLeaveApprovers);
//	Earned::getInstance(LongLeaveApprovers);
//	Unpaid::getInstance(LongLeaveApprovers);
//	//MAKE THE EMPLOYEES ID WITH SEQUENCE ONLY
//	cout << "Creating 2 employees" << endl << endl;
//    Employee emp(1, system->generateEmployeeRecord(1), system->createLeaveRecord(1)); //employee id starts with 1
//    Employee emp2(2, system->generateEmployeeRecord(2), system->createLeaveRecord(2));
//
//	cout << "[Guard Portal]" << endl;
//	//system->markattendenceportal();
//
//	cout << "[INDIVIDUAL REPORT] Generating Employee 2's Attendence Details" << endl;
//    emp2.generateAttendenceReport();
//	cout << endl << endl;
//	
//	IApplyLeave* ApplyManager = IApplyLeave::getInstance();
//	cout << "[Employee 2] Going to apply unpaid Leave: " << endl;
//	IApplication* newapp2 = new UnpaidLeaveApp(emp2.getID(), emp2.getNextLeaveID(), "Unpaid", "2", "2-2-2024", "2-2-2024", "2-2-2024", "noreason", "New-zealand");
//	ApplyManager->applyLeave(emp2.getID(), newapp2, Unpaid::getInstance());
//	cout << endl << endl;
//
//	cout << "[Employee 1] Going to apply Casual Leave: " << endl;
//	IApplication* newapp = new DefaultLeaveApplication(emp.getID(), emp.getNextLeaveID(), "Casual", "2", "2-2-2024", "2-2-2024", "2-2-2024", "noreason");
//	ApplyManager->applyLeave(emp.getID(), newapp, Casual::getInstance());
//	cout << endl;
//
//
//    cout << "[INDIVIDUAL REPORT] Gnerating Employee 1's Leave reports" << endl;
//    emp.generateLeaveReport();
//	cout << endl << endl;
//
//	cout << "[LEAVE BALANCES] Gnerating Employee 1's Leave balances" << endl;
//	emp.generateLeaveSummary();
//	cout << endl << endl;
//
//
//	SupervisorPortal admin(super);
//	cout << "[Supervisor Portal]" << endl;
//	admin.SupervisorPortalDisplay();
//	cout << endl << endl;
//	DirectorPortal admin2(director);
//	cout << "[Director Portal]" << endl;
//	admin2.DirectorPortalDisplay();
//
//	
//	cout << endl;
//	cout << "--------------------------------------------" << endl;
//	cout << "--------------------------------------------" << endl;
//	cout << "--------------------------------------------" << endl;
//	cout << "-- we have generated a file of monthly attendence report:  " << endl;
//	MyTime* time = MyTime::getInstance();
//	time->notifyMonthUpdate();
//	cout << "PS: Since new month has started, we are clearing all employees monthly hours and percentages " << endl;
//	cout << "--------------------------------------------" << endl << endl;
//
//
//	cout << "[Employee 2] Notifications" << endl;
//	notes->getNotificationsForEmployee(emp2.getID());
//	cout << endl;
//
//    return 0;
//}


