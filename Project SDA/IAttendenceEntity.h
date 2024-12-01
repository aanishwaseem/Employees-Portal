#pragma once
#include <string>
using namespace std;
class IAttendenceEntity {
public:
	virtual void display() = 0;
	virtual int getEmpId() = 0;
	virtual int getWeek() = 0;
	virtual int getHours() = 0;
	virtual string getFileObjectFormat() = 0;
};