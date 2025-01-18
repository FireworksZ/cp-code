#include<iostream>
#include<cstdio>
#include<vector>
#include<algorithm>
#include<queue>
#include<string>
#include<cstring>
#include<map>
#include<cmath>
#include<bitset>
#include<set>
#include<cstdlib>
#include<complex>
#include<list>
#include<cassert>
#include<iomanip>
#include<limits>

using namespace std;

#define double long double

typedef pair<int,int> ii;
typedef pair<int,long long> il;
typedef pair<long long,long long> ll;
typedef pair<ll,int> lli;
typedef pair<long long,int> li;
typedef pair<double,double> dd;
typedef pair<ii,int> iii;
typedef pair<double,int> di;
typedef pair<int,ii> iii2;
typedef pair<ii,ii> iiii;
typedef vector<int> vi;
long long mod = 1000000009LL;
long long large = 2000000000000000000LL;



struct MCBM{
	vector<vector<int> > adj;
	vector<int> dis,link,vis;
	int dist;
	int inf = 1000000000;
	int n,m;

	//init adj,n,m

	int bfs(){
		queue<int> q;
		dist = inf;
		dis.assign(n+m,-1);
		for(int i=0;i<n;i++){
			if(link[i]==-1){
				q.push(i);
				dis[i] = 0;
			}
		}

		while(!q.empty()){
			int u = q.front();
			q.pop();
			if(dis[u]>dist) break;
			for(int j=0;j<(int)adj[u].size();j++){
				int v = adj[u][j];
				if(dis[v]==-1){
					dis[v] = dis[u]+1;
					if(link[v]==-1) dist = dis[v];
					else{
						dis[link[v]] = dis[v]+1;
						q.push(link[v]);
					}
				}
			}
		}
		return dist!=inf;
	}

	int find(int u){
		for(int j=0;j<(int)adj[u].size();j++){
			int v = adj[u][j];
			if(!vis[v]&&dis[v]==dis[u]+1){
				vis[v] = 1;
				if(link[v]!=-1&&dis[v]==dist) continue;
				if(link[v]==-1||find(link[v])){
					link[u] = v;
					link[v] = u;
					return 1;
				}
			}
		}
		return 0;
	}

	int MaxMatch(){
		link.assign(n+m,-1);
		int ans = 0;
		while(bfs()){
			vis.assign(n+m,false);
			for(int i=0;i<n;i++) if(link[i]==-1) ans+=find(i);
		}
		return ans;
	}


};



//HDU2389
int main(){



    return 0;
}
