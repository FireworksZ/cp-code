/*
 * BIT_2D.cpp
 *
 *  Created on: 2016��8��10��
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

    // must be called before any update/query; index is 1-based, so allocate rows+1 / cols+1.
    void init(int rows,int cols){
        tree.assign(rows+1, vector<long long>(cols+1, 0));
    }

    void update(int x,int y,int vl){
        if(x<=0||y<=0) return; // index is 1-based; x or y == 0 would infinite-loop
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
