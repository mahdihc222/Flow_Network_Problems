/* QUESTION 10: "Demand" Satisfaction
Given Power Plants with production limits and Cities with specific demands, 
determine if it is possible to satisfy all demands fully.
*/

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <numeric>

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

    int s = 0, t = N + 1;
    adj.assign(t + 1, vector<Edge>());
    parent.resize(t + 1);

    // Read Transmission Network
    for(int i=0; i<M; i++) {
        int u, v, c; cin >> u >> v >> c;
        add_edge(u, v, c);
    }

    int num_plants, num_cities;
    cin >> num_plants >> num_cities;

    /* EXPLANATION:
    To verify if all demands can be met, we model this as a Max Flow problem.
    We create a Super-Source connected to all Power Plants with capacity equal to their Production Limit.
    We create a Super-Sink connected from all Cities with capacity equal to their Demand.
    We then run Max Flow. If the resulting Max Flow is exactly equal to the Sum of All Demands, 
    then it is possible to satisfy everyone. If less, then the network (or supply) is insufficient.
    */

    // Sources (Plants)
    for(int i=0; i<num_plants; i++) {
        int u, production; cin >> u >> production;
        add_edge(s, u, production);
    }

    // Sinks (Cities)
    long long total_demand = 0;
    for(int i=0; i<num_cities; i++) {
        int v, demand; cin >> v >> demand;
        add_edge(v, t, demand);
        total_demand += demand;
    }

    int flow = max_flow(s, t);

    if(flow == total_demand) {
        cout << "YES, all demands satisfied." << endl;
    } else {
        cout << "NO, deficit of " << total_demand - flow << " units." << endl;
    }

    return 0;
}