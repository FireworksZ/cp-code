/*
 * Heap.cpp
 *
 *  Created on: Nov 12, 2015
 *      Author: Zhao Huangjie
 */

#include<iostream>
#include<cstdio>
#include<vector>
#include<algorithm>

using namespace std;
class Heap{
public:
	vector<int> h;
	int n;
	int p;
	Heap(int x){
		n=x;
		h.assign(n+10,1000000000);
		p=1;
	}
	bool check(int x,int y){ //criteria for the top
		if(y==1000000000) return true;
		if(x==1000000000) return false;
		return x<y;
	}
	void balancedown(int o){
		if(o*2>=p) return;
		if(check(h[o*2],h[o*2+1])){
			if(check(h[o*2],h[o])){
				swap(h[o],h[o*2]);
				balancedown(o*2);
			}
		}else{
			if(check(h[o*2+1],h[o])){
				swap(h[o],h[o*2+1]);
				balancedown(o*2+1);
			}
		}
	}
	void balanceup(int o){
		if(o==1) return;
		if(check(h[o],h[o/2])){
			swap(h[o],h[o/2]);
			balanceup(o/2);
		}
	}
	void push(int v){
		h[p]=v;
		balanceup(p);
		p++;
	}
	int top(){
		return h[1];
	}
	void pop(){
		p--;
		swap(h[1],h[p]);
		h[p]=1000000000;
		balancedown(1);
	}
	void erase(int pos){
		if(pos>=p) return;
		p--;
		swap(h[pos],h[p]);
		h[p]=1000000000;
		balancedown(pos);
	}
	bool empty(){
		return p==1;
	}
	void clear(){
		h.clear();
		n=0;
		p=1;
	}
	void assign(int x){
		h.assign(x+10,1000000000);
		n=x;
		p=1;
	}
};
int main(){
	int x;
	Heap a(1000000);
	while(cin>>x&&x){
		a.push(x);
	}
	while(!a.empty()){
		cout<<a.top()<<" ";
		a.pop();
	}
	return 0;
}


