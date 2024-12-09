#pragma once
#include "IApplicationObserve.h"
#include <vector>
#include <iostream>
struct PendingWork {
	int ApproverNumber;
	IApplicationObserver* team;
	IApplication* app;
	PendingWork(int AN, IApplicationObserver* Team, IApplication* appl) {
		ApproverNumber = AN;
		team = Team;
		app = appl;
	}
};
class AdminObserverManager {
	vector<PendingWork> appObservers;
public:
	void AddObserver(IApplication* record, IApplicationObserver* team, int AN) {
		appObservers.push_back(PendingWork(AN, team, record));
	}
	vector<PendingWork> getMyPendingWork() const {
		return appObservers;
	}
};


class Admin : public IApplicationObservable {
protected:
	AdminObserverManager* observerManager = new AdminObserverManager;
public:
	void AddObserver(IApplication* record, IApplicationObserver* team, int AN) {
		observerManager->AddObserver(record, team, AN);
	}
	bool displayPendingWork() {
		return display(observerManager->getMyPendingWork());

	}
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
	void notifyApplicationChange(int index, string status) {
		notifyApplicationChange(observerManager->getMyPendingWork(), index, status);
	}
	void notifyApplicationChange(vector<PendingWork> appObservers, int index, string status) {
		if (index < 0 || index >= appObservers.size()) return; // Validate index
		appObservers[index].team->ForwardApplicationFurther(
			appObservers[index].app,
			++(appObservers[index].ApproverNumber),
			status
		);
		appObservers.erase(appObservers.begin() + index);
	}
};