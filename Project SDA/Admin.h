#pragma once
#include "IApplicationObserve.h"
#include <vector>
#include <iostream>
struct PendingWork {
	int ApproverNumber;
	IApplicationObserver* team;
	IApplication* app;
	Record<IAttendenceEntity>* attenRecords;
	PendingWork(int AN, IApplicationObserver* Team, IApplication* appl, Record<IAttendenceEntity>* AttenRecords) {
		ApproverNumber = AN;
		team = Team;
		app = appl;
		attenRecords = AttenRecords;
	}
};
class AdminObserverManager {
	vector<PendingWork> appObservers;
public:
	void AddObserver(Record<IAttendenceEntity>* attenRecords, IApplication* record, IApplicationObserver* team, int AN) {
		appObservers.push_back(PendingWork(AN, team, record, attenRecords));
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
			appObservers[index].attenRecords,
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
	void AddObserver(Record<IAttendenceEntity>* attenRecords, IApplication* record, IApplicationObserver* team, int AN) {
		observerManager.AddObserver(attenRecords, record, team, AN);
	}
	bool displayPendingWork() {
		return displayStrategy.display(observerManager.getMyPendingWork());
	}
	void notifyApplicationChange(int index, string status) {
		notifier.notifyApplicationChange(observerManager.getMyPendingWork(), index, status);
	}
	virtual int getID() = 0;
};