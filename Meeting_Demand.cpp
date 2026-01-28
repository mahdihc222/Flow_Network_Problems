/* QUESTION: "Demand" Satisfaction
Given Power Plants with production limits and Cities with specific demands,
determine if it is possible to satisfy all demands fully.
*/

#include <bits/stdc++.h>
using namespace std;

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
    int n, m;
    cin >> n >> m;

    int s = 0, t = n + 1;

    vector<vector<int>> adj(t + 1);
    vector<vector<int>> capacity(t + 1, vector<int>(t + 1, 0));
    for (int i = 0; i < m; i++)
    {
        int u, v, c;
        cin >> u >> v >> c;
        capacity[u][v] = c;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    int num_plants, num_cities;
    cin >> num_plants >> num_cities;

    // Sources (Plants)
    for (int i = 0; i < num_plants; i++)
    {
        int u, production;
        cin >> u >> production;
        capacity[s][u] = production;
        adj[s].push_back(u);
        adj[u].push_back(s);
    }

    // Sinks (Cities)
    int total_demand = 0;

    for (int i = 0; i < num_cities; i++)
    {
        int v, demand;
        cin >> v >> demand;
        capacity[v][t] = demand;
        adj[v].push_back(t);
        adj[t].push_back(v);
        total_demand += demand;
    }

    vector<vector<int>> flows(n + 2, vector<int>(n + 2, 0));
    int flow = maxflow(s, t, n + 2, capacity, adj, flows);

    if (flow == total_demand)
    {
        cout << "YES, all demands satisfied." << endl;
    }
    else
    {
        cout << "NO, deficit of " << total_demand - flow << " units." << endl;
    }
    return 0;
}