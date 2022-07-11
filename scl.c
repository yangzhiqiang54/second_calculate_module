/*
    ���μ���ģ��
    1.֧�ֵ�����������ȼ���
        ��ߣ�#ȡֵ 
        �ϸߣ�()���� 
        �еȣ�bn()ȡbitֵ  ~ȡ��  ^10Ϊ�׵�ָ��  f()����λ(0��1��) 
        �ϵͣ�*�� /��
        ��ͣ�+�� -�� 
    2.���������С��ȡ3λ��ЧС��λ
    3.����ʹ�÷��ŵĻ��߷��Ŵ���̶���ֵ�ģ�����#��������Ҫ������������
    4.����f�������������
    5.ȡbitλbn()��n��0��ʼ
*/

#include "stdio.h"
#include "stdint.h"
#include "string.h"
#include "math.h"

#include "scl.h"

/*�궨��*/
#define STACK_TOPMAXNUM 50

/**** start �ṹ���Զ��� ****/
typedef struct scl_stack {
    float val;      //��ֵ
    uint8_t type;   //0-NULL 1-��ֵ 2-�����
    char symbol;    //�������
    uint8_t syb_val;    //����ֵ
}scl_stack_typedef;
/***** end �ṹ���Զ��� ****/

/**** start �������� ****/
static scl_stack_typedef scl_stack_pre[STACK_TOPMAXNUM];   //Ԥ�����ջ
// static scl_stack_typedef scl_stack_main[STACK_TOPMAXNUM];  //����ջ
// static scl_stack_typedef scl_stack_temp[STACK_TOPMAXNUM];  //��ʱ��ջ
static uint16_t pre_top = 0;
// static uint16_t main_top = 0;
// static uint16_t temp_top = 0;
/**** end �������� ****/

/**** start �������� ****/

/**** end �������� ****/

/* ģ���ʼ������ */
static void scl_init(void) {
    memset(scl_stack_pre, 0, sizeof(scl_stack_pre));
    // memset(scl_stack_main, 0, sizeof(scl_stack_main));
    // memset(scl_stack_temp, 0, sizeof(scl_stack_temp));
    pre_top = 0;
    // main_top = 0;
    // temp_top = 0;
}

/* ��ȡ��ջ�� */
static uint16_t* get_stack_top(scl_stack_typedef *ps) {
    if(ps == scl_stack_pre) return &pre_top;
    // else if(ps == scl_stack_main) return &main_top;
    // else if(ps == scl_stack_temp) return &temp_top;
    else return 0;
}

/* ��ջ���� @t:1ֵ 2�����*/
static void in_stack(scl_stack_typedef *pstack, uint16_t *topnum, float v, uint8_t t, char s, uint8_t sval) {
    pstack[*topnum].val = v;
    pstack[*topnum].type = t;
    pstack[*topnum].symbol = s;
    pstack[*topnum].syb_val = sval;
    (*topnum)++;
}

/* ��ջ���� */
static scl_stack_typedef* out_stack(scl_stack_typedef *pstack, uint16_t *topnum) {
    if((*topnum) > 0) {
        (*topnum) -= 1;
        return (pstack + *topnum);
    }
    else return NULL;
}

/* ���Գ�ջ���� */
static scl_stack_typedef* out_try_stack(scl_stack_typedef *pstack, uint16_t *topnum) {
    if((*topnum) > 0) {
        return (pstack + *topnum - 1);
    }
    else return NULL;
}

/* �ߵ�ջ�е�˳�� */
static void reversal_stack(scl_stack_typedef *pstack) {
    scl_stack_typedef temp_stack = {0};
    uint16_t top_num = *get_stack_top(pstack);
    int exnum = top_num / 2;

    for(int i=0; i < exnum; i++) {
        temp_stack = pstack[i];
        pstack[i] = pstack[top_num-1-i];
        pstack[top_num-1-i] = temp_stack;
    }
}

/* ָ��λ��ȡջ���� */
static scl_stack_typedef out_stack_index(scl_stack_typedef *pstack, int num) {
    
}

/* ��ӡջȫ������ */
static void print_stack(scl_stack_typedef *pstack) {
    int i=0;
    uint16_t num = *get_stack_top(pstack);

    if(pstack == scl_stack_pre) printf("\n#pre stakc print: \nnum  type  val    symbol\n");
    // else if(pstack == scl_stack_main) printf("\n#main stakc print: \nnum  type  val    symbol\n");
    // else if(pstack == scl_stack_temp) printf("\n#temp stakc print: \nnum  type  val    symbol\n");
    else return;

    for(i=0; i<num; i++) {
        printf("%-4d %-4d  %4.3f  %-3c%d\n", i, pstack[i].type, pstack[i].val, pstack[i].symbol, pstack[i].syb_val);
    }
}

/* ���ַ���ָ��λ��ȡ�����������֣��������� */
static int get_num(char *pch, int *dignum) {
    int i=0,temp=0;
    // if(pch[0] < '0' || pch[0] > '9') {
    //     return -1;
    // }
    // temp = pch[0] - 0x30;
    // i++;
    while(pch[i]>='0' && pch[i]<='9') {
        temp = temp * 10 + (pch[i] - 0x30);
        i++;
    }
    *dignum = i;
    return temp;
}

/* ��ȡһ���������ֵ�λ�� */
// static int get_num_digit(int num) {
//     uint8_t pos=0;
//     if(num == 0) {
//         pos = 1;
//     }
//     pos = 1;
//     while(num / 10) {
//         pos++;
//         num /= 10;
//     }
//     return pos;
// }

/*
    ����Դ���ݵ�ָ���ַ��ƫ����
    @src:��һ��Դ���ݵ�ַ
    @offest_size:ÿ��Դ����֮���ƫ�������ֽڣ�
*/
static float *psrc_val = NULL;
static int src_offset = 0;
void second_calc_set_source_val(float *src, int offset_byte) {
    psrc_val = src;
    src_offset = offset_byte;
}


/* 
    ��ȡ#id����Ӧ����ֵ,����ֵ���ⲿ�ṩ
    @id:��1��ʼ 
*/
float basedata_test[20] = {1,2,3,4,5,6,7,8,9,10,1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8,1.9};
static float scl_get_value(uint16_t id) {
    #if 1 //��ʼ��ַ��ƫ����ȡʵ������
        uint8_t *add = NULL;
        float getval = 0;
        if(id == 0) return *psrc_val;
        else {
            add = (uint8_t *)psrc_val;
            add += (src_offset * (id - 1));
            getval = *(float *)add;
            return getval;
        }
    #else //�ò�������
        if(id == 0) return 1;
        else return basedata_test[id-1];
    #endif
}

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
    // const char continutchar[10] = {'+','-','*','/','b','f','~','^'};
    // for(i=1; i<stlen-1; i++) {
    //     if(strchr(continutchar, str[i]) != NULL) {
    //         if(strchr(continutchar, str[i+1]) != NULL) {
    //             res = -3;
    //             goto EXIT;
    //         }
    //     }
    // }
        
    /* �������ַ���ʽ����Ԥ����ջ�У����������ַ�����ջ */
    int numid, numdigit;
    for(i=1; i<stlen-1; i++) { //����'['��']'
        switch(str[i]) {
            case '#':
                numid = get_num(&str[i+1], &numdigit);
                /* #����idֵ���� */
                if(numid <= 0) {
                    i += 1;
                    break;
                }
                /* ��ջ */
                in_stack(scl_stack_pre, &pre_top, scl_get_value(numid), 1, 0, 0);
                /* �ַ������ƫ�� λ�� */
                i += numdigit;
            break;

            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                /* �̶���ֵ��ջ */
                numid = get_num(&str[i], &numdigit);
                /* ��ջ */
                in_stack(scl_stack_pre, &pre_top, numid, 1, 0, 0);
                /* �ַ������ƫ�� λ�� */
                i += (numdigit-1);
            break;

            case 'b':
                numid = get_num(&str[i+1], &numdigit);
                if(numid > 15) numid = 15;
                /* ��ջ */
                in_stack(scl_stack_pre, &pre_top, 0, 2, str[i], numid);
                /* �ַ������ƫ�� λ�� */
                i += numdigit;
            break;

            case '+':
            case '-':
            case '*':
            case '/':
            case '(':
            case ')':
            case '^':
            case '~':
            case 'f':
                /* ���������ջ */
                in_stack(scl_stack_pre, &pre_top, 0, 2, str[i], 0);
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
static scl_stack_typedef scl_calc(scl_stack_typedef *tar_stack, uint16_t *tar_topnum) {
    scl_stack_typedef st_recs[STACK_TOPMAXNUM] = {0};
    uint16_t recs_topnum = 0;
    scl_stack_typedef st_res = {0}, st_rec = {0};
    scl_stack_typedef *st_cur, *st_cur2;
    float cval = 0; 
    uint16_t bit_val = 0, turn_val = 0;

    /* ��Ԥ����ջ�����ȡ�����м��� */
    while(st_cur = out_stack(tar_stack, tar_topnum)) {
        /* ȡ������ֵ */
        if(st_cur->type == 1) {
            /* ��ֵ����ջ���ȴ�������� */
            in_stack(st_recs, &recs_topnum, st_cur->val, st_cur->type, st_cur->symbol, 0);
        }
        /* ȡ���Ƿ��� */
        else if(st_cur->type == 2) {
            /* +��- ����ջ�����ͳһ���� */
            if(st_cur->symbol == '+' || st_cur->symbol == '-') {
                in_stack(st_recs, &recs_topnum, st_cur->val, st_cur->type, st_cur->symbol, 0);
            }
            /* �˳����� */
            else if(st_cur->symbol == '*' || st_cur->symbol == '/') {
                /* *��/��������ֵ��ֱ������ */
                if(out_try_stack(tar_stack, tar_topnum)->type == 1) {
                    st_cur2 = out_stack(tar_stack, tar_topnum);
                    if(st_cur->symbol == '*') {
                        cval = st_cur2->val * out_stack(st_recs, &recs_topnum)->val;
                    }else if(st_cur->symbol == '/') {
                        cval = st_cur2->val / out_stack(st_recs, &recs_topnum)->val;
                    }
                    /* ��*��/�����Ľ����ջ */
                    in_stack(st_recs, &recs_topnum, cval, 1, 0, 0);
                }
                /* *��/����������������ݷ��ŷֱ��� */
                else if(out_try_stack(tar_stack, tar_topnum)->type == 2) {
                    /* ���*��/������������ */
                    if( out_try_stack(tar_stack, tar_topnum)->symbol == '(' ) {
                        /* ȡ�������� */
                        out_stack(tar_stack, tar_topnum);
                        /* ����������ǰ�ķ��� */
                        in_stack(st_recs, &recs_topnum, 0, 2, st_cur->symbol, 0);
                        /* �ݹ� */
                        st_rec = scl_calc(tar_stack, tar_topnum);
                        /* �ݹ��Ľ����ջ */
                        in_stack(st_recs, &recs_topnum, st_rec.val, st_rec.type, st_rec.symbol, 0);
                    }
                }
            }
            /* ȡλ�� b */
            else if( st_cur->symbol == 'b' ) {
                /* b������������ */
                if(out_try_stack(tar_stack, tar_topnum)->type == 2 && out_try_stack(tar_stack, tar_topnum)->symbol == '(') {
                    out_stack(tar_stack, tar_topnum);
                    /* �ݹ� */
                    st_rec = scl_calc(tar_stack, tar_topnum);
                }
                /* b��������ֵ */
                else if(out_try_stack(tar_stack, tar_topnum)->type == 1) {
                    st_rec = *out_stack(tar_stack, tar_topnum);
                }
                else {
                    st_rec.val = 0;
                }
                /* ����ȡλ���� */
                bit_val = (uint32_t)(st_rec.val) >> (st_cur->syb_val);
                bit_val &= 0x0001;
                in_stack(st_recs, &recs_topnum, bit_val, 1, 0, 0);
            }
            /* ȡ���� ~ */
            else if( st_cur->symbol == '~' ) {
                /* ~������������ */
                if(out_try_stack(tar_stack, tar_topnum)->type == 2 && out_try_stack(tar_stack, tar_topnum)->symbol == '(') {
                    out_stack(tar_stack, tar_topnum);
                    st_rec = scl_calc(tar_stack, tar_topnum);
                }
                /* ~��������ֵ */
                else if(out_try_stack(tar_stack, tar_topnum)->type == 1) {
                    st_rec = *out_stack(tar_stack, tar_topnum);
                }
                else {
                    st_rec.val = 0;
                }
                /* ����ȡ������ */
                turn_val = ~((uint32_t)st_rec.val);
                in_stack(st_recs, &recs_topnum, turn_val, 1, 0, 0);
            }
            /* ָ������ ^ */
            else if( st_cur->symbol == '^' ) {
                /* ^ǰ����涼����ֵ */
                if(out_try_stack(st_recs, &recs_topnum)->type == 1 && out_try_stack(tar_stack, tar_topnum)->type == 1) {
                    cval = (out_stack(st_recs, &recs_topnum)->val) * pow(10, (int16_t)(out_stack(tar_stack, tar_topnum)->val));
                    in_stack(st_recs, &recs_topnum, cval, 1, 0, 0);
                }
                else {
                    in_stack(st_recs, &recs_topnum, st_cur->val, st_cur->type, st_cur->symbol, st_cur->syb_val);
                }
            }
            /* �������� f */
            else if( st_cur->symbol == 'f' ) {
                /* ȡ�����������Ȼ��ݹ� */
                out_stack(tar_stack, tar_topnum);
                st_rec = scl_calc(tar_stack, tar_topnum);
                if(((uint16_t)st_rec.val & 0x0001) == 1) {
                    in_stack(st_recs, &recs_topnum, -1, 1, 0, 0);
                }
                else {
                    in_stack(st_recs, &recs_topnum, 1, 1, 0, 0);
                }
            }
            /* ����������������ʱջ */
            else if( st_cur->symbol == '(' ) {
                /* �ݹ� */
                st_rec = scl_calc(tar_stack, tar_topnum);
                /* �ݹ��Ľ����ջ */
                in_stack(st_recs, &recs_topnum, st_rec.val, st_rec.type, st_rec.symbol, 0);
            }
            /* ���������ż�����ʱջ��������� */
            else if( st_cur->symbol == ')' ) {
                /* �˳�ѭ��������recsջ�е����� */
                /* ͨ�����������߻��˳���ǰ�ݹ� */
                break;
            }
            /* ��֧�ֵķ��� */
            else {
                continue;
            }
        }
    }

    int i=0, j=0;
    /* ����ֻ�� �Ӽ��˳� ָ�� ���� �ı��ʽ */
    for(i=0,j=0; i<recs_topnum; i++) {
        /* ����������ţ���0�˳� */
        if(st_recs[i].type == 2 && ( st_recs[i].symbol == '(' || st_recs[i].symbol == ')' )) {
            st_res.val = 0;
            return st_res;
        }
        /* ��������֣��ȷŻ� */
        else if(st_recs[i].type == 1) {
            st_recs[j++] = st_recs[i];
        }
        /* ����ǼӺŻ��߼��ţ��ȷŻ� */
        if(st_recs[i].type == 2 && ( st_recs[i].symbol == '+' || st_recs[i].symbol == '-')) {
            st_recs[j++] = st_recs[i];
        }
        /* ����ǳ˺Ż��߳��ţ�ֱ�������һ�������м��㣬Ȼ�󽫽���Ż� */
        else if(st_recs[i].type == 2 && ( st_recs[i].symbol == '*' || st_recs[i].symbol == '/')) {
            if(st_recs[i].symbol == '*') {
                cval = st_recs[i-1].val * st_recs[i+1].val;
            }
            else if(st_recs[i].symbol == '/') {
                cval = st_recs[i-1].val / st_recs[i+1].val;
            }
            i++;
            j--;
            st_recs[j].val = cval;
            st_recs[j].type = 1;
            st_recs[j].symbol = 0;
            j++;
        }
        /* �����ָ�� */
        else if(st_recs[i].type == 2 && st_recs[i].symbol == '^') {
            if(i==0) {
                cval = 1 * pow(10, (int16_t)st_recs[i+1].val);
            }
            else {
                cval = st_recs[i-1].val * pow(10, (int16_t)st_recs[i+1].val);
                j--;
            }
            i++;
            st_recs[j].val = cval;
            st_recs[j].type = 1;
            st_recs[j].symbol = 0;
            j++;
        }
    }

    /* ����ֻ���Ӽ����ֵı��ʽ */
    for(i=0; i<j; i++) {
        if(i==0) {
            if(st_recs[i].type == 1) cval = st_recs[0].val;
            else cval = 0;
        }
        if(st_recs[i].type == 2 && st_recs[i].symbol == '+') {
            cval += st_recs[i+1].val;
            i++;
        }
        else if(st_recs[i].type == 2 && st_recs[i].symbol == '-') {
            cval -= st_recs[i+1].val;
            i++;
        }
    }

    st_res.type = 1;
    st_res.val = cval;
    return st_res;
}

/* 
    �ⲿ���ã����ж��μ��� 
    @express:������ʽ�ַ���
*/
float second_calc_fun(char* express) {
    // printf("\n  second calc module! \n");
    // printf("\n\n src_str: %s \n", express);

    scl_stack_typedef calc_res = {0};
    
    /* ����ǰ��ʼ�� */
    scl_init();

    /* �ַ���Ԥ������ʽ����ջ */
    if(scl_presolv(express) < 0) {
        printf("calc string error: \"%s\"\n", express);
    }

    /* ��ӡջ������ */
    // print_stack(scl_stack_pre);

    /* �ߵ��ַ���˳�򣬷����ջ */
    reversal_stack(scl_stack_pre);

    /* ������������ */
    calc_res = scl_calc(scl_stack_pre, &pre_top);

    return calc_res.val;
}

char srcstr[50] = {"[#1 + #2 * #13 - ( #4 - 3 * ( #5 - #9 ) ) + 100]"};
char srcstr2[50] = {"[8^(1+#2) + 9^(-2)]"};
char srcstr3[50] = {"[ f(~(b1(#2))) * (-12) ]"};

void main(char argc, char* agrv[]) {
    
    float result_val = 0;
    
    second_calc_set_source_val(basedata_test, sizeof(float)); //����Դ���ݵ�ַ��ƫ����

    result_val = second_calc_fun(srcstr); //���ж��μ���
    printf("\n calculate res: %.3f\n", result_val);

    result_val = second_calc_fun(srcstr2); //���ж��μ���
    printf("\n calculate res: %.3f\n", result_val);

    result_val = second_calc_fun(srcstr3); //���ж��μ���
    printf("\n calculate res: %.3f\n", result_val);

}




