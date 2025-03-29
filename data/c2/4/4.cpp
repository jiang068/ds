/*实验4 最短路径
1. 掌握图的存储结构。
2. 掌握Dijkstra算法或Floyd算法。
问题描述
给定全国铁路网，对于任意一对城市，找出它们之间的最短路径经过哪些城市，并输出最短路径的长度。
铁路网的信息可查看dist.txt，其格式为：
城市A编号 城市B编号 距离
城市编号和城市名称信息可查看city.txt，其格式为：
编号 城市名称
实验内容
1. 基本功能：图的存储结构使用邻接矩阵；可选做的高级功能：图的存储结构使用邻接表。
2. 求出下列城市之间的最短路径：沈阳至西安、呼和浩特至成都、上海至乌鲁木齐。
3. 从铁路网中删除一些城市（例如郑州），再重新计算上述城市之间的最短路径。*/

#include <stdio.h>
#include <iostream>// 用于输入输出
#include <stdlib.h>// 用于exit函数

using namespace std;

//定义图的存储结构
#define MAXVEX 100//最大顶点数
#define INFINITY 65535//用65535来代表无穷大

typedef struct {
    int num;
    char name[20];
} City;

typedef struct {
    City vexs[MAXVEX];//顶点表
    int arc[MAXVEX][MAXVEX];//邻接矩阵，可看作边表
    int numVertexes, numEdges;//图中当前的顶点数和边数
} MGraph;         //定义邻接矩阵结构

//抄书上的Dijkstra算法，但是debug时改动了一点
void Dijkstra(MGraph G, int v0, int *P, int *D) {//*P是前驱顶点数组，*D是最短路径长度数组
    int v, w, k, min;
    int final[MAXVEX];//final[w]=1表示求得顶点v0到vw的最短路径
    for (v = 0; v < G.numVertexes; v++) {
        final[v] = 0;//全部顶点初始化为未知最短路径状态
        D[v] = G.arc[v0][v];//将与v0点有连线的顶点加上权值
        P[v] = (G.arc[v0][v] < INFINITY && v != v0) ? v0 : -1; // 初始化前驱数组
    }
    D[v0] = 0;//v0到v0的路径为0
    final[v0] = 1;//v0到v0不需要求路径
    for (v = 1; v < G.numVertexes; v++) {//开始主循环，每次求得v0到某个v顶点的最短路径
        min = INFINITY;//当前所知离v0顶点的最近距离
        for (w = 0; w < G.numVertexes; w++) {//寻找离v0最近的顶点
            if (!final[w] && D[w] < min) {
                k = w;
                min = D[w];//w顶点离v0顶点更近
            }
        }
        if (min == INFINITY) break; // 如果所有剩余节点不可达，直接退出
        final[k] = 1;
        for (w = 0; w < G.numVertexes; w++) {
            if (!final[w] && (min + G.arc[k][w] < D[w])) {
                D[w] = min + G.arc[k][w];//修改D[w]和P[w]
                P[w] = k;
            }
        }
    }
}

//读取城市信息
void ReadFile_city(MGraph &G) {
    FILE *fp;
    if ((fp = fopen("city.txt", "r")) == NULL) {
        printf("city.txt文件打开失败，程序退出。\n");
        exit(0);
    }
    int i = 0;
    while (fscanf(fp, "%d %s", &G.vexs[i].num, G.vexs[i].name) == 2 && i < MAXVEX) {
        i++;
    }
    fclose(fp);
    G.numVertexes = i; // 设置有效的顶点数
    cout << "city.txt读取完毕。" << endl;
}

//读取城市间距离信息
void Readfile_dist(MGraph &G) {
    FILE *fp;
    if ((fp = fopen("dist.txt", "r")) == NULL) {
        printf("dist.txt文件打开失败，程序退出。\n");
        exit(0);
    }
    int v1, v2, w;
    for (int i = 0; i < MAXVEX; i++) {
        for (int j = 0; j < MAXVEX; j++) {
            G.arc[i][j] = INFINITY;
        }
    }
    while (fscanf(fp, "%d %d %d", &v1, &v2, &w) == 3) { // 读取有效记录
        G.arc[v1][v2] = w;
        G.arc[v2][v1] = w;
    }
    fclose(fp);
    cout << "dist.txt读取完毕。" << endl;
}

//检验文件是否正确读取
void TestFile(MGraph G) {
    cout<<"检验city.txt和dist.txt是否正确读取:" << endl;
    cout<<"城市编号 城市名称 相邻城市编号A 距离A 相邻城市编号B 距离B ……"<<endl;
    for (int i = 0; i < G.numVertexes; i++) {
        printf("%d %s", G.vexs[i].num, G.vexs[i].name);
        for (int j = 0; j < G.numVertexes; j++) {
            if (G.arc[i][j] != INFINITY) {
                printf(" %d %d", j, G.arc[i][j]);
            }
        }
        printf("\n");
    }
}

//删除城市及其链接的铁路
void DeleteCity(MGraph &G, int v) {
    for (int i = 0; i < G.numVertexes; i++) {
        G.arc[v][i] = INFINITY;
        G.arc[i][v] = INFINITY;
    }
    cout << "城市" << G.vexs[v].name << "删除成功。" << endl;
}

//主函数
int main() {
    cout << "读取文件city.txt和dist.txt中。。。" << endl;
    MGraph G;
    ReadFile_city(G);
    Readfile_dist(G);
    TestFile(G);
    cout << "是否正确读取？\n正确输入【1】继续，错误输入【0】break：" << endl;
    int flag;
    cin >> flag;
    if (flag == 0) {
        cout << "程序结束。" << endl;
        return 0;
    }
    //==========================第一题
    while (1) {
        int v1, v2;
        int P[MAXVEX], D[MAXVEX];
        printf("请输入两个城市的数字编号：\n");
        scanf("%d %d", &v1, &v2);
        Dijkstra(G, v1, P, D);
        if (D[v2] == INFINITY) {
            printf("无路径\n");
        }else{
            printf("从%s到%s的最短路径长度为：%d\n", G.vexs[v1].name, G.vexs[v2].name, D[v2]);
            printf("路径为：");
            while (v2 != v1) {
                if (v2 == -1) { // 检查前驱是否有效
                    printf("无效路径\n");
                    break;
                }
                printf("%s ", G.vexs[v2].name);
                v2 = P[v2];
            }
            if (v2 == v1) {
                printf("%s\n", G.vexs[v1].name);
            }
        }
        cout<<"输入【1】再来一次，输入【0】进入【删除城市】步骤"<<endl;
        int flag;
        cin>>flag;
        if(flag == 0){
            break;
        }
    }
    //==========================第二题
    while (1) {
        cout << "请输入想要删除的城市编号(删除后其他城市序号不变)\n(输入【-1】进入【距离路径计算】步骤)：" << endl;
        int v;
        cin >> v;
        if (v == -1) {
            break;
        }
        DeleteCity(G, v);
    }
    //==========================第三题
    while (1) {
        int v1, v2;
        int P[MAXVEX], D[MAXVEX];
        printf("请输入两个城市的数字编号：\n");
        scanf("%d %d", &v1, &v2);
        Dijkstra(G, v1, P, D);
        if (D[v2] == INFINITY) {
            printf("无路径\n");
        }else{
            printf("从%s到%s的最短路径长度为：%d\n", G.vexs[v1].name, G.vexs[v2].name, D[v2]);
            printf("路径为：");
            while (v2 != v1) {
                if (v2 == -1) { // 检查前驱是否有效
                    printf("无效路径\n");
                    break;
                }
                printf("%s ", G.vexs[v2].name);
                v2 = P[v2];
            }
            if (v2 == v1) {
                printf("%s\n", G.vexs[v1].name);
            }
        }
        cout<<"再来一次请输入【1】，退出请输入【0】"<<endl;
        int flag;
        cin>>flag;
        if(flag == 0){
            break;
        }
    }
    cout<<"程序退出。"<<endl;
    return 0;
}