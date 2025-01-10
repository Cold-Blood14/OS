#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <climits>
using namespace std;

struct Process {
    string name;      
    int AT;           
    int BT;           
    int CT;           
    int TT;           
    int WT;           
    int priority;     
    int remainingBT;  
};


void priorityScheduling(vector<Process> &processes, vector<pair<int, string>> &ganttChart, double &totalTT, double &totalWT) {
    int time = 0, completed = 0, n = processes.size();  
    
    for (auto &p : processes) {
        p.remainingBT = p.BT;
    }
    
    while (completed < n) {
        int idx = -1, highestPriority = INT_MAX;
                
        for (int i = 0; i < n; i++) {
            if (processes[i].remainingBT > 0 && processes[i].AT <= time && processes[i].priority < highestPriority) {
                highestPriority = processes[i].priority;
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
        cout << "Enter Process Name, Arrival Time, Burst Time, and Priority for process " << i + 1 << ": ";
        cin >> processes[i].name >> processes[i].AT >> processes[i].BT >> processes[i].priority;
    }

    
    sort(processes.begin(), processes.end(), [](Process &a, Process &b) {
        if (a.AT == b.AT) {
            if (a.priority == b.priority) {
                return a.name < b.name;
            }
            return a.priority < b.priority;
        }
        return a.AT < b.AT;
    });

    vector<pair<int, string>> ganttChart;
    double totalTT = 0, totalWT = 0;

    
    priorityScheduling(processes, ganttChart, totalTT, totalWT);

    
    cout << "\nGantt Chart:\n";
    for (size_t i = 0; i < ganttChart.size(); i++) {
        cout << ganttChart[i].first << " | " << ganttChart[i].second << " | ";
    }
    cout << processes.back().CT << endl; 

    
    cout << "\nProcess\tAT\tBT\tPriority\tCT\tTT\tWT\n";
    for (auto &p : processes) {
        cout << p.name << "\t" << p.AT << "\t" << p.BT << "\t" << p.priority << "\t\t" 
             << p.CT << "\t" << p.TT << "\t" << p.WT << "\n";
    }

    double avgTT = totalTT / n;
    double avgWT = totalWT / n;
    cout << "\nAverage Turnaround Time: " << fixed << setprecision(2) << avgTT;
    cout << "\nAverage Waiting Time: " << fixed << setprecision(2) << avgWT << endl;

    return 0;
}
