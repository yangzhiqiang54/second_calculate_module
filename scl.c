/*
    ���μ���ģ��
    1.֧�ֵ�����������ȼ���
        ��ߣ�#ȡֵ 
        �ϸߣ�()���� b(n)ȡbitֵ
        �еȣ�~ȡ�� f(n)����λ(0��1��) ^ָ��
        �ϵͣ�*�� /��
        ��ͣ�+�� -�� 
    2.���������С��ȡ3λ��ЧС��λ
*/

#include "stdio.h"
#include "stdint.h"
#include "string.h"

#include "scl.h"

/**** start �ṹ���Զ��� ****/
typedef struct scl_stack {
    float val;      //��ֵ
    uint8_t type;   //0-NULL 1-ֵ 2-�����
    char symbol;    //�������
}scl_stack_typedef;
/***** end �ṹ���Զ��� ****/

/**** start �������� ****/
static scl_stack_typedef scl_stack_pre[50];   //Ԥ�����ջ
static scl_stack_typedef scl_stack_main[50];  //����ջ
static scl_stack_typedef scl_stack_temp[50];  //��ʱ��ջ
static uint16_t pre_top = 0, main_top = 0, temp_top = 0;
/**** end �������� ****/

/**** start �������� ****/

/**** end �������� ****/

/* ��ȡ��ջ�� */
static uint16_t get_stack_top(scl_stack_typedef *ps) {
    if(ps == scl_stack_pre) return pre_top;
    else if(ps == scl_stack_main) return main_top;
    else if(ps == scl_stack_temp) return temp_top;
    else return 0;
}

/* ��ջ���� */
static void in_stack(scl_stack_typedef *pstack, float v, uint8_t t, char s) {
    uint16_t num = get_stack_top(pstack);
    pstack[num].val = v;
    pstack[num].type = t;
    pstack[num].symbol = s;
    num++;
}

/* ��ջ���� */
static scl_stack_typedef out_stack(scl_stack_typedef *pstack) {
    uint16_t num = get_stack_top(pstack);
    if(num > 0) return pstack[num-1];
    else pstack[0];
}

/* ��ӡջȫ������ */
static void print_stack(scl_stack_typedef *pstack) {
    int i=0;
    uint16_t num = get_stack_top(pstack);

    if(pstack == scl_stack_pre) printf(" #pre stakc print: \n   num    type    val    symbol");
    else if(pstack == scl_stack_main) printf(" #main stakc print: \n");
    else if(pstack == scl_stack_temp) printf(" #temp stakc print: \n");
    else return;

    for(i=0; i<num; i++) {
        printf("   %d    %d    %f    %c \n", i+1, pstack[i].type, pstack[i].val, pstack[i].symbol);
    }
}

/* 
    �������ַ�������Ԥ���� 
    1.�жϱ�����[]ͷβ������
    2.ɾ���ո���޹��ַ�
    @str:����������ַ���
    @return:Ԥ������ 0�ɹ� -1ʧ��
*/
static int scl_prosolv(char* str) {
    int i=0, stlen=0;
    float f1 = 0;
    uint8_t weishu = 0;

    memset(scl_stack_pre, 0, sizeof(scl_stack_pre));
    memset(scl_stack_main, 0, sizeof(scl_stack_main));
    memset(scl_stack_temp, 0, sizeof(scl_stack_temp));

    stlen = strlen(str);
    if(stlen <=2 || str[0] != '[' || str[stlen-1] != ']') 
        return -1;

    /* �������ַ���ʽ����Ԥ����ջ�� */
    for(i=1; i<stlen-1; i++) { //����'['��']'
        if(str[i] >= '0' && str[i] <= '9') {

        }
    }

}








char srcstr[100] = {"#1+#2*#3-(#4-#5)"};
float exval[20] = {0};
void main(char argc, char* agrv[]) {

    int i=0;

    printf("  second calc module! \n\n");

    /* ģ��ɼ���λ���� */
    for(i=0; i<20; i++) {
        exval[i] = i+1;
        printf("#%d=%f ", i+1, exval[i]);
    }

    printf("\n\n src_str: %s \n", srcstr);
    scl_prosolv(srcstr);
    print_stack(scl_stack_pre);
}




