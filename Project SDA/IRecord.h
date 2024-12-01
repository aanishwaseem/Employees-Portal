#pragma once
#include <vector>
using namespace std;
template <typename T>
class Record {
public:
	virtual void addRecord(T*) = 0;
	virtual void viewRecords() = 0;
	virtual void generateSummary(int = 0) = 0;
	virtual void refresh() = 0;
	virtual vector<T*>* getAllRecords() = 0;
};
