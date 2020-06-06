# Project_snake
算分project，强化学习贪吃蛇

//创建Q-table
struct Act{
    double a[4];
    Act(){
        a[0]=a[1]=a[2]=a[3]=0;
    }
};
map<string,Act>Qtable;

//实验参数
double learningRate=0.85;
double discount=0.9;
double randomize=0.05;
reward函数的赋值在 double go_ahead()中

通过调整以上一系列参数，进行了多次实验
