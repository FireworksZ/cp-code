/*
 * scc.cpp
 *
 *  Created on: 2016Äê8ÔÂ3ÈÕ
 *      Author: Administrator
 */

#include<iostream>
#include<cstdio>
#include<vector>
#include<algorithm>

using namespace std;

vector<int> dfs_num,dfs_low,S,visited;
vector<vector<int> > adj;
vector<vector<int> > Scc;
int dfsNumberCounter = 0, numSCC = 0;
void tarjanSCC(int u){
	dfs_low[u]=dfs_num[u]=dfsNumberCounter++;
	S.push_back(u);
	visited[u]=1;
	for(int j=0;j<(int)adj[u].size();j++){
		int v = adj[u][j];
		if(dfs_num[v]==-1){
			tarjanSCC(v);
		}
		if(visited[v]==1){
			dfs_low[u]=min(dfs_low[u],dfs_low[v]);
		}
	}

	if(dfs_low[u]==dfs_num[u]){
		Scc.push_back(vector<int>(0,0));
		while(true){
			int v= S.back();
			S.pop_back();
			visited[v]=0;
			Scc[numSCC].push_back(v);
			if(u==v) break;
		}
		numSCC++;
	}
}

// remark for 2-SAT:
/*
 * To find truth assignment of 2-SAT
 * 1. process SCC and check if it can be satisfied, i.e., if a variable and its negate contains
 * 	  in same SCC, then the CNF can be satisfied.
 * 2. Then the SCCs form a DAG. process this DAG as reverse Topological order. choose the
 *    current one and erase the complement of current one and its previous SCCs.
 *
 * http://blog.csdn.net/pi9nc/article/details/11849843
 *
 */

int main(){
	int n;
	cin>>n;
	dfs_num.assign(n,-1);
	dfs_low.assign(n,0);
	visited.assign(n,0);
	for(int i=0;i<n;i++){
		if(dfs_num[i]==-1){
			tarjanSCC(i);
		}
	}
	return 0;
}



