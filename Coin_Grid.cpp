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
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
    int n;
    cin >> n;
    vector<string> strs;
    for (int i = 0; i < n; i++)
    {
        string t;
        cin >> t;
        strs.push_back(t);
    }
    vector<vector<int>> capacity(2 * n + 2, vector<int>(2 * n + 2, 0));
    vector<vector<int>> adj(2 * n + 2);

    vector<pair<int, int>> edges;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (strs[i][j] == 'o')
            {
                capacity[i + 1][n + j + 1] = 1;
                adj[i + 1].push_back(j + n + 1);
                adj[n + j + 1].push_back(i + 1);
            }
        }
    }
    int s = 0;
    int t = 2 * n + 1;
    for (int i = 1; i <= n; i++)
    {
        capacity[s][i] = 1;
        adj[s].push_back(i);
        adj[i].push_back(s);
    }
    for (int i = n + 1; i <= 2 * n; i++)
    {
        capacity[i][t] = 1;
        adj[i].push_back(t);
        adj[t].push_back(i);
    }
    vector<vector<int>> original = capacity;

    vector<vector<int>> flows(2 * n + 2, vector<int>(2 * n + 2, 0));
    int flow = maxflow(s, t, 2 * n + 2, capacity, adj, flows);

    vector<bool> vis(2 * n + 2, false);
    queue<int> q;

    for (int i = 1; i <= n; i++)
    {
        if (flows[s][i] == 0)
        {
            q.push(i);
            vis[i] = true;
        }
    }

    while (!q.empty())
    {
        int u = q.front();
        q.pop();

        if (u >= 1 && u <= n)
        {
            for (int v : adj[u])
            {
                if (!vis[v] && flows[u][v] == 0 && original[u][v] == 1)
                {
                    vis[v] = true;
                    q.push(v);
                }
            }
        }

        else if (u >= n + 1 && u <= 2 * n)
        {
            for (int v : adj[u])
            {
                if (!vis[v] && flows[v][u] == 1 && original[v][u] == 1)
                {
                    vis[v] = true;
                    q.push(v);
                }
            }
        }
    }

    vector<int> rows, cols;

    for (int i = 1; i <= n; i++)
    {
        if (!vis[i])
            rows.push_back(i);
    }

    for (int i = n + 1; i <= 2 * n; i++)
    {
        if (vis[i])
            cols.push_back(i);
    }

    cout << rows.size() + cols.size() << endl;

    for (int r : rows)
    {
        cout << 1 << " " << r << endl;
    }
    for (int c : cols)
    {
        cout << 2 << " " << (c - n) << endl;
    }
    return 0;
}