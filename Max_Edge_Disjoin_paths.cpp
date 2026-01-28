#include <bits/stdc++.h>
using namespace std;
/* QUESTION 18: Maximum Edge-Disjoint Paths
Find the maximum number of paths from Source to Sink such that no two paths share an edge.
*/

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

    int s = 1, t = N;
    int num_nodes = N + 1;

    vector<vector<int>> capacity(num_nodes, vector<int>(num_nodes, 0));
    vector<vector<int>> adj(num_nodes);
    vector<vector<int>> flows(num_nodes, vector<int>(num_nodes, 0));

    /* EXPLANATION:
    The number of edge-disjoint paths corresponds exactly to the Max Flow in a network
    where every edge has a capacity of 1. If an edge has capacity 1, it can be used
    by at most one path. The Max Flow value, therefore, counts how many such unit-capacity
    paths can exist simultaneously.
    */

    for (int i = 0; i < M; i++)
    {
        int u, v;
        cin >> u >> v;
        // Ignore original weights/capacities if given, use 1.
        capacity[u][v] = 1;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    cout << "Max Edge-Disjoint Paths: " << maxflow(s, t, num_nodes, capacity, adj, flows) << endl;

    return 0;
}