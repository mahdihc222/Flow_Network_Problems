/* PROBLEM 1: THE MOST VITAL EDGE
------------------------------------------------------------------
Question: 
Given a flow network, find the specific edge (u, v) such that removing it 
results in the minimum possible max flow (i.e., causes the biggest drop in flow).
Print the edge and the new reduced flow value.
EXPLANATION:
1. First, calculate the initial Max Flow of the original graph.
2. Identify all edges that carry flow (edges where residual capacity < original capacity). 
   Non-flow edges don't matter because removing them won't change the max flow.
3. For every such "active" edge:
   a. Temporarily set its capacity to 0 (effectively removing it).
   b. Run Max Flow from scratch (or clear flows and re-run).
   c. Record the new flow value.
   d. Restore the edge capacity.
4. The edge that resulted in the smallest "new flow" is the answer.
*/

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <tuple>

using namespace std;

const int INF = 1e9;
struct Edge {
    int v, capacity, original_cap, flow, rev_idx, id;
};

vector<vector<Edge>> adj;
vector<int> parent_edge_idx;
vector<pair<int, int>> parent_node;

void add_edge(int u, int v, int cap, int id) {
    Edge a = {v, cap, cap, 0, (int)adj[v].size(), id};
    Edge b = {u, 0, 0, 0, (int)adj[u].size(), -1}; // Back-edge
    adj[u].push_back(a);
    adj[v].push_back(b);
}

// Standard BFS for Edmonds-Karp
int bfs(int s, int t, vector<pair<int,int>>& parent) {
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
            if (parent[e.v].first == -1 && e.capacity - e.flow > 0) {
                parent[e.v] = {u, i};
                int new_flow = min(flow, e.capacity - e.flow);
                if (e.v == t) return new_flow;
                q.push({e.v, new_flow});
            }
        }
    }
    return 0;
}

int max_flow(int s, int t, int N) {
    int flow = 0;
    int new_flow;
    vector<pair<int, int>> parent(N + 1);

    // Reset flows for a fresh run
    for(int i=0; i<=N; i++) {
        for(auto &e : adj[i]) e.flow = 0;
    }

    while (new_flow = bfs(s, t, parent)) {
        flow += new_flow;
        int cur = t;
        while (cur != s) {
            int prev = parent[cur].first;
            int idx = parent[cur].second;
            adj[prev][idx].flow += new_flow;
            int rev_idx = adj[prev][idx].rev_idx;
            adj[cur][rev_idx].flow -= new_flow;
            cur = prev;
        }
    }
    return flow;
}

int main() {
    int N, M;
    cin >> N >> M; // Nodes, Edges
    int s = 1, t = N; // Assume 1 is Source, N is Sink

    // Store edges to iterate later
    vector<tuple<int, int, int, int>> edge_list; 

    for (int i = 0; i < M; i++) {
        int u, v, c;
        cin >> u >> v >> c;
        add_edge(u, v, c, i);
        edge_list.push_back({u, v, c, i});
    }

    // 1. Get initial max flow
    int initial_flow = max_flow(s, t, N);
    cout << "Initial Max Flow: " << initial_flow << endl;

    int min_flow_after_removal = initial_flow;
    int best_edge_id = -1;

    // 2. Iterate through all edges, temporarily remove, and check flow
    for(auto [u, v, c, id] : edge_list) {
        // Find the edge in adjacency list
        int u_idx = -1;
        for(int k=0; k<adj[u].size(); k++) {
            if(adj[u][k].id == id) {
                u_idx = k; 
                break;
            }
        }

        // Temporarily set capacity to 0
        adj[u][u_idx].capacity = 0; 
        
        // Re-calculate flow
        int current_flow = max_flow(s, t, N);
        
        if (current_flow < min_flow_after_removal) {
            min_flow_after_removal = current_flow;
            best_edge_id = id;
        }

        // Restore capacity
        adj[u][u_idx].capacity = c;
    }

    if(best_edge_id != -1)
        cout << "Remove edge ID " << best_edge_id << " to drop flow to " << min_flow_after_removal << endl;
    else
        cout << "No single edge removal reduces the flow." << endl;

    return 0;
}