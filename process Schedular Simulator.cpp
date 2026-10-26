

#include <iostream>

using namespace std;


class Process {
public:
    char pid;
    int arrivalTime;
    int burstTime;
    int remainingTime;
    int completionTime;
    int waitingTime;
};


class Node {
public:
    Process data;
    Node* next;
};


class Queue {
private:
    Node* front;
    Node* rear;

public:
    Queue() {
        front = NULL;
        rear = NULL;
    }

    void enqueue(Process p) {
        Node* newNode = new Node;
        newNode->data = p;
        newNode->next = NULL;
        if (front == NULL) {
            front = newNode;
            rear = newNode;
            return;
        }
        rear->next = newNode;
        rear = newNode;
    }

    Process dequeue() {
        Process p;
        if (front == NULL) {
            cout << "Queue is Empty" << endl;
            return p;
        }
        Node* temp = front;
        p = temp->data;
        front = front->next;

        if (front == NULL) {
            rear = NULL;
        }
        delete temp;
        return p;
    }

    bool isEmpty() {
        return front == NULL;
    }
};

class LinkedList {
private:
    Node* head;

public:
    LinkedList() {
        head = NULL;
    }

    void insert(Process p) {
        p.remainingTime = p.burstTime;
        Node* newNode = new Node;
        newNode->data = p;
        newNode->next = NULL;

        if (head == NULL) {
            head = newNode;
            return;
        }

        if (p.arrivalTime < head->data.arrivalTime) {
            newNode->next = head;
            head = newNode;
            return;
        }

        Node* temp = head;
        while (temp->next != NULL && temp->next->data.arrivalTime <= p.arrivalTime) {
            temp = temp->next;
        }
        newNode->next = temp->next;
        temp->next = newNode;
    }


    void fcfs() {
        Node* temp = head;
        int currentTime = 0;

        cout << "\nExecution Timeline:\n";
        while (temp != NULL) {
            if (currentTime < temp->data.arrivalTime) {
                currentTime = temp->data.arrivalTime;
            }

            int startTime = currentTime;
            currentTime = currentTime + temp->data.burstTime;
            temp->data.completionTime = currentTime;
            temp->data.waitingTime = temp->data.completionTime - temp->data.arrivalTime - temp->data.burstTime;

            cout << startTime << " -> " << currentTime << " : " << temp->data.pid << endl;
            temp = temp->next;
        }

        temp = head;
        cout << "\nStatistics:\n";
        cout << "Process\tArrival\tBurst\tCompletion\tWaiting\n";
        while (temp != NULL) {
            cout << temp->data.pid << "\t"
                 << temp->data.arrivalTime << "\t"
                 << temp->data.burstTime << "\t"
                 << temp->data.completionTime << "\t\t"
                 << temp->data.waitingTime << endl;
            temp = temp->next;
        }
    }


    void roundRobin(int quantum) {
        Queue q;
        Node* temp = head;
        int currentTime = 0;

        cout << "\nExecution Timeline:\n";

        if (temp != NULL && currentTime < temp->data.arrivalTime) {
            currentTime = temp->data.arrivalTime;
        }

        while (temp != NULL && temp->data.arrivalTime <= currentTime) {
            q.enqueue(temp->data);
            temp = temp->next;
        }

        while (!q.isEmpty() || temp != NULL) {
            if (q.isEmpty()) {
                if (temp != NULL && currentTime < temp->data.arrivalTime) {currentTime = temp->data.arrivalTime;
                }
                while (temp != NULL && temp->data.arrivalTime <= currentTime) {
                    q.enqueue(temp->data);
                    temp = temp->next;
                }
            }

            Process p = q.dequeue();
            int startTime = currentTime;

            if (p.remainingTime > quantum) {
                p.remainingTime -= quantum;
                currentTime += quantum;

                cout << startTime << " -> " << currentTime << " : " << p.pid << endl;

                while (temp != NULL && temp->data.arrivalTime <= currentTime) {
                    q.enqueue(temp->data);
                    temp = temp->next;
                }

                q.enqueue(p);
            }
            else {
                currentTime += p.remainingTime;
                p.remainingTime = 0;

                cout << startTime << " -> " << currentTime << " : " << p.pid << endl;

                while (temp != NULL && temp->data.arrivalTime <= currentTime) {
                    q.enqueue(temp->data);
                    temp = temp->next;
                }


                Node* updateNode = head;
                while (updateNode != NULL) {
                    if (updateNode->data.pid == p.pid) {
                        updateNode->data.completionTime = currentTime;
                        updateNode->data.waitingTime = currentTime - updateNode->data.arrivalTime - updateNode->data.burstTime;
                        break;
                    }
                    updateNode = updateNode->next;
                }
            }
        }


        temp = head;
        cout << "\nStatistics:\n";
        cout << "Process\tArrival\tBurst\tCompletion\tWaiting\n";
        while (temp != NULL) {
            cout << temp->data.pid << "\t"
                 << temp->data.arrivalTime << "\t"
                 << temp->data.burstTime << "\t"
                 << temp->data.completionTime << "\t\t"
                 << temp->data.waitingTime << endl;
            temp = temp->next;
        }
    }
};


int main() {
    LinkedList list;
    int n;

    cout << "Enter number of Process: ";
    cin >> n;

    for (int i = 0; i < n; i++) {
        Process p;
        cout << "Enter Process ID: ";
        cin >> p.pid;
        cout << "Enter Arrival Time: ";
        cin >> p.arrivalTime;
        cout << "Enter Burst Time: ";
        cin >> p.burstTime;
        p.remainingTime = p.burstTime;

        list.insert(p);
    }

    int choice;
    cout << "\n1. FCFS\n";
    cout << "2. Round Robin\n";
    cout << "Choose Algorithm: ";
    cin >> choice;

    if (choice == 1) {
        list.fcfs();
    }
    else if (choice == 2) {
        int quantum;
        cout << "Enter Time Quantum: ";
        cin >> quantum;
        list.roundRobin(quantum);
    }

    return 0;
}
