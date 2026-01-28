// https://atcoder.jp/contests/abc010/tasks/abc010_4

// same as
/*
Problem Statement: Network Defense
Story A malicious computer worm has infected a computer in a large corporate network. This computer is identified as Node 0. The worm spreads automatically to any computer connected via a direct network link.

There is a list of Vulnerable Servers (Marked Nodes) on the network containing sensitive data. Your job is to ensure that none of these Vulnerable Servers get infected by the worm spreading from Node 0.

To secure the network, you can perform the following defense operations. Each operation takes 1 unit of time (cost = 1).

Defense Operations

Sever a Connection:

Physically disconnect the link between two specific computers.

Effect: The worm can no longer travel directly between these two machines in either direction.

Install a Local Patch:

Install a security patch on a specific Vulnerable Server.

Effect: The server itself becomes immune and will not be infected or compromised.

Crucial Note: Patching a server protects only that server. The server remains part of the network infrastructure and will still forward traffic (including the worm) to other connected computers.

Objective Calculate the minimum total number of operations (Severing Connections + Installing Patches) required to ensure that all Vulnerable Servers are safe.

Note: You cannot perform operations on Node 0 (the source).
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
    int n, g, e;
    cin >> n >> g >> e;

    vector<int> marked_friends;
    for (int i = 0; i < g; i++)
    {
        int j;
        cin >> j;
        marked_friends.push_back(j);
    }

    vector<vector<int>> capacity(n + 1, vector<int>(n + 1, 0));
    vector<vector<int>> adj(n + 1);
    int s = 0;
    int t = n; // super sink
    for (int i = 0; i < e; i++)
    {
        int u, v;
        cin >> u >> v;
        capacity[u][v] = 1;
        capacity[v][u] = 1;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    for (int g : marked_friends)
    {
        capacity[g][t] = 1;
        adj[g].push_back(t);
        adj[t].push_back(g);
    }
    vector<vector<int>> flows(n + 1, vector<int>(n + 1, 0));

    cout << maxflow(s, t, n + 1, capacity, adj, flows) << endl;
}