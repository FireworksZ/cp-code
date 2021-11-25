/*
 * MCMF.cpp
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
typedef pair<int,int> ii;
typedef pair<long long,long long> ll;

long long large = 2000000000000000000LL;
struct Edge{
	int from,to;
	long long cap,flow,cost;
};

struct MCMF {
	int n,m,s,t;
	vector<Edge> edges;
	vector<vector<int> > G;
	vector<int> inq;
	vector<long long> dis;
	vector<int> p;
	vector<long long> a;

	void init(int maxn){
		n=maxn;
		G.assign(n+10,vector<int>(0));
		edges.clear();
		inq.assign(n+10,0);
		dis.assign(n+10,1000000000);
		p.assign(n+10,0);
		a.assign(n+10,0);
	}
	void addEdge(int from,int to,long long cap,long long cost){
		edges.push_back((Edge){from,to,cap,0,cost});
		edges.push_back((Edge){to,from,0,0,-cost});
		m=(int)edges.size();
		G[from].push_back(m-2);
		G[to].push_back(m-1);
	}
	vector<vector<int> > path;
	bool SPFA(int s,int t,long long& flow,long long& cost){
		dis.assign(n+10,large);
		inq.assign(n+10,0);
		dis[s]=0;
		inq[s]=1;
		p[s]=0;
		a[s]=large;
		//change the following spfa to DJ if needed
		queue<int> q;
		q.push(s);
		while(!q.empty()){
			int u=q.front();
			q.pop();
			inq[u]=0;
			for(int i=0;i<(int)G[u].size();i++){
				Edge& e=edges[G[u][i]];
				if(e.cap>e.flow&&dis[e.to]>dis[u]+e.cost){
					dis[e.to]=dis[u]+e.cost;
					p[e.to]=G[u][i];
					a[e.to]=min(a[u],e.cap-e.flow);
					if(!inq[e.to]){
						q.push(e.to);
						inq[e.to]=1;
					}
				}
			}
		}
		if(dis[t]==large) return false;
		flow+=a[t];
		cost+=dis[t]*a[t];
		int u=t;
		vector<int> pa;
		while(u!=s){
			pa.push_back(u);
			edges[p[u]].flow+=a[t];
			edges[p[u]^1].flow-=a[t];
			u=edges[p[u]].from;
		}
		pa.push_back(u);
		reverse(pa.begin(),pa.end());
		path.push_back(pa);
		return true;
	}
	vector<vector<int> > flow_decomposition(){
		//Care about the case that the flow edge is bidirectional
		vector<vector<int> > path;
		while(true){
			dis.assign(n,large);
			a.assign(n,large);
			queue<int> q;
			dis[s] = 0;
			p[s] = 0;
			q.push(s);
			while(!q.empty()){
				int u = q.front();
				q.pop();
				for(int j=0;j<(int)G[u].size();j++){
					Edge& e=edges[G[u][j]];
					if(e.flow>0&&e.cap>0){
						int v = e.to;
						if(dis[v]==large){
							q.push(v);
							dis[v] = dis[u]+1;
							p[v] = G[u][j];
							a[v] = min(a[u],e.flow);
						}
					}

				}
			}
			if(dis[t]==large) break;
			int u=t;
			vector<int> pa;
			while(u!=s){
				pa.push_back(u);
				edges[p[u]].flow-=a[t];
				edges[p[u]^1].flow+=a[t];
				u=edges[p[u]].from;
			}
			pa.push_back(u);
			reverse(pa.begin(),pa.end());
			path.push_back(pa);
		}
		return path;
	}
	ll mincost(int s,int t){
		long long flow=0,cost=0;
		this->s=s;
		this->t=t;
		while(SPFA(s,t,flow,cost));
		return ll(flow,cost);
	}
	void clearflow(){
		inq.assign(n+10,0);
		dis.assign(n+10,1000000000);
		p.assign(n+10,0);
		a.assign(n+10,0);
		for(int i=0;i<(int)edges.size();i++){
			edges[i].flow=0;
		}
	}
};

int main(){

	return 0;
}


