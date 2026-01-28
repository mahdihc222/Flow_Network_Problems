/* QUESTION 12: The "Table Seating"
Determine if it is possible to seat N families at K tables such that 
no two members of the same family sit at the same table.
*/

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <numeric>

using namespace std;

const int INF = 1e9;

struct Edge {
    int v, cap, flow, rev;
};

vector<vector<Edge>> adj;
vector<pair<int, int>> parent;

void add_edge(int u, int v, int cap) {
    Edge a = {v, cap, 0, (int)adj[v].size()};
    Edge b = {u, 0, 0, (int)adj[u].size()};
    adj[u].push_back(a);
    adj[v].push_back(b);
}

int bfs(int s, int t) {
    fill(parent.begin(), parent.end(), make_pair(-1, -1));
    parent[s] = {-2, -2};
    queue<pair<int, int>> q;
    q.push({s, INF});

    while (!q.empty()) {
        int u = q.front().first;
        int flow = q.front().second;
        q.pop();

        for (int i = 0; i < adj[u].size(); i++) {
            Edge &e = adj[u][i];
            if (parent[e.v].first == -1 && e.cap - e.flow > 0) {
                parent[e.v] = {u, i};
                int new_flow = min(flow, e.cap - e.flow);
                if (e.v == t) return new_flow;
                q.push({e.v, new_flow});
            }
        }
    }
    return 0;
}

int max_flow(int s, int t) {
    int flow = 0, new_flow;
    while (new_flow = bfs(s, t)) {
        flow += new_flow;
        int cur = t;
        while (cur != s) {
            int prev = parent[cur].first;
            int idx = parent[cur].second;
            adj[prev][idx].flow += new_flow;
            int rev = adj[prev][idx].rev;
            adj[cur][rev].flow -= new_flow;
            cur = prev;
        }
    }
    return flow;
}

int main() {
    int NumFamilies, NumTables;
    if (!(cin >> NumFamilies >> NumTables)) return 0;

    vector<int> familySize(NumFamilies + 1);
    long long totalPeople = 0;
    for(int i=1; i<=NumFamilies; i++) {
        cin >> familySize[i];
        totalPeople += familySize[i];
    }

    vector<int> tableCap(NumTables + 1);
    for(int i=1; i<=NumTables; i++) cin >> tableCap[i];

    /* EXPLANATION:
    We construct a flow network where:
    1. Source -> Each Family_i (Capacity = Family Size). This ensures all members are accounted for.
    2. Each Family_i -> Each Table_j (Capacity = 1). This is the key constraint: "1" means at most 
       one member from Family_i can sit at Table_j.
    3. Each Table_j -> Sink (Capacity = Table Capacity). This ensures the table isn't overcrowded.
    If Max Flow equals the Total Number of People, a valid seating arrangement exists.
    */

    int s = 0, t = NumFamilies + NumTables + 1;
    adj.assign(t + 1, vector<Edge>());
    parent.resize(t + 1);

    // Source -> Families
    for(int i=1; i<=NumFamilies; i++) {
        add_edge(s, i, familySize[i]);
        
        // Family -> Tables (Constraint: 1 per table)
        for(int j=1; j<=NumTables; j++) {
            add_edge(i, NumFamilies + j, 1);
        }
    }

    // Tables -> Sink
    for(int j=1; j<=NumTables; j++) {
        add_edge(NumFamilies + j, t, tableCap[j]);
    }

    if (max_flow(s, t) == totalPeople) {
        cout << "Valid Seating Possible" << endl;
    } else {
        cout << "Impossible" << endl;
    }

    return 0;
}