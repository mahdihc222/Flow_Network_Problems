/* QUESTION 3: The Sabotage (Vertex Cut)
Find the minimum number of intersections (nodes) to block to completely stop 
traffic from Source to Sink. You cannot block Source or Sink themselves.
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
    To solve the Vertex Cut problem using standard Max Flow (which finds Edge Cuts), we use "Node Splitting". 
    We split every node u into two nodes: u_in and u_out, connected by an internal edge with capacity 1 
    (representing the cost to destroy that node). Source and Sink are usually indestructible, so their 
    internal capacity is infinite. Original road edges u->v are transformed into edges u_out->v_in with 
    infinite capacity. The Max Flow in this transformed graph equals the minimum number of internal 
    node-edges we must cut to disconnect Source from Sink.
    */

    // Nodes 1..N are u_in. Nodes N+1..2N are u_out.
    adj.assign(2 * N + 1, vector<Edge>());
    parent.resize(2 * N + 1);

    // 1. Internal Edges: u_in -> u_out
    for (int i = 1; i <= N; i++) {
        // Source (1) and Sink (N) cannot be destroyed (INF), others cost 1 to destroy.
        int cap = (i == 1 || i == N) ? INF : 1;
        add_edge(i, i + N, cap);
    }

    // 2. Connecting Edges: u_out -> v_in
    for (int i = 0; i < M; i++) {
        int u, v;
        cin >> u >> v;
        add_edge(u + N, v, INF);
    }

    // Run flow from Source_in (1) to Sink_out (2N)
    cout << "Min Nodes to Block: " << max_flow(1, 2 * N) << endl;

    return 0;
}