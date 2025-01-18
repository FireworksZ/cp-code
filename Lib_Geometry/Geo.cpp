/*
 * Geo.cpp
 *
 *  Created on: 2016年1月3日
 *      Author: Administrator
 */

#include<iostream>
#include<vector>
#include<algorithm>
#include<cstdio>
#include<cmath>

using namespace std;

const double pi=acos(-1.0);
const double eps=1e-10;

struct Vector{
	double x,y;
	Vector(double x=0,double y=0):x(x),y(y) {}
	void init (double a,double b){
		x=a;y=b;
	}
};

Vector operator + (Vector A,Vector B){
	return Vector(A.x+B.x,A.y+B.y);
}
Vector operator - (Vector A,Vector B){
	return Vector(A.x-B.x,A.y-B.y);
}
Vector operator * (Vector A,double p){
	return Vector(p*A.x,p*A.y);
}
Vector operator / (Vector A,double p){
	return Vector(A.x/p,A.y/p);
}
bool operator < (const Vector &A,const Vector &B){
	return A.x<B.x||(A.x==B.x&&A.y<B.y);
}
int dcmp(double x){
	if(fabs(x)<eps) return 0;
	else return x<0?-1:1;
}
bool operator ==(const Vector &A,const Vector &B){
	return dcmp(A.x-B.x)==0&&dcmp(A.y-B.y)==0;
}
double dot(Vector A,Vector B){
	return A.x*B.x+A.y*B.y;
}
double length(Vector A){
	return sqrt(dot(A,A));
}
double angle(Vector A){
	return atan2(A.y,A.x);
}
double angle(Vector A,Vector B){
	double r = dot(A,B)/length(A)/length(B);
	r = min(r,(double)1.0);
	r = max(r,(double)-1.0);
	return acos(r);
}
double cross(Vector A,Vector B){
	return A.x*B.y-A.y*B.x;
}
Vector rotate(Vector A,double rad){
	return Vector(A.x*cos(rad)-A.y*sin(rad),A.x*sin(rad)+A.y*cos(rad));
}
Vector rotate(Vector A,Vector P,double rad){
	Vector temp=A-P;
	temp=rotate(temp,rad);
	return temp+P;
}
Vector normal(Vector A){
	double L = length(A);
	return Vector(-A.y/L,A.x/L);
}
//uppercase mostly means vector
//lowercase mostly means point
Vector getLineIntersection(Vector P,Vector v,Vector Q,Vector w){
	Vector u=P-Q;
	double t=cross(w,u)/cross(v,w);
	return P+v*t;
}
double getDistanceToLine(Vector P,Vector A,Vector B){
	Vector v1=B-A,v2=P-A;
	return fabs(cross(v1,v2))/length(v1);
}
double getDistanceToSegment(Vector P,Vector A,Vector B){
	if(A==B) return length(P-A);
	Vector v1=B-A,v2=P-A,v3=P-B;
	if(dcmp(dot(v1,v2))<0) return length(v2);
	else if(dcmp(dot(v1,v3))>0) return length(v3);
	else return fabs(cross(v1,v2))/length(v1);
}
Vector getLineProjection(Vector P,Vector A,Vector B){
	Vector v=B-A;
	return A+v*(dot(v,P-A)/dot(v,v));
}
bool isSegmentProperIntersection(Vector A1,Vector A2,Vector B1,Vector B2){
	double c1=cross(A2-A1,B1-A1);
	double c2=cross(A2-A1,B2-A1);
	double c3=cross(B2-B1,A1-B1);
	double c4=cross(B2-B1,A2-B1);
	return dcmp(c1)*dcmp(c2)<0&&dcmp(c3)*dcmp(c4)<0;
}
bool onSegment(Vector P,Vector A,Vector B){
	return dcmp(cross(A-P,B-P))==0&&dcmp(dot(A-P,B-P))<0;
}
bool isSegmentIntersection(Vector A1,Vector A2,Vector B1,Vector B2){
	if(isSegmentProperIntersection(A1,A2,B1,B2)){
		return true;
	}
	if(A1==B1) return true;
	if(A1==B2) return true;
	if(A2==B1) return true;
	if(A2==B2) return true;
	if(onSegment(A1,B1,B2)) return true;
	if(onSegment(A2,B1,B2)) return true;
	if(onSegment(B1,A1,A2)) return true;
	if(onSegment(B2,A1,A2)) return true;
	return false;
}
bool onSegment2(Vector P,Vector A, Vector B){
	if(onSegment(P,A,B)) return true;
	if(P==A) return true;
	if(P==B) return true;
	return false;
}
struct Line{
	Vector P,v; //p is one of the point on the line and v is the direction vector
	//Line(Vector P,Vector v):P(P),v(v) {};
	//void Line
	double ang;
	void init(Vector tP,Vector tv){
		P=tP;v=tv;
		ang=angle(v);
	}
	void init2(Vector A,Vector B){
		P=A;v=B-A;
		ang=angle(v);
	}
	Vector point(double t){
		return P+v*t;
	}
	bool operator < (const Line &L) const{
		return ang<L.ang;
	}
};

Vector getLineIntersection(Line l1,Line l2){
	return getLineIntersection(l1.P,l1.v,l2.P,l2.v);
}
double getDistanceToLine(Vector P,Line L){
	Vector v1=L.v,v2=P-L.P;
	return fabs(cross(v1,v2))/length(v1);
}
Vector getLineProjection(Vector P,Line L){
	Vector v=L.v;
	Vector A=L.P;
	return A+v*(dot(v,P-A)/dot(v,v));
}
bool onLine(Vector P,Line L){
	return dcmp(cross(P-L.P,L.v))==0;
}
struct Circle{
	Vector C;
	double r;
	//Circle(Vector C,double r):C(C),r(r) {};
	void init(Vector tC,double tr){
		C=tC;r=tr;
	}
	void init2(double x,double y,double tr){
		C.init(x,y);
		r=tr;
	}
	Vector point (double a){
		return Vector(C.x+cos(a)*r,C.y+sin(a)*r);
	}
	double area(){
		return pi*r*r;
	}
	double getCentralAngle(Vector A,Vector B){ //return the angle which is smaller than pi
		double a1=angle(A-C);
		double a2=angle(B-C);
		if(dcmp(a1-a2)==0) return 0.0;
		if(a1>a2) swap(a1,a2);
		double ans = a2-a1;
		if(dcmp(ans-pi)>=0) ans=pi-fabs(ans-pi);
		return ans;
	}
	bool isInCircle(Vector A){
		return dcmp(length(A-C)-r)<0;
	}
};

struct Triangle{
	Vector A,B,C;
	double a,b,c;
	double AA,BB,CC;
	/*Triangle(Vector A,Vector B,Vector C):A(A),B(B),C(C) {
		a=Length(B-A);
		b=Length(C-B);
		c=Length(A-C);
	};
	Triangle(double a,double b,double c):a(a),b(b),c(c){};*/
	void init(Vector tA,Vector tB,Vector tC){
		A=tA;B=tB;C=tC;
		if(cross(B-A,C-A)<0) swap(B,C);
		a=length(B-A);
		b=length(C-B);
		c=length(A-C);
		AA=angle(B-A,C-A);
		BB=angle(C-B,A-B);
		CC=angle(A-C,B-C);
	}
	double getAngle(double x,double y,double z){
		return acos((x*x-z*z+y*y)/2*x*y);
	}
	bool init(double ta,double tb,double tc){
		if(ta>tb) swap(ta,tb);
		if(tb>tc) swap(tb,tc);
		if(ta>tb) swap(ta,tb);
		if(dcmp(tc-ta-tb)>0) return false;
		a=ta;b=tb;c=tc;
		AA=getAngle(a,c,b);
		BB=getAngle(b,a,c);
		CC=getAngle(c,b,a);
		return true;
	}
	double getAreaByPoint(){
		return fabs(cross(A-C,B-C)/2.0);
	}
	double getAreaByEdge(){
		double s=(a+b+c)/2.0;
		return sqrt(s*(s-a)*(s-b)*(s-c));
	}
	double getIncricleRadius(){
		double co=(1.0+tan(AA/2.0)/tan(BB/2.0));
		return a/co*tan(AA/2.0);
	}
	Circle getIncircle(){
		double r=getIncricleRadius();
		Line l1,l2;
		l1.init2(A,B-A);
		l2.init2(B,A-B);
		l1.v=rotate(l1.v,AA/2.0);
		l2.v=rotate(l2.v,-BB/2.0);
		Vector c=getLineIntersection(l1,l2);
		Circle temp;
		temp.init(c,r);
		return temp;
	}
};

int getLineCircleIntersection(Line L,Circle C,vector<Vector> &sol){
	double a=L.v.x;
	double b=L.P.x-C.C.x;
	double c=L.v.y;
	double d=L.P.y-C.C.y;
	double e=a*a+c*c;
	double f=2*(a*b+c*d);
	double g=b*b+d*d-C.r*C.r;
	double delta=f*f-4*e*g;
	double t1,t2;
	if(dcmp(delta)<0) return 0;
	if(dcmp(delta)==0){
		t1=t2=-f/(2*e);
		sol.push_back(L.point(t1));
		return 1;
	}
	t1=(-f-sqrt(delta))/(2*e);
	sol.push_back(L.point(t1));
	t2=(-f+sqrt(delta))/(2*e);
	sol.push_back(L.point(t2));
	return 2;
}
int getCircleIntersection(Circle C1,Circle C2,vector<Vector> &sol){
	double d=length(C1.C-C2.C);
	if(dcmp(d)==0){
		if(dcmp(C1.r-C2.r)==0) return -1;
		return 0;
	}
	if(dcmp(C1.r+C2.r-d)<0) return 0;
	if(dcmp(fabs(C1.r-C2.r)-d)>0) return 0;
	double a=angle(C2.C-C1.C);
	double temp = C1.r/2.0/d-C2.r/C1.r*C2.r/d/2.0+d/C1.r/2.0;
	if(dcmp(temp-1.0)==0){
		sol.push_back(C1.point(a));
		return 1;
	}
	if(dcmp(temp+1.0)==0){
		sol.push_back(C1.point(pi+a));
		return 1;
	}
	double da=acos(temp);
	if(dcmp(da)==0){
		sol.push_back(C1.point(a));
		return 1;
	}
	Vector P1=C1.point(a-da);
	Vector P2=C1.point(a+da);
	sol.push_back(P1);
	sol.push_back(P2);
	return 2;
}

int getTangents(Vector P,Circle C,vector<Vector> &sol){
	//sol store the tangent point on the circle
	Vector u=P-C.C;
	double dis=length(u);
	if(dcmp(dis-C.r)<0) return 0;
	else if(dcmp(dis-C.r)==0){
		sol.push_back(P);
		return 1;// P is on the circle;
	}else {
		double rad = acos(C.r/dis);
		Vector temp=u/dis*C.r;
		sol.push_back(C.C+rotate(temp,-rad));
		sol.push_back(C.C+rotate(temp,rad));
		return 2;
	}
}
int getTangents(Circle A,Circle B,vector<Vector> &a,vector<Vector> &b){
	int cnt=0;
	bool flag=false;
	if(A.r<B.r) {
		swap(A,B);
		flag=true;
	}
	double d2=sqrt(dot(A.C-B.C,A.C-B.C));
	double rdiff = A.r-B.r;
	double rsum=A.r+B.r;
	if(dcmp(d2)==0&&dcmp(A.r-B.r)==0) return -1;
	if(dcmp(d2-rdiff)<0) return 0;
	double base=angle(B.C-A.C);
	if(dcmp(d2-rdiff)==0){
		a.push_back(A.point(base));
		b.push_back(B.point(base));
		cnt++;
		if(flag) swap(a,b);
		return 1;
	}
	double ang=acos(rdiff/d2);
	a.push_back(A.point(base+ang));
	b.push_back(B.point(base+ang));
	a.push_back(A.point(base-ang));
	b.push_back(B.point(base-ang));
	cnt+=2;
	if(dcmp(d2-rsum)==0){
		a.push_back(A.point(base));
		b.push_back(B.point(base+pi));
		cnt++;
	}else if(dcmp(d2-rsum)>0){
		ang=acos(rsum/d2);
		a.push_back(A.point(base+ang));
		b.push_back(B.point(pi+base+ang));
		a.push_back(A.point(base-ang));
		b.push_back(B.point(pi+base-ang));
		cnt+=2;
	}
	if(flag) swap(a,b);
	return cnt;
}
vector<Vector> cutPolygon(vector<Vector> p, Line L){
	vector<Vector> newpoly;
	int n=(int)p.size();
	Vector A=L.P;
	Vector v=L.v;
	for(int i=0;i<n;i++){
		Vector C=p[i];
		Vector D=p[(i+1)%n];
		if(dcmp(cross(v,C-A))>=0) newpoly.push_back(C);
		if(dcmp(cross(v,C-D))!=0){
			Vector ip= getLineIntersection(A,v,C,D-C);
			if(onSegment(ip,C,D)) newpoly.push_back(ip);
		}
	}
	return newpoly;
}
int getConvexHull(vector<Vector> p,vector<Vector> &ch){
	sort(p.begin(),p.end());
	p.erase(unique(p.begin(),p.end()),p.end());
	int n=(int)p.size();
	int m=0;
	for(int i=0;i<n;i++){
		while(m>1&&dcmp(cross(ch[m-1]-ch[m-2],p[i]-ch[m-2]))<=0) {
			ch.pop_back();
			m--;
		}
		ch.push_back(p[i]);
		m++;
	}
	int k=m;
	for(int i=n-2;i>=0;i--){
		while(m>k&&dcmp(cross(ch[m-1]-ch[m-2],p[i]-ch[m-2]))<=0) {
			ch.pop_back();
			m--;
		}
		ch.push_back(p[i]);
		m++;
	}
	if(n>1) {ch.pop_back();m--;}
	return m;
}
bool onLeft(Line L,Vector P){
	return dcmp(cross(L.v,P-L.P))>0;
}
int getHalfplaneIntersection(vector<Line> L,vector<Vector> &poly){
	//make sure the result is bonded
	sort(L.begin(),L.end());
	int n=(int)L.size();
	int first,last;//position of the first and the last element in the dequeue
	vector<Vector> p(n);
	vector<Line> q(n);
	q[first=last=0]=L[0];
	for(int i=1;i<n;i++){
		while(first<last&&!onLeft(L[i],p[last-1])) last--;
		while(first<last&&!onLeft(L[i],p[first])) first++;
		q[++last]=L[i];
		if(dcmp(cross(q[last-1].v,q[last].v))<0){
			poly.clear();
			return 0;
		}
		if(dcmp(cross(q[last].v,q[last-1].v))==0){
			if(dcmp(dot(q[last-1].v,q[last].v))<0){
				poly.clear();
				return 0;
			}
			last--;
			if(onLeft(q[last],L[i].P)) q[last]=L[i];
		}
		if(first<last) p[last-1]=getLineIntersection(q[last-1],q[last]);
	}
	while(first<last&&!onLeft(q[first],p[last-1])) last--;
	if(last-first<=1) return 0;
	p[last]=getLineIntersection(q[last],q[first]);
	for(int i=first;i<=last;i++) poly.push_back(p[i]);

	return (int)poly.size();
}
double getArea(vector<Vector> p){
	int n=(int)p.size();
	double sum=0.0;
	for(int i=0;i<n;i++){
		sum+=cross(p[i],p[(i+1)%n])/2.0;
	}
	return sum;
}
// 返回点集直径
double getDiameter(vector<Vector> points) {
  vector<Vector> p;
  getConvexHull(points,p);
  int n = p.size();
  if(n == 1) return 0;
  if(n == 2) return length(p[1]-p[0]);
  p.push_back(p[0]); // 免得取模
  double ans = 0;
  for(int u = 0, v = 1; u < n; u++) {
    // 一条直线贴住边p[u]-p[u+1]
    for(;;) {
      // 当Area(p[u], p[u+1], p[v+1]) <= Area(p[u], p[u+1], p[v])时停止旋转
      // 即Cross(p[u+1]-p[u], p[v+1]-p[u]) - Cross(p[u+1]-p[u], p[v]-p[u]) <= 0
      // 根据Cross(A,B) - Cross(A,C) = Cross(A,B-C)
      // 化简得Cross(p[u+1]-p[u], p[v+1]-p[v]) <= 0
      double diff = cross(p[u+1]-p[u], p[v+1]-p[v]);
      if(dcmp(diff) <= 0) {
        ans = max(ans,length(p[u]-p[v])); // u和v是对踵点
        if(dcmp(diff) == 0) ans = max(ans,length(p[u]-p[v+1])); // diff == 0时u和v+1也是对踵点
        break;
      }
      v = (v + 1) % n;
    }
  }
  return ans;
}


bool cmpy(Vector a,Vector b)
{
	return a.y < b.y;
}
double Closest_Pair(vector<Vector> &p,int left,int right){
	double d = 1e20;
	if(left == right)return d;
	if(left + 1 == right)
		return length(p[right]-p[left]);
	int mid = (left+right)/2;
	double d1 = Closest_Pair(p,left,mid);
	double d2 = Closest_Pair(p,mid+1,right);
	d = min(d1,d2);

	vector<Vector> tmpt;
	for(int i = left;i <= right;i++){
			if(fabs(p[mid].x - p[i].x) <= d) tmpt.push_back(p[i]);
	}
	sort(tmpt.begin(),tmpt.end(),cmpy);
	for(int i = 0;i <(int)tmpt.size();i++){
		for(int j = i+1;j < (int)tmpt.size() && tmpt[j].y - tmpt[i].y < d;j++){
			d = min(d,length(tmpt[i]-tmpt[j]));
		}
	}
	return d;
}


int main(){
	bool flag=true;
	while(flag){
		double x1,x2,x3,y1,y2,y3;
		cin>>x1>>y1>>x2>>y2>>x3>>y3;
		Vector A,B,C;
		A.init(x1,y1);
		B.init(x2,y2);
		C.init(x3,y3);
		Triangle tri;
		tri.init(A,B,C);
		/*Circle c=tri.getIncircle();
		cout<<c.C.x<<" "<<c.C.y<<" "<<c.r<<endl;*/
		cout<<tri.getAreaByPoint()<<endl;
		vector<Vector> p;
		p.push_back(tri.A);
		p.push_back(tri.B);
		p.push_back(tri.C);
		cout<<getArea(p)<<endl;
		int temp;
		cin>>temp;
		if(!temp){
			flag=false;
		}
	}
}


