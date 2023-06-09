#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Function to find the safe sequence
vector<int> findSafeSeq(vector<vector<int>> alloc, vector<vector<int>> mx, vector<int> avail) {
    int n = alloc.size();
    vector<int> work = avail;
    vector<bool> finish(n, false);
    vector<int> safeSeq;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (!finish[j]) {
                bool canExec = true;
                for (int k = 0; k < avail.size(); ++k) {
                    if (mx[j][k] - alloc[j][k] > work[k]) {
                        canExec = false;
                        break;
                    }
                }
                if (canExec) {
                    for (int k = 0; k < avail.size(); ++k) {
                        work[k] += alloc[j][k];
                    }
                    safeSeq.push_back(j);
                    finish[j] = true;
                    break;
                }
            }
        }
    }

    return safeSeq;
}

int main() {
    int n, m;
    cout << "Enter the number of processes: ";
    cin >> n;
    cout << "Enter the number of resource types: ";
    cin >> m;

    vector<vector<int>> alloc(n, vector<int>(m));
    vector<vector<int>> mx(n, vector<int>(m));
    vector<int> avail(m);

    cout << "Enter the allocation matrix: " << endl;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            cin >> alloc[i][j];
        }
    }

    cout << "Enter the maximum matrix: " << endl;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            cin >> mx[i][j];
        }
    }

    cout << "Enter the available resources: " << endl;
    for (int i = 0; i < m; ++i) {
        cin >> avail[i];
    }

    // Find safe sequence
    vector<int> safeSeq = findSafeSeq(alloc, mx, avail);

    if (!safeSeq.empty()) {
        cout << "System is in a safe state. Safe sequence: ";
        for (int i = 0; i < safeSeq.size(); ++i) {
            cout << "P" << safeSeq[i];
            if (i < safeSeq.size() - 1) {
                cout << " -> ";
            }
        }
        cout << endl;
    } else {
        cout << "System is not in a safe state." << endl;
    }

    return 0;
}