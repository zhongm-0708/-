#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <stdio.h>
#include <stdlib.h>

#define M 20  // ��ʼ����
#define EPSILON 1e-6  // �������ȽϾ���

// ����ڵ�ṹ�壬���ڴ洢����ʽ��ÿһ�ϵ����ָ����
typedef struct {
    float coef; // ϵ��
    int exp;    // ָ��
} Node;

// ����˳���ṹ�壬���ڹ������ʽ
typedef struct {
    Node *data;    // ��̬���飬�洢����ʽ�ĸ���
    int last;      // ���һ��Ԫ�ص��±�
    int capacity;  // ��ǰ����
} SeqList;

// ������˳�����
SeqList* createSeqList();

// ������ϲ�ͬ�������ָ������
void insertTerm(SeqList *list, float coef, int exp);

// ����ʽ�ӷ�
SeqList* addPolynomials(SeqList *a, SeqList *b);

// ����ʽ����
SeqList* subtractPolynomials(SeqList *a, SeqList *b);

// ����ʽ�˷�
SeqList* multiplyPolynomials(SeqList *a, SeqList *b);

// ����ʽ����
SeqList* dividePolynomials(SeqList *a, SeqList *b);

// ��ӡ����ʽ
void printPolynomial(SeqList *list);

#endif // POLYNOMIAL_H
