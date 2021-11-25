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


typedef long long LL;
const LL Mod=998244353;
//998244353=119*(2^23)+1
const LL g=3; //g^(Mod-1) = 1
LL fp(LL a,LL k)
{
    LL res=1LL;
    while(k>0)
    {
        if(k&1)res=res*a%Mod;
        a=a*a%Mod;
        k>>=1;
    }
    return res;
}
void change(vector<long long> &y,int len)
{
    for(int i=1,j=len/2;i<len-1;i++)
    {
        if(i<j)swap(y[i],y[j]);
        int k=len/2;
        while(j>=k)
        {
            j-=k;
            k/=2;
        }
        if(j<k)j+=k;
    }
}

//len should be the power of 2.
// on == 1 is ntt
// on == -1 is inverse ntt
void ntt(vector<long long>& y,int len,int on)
{
    change(y,len);
    for(int h=2;h<=len;h<<=1)
    {
        LL wn=fp(g,(Mod-1)/h);
        if(on==-1)wn=fp(wn,Mod-2);
        for(int j=0;j<len;j+=h)
        {
            LL w=1LL;
            for(int k=j;k<j+h/2;k++)
            {
                LL u=y[k];
                LL t=w*y[k+h/2]%Mod;
                y[k]=(u+t)%Mod;
                y[k+h/2]=(u-t+Mod)%Mod;
                w=w*wn%Mod;
            }
        }
    }
    if(on==-1)
    {
        LL t=fp(len,Mod-2);
        for(int i=0;i<len;i++)
            y[i]=y[i]*t%Mod;
    }
}

int main(){




    return 0;

}
