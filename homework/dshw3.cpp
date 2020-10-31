#include<stdio.h>
#include<stdlib.h>

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFIOW -2

typedef int Status;//Status 是函数类型，其值是函数结果状态码

#define CHUNKSIZE 80

typedef char Elemtype;

typedef struct Chunk{
    char ch[CHUNKSIZE];
    struct Chunk *next;
}Chunk;

typedef struct Stack{
    ElemType* base;//栈底指针
    ElemType* top;//栈顶指针
    int stacksize;//已分配的内存大小
}SqStack;

typedef struct{
    Chunk *head,*tail;//串的头和尾指针
    int curlen;//串的当前长度
}LString;

typedef struct OLMode{
    int i,j;//该非零元的行和列下标
    Elemtype e;
    struct OLNode *right,*down;//该非零元所在的行表和列表的后继链域
}OLNode; *OLink;

typedef struct {
    OLink *rhead,*chead;//行和列链表头指针向量基址由CreateSMatrix分配
    int mu,nu,tu;//稀疏矩阵的行数、列数和非零元个数
}CrossList;

Status ChunkJudge(LString T){
    //函数返回1表示对称
    int i,j,k;
    SqStack S;
    Elemtype e;
    Chunk*p;
    if(!T.curlen) 
        return OK;
    k=T.curlen/2;
    InitStack(S);
    p=T.head;
    j=0;
    for(i=1;i<=k;i++){
        Push(S,p->ch[j]);
        j = (j+1)%CHUNKSIZE;
        if(!j)
            p = p->next;
    }
    if(2*k!=T.curlen){
        //奇数跳过一个字符
        i++;
        j = (j+1)%CHUNKSIZE;
        if(!j)
            p = p->next;
    } 
    while(!StackEmpty(S) && i<=T.curlen){
        Pop(S,e);
        if(e!=p->ch[j])
            return ERROR;
        i++;
        j = (j+1)%CHUNKSIZE;
        if(!j)
            p = p->next;
    } 
    if(StackEmpty(S) && i>T.curlen) 
        return OK;//循环结束时满足栈空且串遍历完才是对称的
}

Status MatrixAddition(CrossList &A,CrossList B){
    int k;
    OLink* hl;
    OLink pa,pb,pre,p; 
    pre=NULL;
    if(A.mu!=B.mu || A.nu!=B.nu)
        return ERROR;
    for(k = 1;k<=A.nu;++j)
        hl[k]=A.chead[k];//初始化
    for(k=1;k<=A.mu;k++){
        pa=A.rhead[k];
        pb=B.rhead[k];
        while(pb!=NULL){
            if(pa && pa->j<pb->j){//无需插入继续前进
                pre = pa;
                pa = pa->right;
            }
            if(!pa || pa->j>pb->j){//需要向A插入一个结点
                p = (OLink)malloc(sizeof(OLNode));
                p->i=pb->i;
                p->j=pb->j;
                p->e=pb->e;
                p->right = pb->right;
                p->down = pb->down;
                if(pre == NULL)
                    A.rhead[p->i] = p;
                else 
                    pre->right = p;
                p->right = pa;
                pre = p;
                if(!A.chead[p->j] || A.chead[p->j]->i > p->i){
                    p->down = A.chead[p->j];
                    A.chead[p->j]=p;
                }
                else{
                    p->down = hl[p->j]->down;
                    hl[p->j]->down = p;
                }
                hl[p->j]=p;
            }
            if(pa->j == pb->j){//需要相加的结点
                pa->e+=pb->e;
                if(pa->e == 0){//如果结点相加后为0要释放
                    if(pre == NULL)
                        A.rhead[pa->i] = pa->right;
                    else 
                        pre->right = pa->right;
                    p = pa;
                    pa = pa->right;
                }
                if(A.chead[p->j] == p) 
                    A.chead[p->j] = hl[p->j]=p->down;
                else
                    hl[p->j]->down = p->down;
                free(p);
            }
        }
    }
    return OK;
}