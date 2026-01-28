#include <bits/stdc++.h>
using namespace std;
// identify which edges if capacity increased, will increase max flow
//  --- Standard BFS for Max Flow ---
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

// --- Max Flow Function ---
int max_flow(int s, int t, int n, vector<vector<int>> capacity, vector<vector<int>> &adj, vector<vector<int>> &current_flows)
{
    // Note: We pass 'capacity' by value here to create a temporary copy for this run
    // efficiently without messing up the main graph capacities.

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

            // Track actual flow usage for the optimization step
            current_flows[prev][cur] += new_flow;

            cur = prev;
        }
    }
    return flow;
}

int main()
{
    int N, M;
    if (!(cin >> N >> M))
        return 0;

    // 1-based indexing resizing
    int size = N + 1;
    vector<vector<int>> capacity(size, vector<int>(size, 0));
    vector<vector<int>> adj(size);
    vector<pair<int, int>> edges; // Store original edge pairs

    for (int i = 0; i < M; i++)
    {
        int u, v, c;
        cin >> u >> v >> c;
        adj[u].push_back(v);
        adj[v].push_back(u);
        capacity[u][v] += c;
        edges.push_back({u, v});
    }

    int s = 1, t = N;

    // 1. Calculate Initial Max Flow
    // We need a matrix to store exactly how much flow is passing through each edge
    vector<vector<int>> flows(size, vector<int>(size, 0));
    int initial_max_flow = max_flow(s, t, size, capacity, adj, flows);

    cout << "Initial Max Flow: " << initial_max_flow << endl;

    int min_flow = initial_max_flow;
    pair<int, int> best_edge = {-1, -1};

    // 2. Optimization: Only iterate through edges that carry flow!
    for (auto edge : edges)
    {
        int u = edge.first;
        int v = edge.second;

        // CRITICAL OPTIMIZATION:
        // If the edge carries 0 flow, removing it won't change the max flow. Skip it.
        if (flows[u][v] == 0)
            continue;

        // Temporarily remove edge (u, v)
        int original_cap = capacity[u][v];
        capacity[u][v] = 0;

        // Run Max Flow again
        vector<vector<int>> temp_flows(size, vector<int>(size, 0));
        int current_flow = max_flow(s, t, size, capacity, adj, temp_flows);

        // Check if this is the "Most Vital"
        if (current_flow < min_flow)
        {
            min_flow = current_flow;
            best_edge = {u, v};
        }

        // Restore edge
        capacity[u][v] = original_cap;
    }

    if (best_edge.first != -1)
    {
        cout << "Most Vital Edge: " << best_edge.first << " -> " << best_edge.second << endl;
        cout << "Flow drops to: " << min_flow << endl;
    }
    else
    {
        cout << "No single edge removal reduces the flow." << endl;
    }

    return 0;
}