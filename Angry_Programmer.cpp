#include <bits/stdc++.h>
using namespace std;

const int INF = 1e9;


int bfs(int s, int t, int n, vector<int> &parent, vector<vector<int>> &capacity, vector<vector<int>> &adj)
{
    parent.assign(n, -1);
    parent[s] = -2;
    queue<pair<int, int>> q;
    q.push({s, INT_MAX});

    while (!q.empty())
    {
        int u = q.front().first;
        int flow = q.front().second;
        q.pop();

        for (int v : adj[u])
        {
            if (parent[v] == -1 && capacity[u][v] != 0)
            {
                parent[v] = u;
                int new_flow = min(flow, capacity[u][v]);
                if (v == t)
                    return new_flow;
                q.push({v, new_flow});
            }
        }
    }
    return 0;
}

int maxflow(int s, int t, int n, vector<vector<int>> &capacity, vector<vector<int>> &adj, vector<vector<int>> &flows)
{
    int flow = 0;
    vector<int> parent(n);
    int new_flow;
    while (new_flow = bfs(s, t, n, parent, capacity, adj))
    {
        flow += new_flow;
        int cur = t;
        while (cur != s)
        {
            int prev = parent[cur];
            capacity[prev][cur] -= new_flow;
            capacity[cur][prev] += new_flow;

            flows[prev][cur] += new_flow;
            flows[cur][prev] -= new_flow;
            cur = prev;
        }
    }
    return flow;
}

void solve(int caseNum) {
    int M, W;
    cin >> M >> W;

    // Node Splitting Mapping:
    // Node i (Input)  -> Index i
    // Node i (Output) -> Index i + M
    // Total nodes needed: 2*M + 1 (1-based indexing)
    int num_nodes = 2 * M + 1;
    
    vector<vector<int>> capacity(num_nodes, vector<int>(num_nodes, 0));
    vector<vector<int>> adj(num_nodes);
    vector<vector<int>> flows(num_nodes, vector<int>(num_nodes, 0)); // Dummy for template

    // Helper to add directional edge for the flow network
    auto add_edge = [&](int u, int v, int cap) {
        adj[u].push_back(v);
        adj[v].push_back(u); // Reverse edge for residual graph
        capacity[u][v] = cap;
    };

    // 1. Internal Machine Edges (Input -> Output)
    // Boss (1) and Server (M) cannot be destroyed (Infinite cost)
    add_edge(1, 1 + M, INF);
    add_edge(M, M + M, INF);

    // Read costs for machines 2 to M-1
    for (int i = 2; i < M; i++) {
        int cost;
        cin >> cost;
        add_edge(i, i + M, cost);
    }

    // 2. Wire Edges
    // Wire connects u and v with cost c.
    // Logic: Flow goes from u_out (u+M) -> v_in (v) OR v_out (v+M) -> u_in (u)
    for (int i = 0; i < W; i++) {
        int u, v, c;
        cin >> u >> v >> c;
        add_edge(u + M, v, c);
        add_edge(v + M, u, c);
    }

    // Source: Boss Input (1)
    // Sink: Server Input (M) - Reaching the server's input constitutes communication
    int s = 1;
    int t = M; 

    int result = maxflow(s, t, num_nodes, capacity, adj, flows);

    cout << "Case " << caseNum << ": " << result << endl;
}

int main()
{
    // Removing file IO for standard submission
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
    
    int T;
    if (cin >> T) {
        for (int i = 1; i <= T; i++) {
            solve(i);
        }
    }
    return 0;
}