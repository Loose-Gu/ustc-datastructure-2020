#include<stdio.h>
#include<stdlib.h>

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFIOW -2

typedef int Status;//Status 是函数类型，其值是函数结果状态码

#define STACK_INIT_SIZE 100
#define STACKINCREMENT 10
#define QUEUE_INIT_SIZE 100


typedef int ElemType;

typedef struct DuLNode{
    ElemType data;
    struct DuLNode* pre;
    struct DuLNode* next;
    int freq;//访问频度
}DuLNode, *DuLList;//双向链表

typedef struct DuStack{
    ElemType* base[2];//栈底指针
    ElemType* top[2];//栈顶指针
    int stacksize;//已分配的内存大小
}DuStack;//双向栈

typedef struct Stack{
    ElemType* base;//栈底指针
    ElemType* top;//栈顶指针
    int stacksize;//已分配的内存大小
}SqStack;

typedef struct Queue{
	int tag;
	int front, rear;
	ElemType data[QUEUE_INIT_SIZE];
}Queue;//队列

Status DuLLocate(DuLList &L,ElemType x){
    if(!L)
        return ERROR;
    //空指针检查
    DuLList cur,p;
    cur=L->next;
    while(cur){
        if(cur->data == x)
            break;
        cur=cur->next;
    }
    if(cur == L)
        return  OK;//不需要排序
    cur->freq++;
    p = cur -> pre;
    while (p -> pre <= cur->freq)
        p = p->pre;
    //找到插入位置
    if(p != cur->pre){
        cur->pre->next = cur->next;
        cur->next->pre = cur->pre;
        cur->next = p->next;
        p->next = cur;
        cur->pre = p;
        cur->next->pre = cur;
    }
    return OK;
}

Status InitDuStack(DuStack &tws){
    tws.base[0] = (ElemType*)malloc(sizeof(ElemType));
    tws.base[1] = tws.base[0] + STACK_INIT_SIZE;
    tws.top[0] = tws.base[0];
    tws.top[1] = tws.base[1];
    tws.stacksize=STACK_INIT_SIZE;
    return OK;
}

Status PushDuStack(DuStack &tws,int i,ElemType x){
    if(tws.top[0] > tws.top[1])
        return OVERFIOW;//检查是否栈满
    if(!i){
        tws.top[0]++;
        *tws.top[0] = x;
    }
    else{
        tws.top[1]--;
        *tws.top[1] = x;
    }
    return OK;
}

Status PopDuStack(DuStack tws,int i,ElemType &x){
    //x储存出栈的元素值
    ElemType* temp;
    if(!i){
        if(tws.top[0] == tws.base[0])
            return OVERFIOW;//空栈检查
        x=*tws.top[0];
        temp=tws.top[0];
        tws.top[0]--;
        free(temp);
    }
    else{
        if(tws.top[1] == tws.base[1])
            return OVERFIOW;
        x=*tws.top[1];
        temp=tws.top[1];
        tws.top[0]++;
        free(temp);
    }
    return OK;
}

Status BracketMatch(SqList L){
    SqStack S;
    InitStack(S);
    int i;
    char e=0;
    for(i=1;i<L.length;i++){
        if(L.elem[i]=='(' || L.elem[i]=='[' || L.elem[i]=='{')
            Push(S,L.elem[i]);
        //遇到左括号进栈
        if(L.elem[i]==')' || L.elem[i]==']' || L.elem[i]=='}'){
            Pop(S,e);
            if((e+1) != L.elem[i])
                return ERROR;
        }//遇到右括号出栈比较
    }
    if(!StackEmpty(S))
        return ERROR;//读完数据后仍遗留左括号，报错
    return OK;
}

int Ackerman(int m,int n){
    int g;
    if(!m)
        return n+1;
    if(m && !n)
        return Ackerman(m-1,1);
    g = Ackerman(m,n-1);
    return Ackerman(m-1,g);
}
 
Status EnQueue(Queue &Q,Elemtype x){
	if(Q.front==Q.rear && Q.tag==1)
        return ERROR;//判断队满
	Q.data[Q.rear]=x;
	Q.rear=(Q.rear+1)%QUEUE_INIT_SIZE;
	Q.tag=1;
	return OK;
}

Status DeQueue(SqQueue &Q,Elemtype &x){
	if(Q.front==Q.rear && Q.tag==0)
        return 0;//判断队空 
	x = Q.data[Q.front];
	Q.front = (Q.front+1)%QUEUE_INIT_SIZE;
	Q.tag=0;//重置tag
	return 1;
} 
