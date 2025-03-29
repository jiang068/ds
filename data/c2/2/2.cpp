#include <iostream>
#include <string>
using namespace std;

// 定义数字栈和符号栈
struct NodeNum {
    int data;
    NodeNum* next;
};//数字栈

struct NodeOp {
    char data;
    NodeOp* next;
};//符号栈

void Push_L(NodeNum*& stack, int num) {
    NodeNum* newNode = new NodeNum{ num, stack };
    stack = newNode;
}//数字栈入栈

void Push_L(NodeOp*& stack, char op) {
    NodeOp* newNode = new NodeOp{ op, stack };
    stack = newNode;
}//符号栈入栈

bool Pop_L(NodeNum*& stack, int& num) {
    if (!stack) return false;
    num = stack->data;//弹出栈顶元素
    NodeNum* temp = stack;
    stack = stack->next;
    delete temp;//释放栈顶元素
    return true;
}//数字栈出栈

bool Pop_L(NodeOp*& stack, char& op) {
    if (!stack) return false;
    op = stack->data;//弹出栈顶元素
    NodeOp* temp = stack;
    stack = stack->next;
    delete temp;
    return true;
}//符号栈出栈

// 获取运算符的优先级
int Precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;//乘除优先级高
    return 0;
}

// 简单计算表达式的函数，不包含优先级的判断
void Calculate(NodeNum*& numStack, NodeOp*& opStack) {
    int num1, num2;//两个临时的操作数
    char op;

    if (!Pop_L(numStack, num2) || !Pop_L(numStack, num1)) {
        cout << "数字栈为空，无法继续计算！" << endl;
        return;
    }

    Pop_L(opStack, op);  // 弹出一个运算符

    //cout << "执行计算: " << num1 << " " << op << " " << num2 << endl;//调试接口
    if (op == '+') Push_L(numStack, num1 + num2);
    else if (op == '-') Push_L(numStack, num1 - num2);
    else if (op == '*') Push_L(numStack, num1 * num2);
    else if (op == '/') Push_L(numStack, num1 / num2);
}

void Procesexp(const string&exp) {//表达式优先级的处理

    cout << "处理表达式为: " << exp << endl;//这里是输入接口

    NodeNum* numStack = NULL;
    NodeOp* opStack = NULL;

    for (size_t i = 0; i <exp.size(); i++) {
        char ch =exp[i];//遍历表达式（有点违背初衷）

        // 处理多位数
        if (isdigit(ch)) {//检查是否为数字字符
            int num = 0;
            while (i <exp.size() && isdigit(exp[i])) {
                num = num * 10 + (exp[i] - '0');
                i++;//把数字还原成正确的位数
            }
            i--;  // 回退一位以抵消 for 循环的自增
            Push_L(numStack, num);  // 将完整数字压入数字栈
        }
        else if (ch == '(') {
            Push_L(opStack, ch);  // 将左括号压入符号栈
        }
        else if (ch == ')') {
            // 遇到右括号，处理括号内的表达式
            while (opStack && opStack->data != '(') {
                Calculate(numStack, opStack);
            }
            Pop_L(opStack, ch);  // 弹出左括号
        }
        else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
            // 处理运算符
            while (opStack && Precedence(opStack->data) >= Precedence(ch)) {
                Calculate(numStack, opStack);
            }
            Push_L(opStack, ch);  // 将运算符压入符号栈
        }
    }

    // 最后计算栈中的剩余操作
    while (opStack) {
        Calculate(numStack, opStack);
    }

    // 输出最终结果
    if (numStack) {
        cout << "结果为: " << numStack->data << endl;
    } else {
        cout << "错误！" << endl;
    }
}
//主函数
int main() {
    while (true) {
        string exp;
        cout << "请输入一个数学表达式: （不需要带#号, 输入等号“=”退出）\n";
        getline(cin, exp);  // 从键盘读取输入表达式
        // 判断用户是否输入了等号"="，如果是，则退出程序
        if (exp == "=") {
            cout << "退出程序。" << endl;
            break;  // 退出循环
        }
        
        Procesexp(exp);    // 处理表达式并计算
    }
    return 0;
}