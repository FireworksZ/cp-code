/*
 * test.cpp
 *
 *
 *      Author: Fireworks
 */

#include<iostream>
#include<cstdio>
#include<vector>
#include<algorithm>
#include<queue>
#include<string>
#include<map>
#include<cmath>
#include<bitset>
#include<set>
#include<iomanip>
#include<fstream>
#include<bitset>
#include<cstring>
#include<cstdlib>
#include<complex>
#include<list>
#include<sstream>
#include<cassert>   // assert
#include<ctime>

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
long long mod = 1000000007LL;
long long large = 1000000000000000000LL;

// n = (1<<sz)
// be aware of recalculate rev if mod is changed in main()
long long rev = (mod+1)/2;
void FWT(vector<long long> &a,int sz){
	int n = (1<<sz);
    for(int d=1;d<n;d<<=1)
        for(int m=d<<1,i=0;i<n;i+=m)
            for(int j=0;j<d;j++)
            {
                long long x=a[i+j],y=a[i+j+d];
                a[i+j]=(x+y)%mod,a[i+j+d]=(x-y+mod)%mod;
                //xor:a[i+j]=x+y,a[i+j+d]=(x-y+mod)%mod;
                //and:a[i+j]=x+y;
                //or:a[i+j+d]=x+y;
            }
}

void UFWT(vector<long long> &a,int sz){
	int n = (1<<sz);
    for(int d=1;d<n;d<<=1)
        for(int m=d<<1,i=0;i<n;i+=m)
            for(int j=0;j<d;j++)
            {
                long long x=a[i+j],y=a[i+j+d];
                a[i+j]=1LL*(x+y)*rev%mod,a[i+j+d]=(1LL*(x-y)*rev%mod+mod)%mod;
                //xor:a[i+j]=(x+y)/2,a[i+j+d]=(x-y)/2;
                //and:a[i+j]=x-y;
                //or:a[i+j+d]=y-x;
            }
}
//for subset convolution
//c[k] = sigma [(i&j) == 0]*[(i|j)==k]*a[i]*b[j]
//	   = sigma [(i|j)==k]*[bit(i)+bit(j)==bit(k)]*a[i]*b[j]

void FWT3(vector<long long> &a,int sz){
	int n = (1<<sz);
    for(int d=1;d<n;d<<=1)
        for(int m=d<<1,i=0;i<n;i+=m)
            for(int j=0;j<d;j++)
            {
                long long x=a[i+j],y=a[i+j+d];
                //a[i+j]=(x+y)%mod,a[i+j+d]=(x-y+mod)%mod;
                //xor:a[i+j]=x+y,a[i+j+d]=(x-y+mod)%mod;
                //and:a[i+j]=x+y;
                a[i+j+d]=(x+y)%mod;
            }
}

void UFWT3(vector<long long> &a,int sz){
	int n = (1<<sz);
    for(int d=1;d<n;d<<=1)
        for(int m=d<<1,i=0;i<n;i+=m)
            for(int j=0;j<d;j++)
            {
                long long x=a[i+j],y=a[i+j+d];
                //a[i+j]=1LL*(x+y)*rev%mod,a[i+j+d]=(1LL*(x-y)*rev%mod+mod)%mod;
                //xor:a[i+j]=(x+y)/2,a[i+j+d]=(x-y)/2;
                //and:a[i+j]=x-y;
                a[i+j+d]=(y-x+mod)%mod;
            }
}


//sz: the length of a[] is equal to (1<<sz)
vector<long long> subsetconvolution(vector<long long> a,vector<long long> b,int sz){
	vector<int> bit(1<<sz,0);
	for(int i=1;i<(1<<sz);i++) bit[i] = bit[i-(i&(-i))]+1;

	vector<vector<long long> > ta(sz+1,vector<long long>(1<<sz,0));
	vector<vector<long long> > tb = ta,tc = ta;
	for(int i=0;i<(1<<sz);i++){
		ta[bit[i]][i]+=a[i];
		tb[bit[i]][i]+=b[i];
	}
	for(int i=0;i<sz+1;i++){
		FWT3(ta[i],sz);
		FWT3(tb[i],sz);
	}

	for(int i=0;i<sz+1;i++){
		for(int j=0;j<=i;j++){
			for(int k=0;k<(1<<sz);k++){
				tc[i][k]+=ta[j][k]*tb[i-j][k];
				tc[i][k]%=mod;
			}
		}
	}
	for(int i=0;i<sz+1;i++){
		UFWT3(tc[i],sz);
	}

	vector<long long> cnt(1<<sz,0);

	for(int i=0;i<(1<<sz);i++){
		cnt[i] = tc[bit[i]][i];
	}
	return cnt;
}


int main(){


    return 0;

}
