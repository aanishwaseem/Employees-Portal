#pragma once
#include "IAttendenceEntity.h"
#include <iostream>
class AttendenceEntity : public IAttendenceEntity {
private:
	int id;
	int week;
	int day;
	int hours;
	int year;
public:
	// Constructor
	AttendenceEntity(int id, int w, int d, int h)
		: id(id), week(w), day(d), hours(h) {}


	// Display the record
	void display() {
		cout << "ID: " << id << ", week: " << week << ", day: " << day << " hours: " << hours << endl;
	}
	string getFileObjectFormat() {
		string format = to_string(id) + " " + to_string(week) + " " + to_string(day) + " " + to_string(hours);
		return format;
	}


	// Getters (if needed)
	int getEmpId() { return id; }
	int getWeek() { return week; }
	int getHours() { return hours; }
};
