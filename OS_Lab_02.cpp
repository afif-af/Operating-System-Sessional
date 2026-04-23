#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
using namespace std;

struct Process {
    int id, at, bt, pr;
    int ct = 0, wt = 0, tt = 0;
};


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

void priorityScheduling(vector<Process> p, int n) {
    vector<bool> done(n, false);
    int completed = 0, currentTime = 0;

    vector<int> order, time;
    time.push_back(0);

    while (completed < n) {
        int idx = -1;
        int highestPriority = 1e9;

        for (int i = 0; i < n; i++) {
            if (!done[i] && p[i].at <= currentTime) {
                if (p[i].pr < highestPriority) {
                    highestPriority = p[i].pr;
                    idx = i;
                }
            }
        }

        if (idx == -1) {
            currentTime++;
            order.push_back(0); // idle
            time.push_back(currentTime);
        } else {
            order.push_back(p[idx].id);
            currentTime += p[idx].bt;
            time.push_back(currentTime);

            p[idx].ct = currentTime;
            p[idx].tt = p[idx].ct - p[idx].at;
            p[idx].wt = p[idx].tt - p[idx].bt;

            done[idx] = true;
            completed++;
        }
    }

    cout << "\nPriority Scheduling:\n";
    cout << "ID AT BT PR CT TT WT\n";
    for (auto x : p)
        cout << x.id << "  " << x.at << "  " << x.bt << "  "
             << x.pr << "  " << x.ct << "  " << x.tt << "  " << x.wt << endl;

    printGantt(order, time);
}


void sjfPreemptive(vector<Process> p, int n) {
    vector<int> rem(n);
    for (int i = 0; i < n; i++) rem[i] = p[i].bt;

    int completed = 0, currentTime = 0;

    vector<int> order, time;
    time.push_back(0);

    while (completed < n) {
        int idx = -1;
        int minBT = 1e9;

        for (int i = 0; i < n; i++) {
            if (p[i].at <= currentTime && rem[i] > 0) {
                if (rem[i] < minBT) {
                    minBT = rem[i];
                    idx = i;
                }
            }
        }

        if (idx == -1) {
            currentTime++;
            order.push_back(0);
            time.push_back(currentTime);
            continue;
        }

        order.push_back(p[idx].id);
        rem[idx]--;
        currentTime++;
        time.push_back(currentTime);

        if (rem[idx] == 0) {
            completed++;
            p[idx].ct = currentTime;
            p[idx].tt = p[idx].ct - p[idx].at;
            p[idx].wt = p[idx].tt - p[idx].bt;
        }
    }

    cout << "\nSJF Preemptive:\n";
    cout << "ID AT BT CT TT WT\n";
    for (auto x : p)
        cout << x.id << "  " << x.at << "  " << x.bt << "  "
             << x.ct << "  " << x.tt << "  " << x.wt << endl;

    printGantt(order, time);
}


int main() {
    int n;
    cout << "Enter number of processes: ";
    cin >> n;

    vector<Process> p(n);

    cout << "Enter AT and BT:\n";
    for (int i = 0; i < n; i++) {
        p[i].id = i+1;
        cout << "P" << i+1 << " Arrival Time: ";
        cin >> p[i].at;
        cout << "P" << i+1 << " Burst Time: ";
        cin >> p[i].bt;
    }

    int choice;
    cout << "\n1. Priority\n2. SJF Preemptive\nChoice: ";
    cin >> choice;

    if (choice == 1) {
       cout << "Enter Priority:\n";
       for (int i = 0; i < n; i++) {
            cout << "P" << p[i].id << ": ";
            cin >> p[i].pr;
        }
        priorityScheduling(p, n);
    } else if (choice == 2) {
        sjfPreemptive(p, n);
    } else {
        cout << "Invalid choice\n";
    }

    return 0;
}
