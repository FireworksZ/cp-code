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
vector<int> dfs_low,dfs_num,dfs_parent,articulation_vertex;
int dfsNumberCounter,dfsRoot;
int rootChildren;
vector<vector<ii> > adj;

void articulationPointAndBridge(int u) {
dfs_low[u] = dfs_num[u] = dfsNumberCounter++; // dfs_low[u] <= dfs_num[u]
for (int j = 0; j < (int)adj[u].size(); j++) {
	ii v = adj[u][j];
	if (dfs_num[v.first] == UNVISITED) { // a tree edge
			dfs_parent[v.first] = u;
			if (u == dfsRoot) rootChildren++; // special case if u is a root
			articulationPointAndBridge(v.first);
			if (dfs_low[v.first] >= dfs_num[u]) // for articulation point
				articulation_vertex[u] = true; // store this information first
			if (dfs_low[v.first] > dfs_num[u]) // for bridge
				printf(" Edge (%d, %d) is a bridge\n", u, v.first);
			dfs_low[u] = min(dfs_low[u], dfs_low[v.first]); // update dfs_low[u]
	}
	else if (v.first != dfs_parent[u]) // a back edge and not direct cycle
		dfs_low[u] = min(dfs_low[u], dfs_num[v.first]); // update dfs_low[u]
	}
}

int main(){
	int n;
	cin>>n;
	dfsNumberCounter = 0; dfs_num.assign(n, UNVISITED); dfs_low.assign(n, 0);
	dfs_parent.assign(n, 0); articulation_vertex.assign(n, 0);
	printf("Bridges:\n");
	for (int i = 0; i < n; i++)
		if (dfs_num[i] == UNVISITED) {
			dfsRoot = i; rootChildren = 0;
			articulationPointAndBridge(i);
			articulation_vertex[dfsRoot] = (rootChildren > 1); } // special case
	printf("Articulation Points:\n");
	for (int i = 0; i < n; i++)
		if (articulation_vertex[i])
			printf(" Vertex %d\n", i);
	return 0;

}
