#pragma once

#ifndef INOTIFICATIONENTITY_H
#define INOTIFICATIONENTITY_H

#include <string>
#include <ctime>

class INotificationEntity {
public:
    virtual ~INotificationEntity() = default;

    virtual int getEmployeeID() const = 0;
    virtual std::string getMessage() const = 0;
    virtual void display() = 0;
    virtual std::time_t getTimestamp() const = 0;
};

#endif // INOTIFICATIONENTITY_H
