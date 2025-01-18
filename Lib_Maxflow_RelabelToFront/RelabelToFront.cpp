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


struct Edge {
	int from,to,cap,flow;
	Edge (int from,int to,int cap,int flow):from(from),to(to),cap(cap),flow(flow){}
};


struct Relabel_To_Front{
	int n,s,t;
	vector<Edge> edges;
	vector<vector<int> > G;
	vector<int> h;
	vector<int> excess;

	void init(int maxn){
		n = maxn;
		G.assign(n,vector<int>());
		edges.clear();
		h.assign(n,0);
		excess.assign(n,0);
	}

	void addEdge(int from,int to,int cap){
		edges.push_back(Edge(from,to,cap,0));
		edges.push_back(Edge(to,from,0,0));
		int m = (int)edges.size();
		G[from].push_back(m-2);
		G[to].push_back(m-1);
	}

	void push(int edgeid){
		int u = edges[edgeid].from;
		int v = edges[edgeid].to;
		int delta = min(excess[u],edges[edgeid].cap-edges[edgeid].flow);
		edges[edgeid].flow+=delta;
		edges[edgeid^1].flow-=delta;
		excess[u]-=delta;
		excess[v]+=delta;
	}

	void relabel(int u){
		h[u] = 2*n+10;
		for(int j=0;j<(int)G[u].size();j++){
			Edge &e = edges[G[u][j]];
			if(e.cap-e.flow>0){
				if(h[u]>h[e.to]+1){
					h[u] = h[e.to]+1;
				}else if(h[u]==h[e.to]+1){
				}
			}
		}
	}

	void init_flow(){
		for(int i=0;i<n;i++){
			h[i] = excess[i] = 0;
		}
		for(int i=0;i<(int)edges.size();i++) edges[i].flow = 0;
		h[s] = n;
		for(int j=0;j<(int)G[s].size();j++){
			Edge &e = edges[G[s][j]];
			if(e.cap==0) continue;
			e.flow = e.cap;
			edges[G[s][j]^1].flow-=e.flow;
			excess[e.to] += e.flow;
			excess[s]-=e.flow;

		}
	}

	void discharge(int u){
		int pt = 0;
		while(excess[u]>0){
			if(pt==(int)G[u].size()){
				relabel(u);
				pt = 0;
			}else{
				Edge& e = edges[G[u][pt]];
				if(e.cap>e.flow&&h[u]==h[e.to]+1){
					push(G[u][pt]);
				}else pt++;
			}
		}
	}

	int maxflow(int st,int en){
		s = st;
		t = en;

		init_flow();

		list<int> l;
		for(int i=0;i<n;i++){
			if(i==s||i==t) continue;
			l.push_back(i);
		}
		list<int>::iterator it = l.begin();
		while(it!=l.end()){
			int u = *it;
			int oldh = h[u];
			discharge(u);
			if(h[u]>oldh){
				l.erase(it);
				l.push_front(u);
				it = l.begin();
			}
			it++;
		}

		return excess[t];
	}
};

int main(){

	int n,m,s,t;
	cin>>n>>m>>s>>t;

	Relabel_To_Front rtf;
	rtf.init(n);
	for(int i=0;i<m;i++){
		int u,v,c;
		cin>>u>>v>>c;
		rtf.addEdge(u,v,c);
	}


	int f = rtf.maxflow(s,t);
	int cnt = 0 ;
	for(int i=0;i<(int)rtf.edges.size();i+=2){
		Edge e = rtf.edges[i];
		if(e.flow>0) cnt++;
	}
	cout<<n<<" "<<f<<" "<<cnt<<endl;
	for(int i=0;i<(int)rtf.edges.size();i+=2){
		Edge e = rtf.edges[i];
		if(e.flow>0) cout<<e.from<<" "<<e.to<<" "<<e.flow<<endl;
	}

    return 0;
}
