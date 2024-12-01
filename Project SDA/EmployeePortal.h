#pragma once
#include "Employee.h"
#include "IApplyLeave.h"
#include "Main.h"
class EmployeePortal {
	Employee* emp;
private:
public:
	EmployeePortal(Employee* employee) {
		Main* system = Main::getMain();
		employee->initializeEmployee(system->generateEmployeeRecord(employee->getID()), system->createLeaveRecord(employee->getID()));
		emp = employee;
	}
	void EmployeeMenu()
	{
		//EmployeeMenu
		bool open = true;
		while (open) {
			system("cls");
			cout << "Employee Menu:\n"
				<< "1.Check Attendance Details\n"
				<< "2.Check Leave Details\n"
				<< "3.Apply Leave\n"
				<< "4.Check Notifications\n"
				<< "0.Go Back\n"
				<< endl;
			int type = -1;
			getType(type);
			if (type == 0)
			{
				return;
			}
			if (type == 1)
			{
				emp->generateAttendenceReport();
				//call Attendance Details Function by emp id
			}
			else if (type == 2)
			{
				emp->generateLeaveSummary();
				//call Leave Details Function by emp id

			}
			else if (type == 3)
			{
				//call Apply Leave Function by emp id
			}
			else if (type == 4)
			{
				NotificationManager* notes = NotificationManager::getInstance();
				notes->getNotificationsForEmployee(emp->getID());
				//call Notifications Function by emp 
			}
			system("pause");
		}
	}
	void getType(int& type)
	{
		while (type < 0 || type > 4) {

			cin >> type;
			if (type < 0 || type > 4)
				cout << "Enter valid input\n";
		}
	}
};