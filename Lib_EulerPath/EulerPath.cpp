/*
 * ET.cpp
 *
 *  Created on: Oct 6, 2016
 *      Author: SCE15-0683
 */

#include<iostream>
#include<cstdio>
#include<vector>
#include<algorithm>
#include<list>

using namespace std;
typedef pair<int,int> ii;
vector<vector<ii> > adj;
vector<int> pos;
//note that cyc stores the reverse order of Euler path
list<ii> path;
void EulerPath(list<ii>::iterator i,int u){
	while(pos[u]<(int)adj[u].size()){
		int v = adj[u][pos[u]].first;
		int ind = adj[u][pos[u]].second;
		if(v==-1){
			pos[u]++;
			continue;
		}
		adj[v][ind].first=-1;
		adj[u][pos[u]].first=-1;
		pos[u]++;
		EulerPath(path.insert(i,ii(u,v)),v);
	}
}


int main(){
	int n;
	cin>>n;
	path.clear();
	//build adj list; each pair store ii(node_id, position in array adj[u])
	//init pos;
	int start; //pick the start point;
	EulerPath(path.begin(),start);
	//!!!Reverse path for actual route!!!

	return 0;
}


