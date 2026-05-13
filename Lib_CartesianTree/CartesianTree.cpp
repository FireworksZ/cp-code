#include "bits/stdc++.h"

using namespace std;

#define fastio ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0)

typedef long long ll;
typedef unsigned long long ull;

typedef pair<int,int> ii;
typedef pair<ii,ii> iiii;
typedef pair<ll,int> li;
typedef pair<int,ii> iii;
typedef pair<ii,int> iii2;
typedef pair<ll,ll> pll;
typedef pair<int,ll> il;

ll mod = 998244353ll;

const ll large = 1000000000000000000LL;
const int int_large = 1e9;

int __test_case = 1;


// prepare


void __prepare(){
//	cin>>__test_case;

}


//begin

constexpr int N = 100000 + 10, INF = 0x3f3f3f3f;

struct node {
  int idx, val, par, ch[2];

  friend bool operator<(node a, node b) { return a.idx < b.idx; }

  void init(int _idx, int _val, int _par) {
    idx = _idx, val = _val, par = _par, ch[0] = ch[1] = 0;
  }
} tree[N];

int cartesian_build(int n) {  // 建树，满足小根堆性质
  tree[0].init(0, -INF, 0); // sentinel: smaller than any input so climb terminates
  for (int i = 1; i <= n; i++) {
    int k = i - 1;
    while (tree[k].val > tree[i].val) k = tree[k].par;
    tree[i].ch[0] = tree[k].ch[1];
    tree[k].ch[1] = i;
    tree[i].par = k;
    tree[tree[i].ch[0]].par = i;
  }
  return tree[0].ch[1];
}


void solve(){




}



int main(){
	fastio;

	__prepare();

	while(__test_case--){
		solve();
	}

	return 0;
}
