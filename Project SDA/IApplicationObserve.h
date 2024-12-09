#pragma once
#include "IRecord.h"
#include "IAttendenceEntity.h"
#include "IApplication.h"
class IApplicationObserver {
public:
	virtual void ForwardApplicationFurther(IApplication* application, int ApproverNumber = 0, string = "Pending") = 0;
};
class IApplicationObservable {
public:
	virtual void AddObserver(IApplication* application, IApplicationObserver*, int) = 0;
	virtual void notifyApplicationChange(int, string) = 0;
};