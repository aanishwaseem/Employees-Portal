#pragma once
#include "IPostAction.h"
#include "InactivityManager.h"
class UnpaidLeavePA : public IPostAction {
public:
	UnpaidLeavePA(IApplication* latest) : IPostAction(latest) {
		latestApp = latest;
	}
	void postaction() {
		InactivityManager* inactivityManager = InactivityManager::getInstance();
		inactivityManager->addInactive(latestApp->getEmpId(), latestApp->getDuration());
	}
};
