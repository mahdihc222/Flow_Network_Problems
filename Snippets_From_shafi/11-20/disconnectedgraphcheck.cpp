/* QUESTION 13: Disconnected Graph Check
Find the Max Flow in a graph that may be disconnected. 
(Demonstrates that the algorithm handles 0-flow scenarios natively).
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

    adj.assign(N + 1, vector<Edge>());
    parent.resize(N + 1);

    for(int i=0; i<M; i++) {
        int u, v, c; cin >> u >> v >> c;
        add_edge(u, v, c);
    }

    /* EXPLANATION:
    Often students worry if Ford-Fulkerson or Edmonds-Karp crashes on disconnected graphs. 
    They do not. If there is no path from S to T (e.g., they are in different connected components), 
    the very first BFS will return 0 (no path found). The loop `while(new_flow = bfs(...))` 
    will terminate immediately, and the returned Max Flow will be 0. 
    This confirms robustness: no special "Connectivity Check" (like DFS/Union-Find) is needed beforehand.
    */

    int s = 1, t = N;
    int result = max_flow(s, t);

    if(result == 0) {
        cout << "Source and Sink are disconnected. Flow: 0" << endl;
    } else {
        cout << "Max Flow: " << result << endl;
    }

    return 0;
}