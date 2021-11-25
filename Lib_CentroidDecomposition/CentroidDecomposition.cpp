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
#include<sstream>
#include<cassert>
#include<iomanip>
#include<limits>

using namespace std;

//#define double long double

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
int mod = 1000000007LL;
long long large = 2000000000000000000LL;

vector<vector<int> > adj,pa;
vector<int> lv;
int sz = 20;
void dfs(int u,int parent,int lvl){
	lv[u] = lvl;
	pa[u][0] = parent;
	for(int j=0;j<(int)adj[u].size();j++){
		int v = adj[u][j];
		if(v!=parent){
			dfs(v,u,lvl+1);
		}
	}
}

int lca(int x,int y){
	if(lv[x]<lv[y]) swap(x,y);
	int g = lv[x]-lv[y];
	for(int j = sz-1;j>=0;j--){
		if(g&(1<<j)) x = pa[x][j];
	}
	if(x==y) return x;
	for(int j=sz-1;j>=0;j--){
		if(pa[x][j]!=pa[y][j]){
			x = pa[x][j];
			y = pa[y][j];
		}
	}
	return pa[x][0];
}

int dis(int x,int y){
	int r = lca(x,y);
	return lv[x]+lv[y]-2*lv[r];
}



vector<int> dp,pa1,cc;
vector<bool> er;

vector<vector<int> > adj1;

void dfs1(int u,int parent){
	pa1[u] = parent;
	cc.push_back(u);
	dp[u] = 1;
	for(int j=0;j<(int)adj[u].size();j++){
		int v = adj[u][j];
		if(er[v]) continue;
		if(v!=parent){
			dfs1(v,u);
			dp[u]+=dp[v];
		}
	}
}
int f(int st){
	cc.clear();
	dfs1(st,-1);
	int n = (int)cc.size();
	int cen = -1;
	for(int i=0;i<n;i++){
		int u = cc[i];
		int sz = 0;
		for(int j=0;j<(int)adj[u].size();j++){
			int v = adj[u][j];
			if(er[v]) continue;
			if(v!=pa1[u]){
				sz = max(sz,dp[v]);
			}
		}
		if(u!=st) sz = max(sz,n-dp[u]);
		if(sz<=n/2){
			cen = u;
			break;
		}
	}
	er[cen] = true;
	for(int j=0;j<(int)adj[cen].size();j++){
		int v = adj[cen][j];
		if(er[v]) continue;
		int re = f(v);
		adj1[cen].push_back(re);
		adj1[re].push_back(cen);
	}
	return cen;
}

vector<int> pa2;
void dfs2(int u,int parent){
	pa2[u] = parent;
	for(int j=0;j<(int)adj1[u].size();j++){
		int v = adj1[u][j];
		if(v!=parent) dfs2(v,u);
	}
}

//http://codeforces.com/contest/321/problem/E
//O(nlogn) build centroid tree
int main(){

	int n,m;
	cin>>n>>m;
	adj.assign(n,vector<int>());
	for(int i=0;i<n-1;i++){
		int x,y;
		scanf("%d%d",&x,&y);
		x--;y--;
		adj[x].push_back(y);
		adj[y].push_back(x);
	}
	pa.assign(n,vector<int>(sz,-1));
	lv.assign(n,-1);
	dfs(0,-1,0);
	for(int j=1;j<sz;j++){
		for(int i=0;i<n;i++){
			if(pa[i][j-1]!=-1){
				pa[i][j] = pa[pa[i][j-1]][j-1];
			}
		}
	}

	adj1.assign(n,vector<int>());
	dp.assign(n,-1);
	pa1.assign(n,-1);
	er.assign(n,false);
	int rt = f(0);
	pa2.assign(n,-1);
	dfs2(rt,-1);

	vector<int> ans(n,1000000000);

	int x = 0;
	int u = x;
	while(u!=-1){
		ans[u] = min(ans[u],dis(u,x));
		u = pa2[u];
	}

	while(m--){
		int t,x;
		scanf("%d%d",&t,&x);
		x--;
		if(t==1){
			int u = x;
			while(u!=-1){
				ans[u] = min(ans[u],dis(u,x));
				u = pa2[u];
			}
		}else{
			int re = 1000000000;
			int u = x;
			while(u!=-1){
				re = min(re,ans[u]+dis(u,x));
				u = pa2[u];
			}
			printf("%d\n",re);
		}
	}


    return 0;
}
