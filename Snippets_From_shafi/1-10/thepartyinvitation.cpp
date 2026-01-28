/* QUESTION 8: The "Party" Invitation
Select the maximum number of people to invite such that no two guests are incompatible.
(Assumes the incompatibility graph is Bipartite).
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
    int N, K; // N people, K incompatibility pairs
    if (!(cin >> N >> K)) return 0;

    /* EXPLANATION:
    This is the "Maximum Independent Set" problem. On general graphs, it is NP-Hard, 
    but on Bipartite Graphs, it is efficiently solvable using Max Flow.
    Max Independent Set = Total Vertices - Max Matching.
    We assume the input describes two groups (e.g., Set U and Set V) where conflicts only 
    occur between groups. We build a flow network: Source -> Set U (cap 1), Set V -> Sink (cap 1), 
    and conflicts as edges U -> V (cap INF). Max Flow gives the size of the Minimum Vertex Cover, 
    and subtracting this from N gives the Maximum Independent Set.
    (Note: This code assumes the user provides the partition or node indices can be separated by parity/range).
    */

    // For this implementation, we assume simple 1..N indices and use 2-coloring to detect bipartite sets.
    vector<int> color(N + 1, -1); 
    vector<vector<int>> conflicts(N + 1);
    
    for(int i=0; i<K; i++) {
        int u, v; cin >> u >> v;
        conflicts[u].push_back(v);
        conflicts[v].push_back(u);
    }

    // BFS to color graph (0 and 1)
    bool possible = true;
    for(int i=1; i<=N; i++) {
        if(color[i] == -1) {
            queue<int> q; q.push(i); color[i] = 0;
            while(!q.empty()){
                int u = q.front(); q.pop();
                for(int v : conflicts[u]){
                    if(color[v] == -1) { color[v] = 1 - color[u]; q.push(v); }
                    else if(color[v] == color[u]) possible = false;
                }
            }
        }
    }

    if(!possible) { cout << "Graph not bipartite, Max Flow N/A" << endl; return 0; }

    int s = 0, t = N + 1;
    adj.assign(t + 1, vector<Edge>());
    parent.resize(t + 1);

    for(int i=1; i<=N; i++) {
        if(color[i] == 0) { // Set U (Left)
            add_edge(s, i, 1);
            for(int v : conflicts[i]) add_edge(i, v, INF);
        } else { // Set V (Right)
            add_edge(i, t, 1);
        }
    }

    int matching = max_flow(s, t);
    cout << "Max Invitees: " << N - matching << endl;

    return 0;
}