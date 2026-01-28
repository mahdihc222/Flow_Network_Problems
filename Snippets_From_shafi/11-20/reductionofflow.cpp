/* QUESTION 17: Reduction of Flow
A specific critical road (u, v) has collapsed. Calculate the new Max Flow efficiently.
(For the exam, we implement the robust re-calculation method).
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
    // Reset flows for clean run
    for(auto &list : adj) {
        for(auto &e : list) e.flow = 0;
    }

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

    int s = 1, t = N;
    adj.assign(N + 1, vector<Edge>());
    parent.resize(N + 1);

    struct InputEdge { int u, v, c; };
    vector<InputEdge> edges;

    for (int i = 0; i < M; i++) {
        int u, v, c; cin >> u >> v >> c;
        edges.push_back({u, v, c});
        add_edge(u, v, c);
    }

    int broken_idx; 
    cin >> broken_idx; // Index of edge to remove (0-based)

    /* EXPLANATION:
    When an edge is removed, the Max Flow can decrease or stay the same. 
    While algorithms exist to dynamically update flow (by pushing flow back from the break), 
    they are complex to implement under time pressure. The safest and simplest "Exam Strategy" 
    is to rebuild the graph skipping the broken edge and re-run the Max Flow algorithm from scratch. 
    This ensures correctness without subtle bugs in residual flow redirection.
    */

    // Rebuild graph excluding the broken edge
    adj.assign(N + 1, vector<Edge>());
    for (int i = 0; i < M; i++) {
        if (i == broken_idx) continue; // Skip broken edge
        add_edge(edges[i].u, edges[i].v, edges[i].c);
    }

    cout << "New Max Flow after collapse: " << max_flow(s, t) << endl;

    return 0;
}