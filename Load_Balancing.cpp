#include <bits/stdc++.h>
using namespace std;
/*
QUESTION 20: Fairness (Load Balancing)
Distribute M jobs to N machines such that the maximum load on any single machine is minimized.
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
    int num_jobs, num_machines;
    // Input Format: Jobs Machines
    if (!(cin >> num_jobs >> num_machines))
        return 0;

    int K; // Number of compatible pairs
    cin >> K;

    vector<pair<int, int>> compatible_edges;
    for (int i = 0; i < K; i++)
    {
        int j, m;
        cin >> j >> m;
        compatible_edges.push_back({j, m});
    }

    // Node Mapping:
    // 0: Source
    // 1 to num_jobs: Job Nodes
    // num_jobs + 1 to num_jobs + num_machines: Machine Nodes
    // Total Nodes + 1: Sink
    int s = 0;
    int t = num_jobs + num_machines + 1;
    int total_nodes = t + 1;

    // Binary Search on the "Max Load"
    // Lower bound: 1 job per machine (optimistic)
    // Upper bound: All jobs on one machine (pessimistic)
    int low = 1, high = num_jobs;
    int ans = num_jobs;

    while (low <= high)
    {
        int mid = (low + high) / 2;

        // --- Build Graph for this specific limit (mid) ---
        vector<vector<int>> capacity(total_nodes, vector<int>(total_nodes, 0));
        vector<vector<int>> adj(total_nodes);
        vector<vector<int>> flows(total_nodes, vector<int>(total_nodes, 0)); // Dummy for template

        // 1. Source -> Jobs (Capacity 1)
        for (int i = 1; i <= num_jobs; i++)
        {
            capacity[s][i] = 1;
            adj[s].push_back(i);
            adj[i].push_back(s);
        }

        // 2. Jobs -> Machines (Capacity 1)
        for (auto edge : compatible_edges)
        {
            int u = edge.first;             // Job ID
            int v = num_jobs + edge.second; // Machine ID (shifted)

            capacity[u][v] = 1;
            adj[u].push_back(v);
            adj[v].push_back(u);
        }

        // 3. Machines -> Sink (Capacity = MID)
        // This is the constraint: No machine can take more than 'mid' jobs
        for (int i = 1; i <= num_machines; i++)
        {
            int u = num_jobs + i;
            capacity[u][t] = mid;
            adj[u].push_back(t);
            adj[t].push_back(u);
        }

        // --- Check Max Flow ---
        int current_flow = maxflow(s, t, total_nodes, capacity, adj, flows);

        if (current_flow == num_jobs)
        {
            // Success: It is possible to assign all jobs with max load 'mid'
            ans = mid;
            high = mid - 1; // Try to find a smaller max load
        }
        else
        {
            // Failure: Not enough capacity, machines are overloaded
            low = mid + 1;
        }
    }

    cout << "Minimized Max Load: " << ans << endl;

    return 0;
}