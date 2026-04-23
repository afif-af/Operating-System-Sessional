#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
using namespace std;

struct Process {
    int id, at, bt, wt = 0, tat = 0, ct = 0;
};

// ---------- GANTT ----------
void printGantt(vector<int> order, vector<int> time) {
    cout << "\nGantt Chart:\n ";

    for (int i = 0; i < order.size(); i++) cout << "--------";
    cout << "-\n|";

    for (int i = 0; i < order.size(); i++) {
        if (order[i] == 0)
            cout << " IDLE |";
        else
            cout << " P" << order[i] << "   |";
    }

    cout << "\n ";
    for (int i = 0; i < order.size(); i++) cout << "--------";
    cout << "-\n";

    for (int i = 0; i < time.size(); i++)
        cout << time[i] << setw(8);

    cout << "\n";
}

// ---------- FCFS ----------
void fcfs(vector<Process> p, int n) {
    sort(p.begin(), p.end(), [](Process a, Process b) {
        return a.at < b.at;
    });

    vector<int> order, time;
    int current = 0;

    for (int i = 0; i < n; i++) {


        if (current < p[i].at) {
            order.push_back(0);
            time.push_back(current);
            current = p[i].at;
        }

        order.push_back(p[i].id);
        time.push_back(current);

        p[i].wt = current - p[i].at;
        current += p[i].bt;
        p[i].ct = current;

        p[i].tat = p[i].ct - p[i].at;
    }
    time.push_back(current);

    printGantt(order, time);

    float avg_wt = 0, avg_tat = 0;
    cout << "\nP\tAT\tBT\tWT\tTAT\n";
    for (auto pr : p) {
        cout << "P" << pr.id << "\t" << pr.at << "\t" << pr.bt << "\t"
             << pr.wt << "\t" << pr.tat << endl;
        avg_wt += pr.wt;
        avg_tat += pr.tat;
    }

    cout << "\nAverage WT = " << avg_wt/n;
    cout << "\nAverage TAT = " << avg_tat/n << endl;
}

// ---------- SJF (Non-Preemptive) ----------
void sjf(vector<Process> p, int n) {
    vector<bool> done(n, false);
    vector<int> order, time;
    int completed = 0, current = 0;

    while (completed < n) {
        int idx = -1;
        int mn = 1e9;

        for (int i = 0; i < n; i++) {
            if (!done[i] && p[i].at <= current && p[i].bt < mn) {
                mn = p[i].bt;
                idx = i;
            }
        }

        if (idx == -1) {
            order.push_back(0);
            time.push_back(current);
            current++;
            continue;
        }
        order.push_back(p[idx].id);
        time.push_back(current);

        p[idx].wt = current - p[idx].at;
        current += p[idx].bt;
        p[idx].ct = current;
        p[idx].tat = p[idx].ct - p[idx].at;

        done[idx] = true;
        completed++;
    }
    time.push_back(current);

    printGantt(order, time);

    float avg_wt = 0, avg_tat = 0;
    cout << "\nP\tAT\tBT\tWT\tTAT\n";
    for (auto pr : p) {
        cout << "P" << pr.id << "\t" << pr.at << "\t" << pr.bt << "\t"
             << pr.wt << "\t" << pr.tat << endl;
        avg_wt += pr.wt;
        avg_tat += pr.tat;
    }
    cout << "\nAverage WT = " << avg_wt/n;
    cout << "\nAverage TAT = " << avg_tat/n << endl;
}

// ---------- Round Robin ----------
void roundRobin(vector<Process> p, int n, int q) {
    vector<int> rem(n);
    for (int i = 0; i < n; i++) rem[i] = p[i].bt;

    vector<int> order, time;
    int current = 0, completed = 0;

    while (completed < n) {
        bool progress = false;

        for (int i = 0; i < n; i++) {
            if (p[i].at <= current && rem[i] > 0) {
                progress = true;

                order.push_back(p[i].id);
                time.push_back(current);

                if (rem[i] > q) {
                    current += q;
                    rem[i] -= q;
                } else {
                    current += rem[i];
                    p[i].wt = current - p[i].at - p[i].bt;
                    p[i].ct = current;
                    p[i].tat = p[i].ct - p[i].at;
                    rem[i] = 0;
                    completed++;
                }
            }
        }

        if (!progress) {
            order.push_back(0);
            time.push_back(current);
            current++;
        }
    }
    time.push_back(current);

    printGantt(order, time);

    float avg_wt = 0, avg_tat = 0;
    cout << "\nP\tAT\tBT\tWT\tTAT\n";
    for (auto pr : p) {
        cout << "P" << pr.id << "\t" << pr.at << "\t" << pr.bt << "\t"
             << pr.wt << "\t" << pr.tat << endl;
        avg_wt += pr.wt;
        avg_tat += pr.tat;
    }

    cout << "\nAverage WT = " << avg_wt/n;
    cout << "\nAverage TAT = " << avg_tat/n << endl;
}

// ---------- MAIN ----------
int main() {
    int n;
    cout << "Enter number of processes: ";
    cin >> n;

    vector<Process> p(n);

    for (int i = 0; i < n; i++) {
        p[i].id = i+1;
        cout << "P" << i+1 << " Arrival Time: ";
        cin >> p[i].at;
        cout << "P" << i+1 << " Burst Time: ";
        cin >> p[i].bt;
    }

    int choice;
    cout << "\n1.FCFS\n2.SJF\n3.Round Robin\nChoice: ";
    cin >> choice;

    if (choice == 1) fcfs(p, n);
    else if (choice == 2) sjf(p, n);
    else if (choice == 3) {
        int q;
        cout << "Enter Time Quantum: ";
        cin >> q;
        roundRobin(p, n, q);
    }
    else cout << "Invalid choice";

    return 0;
}
