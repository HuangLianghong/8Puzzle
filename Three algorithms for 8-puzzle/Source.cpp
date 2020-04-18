#include "Timer.h"
#include <iostream>
#include <vector>
#include <string>
#define col 3
#define row 3
#define maxnum 10000
#define maxdistance 10000
using namespace std;

class node {
public:
    int number[row][col];
    string str;
    int distances;
    int depth;
    int findex;

    node() {

    }
    int dis();
    void voluation(int index);
    bool isend();
    bool isequal(node q);
};
vector <node> v;
node father, intent;
#include <iostream>
#include <queue>
#include <stack>
#include <vector>
#include <algorithm>
#include <memory.h>

using namespace std;

// 八数码状态
typedef struct _Status {
    int status[3][3];
    _Status* parent;
    _Status* next;
    string action;
}Status;

// AStar排序依据
bool decComparator(const Status& s1, const Status& s2) {
    int gn1 = 0, gn2 = 0;
    int dn1 = 0, dn2 = 0;
    const Status* ptr1 = &s1;
    const Status* ptr2 = &s2;
    int status[3][3] = { 0,1,2,3,4,5,6,7,8 };
    while (ptr1 != NULL) {
        gn1 += 1;
        ptr1 = ptr1->parent;
    }
    while (ptr2 != NULL) {
        gn2 += 1;
        ptr2 = ptr2->parent;
    }
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (s1.status[i][j] != status[i][j]) {
                dn1 += 1;
            }
            if (s2.status[i][j] != status[i][j]) {
                dn2 += 1;
            }
        }
    }
    return (gn1 + dn1) > (gn2 + dn2);
}

// 八数码搜索
class EightPuzzle {
private:
    unsigned char allHash[362880];
    Status root;
    Status goal;
private:
    //下一可行状态的数量
    int nextNumber;
    Status next[4];
public:
    EightPuzzle(Status* root, Status* goal) {
        memcpy(&this->root.status, &root->status, sizeof(int) * 9);
        this->root.parent = NULL;
        this->root.next = NULL;
        memcpy(&this->goal.status, &goal->status, sizeof(int) * 9);
        this->goal.parent = NULL;
        this->goal.next = NULL;
    }
private:
    // 判断是否是目标状态
    inline int IsGoal(Status* tmp) {
        return memcmp(&tmp->status, &goal.status, sizeof(int) * 9);
    }
    // 下一个可行的状态
    int NextStatus(Status* tmp) {
        nextNumber = 0;
        int posi, posj;
        for (int i = 0; i < 9; i++) {
            posi = i / 3, posj = i - i / 3 * 3;
            if (tmp->status[posi][posj] == 0) {
                break;
            }
        }
        if (posi - 1 >= 0) {
            Status left = *tmp;
            left.status[posi][posj] = left.status[posi - 1][posj];
            left.status[posi - 1][posj] = 0;
            left.action = "up";
            if (allHash[Cantor(left.status)] == 0) {
                next[nextNumber] = left;
                next[nextNumber].parent = tmp;
                nextNumber++;
            }
        }
        if (posi + 1 <= 2) {
            Status right = *tmp;
            right.status[posi][posj] = right.status[posi + 1][posj];
            right.status[posi + 1][posj] = 0;
            right.action = "down";
            if (allHash[Cantor(right.status)] == 0) {
                next[nextNumber] = right;
                next[nextNumber].parent = tmp;
                nextNumber++;
            }
        }
        if (posj - 1 >= 0) {
            Status up = *tmp;
            up.status[posi][posj] = up.status[posi][posj - 1];
            up.status[posi][posj - 1] = 0;
            up.action = "left";
            if (allHash[Cantor(up.status)] == 0) {
                next[nextNumber] = up;
                next[nextNumber].parent = tmp;
                nextNumber++;
            }
        }
        if (posj + 1 <= 2) {
            Status down = *tmp;
            down.status[posi][posj] = down.status[posi][posj + 1];
            down.status[posi][posj + 1] = 0;
            down.action = "right";
            if (allHash[Cantor(down.status)] == 0) {
                next[nextNumber] = down;
                next[nextNumber].parent = tmp;
                nextNumber++;
            }
        }
        return nextNumber;
    }
    // 康托展开
    int Cantor(int arr[][3]) {
        int fac[10] = { 1,1,2,6,24,120,720,5040,40320,362880 };
        int index = 0;
        for (int i = 7; i >= 0; i--) {
            int irow = i / 3, icol = i - i / 3 * 3;
            int count = 0;
            for (int j = 8; j > i; j--) {
                int jrow = j / 3, jcol = j - j / 3 * 3;
                if (arr[jrow][jcol] < arr[irow][icol]) {
                    count++;
                }
            }
            index += (count * fac[8 - i]);
        }
        return index;
    }
private:
    Timer BFSTime, DFSTime, ASTime;
    int stepNum;
public:
    // 广度优先搜索
    int BFS() {
        BFSTime.start();
        stepNum = 0;
        int step = 0;
        memset(allHash, 0, 362880);
        queue<Status> openTable;
        Status* closeTable = new Status;;
        Status* current = closeTable;
        Status* tmp;
        openTable.push(root);
        allHash[Cantor(root.status)] == 1;
        while (!openTable.empty()) {
            tmp = new Status;
            *tmp = openTable.front();
            openTable.pop();
            step++;
            current->next = tmp;
            current = current->next;
            if (IsGoal(tmp) == 0) {
                PrintPath(tmp);
                freeCloseTable(closeTable);
                BFSTime.end();
                return step;
            }
            int nextNumber = NextStatus(tmp);
            if (nextNumber == 0) {
                continue;
            }
            for (int i = 0; i < nextNumber; i++) {
                openTable.push(next[i]);
                allHash[Cantor(next[i].status)] = 1;
            }
        }
        cout << "BFS failed." << endl;
        freeCloseTable(closeTable);
        
        return -1;
    }
    // 深度优先搜索
    int DFS() {
        DFSTime.start();
        stepNum = 0;
        int depth = 0;
        int step = 0;
        stack<Status> openTable;
        Status* closeTable = new Status;;
        Status* current = closeTable;
        Status* last;
        Status* tmp;
        openTable.push(root);
        while (!openTable.empty()) {
            tmp = new Status;
            *tmp = openTable.top();
            openTable.pop();
            step++;
            current->next = tmp;
            current = current->next;
            if (IsGoal(tmp) == 0) {
                PrintPath(tmp);
                freeCloseTable(closeTable);
                DFSTime.end();
                return step;
            }
            memset(allHash, 0, 362880);
            last = tmp;
            depth = 0;
            while (last != NULL) {
                allHash[Cantor(last->status)] = 1;
                last = last->parent;
                depth++;
            }
            
            //可用于限制搜索深度
            if (depth > 100) {
                continue;
            }
            int nextNumber = NextStatus(tmp);
            if (nextNumber == 0) {
                continue;
            }
            for (int i = 0; i < nextNumber; i++) {
                openTable.push(next[i]);
            }
        }
        cout << "DFS failed." << endl;
        freeCloseTable(closeTable);
        
        return -1;
    }
    // 启发式搜索
    int AStar() {
        ASTime.start();
        stepNum = 0;
        int step = 0;
        memset(allHash, 0, 362880);
        vector<Status> openTable;
        Status* closeTable = new Status;;
        Status* current = closeTable;
        Status* tmp;
        openTable.push_back(root);
        allHash[Cantor(root.status)] == 1;
        while (!openTable.empty()) {
            tmp = new Status;
            *tmp = openTable[openTable.size() - 1];
            openTable.pop_back();
            step++;
            current->next = tmp;
            current = current->next;
            if (IsGoal(tmp) == 0) {
                PrintPath(tmp);
                freeCloseTable(closeTable);
                ASTime.end();
                return step;
            }
            int nextNumber = NextStatus(tmp);
            if (nextNumber == 0) {
                continue;
            }
            for (int i = 0; i < nextNumber; i++) {
                openTable.push_back(next[i]);
                allHash[Cantor(next[i].status)] = 1;
            }
            sort(openTable.begin(), openTable.end(), decComparator);
        }
        cout << "AStar failed." << endl;
        freeCloseTable(closeTable);
        
        return -1;
    }
    // 打印时间
    void PrintBFSTime(){
        cout << "BFS耗时：" << BFSTime.aver_time() << endl;
    }
    void PrintDFSTime() {
        cout << "DFS耗时：" << DFSTime.aver_time() << endl;
    }
    void PrintAStime() {
        cout << "A*耗时：" << ASTime.aver_time() << endl;
    }
private:
    // 打印路径
    void PrintPath(Status* head) {
        if (head == NULL) {
            return;
        }
        else {
            PrintPath(head->parent);
            if (stepNum == 0) stepNum++;
            else
                cout <<"step"<< stepNum++ <<":" << head->action << endl;
        }
    }
    // 释放close表
    void freeCloseTable(Status* closeTable) {
        Status* current;
        while (closeTable != NULL) {
            current = closeTable->next;
            free(closeTable);
            closeTable = current;
        }
    }
};

int main()
{
    Status init = { 7,2,4,5,0,6,8,3,1,0,NULL };
    Status goal = { 0,1,2,3,4,5,6,7,8,0,NULL };
    EightPuzzle ep = EightPuzzle(&init, &goal);
    cout << "BFS********" << endl;
    cout << "Total search steps: " << ep.BFS() << endl;
    ep.PrintBFSTime();
    cout << "***********\n" << endl;

    cout << "DFS********" << endl;
    cout << "Total search steps: " << ep.DFS() << endl;
    ep.PrintDFSTime();
    cout << "***********\n" << endl;

    cout << "AStar******" << endl;
    cout << "Total search steps: " << ep.AStar() << endl;
    ep.PrintAStime();
    cout << "***********\n" << endl;
    return 0;
}
