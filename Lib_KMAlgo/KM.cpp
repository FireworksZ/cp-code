#include <bits/stdc++.h>

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
long long mod = 1000000007LL;
long long large = 1000000000000000000LL;

//http://blog.csdn.net/bless924295/article/details/53172487
//二分图最大权匹配
//图必须为n x n的全二分图
//有必要的情况下可以加dummy node去满足n x n全二分图
//O(n^3)
//match[] store the matching result
//match[i] match with i using edge with weight[match[i]][i]
#define inf 0x3f3f3f3f
const int N=1000;
int match[N];
int lx[N],ly[N];
int sx[N],sy[N];
int weight[N][N];
int n;
int dfs(int x){
    sx[x]=true;
    for(int i=0;i<n;i++){
        if(!sy[i]&&lx[x]+ly[i]==weight[x][i]){
            sy[i]=true;
            if(match[i]==-1||dfs(match[i])){
                match[i]=x;
                return true;
            }
        }
    }
    return false;
}
// fax(1) find maximum matching
// fax(0) find minimum matching
int fax(int x){
    if(!x){
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                weight[i][j]=-weight[i][j];
            }
        }
    }
    memset(match,-1,sizeof(match));
    for(int i=0;i<n;i++){
        ly[i]=0;
        lx[i]=-inf;
        for(int j=0;j<n;j++){
            if(weight[i][j]>lx[i]){
                lx[i]=weight[i][j];
            }
        }
    }
    for(int i=0;i<n;i++){
        while(1){
            memset(sx,0,sizeof(sx));
            memset(sy,0,sizeof(sy));
            if(dfs(i))
                break;
            int mic=inf;
            for(int j=0;j<n;j++){
                if(sx[j]){
                    for(int k=0;k<n;k++){
                        if(!sy[k]&&lx[j]+ly[k]-weight[j][k]<mic){
                            mic=lx[j]+ly[k]-weight[j][k];
                        }
                    }
                }
            }
            if(mic==0)
                return -1;
            for(int j=0;j<n;j++){
                if(sx[j]){
                    lx[j]-=mic;
                }
                if(sy[j]){
                    ly[j]+=mic;
                }
            }
        }
    }
    int sum=0;
    for(int i=0;i<n;i++){
        if(match[i]>=0){
            sum+=weight[match[i]][i];
        }
    }
    if(!x){
        sum=-sum;
    }
    return sum;
}

int main(){
    scanf("%d",&n);
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            scanf("%d",&weight[i][j]);
        }
    }
    int h=fax(1);
    cout<<h<<endl;
    /*
    5
    3 4 6 4 9
    6 4 5 3 8
    7 5 3 4 2
    6 3 2 2 5
    8 4 5 4 7
    */


    return 0;

}
