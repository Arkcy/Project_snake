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

在后续实验中，我们调整参数进行了多次实验，发现在有限的训练时间内，吃到食物的奖励难以影响到足够远的状态
为了解决这一问题，引入了蛇头到食物的曼哈顿距离的倒数作为奖励值函数来引导蛇接近食物
并在此基础上继续调整参数进行实验，进一步更新了reward函数的赋值，得到了最终表现较好的snake_manhattan
