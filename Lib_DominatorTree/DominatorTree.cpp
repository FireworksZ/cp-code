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


vector<vector<int> > adj,pre,bkt;
vector<int> fo,vo;
vector<int> dfsl,id,pa,sdom,idom;

int n,m;

int find(int u){
	if(fo[u]==u) return u;
	find(fo[u]);
	if(id[sdom[vo[fo[u]]]]<id[sdom[vo[u]]]) vo[u] = vo[fo[u]];
	fo[u] = fo[fo[u]];
	return fo[u];
}

int eval(int u){
	find(u);
	return vo[u];
}
int cnt = 0;
void dfs(int u,int parent){
	sdom[u] = u;
	dfsl[cnt] = u;
	id[u] = cnt;
	pa[u] = parent;
	cnt++;
	for(int j=0;j<(int)adj[u].size();j++){
		int v = adj[u][j];
		if(id[v]==-1){
			dfs(v,u);
		}
	}
}

void process(int start){
	fo.assign(n,0);
	vo = fo;
	for(int i=0;i<n;i++) vo[i] = fo[i] = i;
	pa.assign(n,-1);
	sdom = idom = id = dfsl = pa;
	pre.assign(n,vector<int>());
	for(int i=0;i<n;i++){
		for(int j=0;j<(int)adj[i].size();j++){
			int v = adj[i][j];
			pre[v].push_back(i);
		}
	}
	bkt.assign(n,vector<int>());
	cnt = 0;

	dfs(start,-1);
	for(int i=cnt-1;i>=1;i--){
		int u = dfsl[i];
		for(int j=0;j<(int)pre[u].size();j++){
			int v = pre[u][j];
			if(id[v]!=-1) if(id[sdom[eval(v)]]<id[sdom[u]]) sdom[u] = sdom[eval(v)];
		}
		bkt[sdom[u]].push_back(u);
		int fa = pa[u];
		for(int j=0;j<(int)bkt[fa].size();j++){
			int v = bkt[fa][j];
			int pv = eval(v);
			idom[v] = (sdom[pv]==fa?fa:pv);
		}
		bkt[fa].clear();
		fo[u] = fa;

	}

	for(int i=1;i<cnt;i++){
		int u = dfsl[i];
		idom[u] = (idom[u]==sdom[u]?idom[u]:idom[idom[u]]);
	}
}

//hdu4694
int main(){

	while(cin>>n>>m){
		adj.assign(n,vector<int>());
		for(int i=0;i<m;i++){
			int x,y;
			cin>>x>>y;
			x--;y--;
			adj[x].push_back(y);
		}

		process(n-1);

		vector<long long> sum(n,0);
		for(int i=0;i<n;i++) if(id[i]!=-1) sum[i] = i+1;
		for(int i=1;i<cnt;i++){
			int u = dfsl[i];
			sum[u]+=sum[idom[u]];
		}
		for(int i=0;i<n;i++){
			if(i) cout<<" ";
			cout<<sum[i];
		}
		cout<<endl;

	}




    return 0;
}
