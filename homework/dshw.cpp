#include<stdio.h>
#include<stdlib.h>

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFIOW -2

typedef int Status;//Status 是函数类型，其值是函数结果状态码

#define LIST_INIT_SIZE 100
#define LISTINCREMENT 10

typedef int ElemType;

typedef struct {
    ElemType *elem;//存储空间基址
    int length;//当前长度
    int listsize;//当前分配的存储容量
}Sqlist;

typedef struct LNode{
    ElemType data;
    struct LNode* next;
}LNode,*LinkList;


Status SqInsert(Sqlist &L,ElemType x){
    int i,j;
    if(!L.elem)
        return ERROR;//空表检查
    if(L.length == L.listsize){
        L.elem=(ElemType *)realloc(L.elem,sizeof(ElemType)*(L.length+LISTINCREMENT));
        L.listsize+=LISTINCREMENT;
        if(!L.elem){
            printf("realloc fail!\n");
            exit(-1);
        }//内存分配正确性检查
    }//如果原有顺序表内存空间已满需重新分配内存空间
    for(i = 0; i < L.length; i++)
        if(L.elem[i] > x)
            break;
    //找到x插入位置
    for(j = L.length; j > i; j--)
        L.elem[j]=L.elem[j-1];
    //将i后面元素后移++
    L.elem[i]=x;//插入
    L.length++;//当前长度++
    return OK;
}

Status Limited_DeLNode(LinkList &L, ElemType mink, ElemType maxk){
    LinkList pre,p,q,temp;
    if(!L)
        return ERROR;
    //空指针检查
    pre=L;//pre为p的前继结点
    p=L->next;//p用来记录开始删除的第一个结点
    while(p){
        if(p->data > mink)
            break;
        p = p->next;
        pre = pre->next;
    }
    if(!p){
        q=p->next;//用q记录被删除的最后一个结点的后一个结点
        while(!q){
            if(q->data >= maxk)
                break;
            q = q->next;
        }
    }
    while(p && p!=q){
        temp=p;
        p=p->next;
        free(temp);
    }//用temp储存临时结点释放，直到p空或到q为止
    pre->next=q;
    return OK;
}

Status SqInversion(Sqlist &L){
    //对顺序表实现就地逆置（不增加额外空间）
    int i,temp;
    if(!L.elem)
        return ERROR;//空指针检查
    for(i = 0 ; i < L.length/2 ; i++){
        temp = L.elem[i];
        L.elem[i] = L.elem[L.length-1-i];
        L.elem[L.length-i-1] = temp;
    }
    return OK;
}

Status LInversion(LinkList &L){
    //使用头插法逆置单链表
    if(!L)
        return ERROR;//空指针检查
    LinkList head,p,temp;
    head->next = NULL;//头结点置空
    p=L->next;
    while(p){
        temp = head->next;//temp储存头结点的后继结点
        head->next = p;
        head->next->next=temp;
        p=p->next;
    }
    free(L);
    L = head;
    return OK;
}

LinkList LInversionMerge(LinkList &La,LinkList &Lb){
    //先将La，Lb分别逆置，后进行归并操作
    if(!LInversion(La) || !LInversion(Lb))
        return ERROR;//空指针检查并分别逆置
    LinkList pre,cur,p;
    pre = La; 
    cur = La->next;
    p = Lb->next;
    while(p){
        while(cur){
            if(cur->data > p->data){
                pre = cur;
                cur = cur->next;
            }
            else
                break;//找到插入位置退出循环
        }
        pre->next = p;
        p = p->next;
        pre->next->next=cur;
        pre = pre->next;
    }//归并
    return La;
}

Status SqInitList(Sqlist &L){
    L.elem=(ElemType*)malloc(sizeof(ElemType)*LIST_INIT_SIZE);
    if(!L.elem)
        exit(OVERFIOW);
    L.length = 0;
    L.listsize = LIST_INIT_SIZE;
    return OK;
}

Status SqListCross(Sqlist A,Sqlist B,Sqlist &C){
    //将A与B的交放入C
    SqInitList(C);//将C初始化
    if(!A.elem || !B.elem)
        return ERROR;//空指针检查
    int i,j=0,flag = 0;
    //flag用来表示A中的某个元素是否是B中的值
    for(i = 0; i < A.length;i++){
        for(;j < B.length;j++){
            if(A.elem[i] == B.elem[j]){
                flag = 1;
                break;
            }
            else 
                if(A.elem[i] < B.elem[j])
                    break;
        }
        if(flag){
            if(C.length >= C.listsize){
                C.elem=(ElemType*)realloc(C.elem,sizeof(ElemType)*(C.listsize+LISTINCREMENT));
                C.listsize+=LISTINCREMENT;
            }//若C内存已满重新分配空间
            C.elem[C.length] = A.elem[i];
            C.length++;
            flag = 0;//flag复位
        }
    }
}


Status SqlistCross_Minus(Sqlist&A,Sqlist B, Sqlist C){
    //先求出B和C的交，再用A减去temp（A∩B）
    Sqlist temp;
    SqInitList(temp);//temp储存B与C的交
    SqListCross(B,C,temp);
    int i,j=0,k,flag = 0,counter_found=0;
    //flag用来表示A中元素是否在temp中
    //counter_found表示已找到的被删除的个数
    for(i = 0; i<A.length;i++){
        for(;j<temp.length;j++){
            if(A.elem[i] == temp.elem[j]){
                flag = 1;
                break;
            }
            else
                if(A.elem[i] < temp.elem[j])
                    break;
        }
        if(flag){
            counter_found++;
            flag = 0;//flag复位   
        }
        else{
            A.elem[i-counter_found] = A.elem[i];
        }
    }
    A.length=A.length-counter_found;
    return OK; 
}