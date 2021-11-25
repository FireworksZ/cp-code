/*
 * BinaryIndexedTree.cpp
 *
 *  Created on: Nov 4, 2015
 *      Author: Zhao Huangjie
 */

#include<iostream>
#include<cstdio>
#include<vector>
#include<algorithm>

using namespace std;
//remember that the base array's index start from 1 instead of 0
//i.e. for(int i=1;i<=n;i++) array[i];
class BIT{
private:
	vector<long long> ft;
public:
	BIT(long long n){
		ft.assign(n+10,0);
	}
	long long LSOne(long long x){
		return x&(-x);
	}
	long long rsq(long long b){
		long long sum=0;
		for(;b>0;b-=LSOne(b)) sum+=ft[b];
		return sum;
	}
	long long rsq(long long a,long long b){  //[a,b]
		return rsq(b)-(a==1?0:rsq(a-1));
	}
	void adjust(long long k,long long v){
		for(;k<(long long)ft.size();k+=LSOne(k)) ft[k]+=v;
	}
};
int main(){

	return 0;
}


