# Employees Portal - Attendance and Leave Management System

## Overview
The **Employees Portal** is a comprehensive application designed to manage attendance and leave records for employees in a commercial organization. Built using **SOLID principles** and leveraging design patterns like the **Observer Pattern**, this system ensures maintainability, scalability, and reliability. It is a C++ Console Application which we are transforming to a desktop Portal.

## Features

### Attendance Management
- Records daily attendance marked by on-duty guards at the gates.
- Provides detailed attendance reports for employees.
- Flags employees with attendance below a given threshold (e.g., 80%).

### Leave Management
- Supports multiple leave types:
  - **Casual Leave:** 15 days/year, max 4 days at a time, no prior approval required.
  - **Earned Leave:** 21 days/year, requires prior approval.
  - **Official Leave:** Used for official visits, full hours are credited.
  - **Unpaid Leave:** Extended leave (e.g., study leave), prior approval required, unpaid.
- Tracks leave applications, including:
  - Employee details (name, ID).
  - Leave type, duration, and reason.
  - Approval status and notifications.
  - Leave balance.
- Notifications for leave approval/rejection by supervisors or directors (if applicable).

### Reporting
- Attendance reports for employees below a given attendance threshold.
- Outstanding leave applications (e.g., pending approvals).
- Individual employee attendance details.
- Individual employee leave details, including balances.

---

## Architecture
This project adheres to **SOLID principles** and uses the **Observer Pattern** to ensure decoupled, scalable components.

### Key Design Patterns
- **Observer Pattern:**
  - Ensures employees and supervisors are notified automatically of attendance updates, leave approvals, and other critical events.
  - Notifications are triggered by changes in attendance or leave records.

- **Factory Pattern:**
  - Simplifies creation of different leave types with specific rules.

- **Singleton Pattern:**
  - Ensures a single instance of key services (e.g., attendance tracker, notification system).

### Core Components
- **Attendance Module:** Tracks daily attendance and calculates monthly attendance percentage.
- **Leave Management Module:** Handles leave applications, balances, and approval workflows.
- **Notification Module:** Notifies employees and supervisors about approvals, rejections, and other updates.
- **Report Generator:** Creates detailed attendance and leave reports.
