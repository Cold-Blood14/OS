// Shortest Job First (SJF) with Ready Queue Implementation
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

void sjf(vector<Process> &processes) {
    int time = 0;
    double totalTT = 0, totalWT = 0;
    vector<pair<int, string>> ganttChart;

    int n = processes.size();
    int completedProcesses = 0;

    while (completedProcesses < n) {
        vector<string> readyQueue; // Ready queue to display
        int idx = -1;
        int minBT = INT_MAX;

        // Find the process with the shortest burst time among the arrived processes
        for (int i = 0; i < n; ++i) {
            if (!processes[i].completed && processes[i].AT <= time) {
                readyQueue.push_back(processes[i].name);
                if (processes[i].BT < minBT) {
                    minBT = processes[i].BT;
                    idx = i;
                }
                // Tie-breaking based on arrival time
                else if (processes[i].BT == minBT && processes[i].AT < processes[idx].AT) {
                    idx = i;
                }
            }
        }

        showReadyQueue(readyQueue);

        if (idx != -1) {
            ganttChart.push_back({time, processes[idx].name});

            time += processes[idx].BT;
            processes[idx].CT = time;
            processes[idx].TT = processes[idx].CT - processes[idx].AT;
            processes[idx].WT = processes[idx].TT - processes[idx].BT;

            processes[idx].completed = true;
            totalTT += processes[idx].TT;
            totalWT += processes[idx].WT;

            completedProcesses++;
        } else {
            ganttChart.push_back({time, "Idle"});
            time++;
        }
    }

    // Display Gantt Chart
    cout << "\nGantt Chart:\n";
    for (size_t i = 0; i < ganttChart.size(); ++i) {
        cout << ganttChart[i].first << " | " << ganttChart[i].second << " | ";
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

    cout << "\nExecuting Shortest Job First (SJF) Algorithm...\n";
    sjf(processes);

    return 0;
}
