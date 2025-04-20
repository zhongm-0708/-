#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <stdio.h>
#include <stdlib.h>

#define M 20  // 初始容量
#define EPSILON 1e-6  // 浮点数比较精度

// 定义节点结构体，用于存储多项式的每一项（系数和指数）
typedef struct {
    float coef; // 系数
    int exp;    // 指数
} Node;

// 定义顺序表结构体，用于管理多项式
typedef struct {
    Node *data;    // 动态数组，存储多项式的各项
    int last;      // 最后一个元素的下标
    int capacity;  // 当前容量
} SeqList;

// 创建空顺序表函数
SeqList* createSeqList();

// 插入项并合并同类项，保持指数递增
void insertTerm(SeqList *list, float coef, int exp);

// 多项式加法
SeqList* addPolynomials(SeqList *a, SeqList *b);

// 多项式减法
SeqList* subtractPolynomials(SeqList *a, SeqList *b);

// 多项式乘法
SeqList* multiplyPolynomials(SeqList *a, SeqList *b);

// 多项式除法
SeqList* dividePolynomials(SeqList *a, SeqList *b);

// 打印多项式
void printPolynomial(SeqList *list);

#endif // POLYNOMIAL_H
