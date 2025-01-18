#include<bits/stdc++.h>

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

long long mod = 1000000007LL;
long long large = 2000000000000000000LL;

typedef unsigned long long ull;

struct TarjanLCA{
	vector<int> fa;
	int n;
	vector<vector<int> > adj;
	vector<ii> q;
	vector<vector<ii> > qt;
	vector<int> re;

	vector<int> vis;
	TarjanLCA(int n,vector<vector<int> > adj,vector<ii> q):n(n),adj(adj),q(q){}

	void dfs(int u,int parent){

		for(int j=0;j<(int)adj[u].size();j++){
			int v = adj[u][j];
			if(v==parent) continue;
			dfs(v,u);
			unionset(v,u);
		}
		vis[u] = 1;
		for(int j=0;j<(int)qt[u].size();j++){
			int t = qt[u][j].second;
			int v = qt[u][j].first;
			if(vis[v]) re[t] = gf(v);
		}
	}

	vector<int> get(){
		fa.assign(n,-1);
		for(int i=0;i<n;i++) fa[i] = i;
		int m = (int)q.size();
		qt.assign(n,vector<ii>());
		for(int i=0;i<m;i++){
			int u = q[i].first;
			int v = q[i].second;
			if(u!=-1&&v!=-1){
				qt[u].push_back(ii(v,i));
				qt[v].push_back(ii(u,i));
			}
		}
		vis.assign(n,0);
		re.assign(m,-1);
		dfs(0,-1);
		return re;
	}

	int gf(int x){
		return fa[x]==x?x:fa[x] = gf(fa[x]);
	}
	void unionset(int x,int y){
		x = gf(x);
		y = gf(y);
		fa[x] = y;
	}
};

int main(){

}
