/*
 * test.cpp
 *
 *
 *      Author: Fireworks
 */

#include<iostream>
#include<cstdio>
#include<vector>
#include<algorithm>
#include<queue>
#include<string>
#include<map>
#include<cmath>
#include<bitset>
#include<set>
#include<iomanip>
#include<fstream>
#include<bitset>
#include<cstring>
#include<cstdlib>
#include<complex>
#include<list>
#include<sstream>
#include<cassert>   // assert

using namespace std;

typedef pair<int,int> ii;
typedef pair<int,long long> il;
typedef pair<long long,long long> ll;
typedef pair<ll,int> lli;
typedef pair<long long,int> li;
typedef pair<double,double> dd;
typedef pair<ii,int> iii;
typedef pair<double,int> di;
long long mod = 1000000007LL;
long long base = 10000007;
long long large = 1000000000000000000LL;

const int UNVISITED = -1;
vector<int> dfs_low,dfs_num,articulation_vertex;
int dfsNumberCounter,dfsRoot;
int rootChildren;
// adj[u] stores pairs of (neighbor, edge_id). Each undirected edge contributes two
// half-edges with consecutive ids 2k and 2k+1, so reverse(id) = id ^ 1.
vector<vector<ii> > adj;
int edge_count;

void addEdge(int u, int v) {
	adj[u].push_back(ii(v, edge_count));
	adj[v].push_back(ii(u, edge_count + 1));
	edge_count += 2;
}

void articulationPointAndBridge(int u, int parent_edge) {
dfs_low[u] = dfs_num[u] = dfsNumberCounter++; // dfs_low[u] <= dfs_num[u]
for (int j = 0; j < (int)adj[u].size(); j++) {
	ii v = adj[u][j];
	// skip only the specific edge we came in on; parallel edges to the parent are real back-edges
	if (v.second == (parent_edge ^ 1)) continue;
	if (dfs_num[v.first] == UNVISITED) { // a tree edge
			if (u == dfsRoot) rootChildren++; // special case if u is a root
			articulationPointAndBridge(v.first, v.second);
			if (dfs_low[v.first] >= dfs_num[u]) // for articulation point
				articulation_vertex[u] = true; // store this information first
			if (dfs_low[v.first] > dfs_num[u]) // for bridge
				printf(" Edge (%d, %d) is a bridge\n", u, v.first);
			dfs_low[u] = min(dfs_low[u], dfs_low[v.first]); // update dfs_low[u]
	}
	else // a back edge (or a parallel edge to parent, which IS a back-edge)
		dfs_low[u] = min(dfs_low[u], dfs_num[v.first]); // update dfs_low[u]
	}
}

int main(){
	int n;
	cin>>n;
	dfsNumberCounter = 0; dfs_num.assign(n, UNVISITED); dfs_low.assign(n, 0);
	articulation_vertex.assign(n, 0);
	adj.assign(n, vector<ii>()); edge_count = 0;
	// caller: read m edges and call addEdge(u, v) for each
	printf("Bridges:\n");
	for (int i = 0; i < n; i++)
		if (dfs_num[i] == UNVISITED) {
			dfsRoot = i; rootChildren = 0;
			// pass -1 as the initial parent_edge; (-1) ^ 1 = -2, which matches no real edge id
			articulationPointAndBridge(i, -1);
			articulation_vertex[dfsRoot] = (rootChildren > 1); } // special case
	printf("Articulation Points:\n");
	for (int i = 0; i < n; i++)
		if (articulation_vertex[i])
			printf(" Vertex %d\n", i);
	return 0;

}
