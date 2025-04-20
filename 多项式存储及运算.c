#include <stdio.h>   
#include <stdlib.h>  

#define M 20  // 初始容量，用于顺序表的动态数组

// 定义多项式的节点结构体
typedef struct//多项式 
{
    float coef; // 系数
    int exp;    // 指数
} 
Node;

// 定义顺序表结构体，用于存储多项式
typedef struct 
{
    Node *data;    // 动态数组，用于存储多项式的各项
    //Node data[M]; 静态数组大小为M 
    int last;      // 当前顺序表中最后一个元素的下标
    int capacity;  // 当前顺序表的容量
} 
SeqList;

// 创建空顺序表，初始化动态数组和相关变量
SeqList* createSeqList() {
    SeqList *list = (SeqList*)malloc(sizeof(SeqList));  // 分配顺序表内存
    //将seqList结构体占用的字节数，使用 (SeqList*)进行强制转换，定义指针变量lsit ，指向刚分配的SeqList内存块 
    list->data = (Node*)malloc(M * sizeof(Node));       // 分配初始容量为M的动态数组
    //静态数组需删除这一句 
    list->last = -1;                                    // 初始化last为-1，表示空表
    list->capacity = M;                                 // 设置初始容量为M
     //静态数组需删除这一句 
    return list;
}

// 插入项并合并同类项，保持指数递增
void insertTerm(SeqList *list, float coef, int exp) {
    if (coef == 0) return; // 忽略零系数项
    /*
    	if(list->last>=M-1)//静态数组已满，无法插入,下标是从0->19=20个数开始的 
	{
		printf("多项式超出最大容量%d，无法插入\n",M); 
		return ;
	 } 
	*/
    // 查找插入位置或合并同类项
    int i;
    for (i = 0; i <= list->last; i++) 
	{
        if (list->data[i].exp == exp) {  // 找到相同指数项
            list->data[i].coef += coef;  // 合并系数
            if (list->data[i].coef == 0) { // 如果合并后系数为0，删除该项
                for (int j = i; j < list->last; j++) {
                    list->data[j] = list->data[j + 1]; // 向前移动元素覆盖当前项
                }
                list->last--; // 更新最后一个元素下标
            }
            return;
        } else if (list->data[i].exp > exp) {
            break; // 找到插入位置i
        }
    }

    // 如果需要扩展容量,静态数组需删 
    if (list->last + 1 >= list->capacity) {
        int new_capacity = list->capacity * 2;  // 新容量为原来的两倍
        Node *new_data = (Node*)realloc(list->data, new_capacity * sizeof(Node));
        //使用realloc函数将顺序表存储空间从原大小 (list->data)扩容到new_capacity 个Node节点的大小
		//并将扩容后的新内存地址赋值给new_data指针 
        if (!new_data) {  // 检查内存分配是否成功
            printf("内存分配失败！\n");
            return;
        }
        list->data = new_data;         // 更新数据指针，下标 
        list->capacity = new_capacity; // 更新容量
    }

    // 后移元素并插入新项
    for (int j = list->last; j >= i; j--) 
     //list->last顺序表存储空间大小
      //插队,后面的人需要向后走 ，把第i的位置空出来我进去okk 
	{
        list->data[j + 1] = list->data[j]; // 向后移动元素
       //  .....2...3.......4...
       //.......2...3.......i(插队的位置）.....4+1=5.
    }
    list->data[i].coef = coef;  // 插入新项的系数
    list->data[i].exp = exp;    // 插入新项的指数
    list->last++;               // 更新最后一个元素下标
}

// 输入多项式
SeqList* inputPolynomial() {
    SeqList *poly = createSeqList();  // 创建新的顺序表
    float coef;
    int exp;
    printf("输入多项式项（系数 指数），以0 0结束：\n");
    while (1) {
        scanf("%f %d", &coef, &exp);  // 读取用户输入的系数和指数
        if (coef == 0 && exp == 0) break;  // 输入终止条件
        if (exp < 0) {  // 检查指数是否为负
            printf("指数不能为负，跳过此项。\n");
            continue;
        }
        insertTerm(poly, coef, exp);  // 插入项
    }
    return poly;
}

// 多项式加法
SeqList* addPolynomials(SeqList *a, SeqList *b) {
    SeqList *result = createSeqList();  // 创建结果多项式
    int i = 0, j = 0;
    while (i <= a->last && j <= b->last) {
        if (a->data[i].exp < b->data[j].exp) {  // 比较指数大小
            insertTerm(result, a->data[i].coef, a->data[i].exp);
            i++;
        } else if (a->data[i].exp > b->data[j].exp) {
            insertTerm(result, b->data[j].coef, b->data[j].exp);
            j++;
        } else {  // 指数相等时，系数相加
            float sum = a->data[i].coef + b->data[j].coef;
            if (sum != 0) {  // 忽略系数为0的项
                insertTerm(result, sum, a->data[i].exp);
            }
            i++;
            j++;
        }
    }
    // 处理剩余项
    while (i <= a->last) {
        insertTerm(result, a->data[i].coef, a->data[i].exp);
        i++;
    }
    while (j <= b->last) {
        insertTerm(result, b->data[j].coef, b->data[j].exp);
        j++;
    }
    return result;
}

// 多项式减法
SeqList* subtractPolynomials(SeqList *a, SeqList *b) {
    SeqList *result = createSeqList();  // 创建结果多项式
    int i = 0, j = 0;
    while (i <= a->last && j <= b->last) {
        if (a->data[i].exp < b->data[j].exp) {
            insertTerm(result, a->data[i].coef, a->data[i].exp);
            i++;
        } else if (a->data[i].exp > b->data[j].exp) {
            insertTerm(result, -b->data[j].coef, b->data[j].exp);  // 减法时系数取反
            j++;
        } else {
            float diff = a->data[i].coef - b->data[j].coef;
            if (diff != 0) {
                insertTerm(result, diff, a->data[i].exp);
            }
            i++;
            j++;
        }
    }
    // 处理剩余项
    while (i <= a->last) {
        insertTerm(result, a->data[i].coef, a->data[i].exp);
        i++;
    }
    while (j <= b->last) {
        insertTerm(result, -b->data[j].coef, b->data[j].exp);
        j++;
    }
    return result;
}

// 多项式乘法
SeqList* multiplyPolynomials(SeqList *a, SeqList *b) {
    SeqList *result = createSeqList();  // 创建结果多项式
    for (int i = 0; i <= a->last; i++) {
        for (int j = 0; j <= b->last; j++) {
            float coef = a->data[i].coef * b->data[j].coef;  // 系数相乘
            int exp = a->data[i].exp + b->data[j].exp;       // 指数相加
            insertTerm(result, coef, exp);                   // 插入结果
        }
    }
    return result;
}

// 打印多项式
void printPolynomial(SeqList *list) {
    if (list->last == -1) {  // 空多项式
        printf("0\n");
        return;
    }
    for (int i = 0; i <= list->last; i++) {
        float coef = list->data[i].coef;
        int exp = list->data[i].exp;

        // 处理符号
        if (i != 0) {
            if (coef > 0) printf(" + ");  // 正数项添加“+”
            else {
                printf(" - ");  // 负数项添加“-”
                coef = -coef;    // 取绝对值
            }
        } else if (coef < 0) printf("-");

        // 处理系数和指数
        if (exp == 0) printf("%.0f", coef);  // 常数项
        else if (exp == 1) printf("%.0fx", coef);  // 一次项
        else printf("%.0fx^%d", coef, exp);  // 高次项
    }
    printf("\n");
}

// 定义示例多项式 f(x)
SeqList* definePolynomialF() {
    SeqList *f = createSeqList();
    insertTerm(f, 8, 6);
    insertTerm(f, 4, 5);
    insertTerm(f, -2, 4);
    insertTerm(f, -123, 3);
    insertTerm(f, -1, 1);
    insertTerm(f, 10, 0);
    return f;
}

// 定义示例多项式 g(x)
SeqList* definePolynomialG() {
    SeqList *g = createSeqList();
    insertTerm(g, 2, 3);
    insertTerm(g, -5, 2);
    insertTerm(g, 1, 1);
    return g;
}

// 主函数
int main() {
    SeqList *f = NULL, *g = NULL, *result = NULL;
    int choice;

    do {
        printf("\n多项式运算菜单：\n");
        printf("1. 输入 f(x)\n");
        printf("2. 输入 g(x)\n");
        printf("3. 计算 f(x) + g(x)\n");
        printf("4. 计算 f(x) - g(x)\n");
        printf("5. 计算 f(x) * g(x)\n");
        printf("6. 使用举例多项式 f(x) 和 g(x)\n");
        printf("7. 退出\n");
        printf("请选择：");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                if (f) free(f->data), free(f);  // 释放之前分配的内存
                f = inputPolynomial();          // 输入多项式f
                printf("f(x) = "); printPolynomial(f);
                break;
            case 2:
                if (g) free(g->data), free(g);  // 释放之前分配的内存
                g = inputPolynomial();          // 输入多项式g
                printf("g(x) = "); printPolynomial(g);
                break;
            case 3:  // 加法
                if (!f || !g) printf("请先输入两个多项式！\n");
                else {
                    if (result) free(result->data), free(result);
                    result = addPolynomials(f, g);
                    printf("f(x) + g(x) = "); printPolynomial(result);
                }
                break;
            case 4:  // 减法
                if (!f || !g) printf("请先输入两个多项式！\n");
                else {
                    if (result) free(result->data), free(result);
                    result = subtractPolynomials(f, g);
                    printf("f(x) - g(x) = "); printPolynomial(result);
                }
                break;
            case 5:  // 乘法
                if (!f || !g) printf("请先输入两个多项式！\n");
                else {
                    if (result) free(result->data), free(result);
                    result = multiplyPolynomials(f, g);
                    printf("f(x) * g(x) = "); printPolynomial(result);
                }
                break;
            case 6:  // 使用示例多项式
                if (f) free(f->data), free(f);
                if (g) free(g->data), free(g);
                f = definePolynomialF();
                g = definePolynomialG();
                printf("已定义的多项式：\n");
                printf("f(x) = "); printPolynomial(f);
                printf("g(x) = "); printPolynomial(g);
                break;
            case 7:  // 退出程序
                printf("退出程序。\n");
                break;
            default:
                printf("无效选择！\n");
        }
    } while (choice != 7);

    // 释放内存
    if (f) free(f->data), free(f);
    //  if (f) free(f);只需释放结构体data，静态数组data无需单独处理 
    if (g) free(g->data), free(g);
    if (result) free(result->data), free(result);
    return 0;
}
