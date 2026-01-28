/* QUESTION 20: Fairness (Load Balancing)
Distribute M jobs to N machines such that the maximum load on any single machine is minimized.
*/

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

const int INF = 1e9;

struct Edge {
    int v, cap, flow, rev;
};

vector<vector<Edge>> adj;
vector<pair<int, int>> parent;

void add_edge(int u, int v, int cap) {
    Edge a = {v, cap, 0, (int)adj[v].size()};
    Edge b = {u, 0, 0, (int)adj[u].size()};
    adj[u].push_back(a);
    adj[v].push_back(b);
}

int bfs(int s, int t) {
    fill(parent.begin(), parent.end(), make_pair(-1, -1));
    parent[s] = {-2, -2};
    queue<pair<int, int>> q;
    q.push({s, INF});

    while (!q.empty()) {
        int u = q.front().first;
        int flow = q.front().second;
        q.pop();

        for (int i = 0; i < adj[u].size(); i++) {
            Edge &e = adj[u][i];
            if (parent[e.v].first == -1 && e.cap - e.flow > 0) {
                parent[e.v] = {u, i};
                int new_flow = min(flow, e.cap - e.flow);
                if (e.v == t) return new_flow;
                q.push({e.v, new_flow});
            }
        }
    }
    return 0;
}

int max_flow(int s, int t) {
    int flow = 0, new_flow;
    while (new_flow = bfs(s, t)) {
        flow += new_flow;
        int cur = t;
        while (cur != s) {
            int prev = parent[cur].first;
            int idx = parent[cur].second;
            adj[prev][idx].flow += new_flow;
            int rev = adj[prev][idx].rev;
            adj[cur][rev].flow -= new_flow;
            cur = prev;
        }
    }
    return flow;
}

// Global Inputs
int Jobs, Machines;
vector<pair<int, int>> compatible; // {job_id, machine_id}

bool check(int max_load) {
    int s = 0, t = Jobs + Machines + 1;
    adj.assign(t + 1, vector<Edge>());
    parent.resize(t + 1);

    // Source -> Jobs (Cap 1)
    for(int i=1; i<=Jobs; i++) add_edge(s, i, 1);

    // Jobs -> Machines (Cap 1)
    for(auto p : compatible) {
        add_edge(p.first, Jobs + p.second, 1);
    }

    // Machines -> Sink (Cap = max_load)
    // This is the variable constraint we are testing.
    for(int j=1; j<=Machines; j++) add_edge(Jobs + j, t, max_load);

    // If Max Flow equals number of jobs, all jobs are assigned.
    return max_flow(s, t) == Jobs;
}

int main() {
    if (!(cin >> Jobs >> Machines)) return 0;

    int K; cin >> K; // Number of compatible pairs
    for(int i=0; i<K; i++) {
        int j, m; cin >> j >> m;
        compatible.push_back({j, m});
    }

    /* EXPLANATION:
    We want to minimize the maximum load K. This suggests Binary Search on Answer.
    For a fixed K, we construct a network:
    Source -> Jobs (cap 1).
    Jobs -> Compatible Machines (cap 1).
    Machines -> Sink (cap K).
    If Max Flow == Total Jobs, then it IS possible to distribute jobs such that no machine 
    exceeds load K. We binary search to find the smallest valid K.
    */

    int low = 1, high = Jobs, ans = Jobs;
    
    while(low <= high) {
        int mid = (low + high) / 2;
        if(check(mid)) {
            ans = mid;
            high = mid - 1; // Try smaller load
        } else {
            low = mid + 1; // Need more capacity
        }
    }

    cout << "Minimized Max Load: " << ans << endl;

    return 0;
}