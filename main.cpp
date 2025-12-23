#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
#include <iomanip> // For formatting/manipulators if needed

using namespace std;

// ==========================================
//          CUSTOM DATA STRUCTURES
// ==========================================

// --- 1. Dynamic Array (Vector) ---
template <typename T>
class DynamicArray {
private:
    T* data;
    int capacity;
    int count;

    void resize() {
        capacity *= 2;
        T* newData = new T[capacity];
        for (int i = 0; i < count; i++) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
    }

public:
    DynamicArray() {
        capacity = 4;
        count = 0;
        data = new T[capacity];
    }
    
    // Copy Constructor (Deep Copy)
    DynamicArray(const DynamicArray& other) {
        capacity = other.capacity;
        count = other.count;
        data = new T[capacity];
        for(int i = 0; i < count; i++) data[i] = other.data[i];
    }

    // Assignment Operator
    DynamicArray& operator=(const DynamicArray& other) {
        if (this != &other) {
            delete[] data;
            capacity = other.capacity;
            count = other.count;
            data = new T[capacity];
            for(int i = 0; i < count; i++) data[i] = other.data[i];
        }
        return *this;
    }

    ~DynamicArray() {
        delete[] data;
    }

    void push_back(T value) {
        if (count == capacity) {
            resize();
        }
        data[count++] = value;
    }

    int size() const {
        return count;
    }

    bool empty() const {
        return count == 0;
    }

    T& operator[](int index) {
        return data[index];
    }
    
    // Iterator support for range-based loops
    T* begin() { return data; }
    T* end() { return data + count; }
    const T* begin() const { return data; }
    const T* end() const { return data + count; }
};

// --- 2. Linked List ---
template <typename T>
struct ListNode {
    T data;
    ListNode* next;
    ListNode* prev;

    ListNode(T val) : data(val), next(nullptr), prev(nullptr) {}
};

template <typename T>
class LinkedList {
private:
    ListNode<T>* head;
    ListNode<T>* tail;
    int count;

public:
    LinkedList() : head(nullptr), tail(nullptr), count(0) {}

    // Destructor
    ~LinkedList() {
        clear();
    }
    
    void clear() {
        ListNode<T>* current = head;
        while (current) {
            ListNode<T>* next = current->next;
            delete current;
            current = next;
        }
        head = tail = nullptr;
        count = 0;
    }
    
    // Deep Copy needed for entity storage
    LinkedList(const LinkedList& other) : head(nullptr), tail(nullptr), count(0) {
        ListNode<T>* curr = other.head;
        while (curr) {
            push_back(curr->data);
            curr = curr->next;
        }
    }
    
    LinkedList& operator=(const LinkedList& other) {
        if (this != &other) {
            clear();
            ListNode<T>* curr = other.head;
            while (curr) {
                push_back(curr->data);
                curr = curr->next;
            }
        }
        return *this;
    }

    void push_back(T val) {
        ListNode<T>* newNode = new ListNode<T>(val);
        if (!tail) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        count++;
    }

    // Insert to keep sorted (Assumes T has operator<)
    void insert_sorted(T val) {
        ListNode<T>* newNode = new ListNode<T>(val);
        if (!head) {
            head = tail = newNode;
            count++;
            return;
        }

        // Special case: Insert at Head
        if (val < head->data) {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
            count++;
            return;
        }

        ListNode<T>* current = head;
        while (current->next && !(val < current->next->data)) {
            current = current->next;
        }
        
        // Insert after current
        newNode->next = current->next;
        if (current->next) {
            current->next->prev = newNode;
        } else {
            tail = newNode; // Updating tail if inserted at end
        }
        current->next = newNode;
        newNode->prev = current;
        count++;
    }

    int size() const { return count; }
    bool empty() const { return count == 0; }
    
    ListNode<T>* getHead() const { return head; }
};

// --- 3. Queue (First-In-First-Out) ---
template <typename T>
class Queue {
private:
    struct QNode {
        T data;
        QNode* next;
        QNode(T val) : data(val), next(nullptr) {}
    };
    
    QNode* frontNode;
    QNode* rearNode;
    
public:
    Queue() : frontNode(nullptr), rearNode(nullptr) {}
    
    ~Queue() {
        while(!empty()) pop();
    }
    
    void push(T val) {
        QNode* newNode = new QNode(val);
        if (!rearNode) {
            frontNode = rearNode = newNode;
            return;
        }
        rearNode->next = newNode;
        rearNode = newNode;
    }
    
    void pop() {
        if (!frontNode) return;
        QNode* temp = frontNode;
        frontNode = frontNode->next;
        if (!frontNode) rearNode = nullptr;
        delete temp;
    }
    
    T front() {
        if (!frontNode) throw runtime_error("Queue is empty"); // Simple error handling
        return frontNode->data;
    }
    
    bool empty() {
        return frontNode == nullptr;
    }
};

// --- 4. Min Heap (Priority Queue) ---
// Note: We want "Max Priority" behavior but where '1' is the highest priority.
// So functionally it's a Min Heap for integer values.
template <typename T>
class MinHeap {
private:
    T* heapArr;
    int capacity;
    int heapSize;

    int parent(int i) { return (i - 1) / 2; }
    int left(int i) { return (2 * i + 1); }
    int right(int i) { return (2 * i + 2); }

    void swap(T& x, T& y) {
        T temp = x;
        x = y;
        y = temp;
    }

    void minHeapify(int i) {
        int l = left(i);
        int r = right(i);
        int smallest = i;

        // Assumes T has overridden operator< (or >) to define priority. 
        // We want the "Top" to be the highest priority.
        // If T is ERPatient (1=Critical), we want 1 to be at root. 
        // So we need "Smallest" numerical value to bubble up. 
        // operator< for ERPatient returns true if p1.priority > p2.priority (reversed logic in previous code).
        // Let's stick to standard Min Heap logic here:
        // Root is the element that behaves as "Smaller" than its children.
        
        if (l < heapSize && heapArr[l] < heapArr[smallest])
            smallest = l;
        if (r < heapSize && heapArr[r] < heapArr[smallest])
            smallest = r;
        if (smallest != i) {
            swap(heapArr[i], heapArr[smallest]);
            minHeapify(smallest);
        }
    }

public:
    MinHeap(int cap = 100) {
        heapSize = 0;
        capacity = cap;
        heapArr = new T[cap];
    }
    
    ~MinHeap() {
        delete[] heapArr;
    }

    void push(T val) {
        if (heapSize == capacity) {
            // Resize logic could go here, for simplicity fixed size or double
            return; 
        }
        heapSize++;
        int i = heapSize - 1;
        heapArr[i] = val;

        while (i != 0 && heapArr[i] < heapArr[parent(i)]) {
            swap(heapArr[i], heapArr[parent(i)]);
            i = parent(i);
        }
    }

    void pop() {
        if (heapSize <= 0) return;
        if (heapSize == 1) {
            heapSize--;
            return;
        }
        heapArr[0] = heapArr[heapSize - 1];
        heapSize--;
        minHeapify(0);
    }

    T top() {
        return heapArr[0]; // Logic assumes heapSize > 0 check before calling
    }

    bool empty() {
        return heapSize == 0;
    }
};

// --- 5. Hash Map ---
// Simple implementation with Chaining for collisions
const int TABLE_SIZE = 101; 

template <typename K, typename V>
struct HashNode {
    K key;
    V value;
    HashNode* next;
    HashNode(K k, V v) : key(k), value(v), next(nullptr) {}
};

template <typename K, typename V>
class HashMap {
private:
    HashNode<K, V>** table;
    
    int hashFunction(int key) {
        return key % TABLE_SIZE;
    }
    // Simple string hash if key is string (not used for ID map but good practice)
    int hashFunction(string key) {
        int hash = 0;
        for (char c : key) hash = (hash * 31 + c) % TABLE_SIZE;
        return hash;
    }

public:
    HashMap() {
        table = new HashNode<K, V>*[TABLE_SIZE];
        for (int i = 0; i < TABLE_SIZE; i++) table[i] = nullptr;
    }
    
    ~HashMap() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            HashNode<K, V>* entry = table[i];
            while (entry) {
                HashNode<K, V>* prev = entry;
                entry = entry->next;
                delete prev;
            }
        }
        delete[] table;
    }

    void insert(K key, V value) {
        int hashIndex = hashFunction(key);
        HashNode<K, V>* prev = nullptr;
        HashNode<K, V>* entry = table[hashIndex];

        while (entry) {
            if (entry->key == key) {
                entry->value = value; // Update
                return;
            }
            prev = entry;
            entry = entry->next;
        }
        
        HashNode<K, V>* newNode = new HashNode<K, V>(key, value);
        if (!prev) {
            table[hashIndex] = newNode;
        } else {
            prev->next = newNode;
        }
    }

    V* get(K key) {
        int hashIndex = hashFunction(key);
        HashNode<K, V>* entry = table[hashIndex];
        while (entry) {
            if (entry->key == key) return &entry->value;
            entry = entry->next;
        }
        return nullptr;
    }
    
    // Iterator helper for saving data: returns a DynamicArray of values
    DynamicArray<V> getAllValues() {
        DynamicArray<V> values;
        for (int i = 0; i < TABLE_SIZE; i++) {
            HashNode<K, V>* entry = table[i];
            while (entry) {
                values.push_back(entry->value);
                entry = entry->next;
            }
        }
        return values;
    }
};

// --- 6. Binary Search Tree (BST) ---
template <typename K, typename V>
struct BSTNode {
    K key;
    V value;
    BSTNode* left;
    BSTNode* right;

    BSTNode(K k, V v) : key(k), value(v), left(nullptr), right(nullptr) {}
};

template <typename K, typename V>
class BST {
private:
    BSTNode<K, V>* root;

    BSTNode<K, V>* insertRec(BSTNode<K, V>* node, K key, V value) {
        if (!node) return new BSTNode<K, V>(key, value);
        if (key < node->key) {
            node->left = insertRec(node->left, key, value);
        } else if (key > node->key) {
            node->right = insertRec(node->right, key, value);
        } else {
            // Key exists. For our use case (Name Index), we might update strict value.
            // But since Value is DynamicArray, we want to append. 
            // The map interface usually returns reference. here we just replace or handle outside?
            // Actually simpler: get() returns pointer, if null call insert.
            // If we are implementing map[key], we need that behavior.
        }
        return node;
    }
    
    BSTNode<K, V>* searchRec(BSTNode<K, V>* node, K key) {
        if (!node || node->key == key) return node;
        if (key < node->key) return searchRec(node->left, key);
        return searchRec(node->right, key);
    }
    
    void inorderRec(BSTNode<K, V>* node, void (*func)(K, V)) {
        if (!node) return;
        inorderRec(node->left, func);
        func(node->key, node->value);
        inorderRec(node->right, func);
    }
    
    // Helper to clear
    void destroyRec(BSTNode<K, V>* node) {
        if (node) {
            destroyRec(node->left);
            destroyRec(node->right);
            delete node;
        }
    }

public:
    BST() : root(nullptr) {}
    ~BST() { destroyRec(root); }

    void insert(K key, V value) {
        if (!search(key)) { // Only insert if new node needed
             root = insertRec(root, key, value);
        }
    }

    V* search(K key) {
        BSTNode<K, V>* res = searchRec(root, key);
        return res ? &res->value : nullptr;
    }
    
    // In-Order Traversal (Sorted)
    void traverse(void (*func)(K, V)) {
        inorderRec(root, func);
    }
    
    // Check if empty
    bool empty() { return root == nullptr; }
};

// ==========================================
//               ENTITIES
// ==========================================

struct Appointment {
    int appointmentID;
    int patientID;
    int doctorID;
    string date; 
    
    bool operator<(const Appointment& other) const {
        return date < other.date; 
    }
};

struct Patient {
    int patientID;
    string name;
    int age;
    string contactInfo;
    int priority; 
    
    LinkedList<string> medicalHistory; // Custom List

    Patient(int id = 0, string n = "", int a = 0, string c = "") 
        : patientID(id), name(n), age(a), contactInfo(c), priority(5) {}
};

struct Doctor {
    int doctorID;
    string name;
    string specialization;
    
    LinkedList<Appointment> schedule; // Custom List

    Doctor(int id = 0, string n = "", string s = "") 
        : doctorID(id), name(n), specialization(s) {}
};

struct ERPatient {
    int patientID;
    int priority; 

    // We used MinHeap. MinHeap pops "Smallest".
    // "Critical - 1" is numerically smaller than "Stable - 5".
    // So if Critical is higher priority, we want 1 at top.
    // normal < operator: 1 < 5 is True. 
    // MinHeap checks: if (child < parent) swap.
    // If child is 1, parent is 5. 1 < 5 is true. 1 swaps up.
    // So 1 ends up at root. Correct.
    bool operator<(const ERPatient& other) const {
        return priority < other.priority; 
    }
};

// ==========================================
//            MANAGERS
// ==========================================

class PatientManager {
private:
    HashMap<int, Patient> patientDB; // Custom Hash Map
    BST<string, DynamicArray<int>> nameIndex; // Custom BST + Dynamic Array

    const string filename = "patients.txt";

    // Static helper for traversal callback
    static PatientManager* instance; 
    
    // Callback function must be static
    static void printPatientCallback(string name, DynamicArray<int> ids) {
        if (!instance) return;
        for (int i = 0; i < ids.size(); i++) {
             Patient* p = instance->getPatient(ids[i]);
             if (p) {
                 cout << p->name << " (ID: " << p->patientID << ")" << endl;
             }
        }
    }

public:
    PatientManager() { instance = this; }

    void registerPatient(int id, string name, int age, string contact) {
        if (patientDB.get(id)) {
            cout << "Error: Patient ID " << id << " already exists." << endl;
            return;
        }

        Patient newP(id, name, age, contact);
        patientDB.insert(id, newP);
        
        // Handle name index
        DynamicArray<int>* existingList = nameIndex.search(name);
        if (existingList) {
            existingList->push_back(id);
        } else {
            DynamicArray<int> newList;
            newList.push_back(id);
            nameIndex.insert(name, newList);
        }
        
        cout << "Patient Registered Successfully." << endl;
    }

    Patient* getPatient(int id) {
        return patientDB.get(id);
    }

    void displayPatient(int id) {
        Patient* p = getPatient(id);
        if (!p) {
            cout << "Patient not found." << endl;
            return;
        }
        cout << "\n--- Patient Details ---" << endl;
        cout << "ID: " << p->patientID << endl;
        cout << "Name: " << p->name << endl;
        cout << "Age: " << p->age << endl;
        cout << "Contact: " << p->contactInfo << endl;
        cout << "Medical History: " << endl;
        
        ListNode<string>* curr = p->medicalHistory.getHead();
        if (!curr) {
            cout << " - None" << endl;
        } else {
            while (curr) {
                cout << " - " << curr->data << endl;
                curr = curr->next;
            }
        }
        cout << "-----------------------" << endl;
    }

    void searchByName(string name) {
        DynamicArray<int>* ids = nameIndex.search(name);
        if (ids) {
            cout << "Found " << ids->size() << " patient(s) with name '" << name << "':" << endl;
            for (int i = 0; i < ids->size(); i++) {
                displayPatient((*ids)[i]);
            }
        } else {
            cout << "No patient found with name '" << name << "'." << endl;
        }
    }

    void addMedicalHistory(int id, string note) {
        Patient* p = getPatient(id);
        if (p) {
            p->medicalHistory.push_back(note);
            cout << "Medical note added." << endl;
        } else {
            cout << "Patient not found." << endl;
        }
    }

    void printAllPatients() {
        if (nameIndex.empty()) {
            cout << "No patients registered." << endl;
            return;
        }
        cout << "\n--- All Patients (Alphabetical) ---" << endl;
        nameIndex.traverse(printPatientCallback);
    }

    void saveData() {
        ofstream outFile(filename);
        if (!outFile) return;
        
        DynamicArray<Patient> allPatients = patientDB.getAllValues();
        for (int i = 0; i < allPatients.size(); i++) {
            Patient p = allPatients[i];
            outFile << p.patientID << "|" << p.name << "|" << p.age << "|" 
                    << p.contactInfo << "|" << p.priority << "|" << p.medicalHistory.size();
            
            ListNode<string>* curr = p.medicalHistory.getHead();
            while(curr) {
                outFile << "|" << curr->data;
                curr = curr->next;
            }
            outFile << endl;
        }
        outFile.close();
        cout << "Patient data saved." << endl;
    }

    void loadData() {
        ifstream inFile(filename);
        if (!inFile) return;

        string line;
        while (getline(inFile, line)) {
            stringstream ss(line);
            string segment;
            DynamicArray<string> parts;

            while (getline(ss, segment, '|')) {
                parts.push_back(segment);
            }

            if (parts.size() < 6) continue;

            int id = stoi(parts[0]);
            string name = parts[1];
            int age = stoi(parts[2]);
            string contact = parts[3];
            int priority = stoi(parts[4]);
            int histCount = stoi(parts[5]);

            // We must call register logically to setup index, but simpler to insert directly
            // Register would re-open file on cout, better do manually
            Patient p(id, name, age, contact);
            p.priority = priority;

            for (int i = 0; i < histCount; ++i) {
                if (6 + i < parts.size()) {
                    p.medicalHistory.push_back(parts[6 + i]);
                }
            }

            patientDB.insert(id, p);
            
            // Update BST
            DynamicArray<int>* existing = nameIndex.search(name);
            if (existing) {
                existing->push_back(id);
            } else {
                DynamicArray<int> nlist;
                nlist.push_back(id);
                nameIndex.insert(name, nlist);
            }
        }
        inFile.close();
    }
};

PatientManager* PatientManager::instance = nullptr;

class DoctorManager {
private:
    HashMap<int, Doctor> doctorDB; // Custom Hash Map
    const string docFilename = "doctors.txt";
    const string apptFilename = "appointments.txt";

public:
    void addDoctor(int id, string name, string specialization) {
        if (doctorDB.get(id)) {
            cout << "Doctor ID already exists." << endl;
            return;
        }
        doctorDB.insert(id, Doctor(id, name, specialization));
        cout << "Doctor Added Successfully." << endl;
    }

    Doctor* getDoctor(int id) {
        return doctorDB.get(id);
    }

    void displayDoctor(int id) {
        Doctor* d = getDoctor(id);
        if (!d) {
            cout << "Doctor not found." << endl;
            return;
        }
        cout << "ID: " << d->doctorID << " | Name: " << d->name << " | Spec: " << d->specialization << endl;
    }

    bool isAvailable(int docID, string time) {
        Doctor* d = getDoctor(docID);
        if (!d) return false;

        ListNode<Appointment>* curr = d->schedule.getHead();
        while(curr) {
            if (curr->data.date == time) return false;
            curr = curr->next;
        }
        return true;
    }

    void bookAppointment(int docID, int patID, int apptID, string time) {
        Doctor* d = getDoctor(docID);
        if (!d) {
            cout << "Doctor not found." << endl;
            return;
        }
        
        Appointment newAppt = {apptID, patID, docID, time};
        d->schedule.insert_sorted(newAppt); // Custom LinkedList sorted insert
        
        cout << "Appointment Booked with Dr. " << d->name << " at " << time << endl;
    }

    void viewAppointments(int docID) {
        Doctor* d = getDoctor(docID);
        if (!d) return;

        cout << "Appointments for Dr. " << d->name << ":" << endl;
        if (d->schedule.empty()) {
            cout << " - No appointments." << endl;
        } else {
            ListNode<Appointment>* curr = d->schedule.getHead();
            while(curr) {
                cout << " - " << curr->data.date << " (Patient ID: " << curr->data.patientID << ")" << endl;
                curr = curr->next;
            }
        }
    }

    void printAllDoctors() {
        cout << "\n--- Doctor List ---" << endl;
        DynamicArray<Doctor> allDocs = doctorDB.getAllValues();
        for(int i=0; i<allDocs.size(); i++) {
            Doctor d = allDocs[i];
            cout << "[" << d.doctorID << "] Dr. " << d.name << " (" << d.specialization << ")" << endl; 
        }
    }

    void saveData() {
        // Save Doctors
        ofstream docFile(docFilename);
        DynamicArray<Doctor> all = doctorDB.getAllValues();
        if (docFile) {
            for(int i=0; i<all.size(); i++) {
                Doctor d = all[i];
                docFile << d.doctorID << "|" << d.name << "|" << d.specialization << endl;
            }
            docFile.close();
            cout << "Doctor data saved." << endl;
        }

        // Save Appointments
        ofstream apptFile(apptFilename);
        if (apptFile) {
            for(int i=0; i<all.size(); i++) {
                Doctor d = all[i];
                ListNode<Appointment>* curr = d.schedule.getHead();
                while(curr) {
                    apptFile << curr->data.appointmentID << "|" << curr->data.doctorID << "|" 
                             << curr->data.patientID << "|" << curr->data.date << endl;
                    curr = curr->next;
                }
            }
            apptFile.close();
            cout << "Appointment data saved." << endl;
        }
    }

    void loadData() {
        ifstream docFile(docFilename);
        if (docFile) {
            string line;
            while (getline(docFile, line)) {
                stringstream ss(line);
                string segment;
                DynamicArray<string> parts;
                while (getline(ss, segment, '|')) parts.push_back(segment);

                if (parts.size() >= 3) {
                    doctorDB.insert(stoi(parts[0]), Doctor(stoi(parts[0]), parts[1], parts[2]));
                }
            }
            docFile.close();
        }

        ifstream apptFile(apptFilename);
        if (apptFile) {
            string line;
            while (getline(apptFile, line)) {
                stringstream ss(line);
                string segment;
                DynamicArray<string> parts;
                while (getline(ss, segment, '|')) parts.push_back(segment);

                if (parts.size() >= 4) {
                    // Logic allows injection because bookAppointment handles lookups
                     bookAppointment(stoi(parts[1]), stoi(parts[2]), stoi(parts[0]), parts[3]);
                }
            }
            apptFile.close();
        }
    }
};

class HospitalSystem {
private:
    PatientManager& patientMgr;
    DoctorManager& doctorMgr;

    Queue<int> waitingRoom; // Custom Queue
    MinHeap<ERPatient> erTriage; // Custom MinHeap (Priority Queue)

public:
    HospitalSystem(PatientManager& pm, DoctorManager& dm) : patientMgr(pm), doctorMgr(dm) {}

    void checkInPatient(int patientID) {
        if (!patientMgr.getPatient(patientID)) {
            cout << "Error: Patient ID " << patientID << " does not exist. Register first." << endl;
            return;
        }
        waitingRoom.push(patientID);
        cout << "Patient " << patientID << " added to Waiting Room Queue." << endl;
    }

    void callNextPatient() {
        if (waitingRoom.empty()) {
            cout << "Waiting Room is empty." << endl;
            return;
        }
        int pid = waitingRoom.front();
        waitingRoom.pop();
        
        Patient* p = patientMgr.getPatient(pid);
        cout << "Calling Patient: " << (p ? p->name : "Unknown") << " (ID: " << pid << ") for General Checkup." << endl;
    }
    
    void addPatientToER(int patientID, int priority) {
        Patient* p = patientMgr.getPatient(patientID);
        if (!p) {
            cout << "Error: Patient not found." << endl;
            return;
        }
        
        p->priority = priority;
        ERPatient erP = {patientID, priority};
        erTriage.push(erP);
        
        cout << "Patient " << p->name << " added to ER Triage with Priority " << priority << "." << endl;
    }

    void callNextERPatient() {
        if (erTriage.empty()) {
            cout << "ER is empty." << endl;
            return;
        }
        
        ERPatient top = erTriage.top();
        erTriage.pop();
        
        Patient* p = patientMgr.getPatient(top.patientID);
        cout << "!!! EMERGENCY !!! Calling Priority " << top.priority << " Patient: " 
             << (p ? p->name : "Unknown") << " (ID: " << top.patientID << ")" << endl;
    }

    void bookAppointment(int patID, int docID, string time) {
        if (!patientMgr.getPatient(patID)) {
            cout << "Patient not found." << endl;
            return;
        }
        if (!doctorMgr.getDoctor(docID)) {
            cout << "Doctor not found." << endl;
            return;
        }
        
        if (!doctorMgr.isAvailable(docID, time)) {
            cout << "Doctor is not available at " << time << "." << endl;
            return;
        }
        
        int apptID = rand() % 10000 + 1; 
        doctorMgr.bookAppointment(docID, patID, apptID, time);
    }
};

void printMenu() {
    cout << "\n=== HOSPITAL MANAGEMENT SYSTEM (Custom DS) ===" << endl;
    cout << "1. Register Patient" << endl;
    cout << "2. Find Patient by ID (HashMap)" << endl;
    cout << "3. Search Patient by Name (BST)" << endl;
    cout << "4. Add Medical History (LinkedList)" << endl;
    cout << "5. Add Doctor" << endl;
    cout << "6. Find Doctor by ID (HashMap)" << endl;
    cout << "7. Book Appointment (Sorted LinkedList)" << endl;
    cout << "8. Check-In Patient (Queue)" << endl;
    cout << "9. Call Next Patient" << endl;
    cout << "10. Add Patient to ER (MinHeap)" << endl;
    cout << "11. Call Next ER Patient" << endl;
    cout << "12. View All (Reports)" << endl;
    cout << "0. ExitAndSave" << endl;
    cout << "Select Option: ";
}

int main() {
    PatientManager patMgr;
    DoctorManager docMgr;
    HospitalSystem hospital(patMgr, docMgr);
    
    // Load Data
    cout << "Loading data..." << endl;
    patMgr.loadData();
    docMgr.loadData();

    int choice;
    while (true) {
        printMenu();
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        
        if (choice == 0) break;

        switch (choice) {
            case 1: {
                int id, age;
                string name, contact;
                cout << "Enter ID: "; cin >> id;
                cout << "Enter Name: "; cin.ignore(); getline(cin, name);
                cout << "Enter Age: "; cin >> age;
                cout << "Enter Contact: "; cin.ignore(); getline(cin, contact);
                patMgr.registerPatient(id, name, age, contact);
                break;
            }
            case 2: {
                int id;
                cout << "Enter ID: "; cin >> id;
                patMgr.displayPatient(id);
                break;
            }
            case 3: {
                string name;
                cout << "Enter Name: "; cin.ignore(); getline(cin, name);
                patMgr.searchByName(name);
                break;
            }
            case 4: {
                int id;
                string note;
                cout << "Enter Patient ID: "; cin >> id;
                cout << "Enter Medical Note: "; cin.ignore(); getline(cin, note);
                patMgr.addMedicalHistory(id, note);
                break;
            }
            case 5: {
                int id;
                string name, spec;
                cout << "Enter ID: "; cin >> id;
                cout << "Enter Name: "; cin.ignore(); getline(cin, name);
                cout << "Enter Specialization: "; getline(cin, spec);
                docMgr.addDoctor(id, name, spec);
                break;
            }
            case 6: {
                int id;
                cout << "Enter ID: "; cin >> id;
                docMgr.displayDoctor(id);
                docMgr.viewAppointments(id);
                break;
            }
            case 7: {
                int pid, did;
                string time;
                cout << "Enter Patient ID: "; cin >> pid;
                cout << "Enter Doctor ID: "; cin >> did;
                cout << "Enter Time (YYYY-MM-DD HH:MM): "; cin.ignore(); getline(cin, time);
                hospital.bookAppointment(pid, did, time);
                break;
            }
            case 8: {
                int id;
                cout << "Enter Patient ID to Check-In: "; cin >> id;
                hospital.checkInPatient(id);
                break;
            }
            case 9: {
                hospital.callNextPatient();
                break;
            }
            case 10: {
                int id, prio;
                cout << "Enter Patient ID: "; cin >> id;
                cout << "Enter Priority (1-Critical, 5-Stable): "; cin >> prio;
                hospital.addPatientToER(id, prio);
                break;
            }
            case 11: {
                hospital.callNextERPatient();
                break;
            }
            case 12: {
                patMgr.printAllPatients();
                docMgr.printAllDoctors();
                break;
            }
            default:
                cout << "Invalid option." << endl;
        }
    }

    patMgr.saveData();
    docMgr.saveData();
    cout << "Data saved. Exiting..." << endl;

    return 0;
}
