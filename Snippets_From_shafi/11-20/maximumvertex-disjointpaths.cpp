/* QUESTION 19: Maximum Vertex-Disjoint Paths
Find the maximum number of paths from Source to Sink such that no two paths share an internal vertex.
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
    To ensure paths are Vertex-Disjoint, we must limit the capacity of each NODE to 1.
    We use Node Splitting: Split every node u into u_in and u_out with an internal edge 
    u_in -> u_out of capacity 1.
    Roads u -> v become edges u_out -> v_in with capacity 1 (or INF).
    The Source and Sink are usually exceptions (capacities INF), as multiple paths originate/terminate there.
    */

    // Nodes 1..N become u_in. Nodes N+1..2N become u_out.
    // Flow is from Source_Out (assuming S handles multiple starts) or simply S_in if structure allows.
    // Standard: S -> S_out (INF), T_in -> T (INF).
    // Here we use S=1, T=N.
    
    int s = 1, t = 2 * N; // We flow from 1_in to N_out (effectively)
    adj.assign(2 * N + 1, vector<Edge>());
    parent.resize(2 * N + 1);

    // 1. Internal Node Edges
    for(int i = 1; i <= N; i++) {
        int cap = 1;
        if(i == 1 || i == N) cap = INF; // Source/Sink allow multiple paths
        add_edge(i, i + N, cap); // u_in -> u_out
    }

    // 2. Graph Edges
    for(int i = 0; i < M; i++) {
        int u, v; cin >> u >> v;
        // u_out -> v_in
        add_edge(u + N, v, INF);
    }

    // Flow from Source_in (1) to Sink_out (2*N)
    // Note: Since Source has INF cap to Source_out, and Sink_in has INF cap to Sink_out,
    // this correctly finds paths constrained by intermediate nodes.
    cout << "Max Vertex-Disjoint Paths: " << max_flow(1, 2 * N) << endl;

    return 0;
}