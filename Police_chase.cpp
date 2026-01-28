// https://cses.fi/problemset/task/1695
#include <bits/stdc++.h>
using namespace std;

long long bfs(int s, int t, vector<vector<long long>> &capacity, vector<vector<int>> &adj, vector<int> &parent)
{
    fill(parent.begin(), parent.end(), -1);
    parent[s] = -2;
    queue<pair<int, long long>> q;
    q.push({s, LLONG_MAX});
    while (!q.empty())
    {
        int cur = q.front().first;
        long long flow = q.front().second;
        q.pop();

        for (int next : adj[cur])
        {
            if (parent[next] == -1 && capacity[cur][next])
            {
                parent[next] = cur;
                long long new_flow = min(flow, capacity[cur][next]);
                if (next == t)
                    return new_flow;
                q.push({next, new_flow});
            }
        }
    }
    return 0;
}

long long max_flow(int s, int t, vector<vector<long long>> &capacity, vector<vector<int>> &adj, int n)
{
    long long flow = 0;
    vector<int> parent(n + 1);
    long long new_flow;

    while (new_flow = bfs(s, t, capacity, adj, parent))
    {
        flow += new_flow;
        int cur = t;
        while (cur != s)
        {
            int prev = parent[cur];
            capacity[prev][cur] -= new_flow;
            capacity[cur][prev] += new_flow;
            cur = prev;
        }
    }

    return flow;
}

int main()
{
    int n, m;
    cin >> n >> m;
    vector<vector<int>> adj(n + 1);
    vector<vector<long long>> capacity(n + 1, vector<long long>(n + 1, 0));
    vector<pair<int, int>> edges;
    for (int i = 0; i < m; i++)
    {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
        capacity[v][u] += 1;
        capacity[u][v] += 1;
        edges.push_back({u, v});
    }

    long long roads_closed = max_flow(1, n, capacity, adj, n);
    cout << roads_closed << endl;

    vector<bool> vis(n + 1, 0);
    queue<int> q;
    vis[1] = true;
    q.push(1);
    while (!q.empty())
    {
        int u = q.front();
        q.pop();

        for (int v : adj[u])
        {
            if (!vis[v] && capacity[u][v])
            {
                vis[v] = true;
                q.push(v);
            }
        }
    }

    for (auto p : edges)
    {
        int u = p.first;
        int v = p.second;
        if (vis[u] && !vis[v])
        {
            cout << u << " " << v << endl;
        }
        else if (vis[v] && !vis[u])
        {
            cout << v << " " << u << endl;
        }
    }

    return 0;
}