/*** 调整参数实验用的代码，引入了曼哈顿距离 ***/
#include<bits/stdc++.h>
using namespace std;

double random(double start, double end){
    return start+(end-start)*rand()/(RAND_MAX + 1.0);
}
 
/*** 定义地图的长宽，蛇的坐标，长度，方向，食物的位置 ***/
int m,n;
 
struct node{int x,y;}snake[1000];
 
int snake_length,dir;
node food;
int direct[4][2]={{-1,0},{1,0},{0,-1},{0,1}};

char st[105][105];

/*** 输出墙 ***/
void print_wall(){
    st[0][0]=st[0][n+1]=st[m+1][0]=st[m+1][n+1]=' ';
    for(int i=1;i<=n;i++)
		st[0][i]=st[m+1][i]='-';
    for(int j=1;j<=m;j++)
		st[j][0]=st[j][n+1]='|';
    for(int i=1;i<=n;i++)
    for(int j=1;j<=m;j++)
		st[i][j]=' ';
}
 
/*** 首次输出蛇，其中snake[0]代表头 ***/
void print_snake(){
    st[snake[0].x][snake[0].y]='@';
    for(int i=1;i<=snake_length-1;i++)
        st[snake[i].x][snake[i].y]='*';
}
 
/*** 判断是否撞墙或者自撞 ***/
bool is_correct(){
    if(snake[0].x==0 || snake[0].y==0 || snake[0].x==m+1 || snake[0].y==n+1) return false;
    for(int i=1;i<=snake_length-1;i++){
        if (snake[0].x==snake[i].x && snake[0].y==snake[i].y) return false;
    }
    return true;
}
 
/*** 随机生成并输出食物位置 ***/
bool print_food(){
    srand((unsigned)time(0));
    bool e;
    while(true){
        e=true;
        int i=(int)random(0,m)+1,j=(int) random(0,n)+1;
        food.x=i;food.y=j;
        for(int k=0;k<=snake_length-1;k++){
            if(snake[k].x==food.x && snake[k].y==food.y){
				e=false;break;
            }
        }
        if(e) break;
    }
    st[food.x][food.y]='$';
    return true;
}

/*** 蛇的前进 ***/
double go_ahead(){
    double reward=0;
    node temp;
    temp=snake[snake_length-1];
    for(int i=snake_length-1;i;i--)
        snake[i]=snake[i-1];
    snake[0].x+=direct[dir][0];
    snake[0].y+=direct[dir][1];
    st[snake[1].x][snake[1].y]='*';
    /*** 吃到了食物 ***/
    if(snake[0].x==food.x&&snake[0].y==food.y){
        snake_length++;
        st[temp.x][temp.y]=' ';
        reward=2;
        snake[snake_length-1]=temp;
        print_food();
    }
    /*** 其他情况下reward正比于曼哈顿距离的倒数 ***/
    else {
        st[temp.x][temp.y]=' ';
		reward=0.1/(abs(snake[0].x-food.x)+abs(snake[0].y-food.y)); 
    }
    st[snake[0].x][snake[0].y]='@';
    /*** 自撞 ***/
    if(!is_correct()) reward=-2;
    return reward;
}
void show_it(){
    system("cls");
    for(int i=0;i<=n+1;i++)
        printf("%s\n",st[i]);
}
struct Act{
    double a[4];
    Act(){a[0]=a[1]=a[2]=a[3]=0;}
};
map<string,Act>Qtable;
double learningRate=0.85;
double discount=0.9;
double randomize=0.05;

//状态生成
string now_state(){
    string st="";
    st+=food.x;st+=food.y;
    for(int i=0;i<snake_length;i++){
        st+=snake[i].x;
        st+=snake[i].y;
    }
    return st;
}
int Action(string s){
    Act q=Qtable[s];
    if(rand()/(RAND_MAX+1.0)<randomize){
        int randAct=rand()%4;
        return randAct;
    }
    double maxValue=q.a[0];
    int action=0,act0[4],l=0;
    for(int i=0;i<4;i++){
        if(q.a[i]==maxValue)act0[l++]=i;
        if(q.a[i]>maxValue){
            maxValue=q.a[i];
            action=i;
            l=1;act0[0]=i;
        }
    }
    return act0[rand()%l];
}
double MaxAct(Act q){
    double mx=q.a[0];
    for(int i=1;i<4;i++)mx=max(mx,q.a[i]);
    return mx;
}
int sh=0;
void updateQtable(string s0,string s1,double reward,int act){
    Act q0=Qtable[s0];
    Act q1=Qtable[s1];
    double newValue = reward + discount*MaxAct(q1)-q0.a[act];
    Qtable[s0].a[act]=q0.a[act]+learningRate * newValue;
    if(sh){
        for(int i=0;i<4;i++)
			printf("%.3lf ",Qtable[s0].a[i]);
        puts("");
    }
}

int Mxlength;
void init(){
    /*** 数据全部初始化，包括蛇长，位置，方向 ***/
	snake_length=4;
    for(int i=0;i<snake_length;i++)
        snake[i].x=n/2,snake[i].y=4-i;
    dir=3;
    print_wall();
    print_food();
    print_snake();
}
/*** 主函数 ***/
int ccc=0;
int main(){
    int Time;
    puts("请输入m，n，和学习时长(s)");
    cin>>m>>n>>Time;
    if(m<4||n<4||m>25||n>40){
        cout<<"ERROR"<<endl;
        return 0;
    }
    clock_t a,b,T1,T2;
    char ch;
    /*** 输出初始地图，蛇与食物 ***/
    system("cls");
    /*** 开始游戏 ***/
    print_wall();
    print_food();
    print_snake();
    show_it();
    T1=clock();
    int TT=1;
    while(true){
        init();
        T2=clock();
		if(T2-T1>Time*1000)sh=1;
        else if(T2-T1>TT*1000){
            printf("%d\n",TT);
            TT++;
        }
	    while(true){
	        /*** 调节时间，单位是ms ***/
	        if(sh){
	        	a=clock();
	        	while(true){
	            	b=clock();
	            	if(b-a>=300)break;
	        	}
	        }
	        string nowState=now_state();
	        int action=Action(nowState);   
	        int fx=rand()%4;
	            switch(action){
	            case 0:
	                if(dir==2||dir==3)dir=0;break;
	            case 1:
	                if(dir==2||dir==3)dir=1;break;
	            case 2:
	                if(dir==0||dir==1)dir=2;break;
	            case 3:
	                if(dir==0||dir==1)dir=3;break;
	            }
	        /*** 前进 ***/
	        double reward=go_ahead();
	        string nextState=now_state();
	        if(reward<=-1)nextState="0";
	        if(sh)show_it();
	        updateQtable(nowState,nextState,reward,action);
	        if(reward<=-1)break;
	        Mxlength=max(Mxlength,snake_length);
	    }
	    if(sh){
		    printf("%d %d\n",snake_length,Mxlength);
		    a=clock();
		    while(true){
		        b=clock();
		        if(b-a>200)break;
		    }
		}
    }
}
