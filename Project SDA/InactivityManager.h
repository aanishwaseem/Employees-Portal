#pragma once
#include "Time.h"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;
class inActivityHandler {
public:
	static void populateData(std::vector<inactive>& inactiveIDs) {
		std::ifstream file("employees.txt");
		if (!file) {
			std::cerr << "Error: Unable to open the file employees.txt for reading.\n";
			return;
		}

		std::string line;

		// Read each line in the file
		while (std::getline(file, line)) {
			std::stringstream ss(line);
			int recordID;
			std::string password;
			int inactivityDays;

			// Parse the line
			ss >> recordID >> password >> inactivityDays;
			if (inactivityDays > 0) {
				inactive entity;
				entity.id = recordID; entity.days = inactivityDays;

				// Populate the inactive vector
				inactiveIDs.emplace_back(entity);
			}
		}

		file.close();
	}
	static void updateFile(int id, int newDays) {
		std::ifstream file("employees.txt");

		std::ostringstream buffer;
		std::string line;
		bool found = false;

		// Read each line and check for the employee ID
		while (std::getline(file, line)) {
			std::stringstream ss(line);
			int recordID;
			std::string password;
			int inactivityDays;

			// Parse the line
			ss >> recordID >> password >> inactivityDays;

			// Check if this is the record to update
			if (recordID == id) {
				found = true;
				buffer << recordID << " " << password << " " << newDays << "\n";
			}
			else {
				// Write the unchanged record back to the buffer
				buffer << line << "\n";
			}
		}

		file.close();
		ofstream outFile("employees.txt", ios::trunc);
		if (!outFile.is_open()) {
			return;
		}

		outFile << buffer.str();
		outFile.close();
	}
};
class InactivityManager : public ITimeObserver {
	vector<inactive> inactiveIDs;
	static InactivityManager* main;
	InactivityManager() {
		inActivityHandler::populateData(inactiveIDs);
	}
public:

	
	static InactivityManager* getInstance() {
		if (main == nullptr) {
			main = new InactivityManager();

		}
		return main;
	}
	void addInactive(int id, int days) {
		inactive entity;
		entity.id = id; entity.days = days;
		inactiveIDs.push_back(entity);
		inActivityHandler::updateFile(entity.id, entity.days);
	}
	//void addInactive(int id, int days) {
	//	// Search for the entity with the given id in the vector
	//	for (auto& entity : inactiveIDs) {
	//		if (entity.id == id) {
	//			// Update the days if the id already exists
	//			entity.days += days;
	//			inActivityHandler::updateFile(entity.id, entity.days);
	//			return;
	//		}
	//	}

	//	// If the id does not exist, add a new entity
	//	inactive entity;
	//	entity.id = id;
	//	entity.days = days;
	//	inactiveIDs.push_back(entity);

	//	// Update the file for the new entity
	//	inActivityHandler::updateFile(entity.id, entity.days);
	//}

	vector<inactive>* getIDs() {
		return &inactiveIDs;
	}
	int isInactive(int id) {
		for (const auto& entity : inactiveIDs) { // Iterate through inactiveIDs
			if (entity.id == id && entity.days > 0) {
				return entity.days; // Found the ID with remaining inactive days
			}
		}
		return 0; // ID not found or no remaining inactive days
	}
	void displayInactives() {
		if (inactiveIDs.empty()) {
			cout << "No inactive employees at the moment.\n";
			return;
		}

		cout << "Inactive Employees:\n";
		cout << "--------------------------------------------\n";
		cout << "ID\t\tDays of Inactivity\n";
		cout << "--------------------------------------------\n";
		for (const auto& entity : inactiveIDs) {
			cout << entity.id << "\t\t" << entity.days << "\n";
		}
		cout << "--------------------------------------------\n";
	}

	void onWeekUpdate() {
		reduceInactiveDays(7);
	};
	void onMonthUpdate() {
		reduceInactiveDays(30);
	};
	void onYearUpdate() {
		reduceInactiveDays(365);
	};
	void onDayUpdate() {
		reduceInactiveDays(1);
	}
	void reduceInactiveDays(int days) {
		for (int i = 0; i < inactiveIDs.size(); ) { // Use a while loop to handle dynamic resizing
			(inactiveIDs)[i].days -= days; // Reduce days
			inActivityHandler::updateFile(inactiveIDs[i].id, inactiveIDs[i].days);
			if ((inactiveIDs)[i].days <= 0) {
				// Remove the inactive element if days <= 0
				inactiveIDs.erase(inactiveIDs.begin() + i);

				// Do not increment i, as the next element shifts into the current index
			}
			else {
				++i; // Only increment if no element was erased
			}
		}
	}
};

