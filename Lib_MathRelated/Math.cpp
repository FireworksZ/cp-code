/*
 * Sieve.cpp
 *
 *  Created on: Oct 30, 2015
 *      Author: Zhao Huangjie
 */


#include<iostream>
#include<cstdio>
#include<vector>
#include<algorithm>
#include<cmath>
#include<bitset>
#include<map>
using namespace std;

int mod = 998244353;

struct mint {
	int x;
	inline mint(int o = 0) { x = o; }
	inline mint & operator = (int o) { return x = o, *this; }
	inline mint & operator += (mint o) { return (x += o.x) >= mod && (x -= mod), *this; }
	inline mint & operator -= (mint o) { return (x -= o.x) < 0 && (x += mod), *this; }
	inline mint & operator *= (mint o) { return x = (ll) x * o.x % mod, *this; }
	inline mint & operator ^= (int b) {
		mint w = *this;
		mint ret(1);
		for(; b; b >>= 1, w *= w) if(b & 1) ret *= w;
		return x = ret.x, *this;
	}
	inline mint & operator /= (mint o) { return *this *= (o ^= (mod - 2)); }
	friend inline mint operator + (mint a, mint b) { return a += b; }
	friend inline mint operator - (mint a, mint b) { return a -= b; }
	friend inline mint operator * (mint a, mint b) { return a *= b; }
	friend inline mint operator / (mint a, mint b) { return a /= b; }
	friend inline mint operator ^ (mint a, int b) { return a ^= b; }
};

bitset<10000000> vis;
vector<int> primes;//leave it as 0 length vector;
//linear
void sieve(long long n){
	vis.set();
	vis[0]=0;
	vis[1]=0;
	for(long long i=2;i<=n;i++){
		if(vis[i]) primes.push_back((int)i);
		for(int j=0;j<(int)primes.size()&&1LL*i*primes[j]<=n;j++){
			vis[i*primes[j]]=0;
			if(i%primes[j]==0) break;
		}
	}
}

int gcd(int a,int b){
	if(b==0) return a;
	return gcd(b,a%b);
}
//|x|+|y| is the minimum value
void gcdE(long long a,long long b,long long& d,long long& x,long long& y){
	if(!b){
		d=a;
		x=1;
		y=0;
	}else{
		gcdE(b,a%b,d,y,x);
		y-=x*(a/b);
	}
}

//to calculate (a/b) mod n, change it to ((a mod n)*inv(b,n)) mod n
long long inv(long long a,long long n){
	long long d,x,y;
	gcdE(a,n,d,x,y);
	return d==1?(x+n)%n:-1;
}

//fast inv
void get_inv(long long n){
	vector<long long> inv;
	long long mod = 1000000007LL;
	inv[0] = inv[1] = 1;
	for(long long i=2;i<n;i++) inv[i]=1ll*(mod-mod/i)*inv[mod%i]%mod;
}


//calculate the number of integers that relativly prime to n;
int euler_phi(int n){
	int m=(int)sqrt(n+0.5);
	int ans=n;
	for(int i=2;i<=m;i++){
		if(n%i==0){
			ans=ans/i*(i-1);
			while(n%i==0) n/=i;
		}
	}
	if(n>1){
		ans=ans/n*(n-1);
	}
	return ans;
}


//remember to use floyd way to test cycle

//phi_table
vector<int> phi;
void phi_table(int n){
	for(int i=2;i<=n;i++){
		phi[i]=0;
	}
	phi[1]=1;
	for(int i=2;i<=n;i++){
		if(!phi[i]){
			for(int j=i;j<=n;j+=i){
				if(!phi[j]){
					phi[j]=j;
				}
				phi[j]=phi[j]/i*(i-1);
			}
		}
	}
}

typedef long long unsigned int llui ;
typedef long long int lli ;
typedef long double float64 ;

llui mul_mod ( llui a, llui b, llui m){
	llui y = (llui)((float64)a*(float64)b/m+(float64)1/2);
	y = y * m;
	llui x = a * b;
	llui r = x - y;
	if((lli)r < 0){
		r = r + m; y = y - 1;
	}
	return r;
}

llui C,a,b;
llui gcd (){
	llui c;
	if(a>b){
		c = a; a = b; b = c;
	}
	while (1) {
		if(a == 1LL) return 1LL;
		if(a == 0 || a == b) return b;
		c = a; a = b%a;
		b = c;
	}
}
llui f( llui a, llui b){
	llui tmp ;
	tmp = mul_mod (a,a,b);
	tmp +=C; tmp %=b;
	return tmp;
}
llui pollard ( llui n){
	if (!(n &1) ) return 2;
	C =0;
	llui iteracoes = 0;
	while ( iteracoes <= 1000) {
		llui x,y,d;
		x = y = 2; d = 1;
		while (d == 1){
			x = f(x,n);
			y = f(f(y,n),n);
			llui m = (x>y)?(x-y):(y-x);
			a = m; b = n; d = gcd ();
		}
		if(d != n) return d;
		iteracoes ++; C = rand ();
	}
	return 1;
}
llui pot ( llui a, llui b, llui c){
	if(b == 0) return 1;
	if(b == 1) return a%c;
	llui resp = pot (a,b>>1,c);
	resp = mul_mod (resp ,resp ,c);
	if(b &1) resp = mul_mod (resp ,a,c);
	return resp ;
}
// Rabin - Miller primality testing algorithm
bool isPrime ( llui n){
	llui d = n -1;
	llui s = 0;
	if(n <=3 || n == 5) return true ;
	if (!(n &1) ) return false ;
	while (!(d &1) ){ s ++; d>>=1; }
	for ( llui i = 0;i <32; i++){
		llui a = rand ();
		a<<=32;
		a+= rand ();
		a %=(n -3) ; a +=2;
		llui x = pot (a,d,n);
		if(x == 1 || x == n -1) continue ;
		for ( llui j = 1;j <= s -1;j ++){
			x = mul_mod (x,x,n);
			if(x == 1) return false ;
			if(x == n -1) break ;
		}
		if(x != n -1) return false ;
	}
	return true ;
}

map <llui ,int > factors ;
// Precondition : factors is an empty map , n is a positive integer
// Postcondition : factors [p] is the exponent of p in prime factorization of n
void fact ( llui n){
	if (! isPrime (n)){
		llui fac = pollard (n);
		fact (n/ fac); fact ( fac );
	} else {
		map <llui ,int >:: iterator it;
		it = factors . find (n);
		if(it != factors . end ()){
			(* it). second ++;
		} else {
			factors [n] = 1;
		}
	}
}

//calculate integral
//define F as global function with other parameters as global variable
double F(double x){

}
//simpson way
double simpson(double a,double b){
	double c = a+(b-a)/2;
	return (F(a)+4*F(c)+F(b))*(b-a)/6.0;
}
//adaptive simpson rule
double asr(double a,double b,double eps,double A){
	double c=a+(b-a)/2.0;
	double L=simpson(a,c);
	double R=simpson(c,b);
	if(fabs(L+R-A)<=15.0*eps) return L+R+(L+R-A)/15.0;//15.0 can be change to other number to set accuracy
	return asr(a,c,eps/2,L)+asr(c,b,eps/2,R);

}

//call this function to get integral result
double asr(double a,double b,double eps){
	return asr(a,b,eps,simpson(a,b));
}


long long china(vector<long long> a,vector<long long> m){
	long long M=1,d,y,x=0;
	int n=(int)a.size();
	for(int i=0;i<n;i++){
		M*=m[i];
	}
	for(int i=0;i<n;i++){
		long long w=M/m[i];
		gcdE(1LL*m[i],w,d,d,y);
		x=(x+y*w*a[i])%M;
	}
	return (x+M)%M;
}

long long pow_mod(long long a,long long p,long long n){
	if(p==0) return 1;
	long long ans = pow_mod(a,p/2,n);
	ans=ans*ans%n;
	if(p%2==1LL) ans=ans*a%n;
	return ans;
}

long long mul_mod(long long a,long long b,long long n){
	a=a%n;
	b=b%n;
	return a*b%n;
}

long long log_mod(long long a,long long b,long long n){
	long long m,v,e=1,i;
	m=(long long)sqrt(n+0.5)+1;
	v=inv(pow_mod(a,m,n),n);
	map<long long,long long> x;
	x[1]=0;
	for(i=1;i<=m;i++){
		e=mul_mod(e,a,n);
		if(!x.count(e)) x[e]=i;
	}
	for(i=0;i<m;i++){
		if(x.count(b)) return i*m+x[b];
		b=mul_mod(b,v,n);
	}
	return -1;
}

// Josephus Problem
// F(n,k) = (F(n-1,k)+k)%n
// where F(1,k) = 0

long long Josephus(long long n,long long q){
	long long D = 1;
	long long end = (q-1)*n;
	while(D<=end){
		D = (D*q-1)/(q-1)+1;
	}
	return q*n+1-D;
}

int main(){

	return 0;
}

