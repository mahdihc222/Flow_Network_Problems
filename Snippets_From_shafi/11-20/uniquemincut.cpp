/* QUESTION 14: Unique Min-Cut?
Determine if the Minimum Cut in the flow network is unique.
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

    adj.assign(N + 1, vector<Edge>());
    parent.resize(N + 1);

    for(int i=0; i<M; i++) {
        int u, v, c; cin >> u >> v >> c;
        add_edge(u, v, c);
    }

    /* EXPLANATION:
    After running Max Flow, a Min-Cut partitions the vertices into two sets: 
    S_Side (reachable from S in residual) and T_Side (not reachable).
    However, there can be multiple Min-Cuts. A simple condition for uniqueness is:
    Does the set of nodes reachable from S (in residual) PLUS the set of nodes that can reach T 
    (in residual) cover ALL vertices? 
    If there are "island" nodes that are neither reachable from S nor can reach T in the residual 
    graph, the cut is generally NOT unique, as these nodes can be shifted between the S-partition 
    and T-partition without changing the cut capacity.
    */

    int s = 1, t = N;
    solve_max_flow(s, t);

    // 1. S-Reachable
    vector<bool> S_reach(N + 1, false);
    queue<int> q;
    q.push(s); S_reach[s] = true;
    while(!q.empty()){
        int u = q.front(); q.pop();
        for(auto &e : adj[u]) {
            if(!S_reach[e.v] && e.cap - e.flow > 0) {
                S_reach[e.v] = true; q.push(e.v);
            }
        }
    }

    // 2. T-Reachable (Reverse BFS)
    vector<vector<int>> rev_res(N + 1);
    for(int u=1; u<=N; u++) {
        for(auto &e : adj[u]) {
            if(e.cap - e.flow > 0) rev_res[e.v].push_back(u);
        }
    }

    vector<bool> T_reach(N + 1, false);
    q.push(t); T_reach[t] = true;
    while(!q.empty()){
        int u = q.front(); q.pop();
        for(int v : rev_res[u]) {
            if(!T_reach[v]) {
                T_reach[v] = true; q.push(v);
            }
        }
    }

    // 3. Check Coverage
    bool unique = true;
    for(int i=1; i<=N; i++) {
        if(!S_reach[i] && !T_reach[i]) {
            unique = false;
            break;
        }
    }

    if(unique) cout << "Min-Cut is Unique" << endl;
    else cout << "Min-Cut is NOT Unique" << endl;

    return 0;
}