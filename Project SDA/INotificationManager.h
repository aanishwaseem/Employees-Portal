#pragma once
#ifndef INOTIFICATIONMANAGER_H
#define INOTIFICATIONMANAGER_H

#include "INotificationEntity.h"
#include <vector>
#include <memory>

class INotificationManager {
public:
    virtual ~INotificationManager() = default;

    virtual void addNotification(int employeeID, const std::string& message) = 0;
    virtual void getNotificationsForEmployee(int employeeID) = 0;
    virtual void loadNotificationsFromFile(const std::string& filename) = 0;
};

#endif // INOTIFICATIONMANAGER_H
