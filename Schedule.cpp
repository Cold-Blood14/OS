#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
using namespace std;

struct Process {
    string name; 
    int AT;      
    int BT;      
    int CT;      
    int TT;     
    int WT;      
    int remainingBT; 
};

void fcfs(vector<Process> &processes, vector<pair<int, string>> &ganttChart, double &totalTT, double &totalWT) {
    int time = 0; 

    for (auto &p : processes) {
        if (time < p.AT) {
            ganttChart.push_back({time, "Idle"});
            time = p.AT; 
        }
        ganttChart.push_back({time, p.name});
        p.CT = time + p.BT;
        p.TT = p.CT - p.AT;
        p.WT = p.TT - p.BT;
        time = p.CT;
        totalTT += p.TT;
        totalWT += p.WT;
    }
}


void sjf(vector<Process> &processes, vector<pair<int, string>> &ganttChart, double &totalTT, double &totalWT) {
    int time = 0, completed = 0, n = processes.size();
    vector<bool> completedFlags(n, false); 

    while (completed < n) {
        int idx = -1, minBT = INT_MAX;

        
        for (int i = 0; i < n; i++) {
            if (!completedFlags[i] && processes[i].AT <= time && processes[i].BT < minBT) {
                minBT = processes[i].BT;
                idx = i;
            }
        }

        if (idx == -1) { 
            ganttChart.push_back({time, "Idle"});
            time++;
        } else {
            ganttChart.push_back({time, processes[idx].name});
            time += processes[idx].BT;
            processes[idx].CT = time;
            processes[idx].TT = processes[idx].CT - processes[idx].AT;
            processes[idx].WT = processes[idx].TT - processes[idx].BT;
            completedFlags[idx] = true;
            totalTT += processes[idx].TT;
            totalWT += processes[idx].WT;
            completed++;
        }
    }
}

void srjf(vector<Process> &processes, vector<pair<int, string>> &ganttChart, double &totalTT, double &totalWT) {
    int time = 0, completed = 0, n = processes.size();
    
    for (auto &p : processes) p.remainingBT = p.BT;

    while (completed < n) {
        int idx = -1, minRemainingBT = INT_MAX;

        for (int i = 0; i < n; i++) {
            if (processes[i].remainingBT > 0 && processes[i].AT <= time && processes[i].remainingBT < minRemainingBT) {
                minRemainingBT = processes[i].remainingBT;
                idx = i;
            }
        }

        if (idx == -1) { 
            ganttChart.push_back({time, "Idle"});
            time++;  
        } else {
            ganttChart.push_back({time, processes[idx].name});
            processes[idx].remainingBT--;  
            time++;

            if (processes[idx].remainingBT == 0) { 
                processes[idx].CT = time;
                processes[idx].TT = processes[idx].CT - processes[idx].AT;
                processes[idx].WT = processes[idx].TT - processes[idx].BT;
                totalTT += processes[idx].TT;
                totalWT += processes[idx].WT;
                completed++;
            }
        }
    }
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

    sort(processes.begin(), processes.end(), [](Process &a, Process &b) {
        if (a.AT == b.AT)
            return a.name < b.name;
        return a.AT < b.AT;
    });

    cout << "\nChoose Scheduling Algorithm:\n1. FCFS\n2. SJF (Non-Preemptive)\n3. SRJF (Preemptive)\nEnter choice: ";
    int choice;
    cin >> choice;

    vector<pair<int, string>> ganttChart;
    double totalTT = 0, totalWT = 0;

    switch (choice) {
        case 1:
            fcfs(processes, ganttChart, totalTT, totalWT);
            break;
        case 2:
            sjf(processes, ganttChart, totalTT, totalWT);
            break;
        case 3:
            srjf(processes, ganttChart, totalTT, totalWT);
            break;
        default:
            cout << "Invalid choice!" << endl;
            return 1;
    }

    cout << "\nGantt Chart:\n";
    for (size_t i = 0; i < ganttChart.size(); i++) {
        cout << ganttChart[i].first << " | " << ganttChart[i].second << " | ";
    }
    cout << processes.back().CT << endl; 

    cout << "\nProcess\tAT\tBT\tCT\tTT\tWT\n";
    for (auto &p : processes) {
        cout << p.name << "\t" << p.AT << "\t" << p.BT << "\t" << p.CT
             << "\t" << p.TT << "\t" << p.WT << "\n";
    }

    double avgTT = totalTT / n;
    double avgWT = totalWT / n;
    cout << "\nAverage Turnaround Time: " << fixed << setprecision(2) << avgTT;
    cout << "\nAverage Waiting Time: " << fixed << setprecision(2) << avgWT << endl;

    return 0;
}
