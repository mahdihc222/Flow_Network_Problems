/* SECTION B1/B2 SOLUTION: Proposed Flyovers
Task: Identify which proposed flyovers (edges) would strictly increase the Max Flow.
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

// Add directed edge
void add_edge(int u, int v, int cap) {
    Edge a = {v, cap, 0, (int)adj[v].size()};
    Edge b = {u, 0, 0, (int)adj[u].size()};
    adj[u].push_back(a);
    adj[v].push_back(b);
}

// Standard BFS for Augmenting Paths
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
            // If not visited AND residual capacity exists
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

// Run Max Flow algorithm
void solve_max_flow(int s, int t) {
    int new_flow;
    while (new_flow = bfs(s, t)) {
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
}

int main() {
    int N, M;
    if (!(cin >> N >> M)) return 0;

    int s = 1, t = N; // Problem states Source is 1, Sink is N
    adj.assign(N + 1, vector<Edge>());
    parent.resize(N + 1);

    // 1. Read Existing Roads
    for (int i = 0; i < M; i++) {
        int u, v, c; cin >> u >> v >> c;
        add_edge(u, v, c);
    }

    // 2. Run Max Flow on existing network
    solve_max_flow(s, t);

    /* EXPLANATION:
    After Max Flow, the network is partitioned by the Minimum Cut.
    - S_reach: Nodes we can still reach from Source using non-saturated edges.
    - T_reach: Nodes from which we can reach Sink using non-saturated edges.
    
    If we add a new edge u -> v:
    - If u is in S_reach, we can push flow from Source to u.
    - If v is in T_reach, any flow arriving at v can eventually reach the Sink.
    - Therefore, connecting u -> v creates a brand new path Source -> u -> v -> Sink.
    */

    // 3. Find S-Reachable Nodes (Forward BFS in Residual)
    vector<bool> S_reach(N + 1, false);
    queue<int> q;
    q.push(s); S_reach[s] = true;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (auto &e : adj[u]) {
            if (!S_reach[e.v] && e.cap - e.flow > 0) {
                S_reach[e.v] = true;
                q.push(e.v);
            }
        }
    }

    // 4. Find T-Reachable Nodes (Backward BFS in Residual)
    // We build the Reverse Residual Graph explicitly for clarity.
    // Edge u->v in residual means we add v->u in reverse graph.
    vector<vector<int>> rev_res(N + 1);
    for (int u = 1; u <= N; u++) {
        for (auto &e : adj[u]) {
            if (e.cap - e.flow > 0) {
                rev_res[e.v].push_back(u);
            }
        }
    }

    vector<bool> T_reach(N + 1, false);
    q.push(t); T_reach[t] = true;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : rev_res[u]) {
            if (!T_reach[v]) {
                T_reach[v] = true;
                q.push(v);
            }
        }
    }

    // 5. Process Proposals
    int P; cin >> P;
    vector<int> results;

    for (int i = 1; i <= P; i++) { // 1-based index for output
        int u, v, c; cin >> u >> v >> c;
        
        // Check if this flyover bridges the cut
        if (S_reach[u] && T_reach[v]) {
            results.push_back(i);
        }
    }

    // 6. Output
    if (results.empty()) {
        cout << "None" << endl;
    } else {
        for (int i = 0; i < results.size(); i++) {
            cout << results[i] << (i == results.size() - 1 ? "" : " ");
        }
        cout << endl;
    }

    return 0;
}