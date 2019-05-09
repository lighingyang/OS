#include<bits/stdc++.h>
using namespace std;

/*
描述：可允许进程最大数量
*/
const int maxn = 10010;

/*
描述：实际到来进程数量
*/
int n;

/*
描述:进程的声明
*/
struct process;

/*
描述：进程PCB的声明
*/
struct PCB;

/*
功能:就绪队列
*/
queue<int> q1;

/*
功能:阻塞队列
*/
queue<int> q2;

/*
功能:进程PCB
*/
struct PCB{
		int id;//进程id
		int cls;//进程等级
		int come;//进程到来时间
		int alltime;//进程所需时间
		int cputime; //已用cpu时间
		int startblock;//一个时间片的时间
		int starttime;//从阻塞状态转为就绪状态的时间
		int blocktime;//已阻塞的时间
		char state;//进程状态
		struct precess *Next;
};

/*
功能:进程
*/
struct process{
    int waittime;
    int runtime;
	PCB pcb;
	process(){
	    runtime = 0;
    }
	setdata(int i,int cl,int cm,int ati,int sb,int st){
		pcb.id = i;
		pcb.cls = cl;
		pcb.come = cm;
		pcb.alltime = ati;
		pcb.cputime = 0;
		pcb.startblock = sb;
		pcb.starttime = st;
		pcb.blocktime = 0;
		pcb.state = 'W';
		runtime = 0;
		waittime = 0;
	}
};

process p[maxn];

/*
功能：查看此刻阻塞队列中的进程
*/
void find_block(){
    printf("阻塞队列中的进程为：");
    for(int i=0;i<n;i++){
        if(p[i].pcb.state == 'B') printf("  进程%d",p[i].pcb.id);
    }
    printf("\n");
}

/*
功能：查看此刻就绪队列中的进程
*/
void find_ready(){
    printf("就绪队列中的进程为：");
    for(int i=0;i<n;i++){
        if(p[i].pcb.state == 'R') printf("  进程%d",p[i].pcb.id);
    }
    printf("\n");
}

/*
功能：检查当前时刻有没有到来的进程
*/
void solve_ready(int now_time){
    for(int i=0;i<n;i++){
        if(now_time == p[i].pcb.come){
            q1.push(i);
            p[i].pcb.state = 'R';
            printf("进程%d来到就绪队列\n",p[i].pcb.id);
        }
    }
    return ;
}

/*
功能：阻塞队列更新
*/
void updata_block(){
    /*
    名称：q3
    功能:阻塞临时队列
    */
    queue<int> q3;

    while(!q2.empty()){
        int now = q2.front();
        q2.pop();
        p[now].pcb.blocktime ++;
        if(p[now].pcb.blocktime == p[now].pcb.starttime){
            q1.push(now);
            p[now].pcb.state = 'R';
            p[now].pcb.blocktime = 0;
            printf("进程%d从阻塞队列进入就绪队列\n",p[now].pcb.id);
        }
        else{
            q3.push(now);
        }
    }
    while(!q3.empty()){
        int now = q3.front();
        q3.pop();
        q2.push(now);
    }
    return ;
}

/*
功能：寻找优先队列中优先级最大的进程
*/
int find_maxcls(){
    int mmmax = 0;
    int idx = -1;
    for(int i=0;i<n;i++){
        if(p[i].pcb.state == 'R'&&p[i].pcb.cls>mmmax){
            mmmax = p[i].pcb.cls;
            idx = i;
        }
    }
    return idx;
}

/*
功能：就绪队列更新
*/
void updata_ready(int pid){
    /*
    名称：q4
    功能:就绪临时队列
    */
    queue<int> q4;
    while(!q1.empty()){
        int now = q1.front();
        q1.pop();
        if(p[now].pcb.id == pid){
            continue;
        }
        else{
            q4.push(now);
        }
    }
    while(!q4.empty()){
        int now = q4.front();
        q4.pop();
        q1.push(now);
    }
    return ;
}

/*
功能:手动输入进程
描述：  cl: cls
        cm: come
        ati: alltime
        sb: startblock
        st: starttime

*/
void input(){
	scanf("%d",&n);
	int i=0;
	int cl;
	int cm,ati,sb,st;
	scanf("%d %d %d %d %d",&cl,&cm,&ati,&sb,&st);
	//printf("%d %.2lf %.2lf %.2lf %.2lf\n",cl,cm,at,sb,st);
	p[0].setdata(0+1,cl,cm,ati,sb,st);
	for(++i;i<n;++i){
		scanf("%d %d %d %d %d",&cl,&cm,&ati,&sb,&st);
		//printf("%d %.2lf %.2lf %.2lf %.2lf\n",cl,cm,at,sb,st);
		p[i].setdata(i+1,cl,cm,ati,sb,st);
	}
	return ;
}

/*
功能：更新就绪队列中的进程的cls
*/
void updata_rdycls(int pid){
    for(int i=0;i<n;i++){
        if(p[i].pcb.state == 'R'){
            p[i].waittime++;
            if(p[i].waittime%p[i].pcb.startblock==0){
                p[i].pcb.cls++;
            }
        }
    }
    return ;
}

/*
功能: sort排序方法
描述：先按照时间到来的顺序进行升序排序，再按照进程的优先级升序排序
*/
bool cmp(process a,process b){
	if(a.pcb.come!=b.pcb.come) return a.pcb.come<b.pcb.come;
	return a.pcb.cls>b.pcb.cls;
}

/*
功能:实际情况处理进程
*/
void solve(){
	int num = 0;
	int now_time = 0;

	while(num<n){
		printf("当前时刻:%d\n",now_time);
		solve_ready(now_time);
		find_ready();
        find_block();
		/*
            当时如果没有进程在就绪队列中就跳过
		*/
		//cout<<"q1 :"<<q1.size()<<endl;
		//printf("11111   %d\n",q1.front());
		if(q1.empty()){
            now_time++;
            updata_block();
            continue;
		}

        int now = find_maxcls();

        if(now == -1) {
            now_time++;
            updata_block();
            continue;
        }

		p[now].pcb.state = 'N';//运行
		printf("进程%d开始运行\n",p[now].pcb.id);
		int ttime = min(p[now].pcb.alltime - p[now].pcb.cputime,p[now].pcb.startblock);
		int ttt = ttime;
		while(ttime){
            p[now].runtime++;
            p[now].pcb.cputime++;
            ttime--;
            now_time++;
            printf("当前时刻:%d\n",now_time);
            updata_block();
            solve_ready(now_time);
            updata_rdycls(p[now].pcb.id);
            printf("当前时刻:%d,进程%d正在运行\n",now_time,p[now].pcb.id);
            find_ready();
            find_block();
		}
		printf("进程%d运行了%d时间，进程结束\n",p[now].pcb.id,ttt);

		if(p[now].pcb.cputime == p[now].pcb.alltime){
            num++;
            p[now].pcb.state = 'F';
            printf("进程%d在%d时刻完成\n",p[now].pcb.id,now_time);
            updata_ready(p[now].pcb.id);
            continue;
		}

		if(p[now].runtime == p[now].pcb.startblock){
            q1.pop();
            q2.push(now);
            p[now].pcb.state = 'B';
            p[now].pcb.cls -= 3;
            printf("进程%d进入阻塞队列\n",p[now].pcb.id);
            updata_ready(p[now].pcb.id);
            p[now].runtime = 0;
            continue;
		}
	}
	printf("运行总时间为：%d\n",now_time);
	printf("已完成进程%d\n",num);
	return ;
}

/*
名称：主函数
*/
int main(){
	freopen("1.txt","r",stdin);
	input();
	sort(p,p+n,cmp);
	for(int i=0;i<n;i++){
		printf("%d %d %d %d %d %d\n",p[i].pcb.id,p[i].pcb.cls,p[i].pcb.come,p[i].pcb.alltime,p[i].pcb.startblock,p[i].pcb.starttime);
	}
	solve();
	return 0;
}

