#pragma once
#ifndef NOTIFICATIONENTITY_H
#define NOTIFICATIONENTITY_H

#include "INotificationEntity.h"
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
using namespace std;
class NotificationEntity : public INotificationEntity {
public:
    NotificationEntity(int employeeID, const std::string& message)
        : employeeID(employeeID), message(message), timestamp(std::time(nullptr)) {}

    int getEmployeeID() const override { return employeeID; }
    std::string getMessage() const override { return message; }
    std::time_t getTimestamp() const override { return timestamp; }

    // Serialize to string for saving to file
    std::string toFileString() const {
        return std::to_string(employeeID) + ";" + message + ";" + std::to_string(timestamp) + "\n";
    }
    void display() {
        cout << message << endl;
    }

    // Static function to create a NotificationEntity from a string
    static NotificationEntity fromFileString(const std::string& line) {
        size_t pos1 = line.find(';');
        size_t pos2 = line.find(';', pos1 + 1);

        int empID = std::stoi(line.substr(0, pos1));
        std::string msg = line.substr(pos1 + 1, pos2 - pos1 - 1);
        std::time_t time = std::stoi(line.substr(pos2 + 1));

        return NotificationEntity(empID, msg);
    }

    // Save the notification to a file
    void addToFile() const {
        std::ofstream file("notifications.txt", std::ios::app);  // Open file in append mode
        if (file.is_open()) {
            file << toFileString();  // Write notification to file
        }
        else {
            std::cerr << "Error opening file to save notification!" << std::endl;
        }
        file.close();
    }

private:
    int employeeID;
    std::string message;
    std::time_t timestamp;
};

#endif // NOTIFICATIONENTITY_H
