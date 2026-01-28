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

vector<int> pathfinderbfs(int s, int t, int n, vector<vector<int>> &adjMat)
{
    vector<int> parent(n);
    parent.assign(n, -1);
    parent[s] = -2;
    queue<int> q;
    q.push(s);
    bool found = false;
    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        for (int v = 0; v < n; v++)
        {
            if (parent[v] == -1 && adjMat[u][v] != 0)
            {
                parent[v] = u;
                if (v == t)
                {
                    found = true;
                    break;
                }
                q.push(v);
            }
        }
    }
    vector<int> path;
    int cur = t;
    while (parent[cur] != -2 && found)
    {
        path.push_back(cur + 1);
        adjMat[parent[cur]][cur]--;
        cur = parent[cur];
    }
    path.push_back(s + 1);
    reverse(path.begin(), path.end());
    return path;
}

int main()
{
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
    int n, m;
    cin >> n >> m;
    vector<vector<int>> capacity(n, vector<int>(n, 0));
    vector<vector<int>> adj(n);
    for (int i = 0; i < m; i++)
    {
        int u, v, c;
        cin >> u >> v;
        u--;
        v--;
        c = 1;
        capacity[u][v] += c;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    vector<vector<int>> flows(n, vector<int>(n, 0));
    vector<vector<int>> original_cap = capacity;
    int s = 0;
    int t = n - 1;

    int flow = maxflow(s, t, n, capacity, adj, flows);

    cout << flow << endl;
    vector<int> p;
    do
    {
        p = pathfinderbfs(s, t, n, flows);
        if (p.size() <= 1)
            break;
        cout << p.size() << endl;
        for (auto i : p)
            cout << i << " ";
        cout << endl;
    } while (p.size() > 1);

    return 0;
}