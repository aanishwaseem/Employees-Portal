#pragma once
#include "IApplication.h"
class IPostAction {
protected:
	IApplication* latestApp;
public:
	IPostAction(IApplication* latest) {
		latestApp = latest;
	}
	virtual void postaction() = 0;
};
