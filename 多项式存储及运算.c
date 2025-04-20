#include <stdio.h>   
#include <stdlib.h>  

#define M 20  // ��ʼ����������˳���Ķ�̬����

// �������ʽ�Ľڵ�ṹ��
typedef struct//����ʽ 
{
    float coef; // ϵ��
    int exp;    // ָ��
} 
Node;

// ����˳���ṹ�壬���ڴ洢����ʽ
typedef struct 
{
    Node *data;    // ��̬���飬���ڴ洢����ʽ�ĸ���
    //Node data[M]; ��̬�����СΪM 
    int last;      // ��ǰ˳��������һ��Ԫ�ص��±�
    int capacity;  // ��ǰ˳��������
} 
SeqList;

// ������˳�����ʼ����̬�������ر���
SeqList* createSeqList() {
    SeqList *list = (SeqList*)malloc(sizeof(SeqList));  // ����˳����ڴ�
    //��seqList�ṹ��ռ�õ��ֽ�����ʹ�� (SeqList*)����ǿ��ת��������ָ�����lsit ��ָ��շ����SeqList�ڴ�� 
    list->data = (Node*)malloc(M * sizeof(Node));       // �����ʼ����ΪM�Ķ�̬����
    //��̬������ɾ����һ�� 
    list->last = -1;                                    // ��ʼ��lastΪ-1����ʾ�ձ�
    list->capacity = M;                                 // ���ó�ʼ����ΪM
     //��̬������ɾ����һ�� 
    return list;
}

// ������ϲ�ͬ�������ָ������
void insertTerm(SeqList *list, float coef, int exp) {
    if (coef == 0) return; // ������ϵ����
    /*
    	if(list->last>=M-1)//��̬�����������޷�����,�±��Ǵ�0->19=20������ʼ�� 
	{
		printf("����ʽ�����������%d���޷�����\n",M); 
		return ;
	 } 
	*/
    // ���Ҳ���λ�û�ϲ�ͬ����
    int i;
    for (i = 0; i <= list->last; i++) 
	{
        if (list->data[i].exp == exp) {  // �ҵ���ָͬ����
            list->data[i].coef += coef;  // �ϲ�ϵ��
            if (list->data[i].coef == 0) { // ����ϲ���ϵ��Ϊ0��ɾ������
                for (int j = i; j < list->last; j++) {
                    list->data[j] = list->data[j + 1]; // ��ǰ�ƶ�Ԫ�ظ��ǵ�ǰ��
                }
                list->last--; // �������һ��Ԫ���±�
            }
            return;
        } else if (list->data[i].exp > exp) {
            break; // �ҵ�����λ��i
        }
    }

    // �����Ҫ��չ����,��̬������ɾ 
    if (list->last + 1 >= list->capacity) {
        int new_capacity = list->capacity * 2;  // ������Ϊԭ��������
        Node *new_data = (Node*)realloc(list->data, new_capacity * sizeof(Node));
        //ʹ��realloc������˳���洢�ռ��ԭ��С (list->data)���ݵ�new_capacity ��Node�ڵ�Ĵ�С
		//�������ݺ�����ڴ��ַ��ֵ��new_dataָ�� 
        if (!new_data) {  // ����ڴ�����Ƿ�ɹ�
            printf("�ڴ����ʧ�ܣ�\n");
            return;
        }
        list->data = new_data;         // ��������ָ�룬�±� 
        list->capacity = new_capacity; // ��������
    }

    // ����Ԫ�ز���������
    for (int j = list->last; j >= i; j--) 
     //list->last˳���洢�ռ��С
      //���,���������Ҫ����� ���ѵ�i��λ�ÿճ����ҽ�ȥokk 
	{
        list->data[j + 1] = list->data[j]; // ����ƶ�Ԫ��
       //  .....2...3.......4...
       //.......2...3.......i(��ӵ�λ�ã�.....4+1=5.
    }
    list->data[i].coef = coef;  // ���������ϵ��
    list->data[i].exp = exp;    // ���������ָ��
    list->last++;               // �������һ��Ԫ���±�
}

// �������ʽ
SeqList* inputPolynomial() {
    SeqList *poly = createSeqList();  // �����µ�˳���
    float coef;
    int exp;
    printf("�������ʽ�ϵ�� ָ��������0 0������\n");
    while (1) {
        scanf("%f %d", &coef, &exp);  // ��ȡ�û������ϵ����ָ��
        if (coef == 0 && exp == 0) break;  // ������ֹ����
        if (exp < 0) {  // ���ָ���Ƿ�Ϊ��
            printf("ָ������Ϊ�����������\n");
            continue;
        }
        insertTerm(poly, coef, exp);  // ������
    }
    return poly;
}

// ����ʽ�ӷ�
SeqList* addPolynomials(SeqList *a, SeqList *b) {
    SeqList *result = createSeqList();  // �����������ʽ
    int i = 0, j = 0;
    while (i <= a->last && j <= b->last) {
        if (a->data[i].exp < b->data[j].exp) {  // �Ƚ�ָ����С
            insertTerm(result, a->data[i].coef, a->data[i].exp);
            i++;
        } else if (a->data[i].exp > b->data[j].exp) {
            insertTerm(result, b->data[j].coef, b->data[j].exp);
            j++;
        } else {  // ָ�����ʱ��ϵ�����
            float sum = a->data[i].coef + b->data[j].coef;
            if (sum != 0) {  // ����ϵ��Ϊ0����
                insertTerm(result, sum, a->data[i].exp);
            }
            i++;
            j++;
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
    SeqList *result = createSeqList();  // �����������ʽ
    int i = 0, j = 0;
    while (i <= a->last && j <= b->last) {
        if (a->data[i].exp < b->data[j].exp) {
            insertTerm(result, a->data[i].coef, a->data[i].exp);
            i++;
        } else if (a->data[i].exp > b->data[j].exp) {
            insertTerm(result, -b->data[j].coef, b->data[j].exp);  // ����ʱϵ��ȡ��
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
    SeqList *result = createSeqList();  // �����������ʽ
    for (int i = 0; i <= a->last; i++) {
        for (int j = 0; j <= b->last; j++) {
            float coef = a->data[i].coef * b->data[j].coef;  // ϵ�����
            int exp = a->data[i].exp + b->data[j].exp;       // ָ�����
            insertTerm(result, coef, exp);                   // ������
        }
    }
    return result;
}

// ��ӡ����ʽ
void printPolynomial(SeqList *list) {
    if (list->last == -1) {  // �ն���ʽ
        printf("0\n");
        return;
    }
    for (int i = 0; i <= list->last; i++) {
        float coef = list->data[i].coef;
        int exp = list->data[i].exp;

        // �������
        if (i != 0) {
            if (coef > 0) printf(" + ");  // ��������ӡ�+��
            else {
                printf(" - ");  // ��������ӡ�-��
                coef = -coef;    // ȡ����ֵ
            }
        } else if (coef < 0) printf("-");

        // ����ϵ����ָ��
        if (exp == 0) printf("%.0f", coef);  // ������
        else if (exp == 1) printf("%.0fx", coef);  // һ����
        else printf("%.0fx^%d", coef, exp);  // �ߴ���
    }
    printf("\n");
}

// ����ʾ������ʽ f(x)
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

// ����ʾ������ʽ g(x)
SeqList* definePolynomialG() {
    SeqList *g = createSeqList();
    insertTerm(g, 2, 3);
    insertTerm(g, -5, 2);
    insertTerm(g, 1, 1);
    return g;
}

// ������
int main() {
    SeqList *f = NULL, *g = NULL, *result = NULL;
    int choice;

    do {
        printf("\n����ʽ����˵���\n");
        printf("1. ���� f(x)\n");
        printf("2. ���� g(x)\n");
        printf("3. ���� f(x) + g(x)\n");
        printf("4. ���� f(x) - g(x)\n");
        printf("5. ���� f(x) * g(x)\n");
        printf("6. ʹ�þ�������ʽ f(x) �� g(x)\n");
        printf("7. �˳�\n");
        printf("��ѡ��");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                if (f) free(f->data), free(f);  // �ͷ�֮ǰ������ڴ�
                f = inputPolynomial();          // �������ʽf
                printf("f(x) = "); printPolynomial(f);
                break;
            case 2:
                if (g) free(g->data), free(g);  // �ͷ�֮ǰ������ڴ�
                g = inputPolynomial();          // �������ʽg
                printf("g(x) = "); printPolynomial(g);
                break;
            case 3:  // �ӷ�
                if (!f || !g) printf("����������������ʽ��\n");
                else {
                    if (result) free(result->data), free(result);
                    result = addPolynomials(f, g);
                    printf("f(x) + g(x) = "); printPolynomial(result);
                }
                break;
            case 4:  // ����
                if (!f || !g) printf("����������������ʽ��\n");
                else {
                    if (result) free(result->data), free(result);
                    result = subtractPolynomials(f, g);
                    printf("f(x) - g(x) = "); printPolynomial(result);
                }
                break;
            case 5:  // �˷�
                if (!f || !g) printf("����������������ʽ��\n");
                else {
                    if (result) free(result->data), free(result);
                    result = multiplyPolynomials(f, g);
                    printf("f(x) * g(x) = "); printPolynomial(result);
                }
                break;
            case 6:  // ʹ��ʾ������ʽ
                if (f) free(f->data), free(f);
                if (g) free(g->data), free(g);
                f = definePolynomialF();
                g = definePolynomialG();
                printf("�Ѷ���Ķ���ʽ��\n");
                printf("f(x) = "); printPolynomial(f);
                printf("g(x) = "); printPolynomial(g);
                break;
            case 7:  // �˳�����
                printf("�˳�����\n");
                break;
            default:
                printf("��Чѡ��\n");
        }
    } while (choice != 7);

    // �ͷ��ڴ�
    if (f) free(f->data), free(f);
    //  if (f) free(f);ֻ���ͷŽṹ��data����̬����data���赥������ 
    if (g) free(g->data), free(g);
    if (result) free(result->data), free(result);
    return 0;
}
