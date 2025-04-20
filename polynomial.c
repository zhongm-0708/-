#include "polynomial.h"

// 创建空顺序表函数
SeqList* createSeqList() {
    SeqList *list = (SeqList*)malloc(sizeof(SeqList));  // 分配顺序表内存
    list->data = (Node*)malloc(M * sizeof(Node));       // 初始化动态数组
    list->last = -1;                                    // 设置最后一个元素下标为-1，表示目前为空
    list->capacity = M;                                 // 设置初始容量
    return list;
}

// 插入项并合并同类项，保持指数递增
void insertTerm(SeqList *list, float coef, int exp) {
    if (coef == 0) return; // 忽略零系数项

    // 查找插入位置或合并同类项
    int i;
    for (i = 0; i <= list->last; i++) {
        if (list->data[i].exp == exp) {  // 找到相同指数项
            list->data[i].coef += coef;  // 合并系数
            if (list->data[i].coef == 0) { // 如果合并后的系数为0，则删除该项
                int j;
                for (j = i; j < list->last; j++) {
                    list->data[j] = list->data[j + 1]; // 向前移动元素覆盖当前项
                }
                list->last--; // 更新最后一个元素下标
            }
            return;
        } else if (list->data[i].exp > exp) { // 找到插入点
            break;
        }
    }

    // 扩展容量
    if (list->last + 1 >= list->capacity) {
        int new_capacity = list->capacity * 2;
        Node *new_data = (Node*)realloc(list->data, new_capacity * sizeof(Node));//realloc分配空间
        if (!new_data) {
            printf("内存分配失败！\n");
            return;
        }
        list->data = new_data;
        list->capacity = new_capacity;
    }

    // 插入新项
    int j;
    for (j = list->last; j >= i; j--) {
        list->data[j + 1] = list->data[j];
    }
    list->data[i].coef = coef;
    list->data[i].exp = exp;
    list->last++;
}

// 多项式加法
SeqList* addPolynomials(SeqList *a, SeqList *b) {
    SeqList *result = createSeqList();
    int i = 0, j = 0;
    while (i <= a->last && j <= b->last) {
        if (a->data[i].exp < b->data[j].exp) {
            insertTerm(result, a->data[i].coef, a->data[i].exp);
            i++;
        } else if (a->data[i].exp > b->data[j].exp) {
            insertTerm(result, b->data[j].coef, b->data[j].exp);
            j++;
        } else {
            float sum = a->data[i].coef + b->data[j].coef;
            if (sum != 0) {
                insertTerm(result, sum, a->data[i].exp);
            }
            i++; j++;
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
    SeqList *result = createSeqList();
    int i = 0, j = 0;
    while (i <= a->last && j <= b->last) {
        if (a->data[i].exp < b->data[j].exp) {
            insertTerm(result, a->data[i].coef, a->data[i].exp);
            i++;
        } else if (a->data[i].exp > b->data[j].exp) {
            insertTerm(result, -b->data[j].coef, b->data[j].exp);
            j++;
        } else {
            float diff = a->data[i].coef - b->data[j].coef;
            if (diff != 0) {
                insertTerm(result, diff, a->data[i].exp);
            }
            i++; j++;
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
    SeqList *result = createSeqList();
    int i, j;
    for (i = 0; i <= a->last; i++) {
        for (j = 0; j <= b->last; j++) {
            float coef = a->data[i].coef * b->data[j].coef;
            int exp = a->data[i].exp + b->data[j].exp;
            insertTerm(result, coef, exp);
        }
    }
    return result;
}

// 多项式除法
SeqList* dividePolynomials(SeqList *a, SeqList *b) {
    SeqList *quotient = createSeqList();  // 商
    SeqList *remainder = createSeqList(); // 余数
    SeqList *temp = createSeqList();     // 临时多项式

    // 初始化余数为被除数a
    int i;
    for (i = 0; i <= a->last; i++) {
        insertTerm(remainder, a->data[i].coef, a->data[i].exp);
    }

    // 当余数的最高次项大于等于除数的最高次项时，继续除法
    while (remainder->last != -1 && remainder->data[remainder->last].exp >= b->data[b->last].exp) {
        // 计算当前商的一项
        float coef = remainder->data[remainder->last].coef / b->data[b->last].coef;
        int exp = remainder->data[remainder->last].exp - b->data[b->last].exp;
        insertTerm(quotient, coef, exp);

        // 计算当前商项乘以除数
        for (i = 0; i <= b->last; i++) {
            insertTerm(temp, coef * b->data[i].coef, exp + b->data[i].exp);
        }

        // 余数减去temp
        remainder = subtractPolynomials(remainder, temp);

        // 清空temp
        free(temp->data);
        temp->last = -1;
    }

    // 释放临时多项式
    free(temp->data);
    free(temp);

    // 返回商
    return quotient;
}

// 打印多项式
void printPolynomial(SeqList *list) {
    if (list->last == -1) {
        printf("0\n");
        return;
    }
    int i;
    for (i = 0; i <= list->last; i++) {
        float coef = list->data[i].coef;
        int exp = list->data[i].exp;
        // 处理符号
        if (i != 0) {
            if (coef > 0) printf(" + ");
            else {
                printf(" - ");
                coef = -coef;
            }
        } else if (coef < 0) printf("-");
        // 处理系数和指数
        if (exp == 0) printf("%.0f", coef);
        else if (exp == 1) printf("%.0fx", coef);
        else printf("%.0fx^%d", coef, exp);
    }
    printf("\n");
}
