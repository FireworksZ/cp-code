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

using namespace std;
//#define double long double

typedef pair<int,int> ii;
typedef pair<int,long long> il;
typedef pair<long long,long long> ll;
typedef pair<ll,int> lli;
typedef pair<long long,int> li;
typedef pair<double,double> dd;
typedef pair<ii,int> iii;
typedef pair<double,int> di;
long long mod = 1000000007LL;
long long base = 10000007;
long long large = 1000000000000000000LL;




struct Treap{
    int size;
    int key,fix;
    Treap *ch[2];
    Treap(int key)
    {
        size=1;
        fix=rand();
        this->key=key;
        ch[0]=ch[1]=NULL;
    }
    int compare(int x) const
    {
        if(x==key) return -1;
        return x<key? 0:1;
    }
    void Maintain()
    {
        size=1;
        if(ch[0]!=NULL) size+=ch[0]->size;
        if(ch[1]!=NULL) size+=ch[1]->size;
    }
};

void Rotate(Treap* &t,int d){
    Treap *k=t->ch[d^1];
    t->ch[d^1]=k->ch[d];
    k->ch[d]=t;
    t->Maintain();  //必须先维护t，再维护k，因为此时t是k的子节点
    k->Maintain();
    t=k;
}

//Following Insert() support insert duplicate element.
//You may call Find() first to check whether there already exists the element,
//and then decide whether to insert or not.
void Insert(Treap* &t,int x){
    if(t==NULL) t=new Treap(x);
    else
    {
        //int d=t->compare(x);   //如果值相等的元素只插入一个
        int d=x < t->key ? 0:1;  //如果值相等的元素都插入
        Insert(t->ch[d],x);
        if(t->ch[d]->fix > t->fix)
            Rotate(t,d^1);
    }
    t->Maintain();
}

//一般来说，在调用删除函数之前要先用Find()函数判断该元素是否存在
void Delete(Treap* &t,int x){
	if(t==NULL) return ;
    int d=t->compare(x);
    if(d==-1)
    {
        Treap *tmp=t;
        if(t->ch[0]==NULL)
        {
            t=t->ch[1];
            delete tmp;
            tmp=NULL;
        }
        else if(t->ch[1]==NULL)
        {
            t=t->ch[0];
            delete tmp;
            tmp=NULL;
        }
        else
        {
            int k=t->ch[0]->fix > t->ch[1]->fix ? 1:0;
            Rotate(t,k);
            Delete(t->ch[k],x);
        }
    }
    else Delete(t->ch[d],x);
    if(t!=NULL) t->Maintain();
}

bool Find(Treap *t,int x){
    while(t!=NULL)
    {
        int d=t->compare(x);
        if(d==-1) return true;
        t=t->ch[d];
    }
    return false;
}

int Kth(Treap *t,int k){
    if(t==NULL||k<=0||k>t->size)
        return -1;
    if(t->ch[0]==NULL&&k==1)
        return t->key;
    if(t->ch[0]==NULL)
        return Kth(t->ch[1],k-1);
    if(t->ch[0]->size>=k)
        return Kth(t->ch[0],k);
    if(t->ch[0]->size+1==k)
        return t->key;
    return Kth(t->ch[1],k-1-t->ch[0]->size);
}

int Rank(Treap *t,int x){
    int r;
    if(t->ch[0]==NULL) r=0;
    else  r=t->ch[0]->size;
    if(x==t->key) return r+1;
    if(x<t->key)
        return Rank(t->ch[0],x);
    return r+1+Rank(t->ch[1],x);
}

void DeleteTreap(Treap* &t){
    if(t==NULL) return;
    if(t->ch[0]!=NULL) DeleteTreap(t->ch[0]);
    if(t->ch[1]!=NULL) DeleteTreap(t->ch[1]);
    delete t;
    t=NULL;
}
void dfs(Treap *t){
	if(t==NULL) return ;
	//do something here with t

	dfs(t->ch[0]);
	dfs(t->ch[1]);

}



void Print(Treap *t){
    if(t==NULL) return;
    Print(t->ch[0]);
    cout<<t->key<<endl;
    Print(t->ch[1]);
}


int main(){


	int n,x;
	char str[5];
	scanf("%d",&n);
	Treap *root=NULL;
	while(n--)
	{
		scanf("%s%d",str,&x);
		if(str[0]=='I')
		{
			if(!Find(root,x))
				Insert(root,x);
		}
		else if(str[0]=='D')
		{
			if(Find(root,x))
				Delete(root,x);
		}
		else if(str[0]=='K')
		{
			int tmp=Kth(root,x);
			if(tmp==-1) puts("invalid");
			else        printf("%d\n",tmp);
		}
		else
		{
			bool flag=1;
			if(!Find(root,x))
				Insert(root,x);
			else
				flag=0;
			printf("%d\n",Rank(root,x)-1);
			if(flag)
				Delete(root,x);
		}
	}
	DeleteTreap(root);
	return 0;
}
