// Shortest Remaining Job First (SRJF) with Ready Queue Implementation
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <iomanip>
using namespace std;

struct Process {
    string name;
    int AT; // Arrival Time
    int BT; // Burst Time
    int RT; // Remaining Time
    int CT; // Completion Time
    int TT; // Turnaround Time
    int WT; // Waiting Time
    bool completed = false; // Flag for process completion
};

void showReadyQueue(vector<string> readyQueue) {
    cout << "Ready Queue: ";
    if (readyQueue.empty()) {
        cout << "[Empty]" << endl;
    } else {
        cout << "[";
        for (size_t i = 0; i < readyQueue.size(); ++i) {
            cout << readyQueue[i];
            if (i != readyQueue.size() - 1) cout << ", ";
        }
        cout << "]" << endl;
    }
}

void srjf(vector<Process> &processes) {
    int time = 0;
    double totalTT = 0, totalWT = 0;
    vector<pair<int, string>> ganttChart;

    int n = processes.size();
    int completedProcesses = 0;

    for (auto &p : processes) {
        p.RT = p.BT; // Initialize Remaining Time
    }

    while (completedProcesses < n) {
        vector<string> readyQueue; // Ready queue to display
        int idx = -1;
        int minRT = INT_MAX;

        // Find the process with the shortest remaining time among the arrived processes
        for (int i = 0; i < n; ++i) {
            if (!processes[i].completed && processes[i].AT <= time) {
                readyQueue.push_back(processes[i].name);
                if (processes[i].RT < minRT) {
                    minRT = processes[i].RT;
                    idx = i;
                }
                // Tie-breaking based on arrival time
                else if (processes[i].RT == minRT && processes[i].AT < processes[idx].AT) {
                    idx = i;
                }
            }
        }

        showReadyQueue(readyQueue);

        if (idx != -1) {
            ganttChart.push_back({time, processes[idx].name});

            time++; // Advance time by 1 unit
            processes[idx].RT--;

            if (processes[idx].RT == 0) {
                processes[idx].CT = time;
                processes[idx].TT = processes[idx].CT - processes[idx].AT;
                processes[idx].WT = processes[idx].TT - processes[idx].BT;

                processes[idx].completed = true;
                totalTT += processes[idx].TT;
                totalWT += processes[idx].WT;

                completedProcesses++;
            }
        } else {
            ganttChart.push_back({time, "Idle"});
            time++;
        }
    }

    // Display Gantt Chart
    cout << "\nGantt Chart:\n";
    for (size_t i = 0; i < ganttChart.size(); ++i) {
        if (i == 0 || ganttChart[i].second != ganttChart[i - 1].second) {
            cout << ganttChart[i].first << " | " << ganttChart[i].second << " | ";
        }
    }
    cout << time << endl;

    // Display Process Details
    cout << "\nProcess\tAT\tBT\tCT\tTT\tWT\n";
    for (auto &p : processes) {
        cout << p.name << "\t" << p.AT << "\t" << p.BT << "\t" << p.CT
             << "\t" << p.TT << "\t" << p.WT << "\n";
    }

    // Display Average Times
    double avgTT = totalTT / n;
    double avgWT = totalWT / n;
    cout << "\nAverage Turnaround Time: " << fixed << setprecision(2) << avgTT;
    cout << "\nAverage Waiting Time: " << fixed << setprecision(2) << avgWT << endl;
}

int main() {
    int n;
    cout << "Enter the number of processes: ";
    cin >> n;

    vector<Process> processes(n);
    for (int i = 0; i < n; i++) {
        cout << "Enter Process Name, Arrival Time, and Burst Time for process " << i + 1 << ": ";
        cin >> processes[i].name >> processes[i].AT >> processes[i].BT;
    }

    cout << "\nExecuting Shortest Remaining Job First (SRJF) Algorithm...\n";
    srjf(processes);

    return 0;
}
