#include <stdio.h>
#include <iostream> // 用于输入输出

//#include <list>//使用list函数库

using namespace std; // c++常规写法
typedef int ElemType;

typedef struct LNode {
    ElemType data;
    struct LNode *next;
} LNode, *LinkList; // LinkList是一个单链表（结构体）

// =====================================================(1)创建l1和l2两个表代表两个一元多项式
void CreatList(LinkList *head, LinkList *head2){
    *head = NULL; // 初始化表头
    *head2 = NULL;
    LinkList tail = NULL; // 初始化尾指针
    LinkList tail2 = NULL;
    int n; // 表长
    // 确定两个多项式的每项系数
    printf("两个多项式中次数【最高】的项是几次幂？\n");
    scanf("%d", &n); // 用最高次数+1确定表长，二次就表长为3
    ++n;
    /*
    printf("第二个多项式中次数最高的项是几次？\n");
    int m;
    scanf("%d", &m);
    ++m;
    if (m > n) n = m; // 比较两个式子长度，取最长的做表长
    */
    // 接下来把链表创建起来
    printf("请按照【升幂】顺序将多项式的所有系数在下方输入，缺的幂次【用0补全】但不要跳过不写：\n注意常数项是【第0项】，算【第0次幂】。所以你应该先输入常数。\n");
    printf("请输入第一个式子的各项系数：\n");
    int i;
    int tmp;
    for (i = 0; i < n; ++i) {
        scanf("%d", &tmp); // 传入系数
        LNode *newNode = new LNode; // 更新为c++写法
        newNode->data=tmp;
        newNode->next=NULL;

        if(*head == NULL){
            *head = newNode; // 第一个节点
            tail = newNode; // 初始化尾指针
        }else{
            tail->next = newNode; // 将新节点添加到链表末尾
            tail = newNode; // 更新尾指针
        }
    }

    printf("请输入第二个式子的各项系数：\n");
    for(i = 0; i < n; ++i){
        scanf("%d", &tmp);
        LNode *newNode = new LNode; // 是c++语言的写法
        newNode->data = tmp; // 用于传递数据
        newNode->next = NULL;
        if(*head2 == NULL) {
            *head2=newNode; // 第一个节点
            tail2 = newNode; // 初始化尾指针
        }else{
            tail2->next = newNode; // 将新节点添加到链表末尾
            tail2 = newNode; // 更新尾指针
        }
    }
}

// ========================================================(2)打印显示多项式L1和L2和L3之类的等等
void PrintList(LinkList head){ // 需要打印哪个就引用哪个L，后续可以直接输入表名调用输出
    LNode *p = head;
    int exponent = 0;
    bool first = true;
    while(p){
        if(p->data != 0) {
            if(!first) {
                if (p->data > 0){
                    printf(" +"); // 用+把正数项连接起来
                }else{
                    printf(" -"); // 用-把负数项连接起来
                }
            }else{
                if(p->data < 0){
                    printf("-"); // 第一个负数项前不需要空格
                }
            }
            int absData=abs(p->data); // 取绝对值
            if(exponent == 0){
                printf("%d", absData);
            }else if(exponent == 1){
                printf("%dx", absData); // 系数
            }else{
                printf("%dx^%d", absData, exponent); // 系数和幂次
            }
            first = false;
        }
        p = p->next;
        exponent++;
    }
    printf("\n");
}

// ======================================================(3)销毁指定多项式==========case:4
void DestroyList(LinkList *head){ // 销毁需要引用指针地址，吗？
    LNode *p = *head;
    while(p){
        LNode *q = p->next;
        delete p;
        p = q;
    }
    *head = NULL;
}

// =================================================================(4)求L1和L2多项式的和
void AddList(LinkList L1, LinkList L2, LinkList *L3){
    LNode *p = L1;
    LNode *q = L2;
    LNode *s = NULL;
    *L3 = NULL;
    while(p && q){
        LNode *newNode = new LNode; // 更新为c++写法
        newNode->next = NULL;
        if(*L3 == NULL){
            *L3 = newNode;
            s = newNode;
        }else{
            s->next = newNode;
            s = newNode;
        }
        // 对应幂次的项进行系数相加
        s->data = p->data + q->data;
        p = p->next;
        q = q->next;
    }
    // 如果L1还有剩余项
    while(p){
        LNode *newNode = new LNode; // 更新c++写法
        newNode->data = p->data;
        newNode->next = NULL;
        if(*L3 == NULL) {
            *L3 = newNode;
            s = newNode;
        } else {
            s->next = newNode;
            s = newNode;
        }
        p = p->next;
    }
    // 如果L2还有剩余项
    while(q){
        LNode *newNode = new LNode; // c++写法
        newNode->data = q->data;
        newNode->next = NULL;
        if(*L3 == NULL){
            *L3 = newNode;
            s = newNode;
        }else{
            s->next = newNode;
            s = newNode;
        }
        q = q->next;
    }
}
// ===================================================================(5)求两个多项式的差
void SubList(LinkList L1, LinkList L2, LinkList *L3){
    LNode *p = L1;
    LNode *q = L2;
    LNode *s = NULL;
    *L3 = NULL;
    while(p && q){
        LNode *newNode = new LNode; // 更新为c++写法
        newNode->next = NULL;
        if (*L3 == NULL) {
            *L3 = newNode;
            s = newNode;
        } else {
            s->next = newNode;
            s = newNode;
        }
        // 对应幂次的项进行系数相加
        s->data = p->data - q->data;     //默认L1减去L2
        p = p->next;
        q = q->next;
    }
    // 如果L1还有剩余项
    while(p){
        LNode *newNode = new LNode; // 更新c++写法
        newNode->data = p->data;
        newNode->next = NULL;
        if (*L3 == NULL) {
            *L3 = newNode;
            s = newNode;
        } else {
            s->next = newNode;
            s = newNode;
        }
        p = p->next;
    }
    // 如果L2还有剩余项，需要转化成相反数
    while(q){
        LNode *newNode = new LNode; // c++写法
        newNode->data = -q->data;
        newNode->next = NULL;
        if (*L3 == NULL) {
            *L3 = newNode;
            s = newNode;
        }else{
            s->next = newNode;
            s = newNode;
        }
        q = q->next;
    }

}

// =====================================================================(6)求两个多项式的积
void MulList(LinkList L1, LinkList L2, LinkList *L3){
    *L3 = NULL;
    LNode *s = NULL;  // 定义s

    // 计算L1和L2的最大幂次
    /*
    发现用不了ListLength函数，所以只好自己写了
    // 使用ListLength函数获取L1和L2的长度
    int len1 = ListLength(L1);
    int len2 = ListLength(L2);
    */
    int len1 = 0, len2 = 0;
    LNode *p = L1;
    while(p){
        len1++;
        p = p->next;
    }

    LNode *q = L2;
    while(q){
        len2++;
        q = q->next;
    }
    // 最大幂次之和决定结果数组大小
    int maxExp = len1 + len2 - 1;
    int *result = new int[maxExp]();  // 初始化为0
    // 创建一个临时数组来存储结果，以使计算方便（。因为不会直接算出来）

    // 计算多项式乘积
    p = L1;
    int exp1 = 0;  // exp1是第一个式子里的幂次
    while (p) {
        q = L2;
        int exp2 = 0;  // exp2是第二个式子里的幂次
        while(q){
            result[exp1 + exp2] += p->data * q->data;
            //在对应幂次的数组位置上加进去系数的乘积的计算结果
            //没有的幂次默认为空
            q = q->next;
            exp2++;
        }
        p = p->next;
        exp1++;
    }

    // 将结果数组转换为链表L3
    for(int i = 0; i < maxExp; i++) {
        // 即使系数为0，也需要保留位置，但不需要输出为链表节点
        LNode *newNode = new LNode;
        newNode->data = result[i];  // 保存数组中的结果到链表节点中
        newNode->next = NULL;
        if (*L3 == NULL) {
            *L3 = newNode;
            s = newNode;
        } else {
            s->next = newNode;
            s = newNode;
        }
    }
    //delete[] result;  // 释放数组内存，但是目前会报错
}

int main(){ // ================================================================计算入口
    LinkList l1, l2, l3;
    printf("程序开始！\n");
    while(1){
        CreatList(&l1, &l2);
        printf("下面显示你刚刚输入的多项式：\n");
        PrintList(l1);
        PrintList(l2);
        printf("正确吗？\n输入1：正确！可以继续操作；\n输入2：重新输入！\n");
        int choose1;
        scanf("%d", &choose1);
        switch(choose1) {
            case 1:
                // 继续操作，跳出循环
                break;
            case 2:
                DestroyList(&l1);
                DestroyList(&l2);
                printf("L1和L2销毁成功！请重新输入！\n");
                continue; // 重新输入
            default:
                printf("输入错误，请重新选择！\n");
                continue; // 重新输入
        }
        break; // 跳出while循环
    }
    while(1){
        printf("操作菜单：\n输入你想进行的操作：\n");
        printf("输入1：求和\n输入2：求差\n输入3：求积\n输入4：销毁\n输入5：退出\n");
        int op;
        scanf("%d", &op);
        switch (op) {
            case 1:
                AddList(l1, l2, &l3);
                printf("求和结果：\n");
                PrintList(l3);
                break;
            case 2:
                SubList(l1, l2, &l3);
                printf("求差结果：\n");
                PrintList(l3);
                break;
            case 3:
                MulList(l1, l2, &l3);
                printf("求积结果：\n");
                PrintList(l3);
                break;
            case 4:
                DestroyList(&l1);
                DestroyList(&l2);
                DestroyList(&l3);
                printf("L1、L2和L3销毁成功！\n");
                break;
            case 5:
                printf("程序结束！\n");
                return 0;
            default:
                printf("输入错误！请重新输入！\n");
                break;
        }
        printf("还要做其他操作吗？\n输入1：回到操作菜单\n输入2：退出\n");
        int op2;
        scanf("%d", &op2);
        if (op2 != 1) {
            printf("程序结束或操作错误意外退出\n");
            break;
        }
    }
    return 0;
}