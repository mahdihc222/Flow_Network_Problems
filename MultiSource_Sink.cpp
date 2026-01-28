/* QUESTION 9: Multi-Source/Multi-Sink Traffic
Calculate Max Flow in a network where traffic enters from multiple start cities
and exits at multiple destination districts.
*/

#include <bits/stdc++.h>
using namespace std;

const int INF = 1e9;

// Standard BFS to find augmenting path in residual graph
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
            // Check if unvisited and residual capacity exists
            if (parent[v] == -1 && capacity[u][v] > 0)
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

// Standard MaxFlow using Edmonds-Karp
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
            // Update Residual Graph
            capacity[prev][cur] -= new_flow;
            capacity[cur][prev] += new_flow;

            // Track Flows
            flows[prev][cur] += new_flow;
            flows[cur][prev] -= new_flow;
            cur = prev;
        }
    }
    return flow;
}

int main()
{
    int N, M; // Nodes, Edges
    if (!(cin >> N >> M))
        return 0;

    // Node 0 is Super-Source (S)
    // Node N+1 is Super-Sink (T)
    int s = 0;
    int t = N + 1;
    int total_nodes = N + 2;

    // Resize vectors to handle 0 to N+1
    vector<vector<int>> capacity(total_nodes, vector<int>(total_nodes, 0));
    vector<vector<int>> adj(total_nodes);
    vector<vector<int>> flows(total_nodes, vector<int>(total_nodes, 0));

    // 1. Read Graph Edges (Roads)
    for (int i = 0; i < M; i++)
    {
        int u, v, c;
        cin >> u >> v >> c;
        // Check if edge already exists to handle parallel edges if necessary,
        // though typically we just add capacity.
        bool exists = false;
        for (int neighbor : adj[u])
            if (neighbor == v)
                exists = true;

        if (!exists)
        {
            adj[u].push_back(v);
            adj[v].push_back(u); // Add reverse edge for residual graph
        }
        capacity[u][v] += c; // Add to capacity in case of multiple lines
    }

    int nSrc, nSnk;
    cin >> nSrc >> nSnk;

    // 2. Connect Super-Source (S) to all Entry Cities
    // Capacity is INF because the limit is determined by the road network, not the entry itself
    for (int i = 0; i < nSrc; i++)
    {
        int u;
        cin >> u;
        adj[s].push_back(u);
        adj[u].push_back(s);
        capacity[s][u] = INF;
    }

    // 3. Connect all Exit Cities to Super-Sink (T)
    for (int i = 0; i < nSnk; i++)
    {
        int v;
        cin >> v;
        adj[v].push_back(t);
        adj[t].push_back(v);
        capacity[v][t] = INF;
    }

    // 4. Calculate Max Flow
    cout << "Total Network Flow: " << maxflow(s, t, total_nodes, capacity, adj, flows) << endl;

    return 0;
}