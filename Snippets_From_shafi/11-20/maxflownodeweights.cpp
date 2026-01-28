/* 
QUESTION 15: Max Flow with Node Weights
Find the Max Flow (or Min Cut) where Nodes also have capacities (weights).
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

int main() {
    int N, M;
    if (!(cin >> N >> M)) return 0;

    /* EXPLANATION:
    To handle node capacities (weights), we use Node Splitting.
    Every node u is split into u_in and u_out.
    1. Add an edge u_in -> u_out with Capacity = Node Weight.
    2. Any original edge u -> v is now u_out -> v_in with Capacity = INF.
    This effectively forces all flow passing through 'u' to go through the bottleneck 
    edge (u_in -> u_out), respecting the node's weight.
    */

    // Nodes 1..N are u_in. Nodes N+1..2N are u_out.
    int s = 1, t = 2 * N;
    adj.assign(2 * N + 1, vector<Edge>());
    parent.resize(2 * N + 1);

    // Read Node Weights
    for(int i=1; i<=N; i++) {
        int weight; cin >> weight;
        // u_in -> u_out
        add_edge(i, i + N, weight);
    }

    // Read Edges
    for(int i=0; i<M; i++) {
        int u, v; cin >> u >> v;
        // u_out -> v_in
        add_edge(u + N, v, INF);
    }

    cout << "Max Flow with Node Weights: " << max_flow(s, t) << endl;

    return 0;
}