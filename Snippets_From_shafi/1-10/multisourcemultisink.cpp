/* QUESTION 9: Multi-Source/Multi-Sink Traffic
Calculate Max Flow in a network where traffic enters from multiple start cities 
and exits at multiple destination districts.
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
    int N, M; // Nodes, Edges
    if (!(cin >> N >> M)) return 0;

    /* EXPLANATION:
    Standard Max Flow algorithms (Ford-Fulkerson/Edmonds-Karp) work on Single-Source, 
    Single-Sink networks. To handle multiple sources {s1, s2...} and multiple sinks {t1, t2...}, 
    we construct a "Super-Source" S and a "Super-Sink" T. 
    We add edges from S -> s_i with capacity Infinity (or the source's specific capacity). 
    We add edges from t_j -> T with capacity Infinity (or the sink's specific capacity). 
    Running Max Flow from S to T solves the problem.
    */

    // Nodes 1..N are graph nodes. 0 is Super-Source, N+1 is Super-Sink.
    int s = 0, t = N + 1;
    adj.assign(t + 1, vector<Edge>());
    parent.resize(t + 1);

    // Read Graph Edges
    for(int i=0; i<M; i++) {
        int u, v, c; cin >> u >> v >> c;
        add_edge(u, v, c);
    }

    int nSrc, nSnk;
    cin >> nSrc >> nSnk;

    // Connect Sources to Super-Source
    for(int i=0; i<nSrc; i++) {
        int u; cin >> u;
        add_edge(s, u, INF);
    }

    // Connect Sinks to Super-Sink
    for(int i=0; i<nSnk; i++) {
        int v; cin >> v;
        add_edge(v, t, INF);
    }

    cout << "Total Network Flow: " << max_flow(s, t) << endl;

    return 0;
}