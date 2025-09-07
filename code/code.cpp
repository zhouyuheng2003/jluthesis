#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
#define fi first
#define se second
#define pb push_back
#define mp make_pair
typedef pair<int,int> pii;
template <typename T> inline T gcd(const T a,const T b){if(b==0)return a;return gcd(b,a%b);}
template <typename T> inline void read(T&x)
{
    char cu=getchar();x=0;bool fla=0;
    while(!isdigit(cu)){if(cu=='-')fla=1;cu=getchar();}
    while(isdigit(cu))x=x*10+cu-'0',cu=getchar();
    if(fla)x=-x; 
}
template <typename T> void printe(const T x)
{
    if(x>=10)printe(x/10);
    putchar(x%10+'0');
}
template <typename T> inline void print(const T x)
{
    if(x<0)putchar('-'),printe(-x);
    else printe(x);
}
const int maxn=10001;
int n,N,c=2;//step1，设置最小割值 
map<int,int>E[maxn];
set<int> phi[maxn];
void add(int u,int v,int w){
	E[u][v]+=w;//step2，用加法合并二元环 
	E[v][u]+=w;
}
vector<int>circle[maxn];
int circle_number;
int vis[maxn];
void find_connected_components() {//找环模块 
    memset(vis,0,sizeof(vis));
    circle_number=0;
    for(int i=1;i<=n;++i){
        if(!vis[i]){
        	circle_number++;
        	circle[circle_number].clear();
            circle[circle_number].push_back(i);
            queue<int> q;
            q.push(i);
            vis[i]=1;
            while(!q.empty()) {
                int u=q.front();
                q.pop();
                for(auto e:E[u]) {
                    if (!vis[e.fi]&&e.se==c/2) {
                        vis[e.fi]=1;
                        q.push(e.fi);
                        circle[circle_number].push_back(e.fi);
                        break;//只枚举一条边，确保点按环上顺序列出 
                    }
                }
            }
        }
    }
}
int add_node(){
	return ++n;
}
int del_sign[maxn];
void del_node(int u){
	del_sign[u]=1;
}
int id[maxn],id_n;
void output(){
	id_n=0;
	for(int i=1;i<=n;i++){
		if(del_sign[i])continue;
		id[i]=++id_n;
	}
	for(int i=1;i<=n;i++){
		if(del_sign[i])continue;
		for(auto e:E[i]){
			if(e.se==0)continue;
			if(e.fi<i)continue;
			cout<<id[i]<<" "<<id[e.fi]<<" "<<e.se<<endl;
		}
	}
	for(int i=1;i<=n;i++){
		if(del_sign[i])continue;
		cout<<id[i]<<":";
		for(auto v:phi[i]){
			cout<<" "<<v;
		}
		if(phi[i].size()==0)cout<<"empty"; 
		cout<<endl;	
	}
	cout<<endl;
}
int main(){
	N=4;
	//Case 1
//	n=4;
//	add(1,2,1);
//	add(1,3,1);
//	add(1,4,1);
//	add(1,4,1);
//	add(2,3,1);
//	phi[1].insert(1);
//	phi[2].insert(2);
//	phi[3].insert(3);
//	phi[4].insert(4);
	//Case 2
	n=6;
	add(1,6,1);
	add(2,6,1);
	add(3,6,1);
	add(1,5,1);
	add(4,5,1);
	add(1,6,1);
	add(2,6,1);
	add(3,6,1);
	add(1,5,1);
	add(4,5,1);
	phi[1].insert(1);
	phi[2].insert(2);
	phi[3].insert(3);
	phi[4].insert(4);
	
	output();
	find_connected_components();
	for(int i=1;i<=circle_number;i++){
		if(circle[i].size()==1)continue;
		for(auto k2:circle[i]){//Step5,由于映射由k2继承，因此用k2替换k 
			int k1=add_node();
			for(auto e:E[k2]){
				if(e.se!=c/2)continue;//Step10，环边连向k1，其它边连向k2 
				int v=e.fi;
				E[v][k2]=0;
				E[k2][v]=0;
				E[v][k1]=c/2;
				E[k1][v]=c/2;
			}
			E[k2][k1]=c;//Step7,k1,k2连边 
			E[k1][k2]=c;
		}
	}
	
	//Step17，收缩三元环 
	find_connected_components();
	for(int i=1;i<=circle_number;i++){
		if(circle[i].size()!=3)continue;
		int k=add_node();
		for(auto v1:circle[i]){
			for(auto e:E[v1]){
				if(e.se!=c)continue;
				int v2=e.fi; 
				E[v1][v2]=0;
				E[v2][v1]=0;
				E[v2][k]=c;
				E[k][v2]=c;
			}
			del_node(v1);
		}
	}
	
	
	//Step18，删除链冗余节点 
	for(int i=1;i<=n;i++){
		if(del_sign[i])continue;
		int v[1001],cnt=0;
		for(auto e:E[i]){
			if(e.se==0)continue;
			if(e.se==c/2){
				cnt=0;
				break;
			}
			v[++cnt]=e.fi;
		}
		if(cnt==2&&phi[i].size()==0){
			E[i][v[1]]=0;
			E[v[1]][i]=0;
			E[i][v[2]]=0;
			E[v[2]][i]=0;
			E[v[1]][v[2]]=c;
			E[v[2]][v[1]]=c;
			del_node(i);
		}
	}
	
	
	output();//Step25
	return 0;
} 
