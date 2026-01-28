#include <bits/stdc++.h>
using namespace std;

/*
QUESTION 15: Max Flow with Node Weights
Find the Max Flow (or Min Cut) where Nodes also have capacities (weights).
*/

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

int main()
{
    int N, M;
    if (!(cin >> N >> M))
        return 0;

    // Node Mapping:
    // Node i (Input Side)  -> Index i
    // Node i (Output Side) -> Index i + N
    // Total indices needed: 2*N + 1 (since 1-based)
    int num_nodes = 2 * N + 1;

    vector<vector<int>> capacity(num_nodes, vector<int>(num_nodes, 0));
    vector<vector<int>> adj(num_nodes);
    vector<vector<int>> flows(num_nodes, vector<int>(num_nodes, 0)); // Dummy for template

    // 1. Read Node Weights (Create Internal Edges)
    for (int i = 1; i <= N; i++)
    {
        int weight;
        cin >> weight;

        // Edge: i_in -> i_out with Capacity = Weight
        capacity[i][i + N] = weight;

        // Add to adjacency list (and allow reverse traversal for residual graph)
        adj[i].push_back(i + N);
        adj[i + N].push_back(i);
    }

    // 2. Read Graph Connections (Create External Edges)
    for (int i = 0; i < M; i++)
    {
        int u, v;
        cin >> u >> v;

        // Original: u -> v
        // Split: u_out (u+N) -> v_in (v)
        // Capacity: INF (assuming the bottleneck is the node, not the wire)
        capacity[u + N][v] = INF;

        adj[u + N].push_back(v);
        adj[v].push_back(u + N);
    }

    // Source: Input side of Node 1
    // Sink: Output side of Node N (to ensure we pass through Node N's capacity)
    int s = 1;
    int t = N + N;

    cout << "Max Flow with Node Weights: " << maxflow(s, t, num_nodes, capacity, adj, flows) << endl;

    return 0;
}