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
long long mod = 1000000007LL;
long long large = 2000000000000000000LL;

const double eps = 1e-10;
void norm(double &a){
	if(fabs(a)<eps) a = 0;
}

//standard form minimize cx s.t. Ax<=b,x>=0
//full tableau implementation
//lexicographic pivoting rule for anti-cycling

struct Simplex{
	int n,m;
	vector<vector<double> > ta;
	vector<int> B;
	//n, m, ta, B changes through the process
	//ta is tableau with m+1 rows and n+1 cols, refer to Linear Optimization page 99
	//B[i-1] store the basic variable belongs to row i in tableau(do note about the index)

	vector<vector<double> > a;
	vector<double> b;
	vector<double> c;

	Simplex(vector<vector<double> > aa,vector<double> bb,vector<double> cc):a(aa),b(bb),c(cc){
		m = (int)b.size();
		n = (int)c.size();
		assert((int)a.size()==m);
		for(int i=0;i<m;i++) assert((int)a[i].size()==n);
		//convert to standard form Ax = b, x>=0
		for(int i=0;i<m;i++) c.push_back(0);
		for(int i=0;i<m;i++){
			for(int j=0;j<m;j++) a[i].push_back(0);
			a[i][n+i] = 1;
		}
		n+=m;
	}



	void pivot(int r,int s){
		double inv = 1/ta[r][s];
		for(int j=0;j<=n;j++){
			ta[r][j]*=inv;
		}
		for(int i=0;i<=m;i++){
			if(i!=r){
				double co = ta[i][s];
				for(int j=0;j<=n;j++){
					ta[i][j]-=co*ta[r][j];
					norm(ta[i][j]);
				}
			}
		}
		B[r-1] = s-1;
	}
	void check(){
		vector<double> x(n,0);
		for(int i=0;i<m;i++){
			x[B[i]] = ta[i+1][0];
		}
		for(int i=0;i<m;i++){
			double re = 0;
			for(int j=0;j<n-m;j++) re+=a[i][j]*x[j];
			re+=x[n-m+i];
			cout<<b[i]-re<<" ";
		}
		cout<<endl;
	}
	bool phase2(){
		m = (int)ta.size();
		n = (int)ta[0].size();
		n--;m--;
		while(true){
			int s = -1;
			for(int j=1;j<=n;j++){
				if(s==-1||ta[0][j]<ta[0][s]){
					s = j;
				}
			}
			if(ta[0][s]>-eps) return true;
			int r = -1;
			for(int i=1;i<=m;i++){
				if(ta[i][s]>eps){
					if(r==-1) r = i;
					else{
						for(int j=0;j<=n;j++){
							if(ta[i][j]/ta[i][s]<ta[r][j]/ta[r][s]){
								r = i;
							}else if(ta[i][j]/ta[i][s]>ta[r][j]/ta[r][s]) break;
						}
					}
				}
			}
			if(r==-1) return false;
			pivot(r,s);
		}
	}

	bool phase1(){
		for(int i=0;i<m;i++){
			if(b[i]<0){
				b[i] = -b[i];
				for(int j=0;j<n;j++) a[i][j] = -a[i][j];
			}
		}
		ta.assign(m+1,vector<double>(n+m+1,0));
		for(int i=1;i<=m;i++){
			for(int j=1;j<=n;j++){
				ta[i][j] = a[i-1][j-1];
			}
			ta[i][n+i] = 1;
		}
		B.assign(m,0);
		for(int i=0;i<m;i++) B[i] = n+i;
		for(int i=1;i<=m;i++){
			ta[i][0] = b[i-1];
			ta[0][0]+= -b[i-1];
		}
		for(int j=1;j<=n;j++){
			for(int i=1;i<=m;i++){
				ta[0][j]-=ta[i][j];
			}
		}
		n+=m;

		phase2();
		if(-ta[0][0]>0) return false;

		while(true){
			bool flag = false;
			for(int i=1;i<=m;i++) if(B[i-1]>=(int)c.size()){
				flag = true;
				int s = -1;
				for(int j=1;j<=(int)c.size();j++){
					if(ta[i][j]!=0){
						s = j;
						break;
					}
				}
				if(s==-1){ //find redundant equality constraint and therefore removed
					ta.erase(ta.begin()+i);
					B.erase(B.begin()+i-1);
					m = (int)ta.size()-1;
				}else{
					pivot(i,s);
				}
				break;
			}
			if(!flag) break;
		}
		n = (int)c.size(); //No. of initial variables
		while((int)ta[0].size()>n+1){
			for(int i=0;i<=m;i++){
				ta[i].pop_back();
			}
		}
		return true;
	}
	// phase 1 find initial basic feasible solution (BFS), phase 2 get the optimal solution
	// OUTPUT: value of the optimal solution (infinity if unbounded
	//         below(nan), inf if infeasible, since it is a minimization)
	double Solve(vector<double> &x){
		if(!phase1()) return numeric_limits<double>::infinity();
		for(int j=1;j<=n;j++){
			ta[0][j]+=c[j-1];
			for(int i=1;i<=m;i++) ta[0][j]-=c[B[i-1]]*ta[i][j];
		}
		for(int i=1;i<=m;i++) ta[0][0]-=c[B[i-1]]*ta[i][0];
		if(!phase2()) return -numeric_limits<double>::infinity();
		x.assign(n,0);
		for(int i=0;i<m;i++) x[B[i]] = ta[i+1][0];
		return -ta[0][0];
	}

};






int main(){

	vector<vector<double> > a;
	vector<double> b,c;
	int n,m;
	cin>>n>>m;
	c.assign(n,0);
	for(int i=0;i<n;i++) cin>>c[i];
	b.assign(m,0);
	for(int i=0;i<m;i++) cin>>b[i];
	a.assign(m,vector<double>(n,0));
	for(int i=0;i<m;i++) for(int j=0;j<n;j++) cin>>a[i][j];
	for(int i=0;i<m;i++){
		for(int j=0;j<n;j++) a[i][j] = -a[i][j];
		b[i] = -b[i];
	}


	Simplex d(a,b,c);
	//d.phase1();

	vector<double> x;
	cout<<setprecision(10)<<fixed;
	cout<<d.Solve(x)<<endl;


    return 0;
}
