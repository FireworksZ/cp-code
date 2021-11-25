/*
 * BIT_2D.cpp
 *
 *  Created on: 2016Äê8ÔÂ10ÈÕ
 *      Author: Administrator
 */
#include<iostream>
#include<cstdio>
#include<vector>
#include<algorithm>


using namespace std;

//remember that the base array's index start from 1 instead of 0
//i.e. for(int i=1;i<=n;i++) array[i];
class BIT{
public:

	vector<vector<long long> > tree;

    void update(int x,int y,int vl){
        while(x<(int)tree.size()){
            int yy=y;
            while(yy<(int)tree[x].size()){
                tree[x][yy]+=(long long)vl;
                yy+=(yy&-yy);
            }
            x+=(x&-x);
        }
    }

    long long query(int x,int y){
        long long sum=0;
        while(x>0){
            int yy=y;
            while(yy>0){
                sum+=tree[x][yy];
                yy-=(yy&-yy);
            }
            x-=(x&-x);
        }
        return sum;
    }
};

int main(){
	return 0;
}
