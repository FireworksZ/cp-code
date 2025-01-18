#include "bits/stdc++.h"

using namespace std;

#define fastio ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0)
#define double long double

typedef long long ll;
typedef unsigned long long ull;

typedef pair<int,int> ii;

long long mod = 1000000007LL;
const long long large = 4000000000000000000LL;

struct SAT2{
	int n;
	vector<vector<int> > adj;

	void init(int n){
		this->n=n;
		adj.assign(n,vector<int>());
	}

	int term(int x,int j){
		return 2*x+j;
	}
	int neg(int x){
		return x^1;
	}

	void add_clause(int x,int i,int y,int j){
		int tx = term(x,i);
		int ty = term(y,j);
		adj[neg(tx)].push_back(ty);
		adj[neg(ty)].push_back(tx);
	}
	vector<int> dfs_num,dfs_low,S,visited;
	vector<vector<int> > Scc;
	vector<int> bel;
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

	void get_SCC(){
		dfs_num.assign(n,-1);
		dfs_low.assign(n,0);
		visited.assign(n,0);
		for(int i=0;i<n;i++){
			if(dfs_num[i]==-1){
				tarjanSCC(i);
			}
		}
		bel.assign(n,-1);
		for(int i=0;i<(int)Scc.size();i++){
			for(int j=0;j<(int)Scc[i].size();j++){
				int u = Scc[i][j];
				bel[u] = i;
			}
		}
	}
	bool check(){
		for(int i=0;i<n;i+=2){
//			cout<<bel[i]<<" "<<bel[i+1]<<endl;
			if(bel[i]==bel[i+1])return false;
		}
		return true;
	}

	vector<int> val;
	vector<int> inq;
	vector<vector<int> > adj2;
	void gen(){
		inq.assign(numSCC,0);
		adj2.assign(numSCC,vector<int>());
		for(int i=0;i<n;i++){
			int u = bel[i];
			for(int j=0;j<(int)adj[i].size();j++){
				int v = bel[adj[i][j]];
				if(u==v) continue;
				inq[v]++;
				adj2[u].push_back(v);
			}
		}
		queue<int> q;
		for(int i=0;i<numSCC;i++) if(inq[i]==0) q.push(i);
		vector<int> tp;
		while(!q.empty()){
			int u = q.front();
			q.pop();
			tp.push_back(u);
			for(int j=0;j<(int)adj2[u].size();j++){
				int v = adj2[u][j];
				inq[v]--;
				if(inq[v]==0) q.push(v);
			}
		}
		val.assign(numSCC,-1);
		for(int i=0;i<(int)tp.size();i++){
			int u = tp[i];
			if(val[u]!=-1) continue;
			val[u] = 0;
			u=bel[neg(Scc[u][0])];
			val[u] = 1;
		}
//		for(int i=0;i<numSCC;i++) cout<<val[i]<<" ";
//		cout<<endl;

	}
};


int main(){
	fastio;



	return 0;
}

