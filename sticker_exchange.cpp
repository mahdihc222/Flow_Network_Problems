#include <bits/stdc++.h>
using namespace std;

const int INF = 1e9;

// --- YOUR TEMPLATE BFS ---
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

// --- YOUR TEMPLATE MAXFLOW ---
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

// Structure to hold counts
struct Person
{
    map<int, int> counts;
};

int main()
{
    int n_friends, m_stickers;
    if (!(cin >> n_friends >> m_stickers))
        return 0;

    // --- 1. Parsing Input ---
    Person me;
    vector<Person> friends(n_friends);

    // Read My Stickers
    int my_count;
    cin >> my_count;
    for (int i = 0; i < my_count; i++)
    {
        int s_id;
        cin >> s_id;
        me.counts[s_id]++;
    }

    // Read Friends' Stickers
    for (int i = 0; i < n_friends; i++)
    {
        int f_count;
        cin >> f_count;
        for (int j = 0; j < f_count; j++)
        {
            int s_id;
            cin >> s_id;
            friends[i].counts[s_id]++;
        }
    }

    // --- 2. Node Mapping ---
    // Source: 0
    // Stickers: 1 to m_stickers
    // Friends: m_stickers + 1 to m_stickers + n_friends
    // Sink: m_stickers + n_friends + 1

    int s = 0;
    int sticker_start = 1;
    int friend_start = m_stickers + 1;
    int t = m_stickers + n_friends + 1;
    int total_nodes = t + 1;

    vector<vector<int>> capacity(total_nodes, vector<int>(total_nodes, 0));
    vector<vector<int>> adj(total_nodes);
    vector<vector<int>> flows(total_nodes, vector<int>(total_nodes, 0));

    // Helper to add edges properly for your template
    auto add_edge = [&](int u, int v, int cap)
    {
        if (cap > 0)
        {
            adj[u].push_back(v);
            adj[v].push_back(u); // Reverse edge for residual
            capacity[u][v] = cap;
        }
    };

    // --- 3. Graph Construction ---

    // A. Source -> Sticker (My Supply)
    // Capacity = How many of this sticker I have.
    for (int i = 1; i <= m_stickers; i++)
    {
        if (me.counts[i] > 0)
        {
            add_edge(s, sticker_start + (i - 1), me.counts[i]);
        }
    }

    // B. Sticker -> Friend (Trading Opportunity: GIVE)
    // Capacity = 1 (We give 1 at a time).
    // Condition: Friend must NOT have this sticker.
    for (int i = 1; i <= m_stickers; i++)
    {
        for (int f = 0; f < n_friends; f++)
        {
            if (friends[f].counts[i] == 0)
            {
                int u = sticker_start + (i - 1);
                int v = friend_start + f;
                add_edge(u, v, 1);
            }
        }
    }

    // C. Friend -> Sticker (Trading Opportunity: RECEIVE)
    // Capacity = Duplicates available (Count - 1).
    // Condition: Friend must have > 1 of this sticker.
    for (int f = 0; f < n_friends; f++)
    {
        for (auto const &[sticker_id, count] : friends[f].counts)
        {
            if (count > 1)
            {
                int u = friend_start + f;
                int v = sticker_start + (sticker_id - 1);
                add_edge(u, v, count - 1);
            }
        }
    }

    // D. Sticker -> Sink (Collection Goal)
    // Capacity = 1 (I only care about having 1 distinct copy).
    for (int i = 1; i <= m_stickers; i++)
    {
        add_edge(sticker_start + (i - 1), t, 1);
    }

    // --- 4. Compute Max Flow ---
    int result = maxflow(s, t, total_nodes, capacity, adj, flows);
    cout << result << endl;

    // --- 5. Print Exchanges ---
    // We look at the flow passing through Friend Nodes.
    // If flow enters Friend from Sticker A, and leaves Friend to Sticker B,
    // that represents an exchange of A for B.

    for (int f = 0; f < n_friends; f++)
    {
        int f_node = friend_start + f;

        vector<int> given;    // Stickers I gave to friend (Sticker -> Friend)
        vector<int> received; // Stickers friend gave me (Friend -> Sticker)

        // Iterate all stickers to check interactions with this friend
        for (int i = 1; i <= m_stickers; i++)
        {
            int s_node = sticker_start + (i - 1);

            // Check if I gave sticker i
            if (flows[s_node][f_node] > 0)
            {
                given.push_back(i);
            }
            // Check if I received sticker i
            if (flows[f_node][s_node] > 0)
            {
                received.push_back(i);
            }
        }

        // Pair them up 1:1
        int exchanges = min(given.size(), received.size());
        for (int k = 0; k < exchanges; k++)
        {
            cout << "Exchange a sticker with number " << given[k]
                 << " against a sticker with number " << received[k]
                 << " of friend " << (f + 1) << endl;
        }
    }

    return 0;
}