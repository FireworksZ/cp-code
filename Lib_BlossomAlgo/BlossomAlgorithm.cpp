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



int n,m;
vector<vector<int> > adj;
vector<int> match,belong,pt,tp;
vector<int> used;
int tim = 0;

queue<int> q;

int find(int u){
	if(belong[u]==u) return u;
	return belong[u] = find(belong[u]);
}

int lca(int u,int v){
	tim++;
	while(true){
		u = find(u);
		used[u] = tim;
		if(match[u]==-1) break;
		u = pt[match[u]];
	}
	while(true){
		v = find(v);
		if(used[v]==tim) return v;
		v = pt[match[v]];
	}
}

void change(int x,int y,int r){
	while(find(y)!=r){
		int z = match[y];
		pt[y] = x;
		if(!tp[z]){
			tp[z] = 1;
			q.push(z);
		}
		belong[z] = r;
		belong[y] = r;
		x = z;
		y = pt[z];
	}
}

int find_path(int rt){
	while(!q.empty()) q.pop();
	tp.assign(n,0);
	belong.assign(n,-1);
	used.assign(n,0);
	tim = 0;
	for(int i=0;i<n;i++) belong[i] = i;
	pt.assign(n,-1);
	tp[rt] = 1;
	q.push(rt);

	while(!q.empty()){
		int u = q.front();
		q.pop();
		for(int j=0;j<(int)adj[u].size();j++){
			int v = adj[u][j];
			if(find(u)==find(v)||match[u]==v) continue;
			if(tp[v]){
				int r = lca(u,v);
				change(u,v,r);
				change(v,u,r);
			}else if(pt[v]==-1){
				pt[v] = u;
				if(match[v]==-1) return v;
				if(tp[match[v]]==0){
					tp[match[v]] = 1;
					q.push(match[v]);
				}
			}

		}
	}
	return -1;
}


void process(){
	match.assign(n,-1);
	for(int i=0;i<n;i++){
		if(match[i] == -1){
			int v = find_path(i);
			while(v!=-1){
				int pv = pt[v];
				int ppv = match[pv];
				match[v] = pv;
				match[pv] = v;
				v = ppv;
			}
		}
	}

}

//uoj79
int main(){

	cin>>n>>m;
	adj.assign(n,vector<int>());
	for(int i=0;i<m;i++){
		int x,y;
		cin>>x>>y;
		x--;y--;
		adj[x].push_back(y);
		adj[y].push_back(x);
	}

	process();

	int cnt = 0;
	for(int i=0;i<n;i++) if(match[i]>i) cnt++;
	cout<<cnt<<endl;
	for(int i=0;i<n;i++) cout<<match[i]+1<<" ";
	cout<<endl;



    return 0;
}
