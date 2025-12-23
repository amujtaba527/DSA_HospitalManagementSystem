# Hospital Management System (HMS)

## Overview
This is a Command-Line Interface (CLI) application for managing hospital operations using **Custom Data Structures** in C++. The system avoids standard libraries like `vector`, `list`, `map`, and `queue` to demonstrate core low-level understanding of data organization and algorithmic efficiency.

## Workflow

### 1. Data Structures Mapping
The system uses the following custom-built structures for specific tasks:
-   **Patient Database**: `HashMap` (O(1) Access ID → Patient).
-   **Name Search**: `Binary Search Tree (BST)` (Sorted Name → List of IDs).
-   **Medical History**: `Doubly Linked List` (Sequential Append).
-   **Doctor Schedule**: `Sorted Linked List` (Appointments sorted by time).
-   **Waiting Room**: `Queue` (FIFO - First In, First Out).
-   **Emergency Room (ER)**: `MinHeap` (Priority Queue - Critical "1" first).

### 2. Operational Flow

#### Registration
1.  **Input**: User enters ID, Name, Age, Contact.
2.  **Validation**: Check if ID exists in `HashMap`.
3.  **Storage**:
    -   Store `Patient` object in `HashMap`.
    -   Insert ID into `BST` under the Name key (handling duplicate names with a `DynamicArray` of IDs).

#### Appointment Booking
1.  **Check**: Verify Patient ID and Doctor ID using `HashMap`.
2.  **Availability**: Traverse Doctor's schedule (`LinkedList`) to check for time conflicts.
3.  **Booking**: Create `Appointment` object and insert it into the Doctor's linked list **in sorted order** (by date string).

#### Triage System (ER)
1.  **Input**: Patient ID and Priority Level (1=Critical, 5=Stable).
2.  **Process**:
    -   Update patient record.
    -   Push to `MinHeap`. The heap automatically bubbles the lowest number (1) to the root.
3.  **Treatment**: "Call Next ER Patient" pops the root of the heap (Highest Priority).

#### General Checkup (Waiting Room)
1.  **Check-In**: Push Patient ID to `Queue` rear.
2.  **Consultation**: Pop Patient ID from `Queue` front.

#### File Persistence
-   **Save**: On exit, iterate through:
    -   `HashMap` keys for Patients -> write to `patients.txt`.
    -   `HashMap` keys for Doctors -> write to `doctors.txt`.
    -   `Doctor` schedule lists -> write to `appointments.txt`.
-   **Load**: On startup, parse text files line-by-line and populate the data structures.

## Build Instructions
Compile the single source file:
```bash
g++ src/main.cpp -o hms.exe
```
Run the executable:
```bash
./hms.exe
```
