#include "polynomial.h"

// ������˳�����
SeqList* createSeqList() {
    SeqList *list = (SeqList*)malloc(sizeof(SeqList));  // ����˳����ڴ�
    list->data = (Node*)malloc(M * sizeof(Node));       // ��ʼ����̬����
    list->last = -1;                                    // �������һ��Ԫ���±�Ϊ-1����ʾĿǰΪ��
    list->capacity = M;                                 // ���ó�ʼ����
    return list;
}

// ������ϲ�ͬ�������ָ������
void insertTerm(SeqList *list, float coef, int exp) {
    if (coef == 0) return; // ������ϵ����

    // ���Ҳ���λ�û�ϲ�ͬ����
    int i;
    for (i = 0; i <= list->last; i++) {
        if (list->data[i].exp == exp) {  // �ҵ���ָͬ����
            list->data[i].coef += coef;  // �ϲ�ϵ��
            if (list->data[i].coef == 0) { // ����ϲ����ϵ��Ϊ0����ɾ������
                int j;
                for (j = i; j < list->last; j++) {
                    list->data[j] = list->data[j + 1]; // ��ǰ�ƶ�Ԫ�ظ��ǵ�ǰ��
                }
                list->last--; // �������һ��Ԫ���±�
            }
            return;
        } else if (list->data[i].exp > exp) { // �ҵ������
            break;
        }
    }

    // ��չ����
    if (list->last + 1 >= list->capacity) {
        int new_capacity = list->capacity * 2;
        Node *new_data = (Node*)realloc(list->data, new_capacity * sizeof(Node));//realloc����ռ�
        if (!new_data) {
            printf("�ڴ����ʧ�ܣ�\n");
            return;
        }
        list->data = new_data;
        list->capacity = new_capacity;
    }

    // ��������
    int j;
    for (j = list->last; j >= i; j--) {
        list->data[j + 1] = list->data[j];
    }
    list->data[i].coef = coef;
    list->data[i].exp = exp;
    list->last++;
}

// ����ʽ�ӷ�
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
    // ����ʣ����
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

// ����ʽ����
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
    // ����ʣ����
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

// ����ʽ�˷�
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

// ����ʽ����
SeqList* dividePolynomials(SeqList *a, SeqList *b) {
    SeqList *quotient = createSeqList();  // ��
    SeqList *remainder = createSeqList(); // ����
    SeqList *temp = createSeqList();     // ��ʱ����ʽ

    // ��ʼ������Ϊ������a
    int i;
    for (i = 0; i <= a->last; i++) {
        insertTerm(remainder, a->data[i].coef, a->data[i].exp);
    }

    // ����������ߴ�����ڵ��ڳ�������ߴ���ʱ����������
    while (remainder->last != -1 && remainder->data[remainder->last].exp >= b->data[b->last].exp) {
        // ���㵱ǰ�̵�һ��
        float coef = remainder->data[remainder->last].coef / b->data[b->last].coef;
        int exp = remainder->data[remainder->last].exp - b->data[b->last].exp;
        insertTerm(quotient, coef, exp);

        // ���㵱ǰ������Գ���
        for (i = 0; i <= b->last; i++) {
            insertTerm(temp, coef * b->data[i].coef, exp + b->data[i].exp);
        }

        // ������ȥtemp
        remainder = subtractPolynomials(remainder, temp);

        // ���temp
        free(temp->data);
        temp->last = -1;
    }

    // �ͷ���ʱ����ʽ
    free(temp->data);
    free(temp);

    // ������
    return quotient;
}

// ��ӡ����ʽ
void printPolynomial(SeqList *list) {
    if (list->last == -1) {
        printf("0\n");
        return;
    }
    int i;
    for (i = 0; i <= list->last; i++) {
        float coef = list->data[i].coef;
        int exp = list->data[i].exp;
        // �������
        if (i != 0) {
            if (coef > 0) printf(" + ");
            else {
                printf(" - ");
                coef = -coef;
            }
        } else if (coef < 0) printf("-");
        // ����ϵ����ָ��
        if (exp == 0) printf("%.0f", coef);
        else if (exp == 1) printf("%.0fx", coef);
        else printf("%.0fx^%d", coef, exp);
    }
    printf("\n");
}
