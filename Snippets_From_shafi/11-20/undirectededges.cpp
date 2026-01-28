/* QUESTION 16: Undirected Edges
Calculate Max Flow in a network where roads are two-way (undirected).
Traffic can flow in either direction, up to the road's capacity in that direction.
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

    int s = 1, t = N;
    adj.assign(N + 1, vector<Edge>());
    parent.resize(N + 1);

    /* EXPLANATION:
    Standard Max Flow requires directed edges. If an input edge (u, v) with capacity C 
    is undirected, it effectively allows C units of flow from u->v AND C units of flow 
    from v->u. (Or, in some problem variations, the SUM of flows must be <= C, but 
    typically in CP, "undirected pipes" mean bidirectional capacity).
    We model this by adding TWO directed edges for every undirected input edge:
    1. u -> v with Capacity C
    2. v -> u with Capacity C
    The algorithm then naturally decides which direction to utilize.
    */

    for (int i = 0; i < M; i++) {
        int u, v, c;
        cin >> u >> v >> c;
        // Add u -> v
        add_edge(u, v, c);
        // Add v -> u
        add_edge(v, u, c);
    }

    cout << "Max Flow (Undirected): " << max_flow(s, t) << endl;

    return 0;
}