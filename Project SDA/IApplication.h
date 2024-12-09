#pragma once
#include <string>
using namespace std;
class IApplication {
public:
	virtual void display() = 0;
	virtual int getEmpId() = 0;
	virtual void setStatus(string) = 0;
	virtual string getApplicationType() = 0;
	virtual string getToDate() = 0;
	virtual void processPendingApplication() = 0;
	virtual int getDuration() = 0;
	virtual string getStatus() = 0;
	virtual void addRecordInFile() = 0;
};