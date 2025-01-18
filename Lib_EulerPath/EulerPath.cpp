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


struct Euler_Path{

	int n;
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


	void init(int n){
		this->n = n;
		path.clear();
		adj.assign(n,vector<ii>());
		pos.assign(n,0);
	}

	void addEdge(int u,int v){
//		cout<<u+1<<" "<<v+1<<endl;
		if(u==v){
			int su = adj[u].size();
			adj[u].push_back(ii(u,su+1));
			adj[u].push_back(ii(u,su));
		}else{
			int su = adj[u].size();
			int sv = adj[v].size();
			adj[u].push_back(ii(v,sv));
			adj[v].push_back(ii(u,su));
		}

	}

	void compute(){
		EulerPath(path.begin(),0);

		cout<<path.size()<<endl;
		vector<ii> e;
		for(list<ii>::iterator it = path.begin();it!=path.end();it++){
			e.push_back(*it);
		}
		reverse(e.begin(),e.end());
		for(int i=0;i<(int)e.size();i++) cout<<e[i].first+1<<" ";
		cout<<e[0].first+1<<endl;
	}

};

int main(){


	return 0;
}


