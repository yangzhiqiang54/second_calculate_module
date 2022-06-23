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
// static scl_stack_typedef scl_stack_temp[50];  //��ʱ��ջ
static uint16_t pre_top = 0;
static uint16_t main_top = 0;
// static uint16_t temp_top = 0;
/**** end �������� ****/

/**** start �������� ****/

/**** end �������� ****/

/* ģ���ʼ������ */
static void scl_init(void) {
    memset(scl_stack_pre, 0, sizeof(scl_stack_pre));
    memset(scl_stack_main, 0, sizeof(scl_stack_main));
    // memset(scl_stack_temp, 0, sizeof(scl_stack_temp));
    pre_top = 0;
    main_top = 0;
    // temp_top = 0;
}

/* ��ȡ��ջ�� */
static uint16_t* get_stack_top(scl_stack_typedef *ps) {
    if(ps == scl_stack_pre) return &pre_top;
    else if(ps == scl_stack_main) return &main_top;
    // else if(ps == scl_stack_temp) return &temp_top;
    else return 0;
}

/* ��ջ���� @t:1ֵ 2�����*/
static void in_stack(scl_stack_typedef *pstack, float v, uint8_t t, char s) {
    uint16_t *pnum = get_stack_top(pstack);
    pstack[*pnum].val = v;
    pstack[*pnum].type = t;
    pstack[*pnum].symbol = s;
    (*pnum)++;
}

/* ��ջ���� */
static scl_stack_typedef out_stack(scl_stack_typedef *pstack) {
    scl_stack_typedef sta = {0};
    uint16_t *pnum = get_stack_top(pstack);
    if((*pnum) > 0) {
        (*pnum) -= 1;
        sta = pstack[(*pnum)];
        pstack[(*pnum)].val = 0;
        pstack[(*pnum)].type = 0;
        pstack[(*pnum)].symbol = 0;
        return sta;
    }
    else pstack[0];
}

/* ָ��λ��ȡջ���� */
static scl_stack_typedef out_stack_index(scl_stack_typedef *pstack, int num) {
    
}

/* ��ӡջȫ������ */
static void print_stack(scl_stack_typedef *pstack) {
    int i=0;
    uint16_t num = *get_stack_top(pstack);

    if(pstack == scl_stack_pre) printf("\n#pre stakc print: \nnum  type  val    symbol\n");
    else if(pstack == scl_stack_main) printf("\n#main stakc print: \nnum  type  val    symbol\n");
    // else if(pstack == scl_stack_temp) printf("\n#temp stakc print: \nnum  type  val    symbol\n");
    else return;

    for(i=0; i<num; i++) {
        printf("%-4d %-4d  %4.3f  %c\n", i, pstack[i].type, pstack[i].val, pstack[i].symbol);
    }
}

/* ���ַ���ָ��λ��ȡ�����������֣��������� */
static int get_num(char *pch) {
    int i=0,temp=0;
    if(pch[0] < '1' || pch[0] > '9') {
        return -1;
    }
    temp = pch[0] - 0x30;
    i++;
    while(pch[i]>='0' && pch[i]<='9') {
        temp = temp * 10 + (pch[i] - 0x30);
        i++;
    }
    return temp;
}

/* ��ȡһ���������ֵ�λ�� */
static int get_num_digit(int num) {
    uint8_t pos=0;
    if(num == 0) {
        pos = 0;
    }
    pos = 1;
    while(num / 10) {
        pos++;
        num /= 10;
    }
    return pos;
}

const float basedata[20] = {1,2,3,4,5,6,7,8,9,10,1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8,1.9};

/* 
    �������ַ�������Ԥ���� 
    1.�жϱ�����[]ͷβ������
    2.���ű��밴����˳��ɶԳ���
    3.ɾ���ո���޹��ַ�
    4.�������ţ�����򱨴��Ƴ�
    @str:����������ַ���
    @return:Ԥ������ 0�ɹ� -1ʧ��
*/
static int scl_presolv(char* str) {
    int res = 0, i=0, j=0, stlen=0;
    scl_stack_typedef st_scl_temp;

    /* �ж��Ƿ��� [ ] ���� */
    stlen = strlen(str);
    if(stlen <=2 || str[0] != '[' || str[stlen-1] != ']') {
        res = -1;
        goto EXIT;
    }

    /* �ж������Ƿ�����˳��ɶԳ��� */
    for(i=0,j=0; i<stlen; i++) {
        if(str[i] == '(') j++;
        if(str[i] == ')') j--;
        if(j < 0) {
            res = -2;
            goto EXIT;
        }
    }
    if(j != 0) {
        res = -2;
        goto EXIT;
    }

    /* ȥ����Ч�ַ����ո� */
    for(i=0; i<stlen; i++) {
        if(str[i] == ' ') {
            memcpy(str+i, str+i+1, stlen-i);
            j++;
        }
    }
    stlen -= j; //ȥ����Ч�ַ�����ַ�������

    /* ������������ */
    const char continutchar[10] = {'+','-','*','/','b','f','~','^'};
    for(i=1; i<stlen-1; i++) {
        if(strchr(continutchar, str[i]) != NULL) {
            if(strchr(continutchar, str[i+1]) != NULL) {
                res = -3;
                goto EXIT;
            }
        }
    }
        
    /* �������ַ���ʽ����Ԥ����ջ�� */
    int numid, numdigit;
    for(i=1; i<stlen-1; i++) { //����'['��']'
        switch(str[i]) {
            case '#':
                numid = get_num(&str[i+1]);
                /* #����idֵ���� */
                if(numid <= 0) {
                    i += 1;
                    break;
                }
                /* ��ջ */
                in_stack(scl_stack_pre, basedata[numid-1], 1, 0);
                /* �ַ������ƫ�� λ�� */
                numdigit = get_num_digit(numid);
                i += numdigit;
            break;

            case '+':
            case '-':
            case '*':
            case '/':
            case '(':
            case ')':
            case 'b':
            case 'f':
            case '~':
            case '^':
                /* ���������ջ */
                in_stack(scl_stack_pre, 0, 2, str[i]);
            break;

            default:break;
        }
    }

    EXIT:
    return res;
}

/*
    ���г�ջ������������������
*/
static void scl_cacl(scl_stack_typedef *tar_stack) {
    int i, pre_statck_num;
    scl_stack_typedef st_cur, st_cur2, st_temp[10];
    float cval=0; 

    /* ��Ԥ����ջ�����ȡ�����м��㣨�������ң� */
    pre_statck_num = *get_stack_top(tar_stack);
    for(int i=0; i<pre_statck_num; i++) {
        st_cur = tar_stack[i];
        if(st_cur.type == 1) {
            /* ��ֵ����ջ���ȴ�������� */
            in_stack(scl_stack_main, st_cur.val, st_cur.type, st_cur.symbol);
        }
        else if(st_cur.type == 2) {
            /* ������� */
            /* +��- ����ջ�����ͳһ���� */
            if(st_cur.symbol == '+' || st_cur.symbol == '-') {
                in_stack(scl_stack_main, st_cur.val, st_cur.type, st_cur.symbol);
            }
            /* �˳����� */
            else if(st_cur.symbol == '*' || st_cur.symbol == '/') {
                /* *��/��������ֵ��ֱ������ */
                if(tar_stack[i+1].type == 1) {
                    st_cur2 = out_stack(scl_stack_main);
                    if(st_cur.symbol == '*') {
                        cval = st_cur2.val * tar_stack[i+1].val;
                    }else if(st_cur.symbol == '/') {
                        cval = st_cur2.val / tar_stack[i+1].val;
                    }
                    i++;
                    /* ��*��/�����Ľ����ջ */
                    in_stack(scl_stack_main, cval, 1, 0);
                }
                /* *��/��������������Ȱѵ�ǰ������ջ */
                else {
                    in_stack(scl_stack_main, st_cur.val, st_cur.type, st_cur.symbol);
                }
            }
            /* ����������������ʱջ */
            else if(st_cur.symbol == '(') {

            }
            /* ���������ż�����ʱջ��������� */
            else if(st_cur.symbol == ')') {
                
            }
        }
    }

#if 1
    /* �ж��Ƿ�ֻʣ���ֺͼӼ� */
    uint8_t num_add_reduce_flag = 0;
    for(i=0; i<main_top; i++) {
        if(scl_stack_main[i].type == 2) {
            if(scl_stack_main[i].symbol == '(') {
                num_add_reduce_flag = 1;
            }
        }
    }
#endif

    /* ������������ţ����еݹ����� */
    // if(num_add_reduce_flag) {

    // }

    /* ����ʣ������ֺͼӼ����ţ��������� */


    print_stack(scl_stack_main);

}


char srcstr[50] = {"[#15 + #2*#3 -(#4-#5)]"};
char srcstr2[50] = {"[#1 + #2 * (#3-#5)]"};
float exval[20] = {0};
void main(char argc, char* agrv[]) {
    printf("  second calc module! \n\n");

    int i=0;
    
    /* ģ��ɼ���λ���� */
    for(i=0; i<20; i++) {
        exval[i] = i+1;
        if(i % 5 == 0) printf("\n");
        printf("#%d=%.3f ", i+1, exval[i]);
    }

    // scl_init();
    // printf("\n\n src_str: %s \n", srcstr);
    // scl_prosolv(srcstr);

    scl_init();
    printf("\n\n src_str: %s \n", srcstr2);
    if(scl_presolv(srcstr2) < 0) {
        printf("calc string error: \"%s\"\n", srcstr2);
    }
    print_stack(scl_stack_pre);

    // scl_cacl(scl_stack_pre);
    
}




