/*
 * Dinic.cpp
 *
 *  Created on: Nov 28, 2015
 *      Author: Zhao Huangjie
 */

#include<iostream>
#include<cstdio>
#include<vector>
#include<algorithm>
#include<queue>

using namespace std;


struct Edge {
	int from,to,cap,flow;
};

struct Dinic {
	int n,m,s,t;
	vector<Edge> edges;
	vector<vector<int> > G;
	vector<bool> vis;
	vector<int> dis;
	vector<int> cur;
	void init(int maxn){
		edges.clear();
		G.assign(maxn+10,vector<int>(0));
		vis.assign(maxn+10,false);
		dis.assign(maxn+10,0);
		cur.assign(maxn+10,0);
		n=maxn;
	}
	void addEdge(int from,int to,int cap){
		edges.push_back((Edge){from,to,cap,0});
		edges.push_back((Edge){to,from,0,0});
		m=edges.size();
		G[from].push_back(m-2);
		G[to].push_back(m-1);
	}
	bool BFS() {
		vis.assign(n+10,false);
		queue<int> q;
		q.push(s);
		dis[s]=0;
		vis[s]=true;
		while(!q.empty()){
			int u=q.front();
			q.pop();
			for(int i=0;i<(int)G[u].size();i++){
				Edge& e=edges[G[u][i]];
				if(!vis[e.to]&&e.cap>e.flow){
					vis[e.to]=1;
					dis[e.to]=dis[u]+1;
					q.push(e.to);
				}
			}
		}
		return vis[t];
	}
	int DFS(int x,int a){
		if(x==t||a==0) return a;
		int flow=0,f;
		for(int& i=cur[x];i<(int)G[x].size();i++){
			Edge& e=edges[G[x][i]];
			if(dis[x]+1==dis[e.to]&&(f=DFS(e.to,min(a,e.cap-e.flow)))>0){
				e.flow+=f;
				edges[G[x][i]^1].flow-=f;
				flow+=f;
				a-=f;
				if(a==0) break;
			}
		}
		return flow;
	}
	int maxflow(int s,int t){
		this->s=s;
		this->t=t;
		int flow=0;
		while(BFS()){
			cur.assign(n+10,0);
			flow+=DFS(s,1000000000);
		}
		return flow;
	}
	void clearflow(){
		vis.assign(n+10,false);
		dis.assign(n+10,0);
		cur.assign(n+10,0);
		for(int i=0;i<(int)edges.size();i++){
			edges[i].flow=0;
		}
	}
};

//feasible flow problem:
//For a graph with no source and no sink,
//each node have outgoing flow - incoming flow = bi.
//Summation of bi = 0.
//calculate whether there is a feasible flow the satisfy the condition.
//Solution:
//creat source st and source en.
//for bi > 0, create edge from st to i with cap |bi|
//for bi < 0, create edge from i to en with cap |bi|
//run maxflow algo
//if all added edge have full load, then the flow is a feasible flow.
//otherwise there is no feasible flow.



int main(){
	/*int n;
	cin>>n;
	Dinic d;
	d.ini(n);*/



	return 0;
}

