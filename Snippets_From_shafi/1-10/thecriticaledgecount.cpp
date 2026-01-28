/* PROBLEM 2: CRITICAL EDGE COUNT
------------------------------------------------------------------
Question: 
Count the number of edges that belong to EVERY possible minimum cut in the graph. 
These are the most critical bottlenecks.

EXPLANATION:
An edge (u, v) is in ALL Min-Cuts if and only if:
1. The edge is fully saturated (flow == capacity).
2. 'u' is reachable from Source in the residual graph.
3. 'Sink' is reachable from 'v' in the residual graph.

Algorithm:
1. Run Max Flow.
2. Build the Residual Graph (conceptually).
3. Run a BFS/DFS from Source on the residual graph to find all reachable nodes (Set S_reach).
4. Run a BFS/DFS from Sink on the REVERSE residual graph to find all nodes from which Sink is reachable (Set T_reach).
5. Count edges (u, v) where u is in S_reach AND v is in T_reach.
*/

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

struct Edge {
    int v, capacity, flow, rev_idx;
};

vector<vector<Edge>> adj;
vector<bool> visited_from_s;
vector<bool> visited_from_t;

void add_edge(int u, int v, int cap) {
    Edge a = {v, cap, 0, (int)adj[v].size()};
    Edge b = {u, 0, 0, (int)adj[u].size()}; 
    adj[u].push_back(a);
    adj[v].push_back(b);
}

// Basic Edmonds-Karp BFS
int bfs_flow(int s, int t, int N, vector<pair<int, int>>& parent) {
    fill(parent.begin(), parent.end(), make_pair(-1, -1));
    parent[s] = {-2, -2};
    queue<pair<int, int>> q;
    q.push({s, 1e9});

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

// Reachability BFS
void bfs_reach(int start, vector<bool>& visited, bool reverse_graph) {
    queue<int> q;
    q.push(start);
    visited[start] = true;
    while(!q.empty()){
        int u = q.front(); q.pop();
        for(auto &e : adj[u]) {
            // Forward Reachability: standard residual check (cap - flow > 0)
            if(!reverse_graph) {
                if(!visited[e.v] && e.capacity - e.flow > 0) {
                    visited[e.v] = true;
                    q.push(e.v);
                }
            } 
            // Backward Reachability: we traverse edges BACKWARDS.
            // In the residual graph, a back-edge v->u exists if flow u->v > 0.
            // But we are using the 'rev_idx' to check connectivity from T backwards.
            // Easier way: Check adjacency of u. If e is an edge u->v with flow>0, 
            // then in residual there is v->u. 
            // WAIT! The standard logic is: Can we reach T from v? 
            // This is equivalent to: Can we reach v from T in the "Transposed Residual Graph"?
            // In Transposed Residual: edge v->u exists if u->v exists in Residual.
        }
    }
}

// Simple DFS for T-reachability on the REVERSED residual graph
// Residual u->v exists if cap-flow > 0.
// We want to find nodes v such that v -> ... -> T exists in Residual.
// Equivalent to T -> ... -> v in "Reversed Residual".
void dfs_reverse_reach(int u, vector<bool>& visited) {
    visited[u] = true;
    // Look at all edges COMING INTO u. In our adjacency list, these are edges stored at OTHER nodes.
    // This is slow (O(E)). Better: Look at edges starting at u.
    // In residual: edge u->v exists if e.cap - e.flow > 0.
    // We want to go BACKWARDS against this arrow.
    // So we look for incoming edges v->u where v->u has residual capacity.
    // In our structure, 'adj[u]' contains the back-edge for v->u. 
    // The back-edge is 'adj[u][i]'. Its 'capacity' is 0, its 'flow' is -flow(v->u).
    // Residual capacity of v->u is cap(v->u) - flow(v->u).
    // If we use the back-edge at u (which points to v), it represents the reverse direction.
    
    // Correct logic using existing struct:
    // Iterate all edges starting at u.
    // If adj[u][i] is a back-edge (original capacity 0), it corresponds to forward edge v->u.
    // If that forward edge v->u has residual capacity, we can travel v <- u.
    
    // Actually, simpler: just iterate all edges in graph. Build explicit reverse graph.
    // Or just use the property:
    // Edge v->u exists in Transposed Residual if u->v exists in Residual.
    // u->v in Residual if (u->v).cap - (u->v).flow > 0.
    // So at node u, check all neighbors v. If (v->u) has residual, visit v.
    // But we don't store "incoming" efficiently. 
    // FOR EXAM SIMPLICITY: Just iterate all edges in global list to run DFS.
} 

int main() {
    int N, M; cin >> N >> M;
    // ... Input handling same as Prob 1 ...
    // Assume graph built and max_flow ran.
    
    // 1. Run Max Flow (Code omitted for brevity, same as Prob 1)
    
    // 2. Find S-Reachable (Standard Residual BFS)
    vector<bool> S_reachable(N+1, false);
    queue<int> q;
    q.push(1); S_reachable[1] = true;
    while(!q.empty()){
        int u = q.front(); q.pop();
        for(auto &e : adj[u]) {
            if(!S_reachable[e.v] && e.capacity - e.flow > 0) {
                S_reachable[e.v] = true;
                q.push(e.v);
            }
        }
    }

    // 3. Find T-Reachable (BFS backwards from T in Residual)
    vector<bool> T_reachable(N+1, false);
    q.push(N); T_reachable[N] = true;
    while(!q.empty()){
        int u = q.front(); q.pop();
        // Check ALL incoming edges to u? 
        // Trick: The back-edge stored at u (adj[u]) represents v->u in original.
        // If v->u has residual capacity, we can go u->v in REVERSE residual.
        // Wait, the back-edge in adjacency list usually has cap=0.
        // Residual of v->u is cap(v->u) - flow(v->u).
        // If this is > 0, we can go v -> u. In reverse, u -> v.
        // So we look for edges connected to u.
        for(auto &e : adj[u]) {
            // e is u->v. We want to traverse if v->u exists in residual.
            // Edge v->u in residual corresponds to original back-edge logic or flow cancellation.
            // Simpler approach for T-reachable:
            // Just iterate ALL edges. If u->v has residual > 0, add reverse edge v->u to a temp graph.
        }
    }
    
    // REVISED SIMPLE IMPLEMENTATION FOR EXAM (O(V+E)):
    // Just build the adjacency for the reversed residual graph explicitly.
    vector<vector<int>> rev_residual(N+1);
    for(int u=1; u<=N; u++){
        for(auto &e : adj[u]){
            if(e.capacity - e.flow > 0) {
                // Residual edge u -> v exists.
                // In Reverse Residual, we have v -> u.
                rev_residual[e.v].push_back(u);
            }
        }
    }
    
    // BFS on rev_residual from T
    q.push(N); 
    // reset T_reachable and use rev_residual...
    // (Implementation straightforward BFS)
    
    // 4. Count Critical Edges
    int critical_count = 0;
    for(int u=1; u<=N; u++) {
        for(auto &e : adj[u]) {
            if(e.capacity > 0) { // original edge
                if(e.flow == e.capacity) { // saturated
                    if(S_reachable[u] && T_reachable[e.v]) {
                        critical_count++;
                    }
                }
            }
        }
    }
    cout << critical_count << endl;
    return 0;
}