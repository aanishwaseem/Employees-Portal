#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <string>
#include <sstream>
using namespace std;
// Interface for Observers (Attendance)

struct inactive {
    int id;
    int days;
};
class ITimeObserver {
public:
    virtual void onWeekUpdate() = 0;
    virtual void onMonthUpdate() = 0;
    virtual void onYearUpdate() = 0;
    virtual ~ITimeObserver() = default;
};

class ITimeObservable {
public:
    virtual void addObserver(ITimeObserver* observer) = 0;
    virtual void removeObserver(ITimeObserver* observer) = 0;
    virtual void notifyWeekUpdate() = 0;
    virtual void notifyMonthUpdate() = 0;
    virtual void notifyYearUpdate() = 0;
    virtual ~ITimeObservable() = default;
};


// Time class with static members
class MyTime : public ITimeObservable {
private:
    std::vector<ITimeObserver*> observers;
    static MyTime* instance;
    MyTime() {}
public:
    static int Week;   // Current week
    static int Month;  // Current month
    static int Day;    // Current day
    static int Year;    // Current day
    static MyTime* getInstance() {
        if (!instance) {
            instance = new MyTime();
        }
        return instance;
    }

    void addObserver(ITimeObserver* observer) override {
        observers.push_back(observer);
    }

    void removeObserver(ITimeObserver* observer) override {
        observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
    }

    void notifyWeekUpdate() override {
        for (auto* observer : observers) {
            observer->onWeekUpdate();
        }
    }

    void notifyMonthUpdate() override {
        for (auto* observer : observers) {
            observer->onMonthUpdate();
        }
    }
    void notifyYearUpdate() override {
        for (auto* observer : observers) {
            observer->onWeekUpdate();
        }
    }
    void updateYear() {
        Year++;
        Month = 0;  // Reset months at the start of a new year
        std::cout << "Year updated to " << Year << ".\n";
        notifyYearUpdate();
    }
    void updateWeek() {
        Week++;
        std::cout << "Week updated to " << Week << ".\n";
        notifyWeekUpdate();
    }

    void updateMonth() {
        Month++;
        Week = 0;  // Reset weeks at the start of a new month
        std::cout << "Month updated to " << Month << ".\n";
        notifyMonthUpdate();
    }

    void incrementDay() {
        Day++;
        std::cout << "Day updated to " << Day << ".\n";
        if (Day % 7 == 0) {
            updateWeek();
        }
        if (Day % 30 == 0) { // Simulating a 30-day month
            updateMonth();
        }
    }
};

// we have leave records in file , what to do with those? do in check in we have to do something? on checkin we add inactives
// in earned leave we also have to deduct timer in file (additional)
//MENU:
// manager approvers
// add a new supervisor / director
//at the start of program , give hours  from apps
// Inactive at the employees.txt file
//WE NEED TO TEST ALL GIVEN FUNCTIONALITIES in instructions:
// system whether generates the 80% attendence or not
int getYear(string date);
int getWeekNumberInMonth(const std::string& dateStr);
int getDayGivenMonth(const std::string& dateStr);
int getDayInWeek(int dayOfYear);
