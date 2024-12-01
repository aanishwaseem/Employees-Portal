#pragma once
#ifndef NOTIFICATIONMANAGER_H
#define NOTIFICATIONMANAGER_H

#include "INotificationManager.h"
#include "NotificationEntity.h"
#include <fstream>
#include <memory>

class NotificationManager : public INotificationManager {
public:
    // Delete copy constructor and assignment operator to prevent copying
    NotificationManager(const NotificationManager&) = delete;
    NotificationManager& operator=(const NotificationManager&) = delete;

    // Static method to get the instance of NotificationManager
    static NotificationManager* getInstance() {
        if (!instance) {
            instance = new NotificationManager;
        }
        return instance;
    }

    // Add notification to both memory and file
    void addNotification(int employeeID, const std::string& message) override {
        auto newNotification = std::make_shared<NotificationEntity>(employeeID, message);
        objects.push_back(newNotification);

        // Save the notification to file
        newNotification->addToFile();
    }

    // Get notifications for a specific employee
    void getNotificationsForEmployee(int employeeID) override {
        for (const auto& notification : objects) {
            if (notification->getEmployeeID() == employeeID) {
                notification->display();
            }
        }
    }

    // Load notifications from a file into memory
    void loadNotificationsFromFile(const std::string& filename = "notifications.txt") override {
        std::ifstream file(filename);
        std::string line;

        if (!file.is_open()) {
            return;
        }
        // Read the notifications from the file
        while (std::getline(file, line)) {
            if (!line.empty()) {
                // Create a NotificationEntity from the file string and add to memory
                auto notification = std::make_shared<NotificationEntity>(NotificationEntity::fromFileString(line));
                objects.push_back(notification);
            }
        }

        file.close();
    }

private:
    static NotificationManager* instance;  // Guaranteed to be destroyed, instantiated on first use

    // Private constructor to prevent instantiation
    NotificationManager() {}

    // List of notifications in memory
    std::vector<std::shared_ptr<INotificationEntity>> objects;
};

#endif // NOTIFICATIONMANAGER_H
