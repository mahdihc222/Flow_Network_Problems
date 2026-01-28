/* QUESTION 5: Preferential Matching
Maximize matches for Priority 1 choices first, then maximize Priority 2 
without reducing the number of Priority 1 matches.
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

// Helper to run flow without resetting current flow state
int augment_flow(int s, int t) {
    int added_flow = 0, new_flow;
    while (new_flow = bfs(s, t)) {
        added_flow += new_flow;
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
    return added_flow;
}

int main() {
    int Students, Topics;
    if (!(cin >> Students >> Topics)) return 0;

    /* EXPLANATION:
    We use a Two-Phase Max Flow strategy. In Phase 1, we build the graph containing ONLY the Priority 1 
    edges and run Max Flow to lock in the maximum number of first-choice matches. In Phase 2, we 
    add the Priority 2 edges to the EXISTING graph state (preserving the current flows) and run 
    Max Flow again. The algorithm will automatically find augmenting paths for 2nd choices using 
    remaining capacity or valid re-routing, without ever decreasing the total count of matches achieved in Phase 1.
    */

    int S = 0, T = Students + Topics + 1;
    adj.assign(T + 1, vector<Edge>());
    parent.resize(T + 1);

    vector<pair<int, int>> choices(Students + 1);
    for (int i = 1; i <= Students; i++) {
        cin >> choices[i].first >> choices[i].second;
    }

    // Edges from Source -> Students and Topics -> Sink
    for (int i = 1; i <= Students; i++) add_edge(S, i, 1);
    for (int j = 1; j <= Topics; j++) add_edge(Students + j, T, 1);

    // --- PHASE 1: Priority 1 Edges ---
    for (int i = 1; i <= Students; i++) {
        add_edge(i, Students + choices[i].first, 1);
    }
    int p1_matches = augment_flow(S, T);

    // --- PHASE 2: Priority 2 Edges ---
    for (int i = 1; i <= Students; i++) {
        add_edge(i, Students + choices[i].second, 1);
    }
    int p2_matches = augment_flow(S, T);

    cout << "Priority 1 Matches: " << p1_matches << endl;
    cout << "Priority 2 Matches (Added): " << p2_matches << endl;
    cout << "Total Matches: " << p1_matches + p2_matches << endl;

    return 0;
}