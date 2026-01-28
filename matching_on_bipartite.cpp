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
    int n, k, m;
    cin >> n >> m >> k;
    // boys from 1 to n
    // girls from n+1 to n+m
    int size = n + m;
    vector<vector<int>> capacity(size + 100, vector<int>(size + 100, 0));
    vector<vector<int>> adj(size + 100);
    vector<pair<int, int>> edges;
    int s = 0, t = size + 1;
    for (int i = 0; i < k; i++)
    {
        int u, v;
        cin >> u >> v;
        u++;
        v++;
        v += n;
        if (!capacity[u][v])
            edges.push_back({u, v});
        capacity[u][v] = 1;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    for (int i = 1; i <= n; i++)
    {
        capacity[s][i] = 1;
        adj[s].push_back(i);
        adj[i].push_back(s);
    }
    for (int i = n + 1; i <= n + m; i++)
    {
        capacity[i][t] = 1;
        adj[i].push_back(t);
        adj[t].push_back(i);
    }

    vector<vector<int>> flows(size + 100, vector<int>(size + 100, 0));

    int flow = maxflow(s, t, size + 100, capacity, adj, flows);

    cout << flow << endl;
    for (auto e : edges)
    {
        if (flows[e.first][e.second])
        {
            cout << e.first -1<< " " << e.second - n-1 << endl;
        }
    }
    return 0;
}