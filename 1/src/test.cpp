#include<bits/stdc++.h>

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
typedef pair<long long,ii> lii;
typedef unsigned long long ull;

long long mod = 1000000007LL;
const long long large = 2000000000000000000LL;

int n,m;
vector<vector<int> > adj;
vector<vector<int> > pa;
vector<int> lv,lb,rb;
int rp = 0;
int sz = 20;
void dfs(int u,int parent,int lvl){
	pa[u][0] = parent;
	lv[u] = lvl;
	lb[u] = rp++;
	for(int j=0;j<(int)adj[u].size();j++){
		int v = adj[u][j];
		if(v==parent) continue;
		dfs(v,u,lvl+1);
	}
	rb[u] = rp-1;
}

int lca(int x,int y){
	if(lv[x]<lv[y]) swap(x,y);
	int g = lv[x]-lv[y];
	for(int j=sz-1;j>=0;j--){
		if((g>>j)&1) x = pa[x][j];
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

struct path{
	int l;int r;long long v;
};
vector<vector<path> > qu;



vector<long long> addv;
int ul,ur;
long long uv;
void update(int o,int l,int r){
	if(ul<=l&&ur>=r){
		addv[o]+=uv;
	}else{
		int m = (l+r)/2;
		if(ul<=m) update(o*2,l,m);
		if(ur>m) update(o*2+1,m+1,r);
	}
}

int qp;
long long qv;
void query(int o,int l,int r,long long add){
	if(l==r){
		qv = addv[o]+add;
	}else{
		int m = (l+r)/2;
		if(qp<=m) query(o*2,l,m,add+addv[o]);
		if(qp>m) query(o*2+1,m+1,r,add+addv[o]);
	}
}

long long getv(int u){
	if(u==-1) return 0;
	qp = lb[u];
	qv = 0;
	query(1,0,n-1,0);
	return qv;
}

long long ans = 0;
void dfs2(int u,int parent){

	for(int j=0;j<(int)adj[u].size();j++){
		int v = adj[u][j];
		if(v==parent) continue;
		dfs2(v,u);
	}
	for(int j=0;j<(int)qu[u].size();j++){
		path p = qu[u][j];
		long long cv = getv(p.l)+getv(p.r)-getv(u)-getv(pa[u][0]);
		if(cv<p.v){
			ans+=p.v-cv;
			ul = lb[u];
			ur = rb[u];
			uv = p.v-cv;
			update(1,0,n-1);
		}
	}

}


int main(){

    ios::sync_with_stdio(false);


    freopen("input.txt","r",stdin);
//    freopen("output.txt","w",stdout);

    int t;
    cin>>t;
    while(t--){
    	cin>>n>>m;

    	adj.assign(n,vector<int>());
    	for(int i=0;i<n-1;i++){
    		int x,y;
    		cin>>x>>y;
    		x--;y--;
    		adj[x].push_back(y);
    		adj[y].push_back(x);
    	}
    	pa.assign(n,vector<int>(sz,-1));
    	lv.assign(n,-1);
    	rp = 0;
    	lb = rb = lv;
    	dfs(0,-1,0);

    	for(int j=1;j<sz;j++){
    		for(int i=0;i<n;i++){
    			if(pa[i][j-1]!=-1){
    				pa[i][j] = pa[pa[i][j-1]][j-1];
    			}
    		}
    	}
//    	cout<<"!"<<endl;
    	qu.assign(n,vector<path>());
    	for(int i=0;i<m;i++){
    		int l;
    		cin>>l;
    		l--;
    		for(int j=0;j<3;j++){
    			int r;
    			long long v;
    			cin>>r>>v;
    			r--;
    			v = 1000000LL*(m+1)-v;
    			path p;
    			p.l = l;
    			p.r = r;
    			p.v = v;
    			qu[lca(l,r)].push_back(p);
    		}
    	}
    	addv.assign(4*n+10,0);
    	ans = 0;
    	dfs2(0,-1);
//    	if(n>100000) continue;
//    	cout<<ans<<endl;
    	if(ans<1000000LL*m*m) cout<<-1<<endl;
    	else cout<<1000000LL*(m+1)*m-ans<<endl;
//    	cout<<n<<" "<<m<<endl;

    }



    return 0;
}
