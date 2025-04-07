#include "bits/stdc++.h"

using namespace std;

#define fastio ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0)
#define double long double

typedef long long ll;
typedef unsigned long long ull;

typedef pair<int,int> ii;
typedef pair<ii,ii> iiii;
typedef pair<ll,int> li;
typedef pair<int,ii> iii;
typedef pair<ii,int> iii2;
typedef pair<ll,ll> pll;
typedef pair<int,ll> il;

ll mod = 1000000007;

const ll large = 1000000000000000000LL;
const int int_large = 1e9;

//begin

vector<long long> fast_subset_convolution(vector<long long> a,int sz){
	for(int b=0;b<sz;b++){
		for(int i=0;i<(1<<sz);i++){
			if(i&(1<<b)){
				a[i]+=a[i^(1<<b)];
				a[i]%=mod;
			}
		}
	}
	return a;
}

void solve(){

	int sz;
	cin>>sz;
	int l = (1<<sz);
	vector<ll> a(l,1);

	a = fast_subset_convolution(a,sz);
	for(int i=0;i<l;i++) cout<<a[i]<<" ";
	cout<<endl;


}


int main(){
	fastio;

	int t=1;

	cin>>t;

	while(t--){
		solve();
	}

	return 0;
}
