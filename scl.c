/*
    二次计算模块
    1.支持的运算符号优先级：
        最高：#取值 
        较高：()括号 
        中等(暂不支持)：b(n)取bit值 ~取反 f(n)符号位(0正1负) ^指数
        较低：*乘 /除
        最低：+加 -减 
    2.对于输入的小数取3位有效小数位
*/

#include "stdio.h"
#include "stdint.h"
#include "string.h"

#include "scl.h"

/*宏定义*/
#define STACK_TOPMAXNUM 50

/**** start 结构体自定义 ****/
typedef struct scl_stack {
    float val;      //数值
    uint8_t type;   //0-NULL 1-值 2-运算符
    char symbol;    //运算符号
}scl_stack_typedef;
/***** end 结构体自定义 ****/

/**** start 声明变量 ****/
static scl_stack_typedef scl_stack_pre[STACK_TOPMAXNUM];   //预处理堆栈
static scl_stack_typedef scl_stack_main[STACK_TOPMAXNUM];  //主堆栈
// static scl_stack_typedef scl_stack_temp[STACK_TOPMAXNUM];  //临时堆栈
static uint16_t pre_top = 0;
static uint16_t main_top = 0;
// static uint16_t temp_top = 0;
/**** end 声明变量 ****/

/**** start 声明函数 ****/

/**** end 声明函数 ****/

/* 模块初始化函数 */
static void scl_init(void) {
    memset(scl_stack_pre, 0, sizeof(scl_stack_pre));
    memset(scl_stack_main, 0, sizeof(scl_stack_main));
    // memset(scl_stack_temp, 0, sizeof(scl_stack_temp));
    pre_top = 0;
    main_top = 0;
    // temp_top = 0;
}

/* 获取入栈数 */
static uint16_t* get_stack_top(scl_stack_typedef *ps) {
    if(ps == scl_stack_pre) return &pre_top;
    else if(ps == scl_stack_main) return &main_top;
    // else if(ps == scl_stack_temp) return &temp_top;
    else return 0;
}

/* 入栈函数 @t:1值 2运算符*/
static void in_stack(scl_stack_typedef *pstack, uint16_t *topnum, float v, uint8_t t, char s) {
    pstack[*topnum].val = v;
    pstack[*topnum].type = t;
    pstack[*topnum].symbol = s;
    (*topnum)++;
}

/* 出栈函数 */
static scl_stack_typedef* out_stack(scl_stack_typedef *pstack, uint16_t *topnum) {
    if((*topnum) > 0) {
        (*topnum) -= 1;
        return (pstack + *topnum);
    }
    else return NULL;
}

/* 尝试出栈函数 */
static scl_stack_typedef* out_try_stack(scl_stack_typedef *pstack, uint16_t *topnum) {
    if((*topnum) > 0) {
        return (pstack + *topnum - 1);
    }
    else return NULL;
}

/* 颠倒栈中的顺序 */
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

/* 指定位置取栈函数 */
static scl_stack_typedef out_stack_index(scl_stack_typedef *pstack, int num) {
    
}

/* 打印栈全部内容 */
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

/* 从字符串指定位置取出连续的数字，返回整形 */
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

/* 获取一个整形数字的位数 */
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
    对输入字符串进行预处理 
    1.判断必须以[]头尾括起来
    2.括号必须按左右顺序成对出现
    3.删除空格等无关字符
    4.连续符号，处理或报错推出
    @str:输入的运算字符串
    @return:预处理结果 0成功 -1失败
*/
static int scl_presolv(char* str) {
    int res = 0, i=0, j=0, stlen=0;
    scl_stack_typedef st_scl_temp;

    /* 判断是否由 [ ] 包括 */
    stlen = strlen(str);
    if(stlen <=2 || str[0] != '[' || str[stlen-1] != ']') {
        res = -1;
        goto EXIT;
    }

    /* 判断括号是否按左右顺序成对出现 */
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

    /* 去除无效字符：空格 */
    for(i=0; i<stlen; i++) {
        if(str[i] == ' ') {
            memcpy(str+i, str+i+1, stlen-i);
            j++;
        }
    }
    stlen -= j; //去除无效字符后的字符串长度

    /* 处理连续符号 */
    const char continutchar[10] = {'+','-','*','/','b','f','~','^'};
    for(i=1; i<stlen-1; i++) {
        if(strchr(continutchar, str[i]) != NULL) {
            if(strchr(continutchar, str[i+1]) != NULL) {
                res = -3;
                goto EXIT;
            }
        }
    }
        
    /* 将输入字符格式化到预处理栈中，从右往左将字符串入栈 */
    int numid, numdigit;
    for(i=1; i<stlen-1; i++) { //跳过'['和']'
        switch(str[i]) {
            case '#':
                numid = get_num(&str[i+1]);
                /* #后面id值错误 */
                if(numid <= 0) {
                    i += 1;
                    break;
                }
                /* 入栈 */
                in_stack(scl_stack_pre, &pre_top, basedata[numid-1], 1, 0);
                /* 字符串向后偏移 位数 */
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
                /* 运算符号入栈 */
                in_stack(scl_stack_pre, &pre_top, 0, 2, str[i]);
            break;

            default:break;
        }
    }

    EXIT:
    return res;
}

/*
    进行出栈操作并进行算数运算
*/
uint8_t main_pos = 0;
static scl_stack_typedef scl_calc(scl_stack_typedef *tar_stack, uint16_t *tar_topnum) {
    scl_stack_typedef st_recs[STACK_TOPMAXNUM] = {0};
    uint16_t recs_topnum = 0;
    scl_stack_typedef st_res = {0}, st_rec = {0};
    scl_stack_typedef *st_cur, *st_cur2;
    float cval = 0; 
    char cchar = 0;

    /* 从预处理栈中逐个取出进行计算 */
    while(st_cur = out_stack(tar_stack, tar_topnum)) {
        /* 取出是数值 */
        if(st_cur->type == 1) {
            /* 数值先入栈，等待运算符号 */
            in_stack(st_recs, &recs_topnum, st_cur->val, st_cur->type, st_cur->symbol);
        }
        /* 取出是符号 */
        else if(st_cur->type == 2) {
            /* +和- 先入栈，最后统一计算 */
            if(st_cur->symbol == '+' || st_cur->symbol == '-') {
                in_stack(st_recs, &recs_topnum, st_cur->val, st_cur->type, st_cur->symbol);
            }
            /* 乘除运算 */
            else if(st_cur->symbol == '*' || st_cur->symbol == '/') {
                /* *和/后面是数值，直接运算 */
                if(out_try_stack(tar_stack, tar_topnum)->type == 1) {
                    st_cur2 = out_stack(tar_stack, tar_topnum);
                    if(st_cur->symbol == '*') {
                        cval = st_cur2->val * out_stack(st_recs, &recs_topnum)->val;
                    }else if(st_cur->symbol == '/') {
                        cval = st_cur2->val / out_stack(st_recs, &recs_topnum)->val;
                    }
                    /* 将*和/计算后的结果入栈 */
                    in_stack(st_recs, &recs_topnum, cval, 1, 0);
                }
                /* *和/后面是运算符，根据符号分别处理 */
                else if(out_try_stack(tar_stack, tar_topnum)->type == 2) {
                    /* 如果*和/后面是左括号 */
                    if( out_try_stack(tar_stack, tar_topnum)->symbol == '(' ) {
                        /* 取出左括号 */
                        out_stack(tar_stack, tar_topnum);
                        /* 补入左括号前的符号 */
                        in_stack(st_recs, &recs_topnum, 0, 2, st_cur->symbol);
                        /* 递归 */
                        st_rec = scl_calc(tar_stack, tar_topnum);
                        /* 递归后的结果入栈 */
                        in_stack(st_recs, &recs_topnum, st_rec.val, st_rec.type, st_rec.symbol);
                    }
                }
            }
            /* 遇到左括号先入临时栈 */
            else if( st_cur->symbol == '(' ) {
                /* 递归 */
                st_rec = scl_calc(tar_stack, tar_topnum);
                /* 递归后的结果入栈 */
                in_stack(st_recs, &recs_topnum, st_rec.val, st_rec.type, st_rec.symbol);
            }
            /* 遇到右括号计算临时栈里面的内容 */
            else if( st_cur->symbol == ')' ) {
                /* 退出循环，计算recs栈中的内容 */
                /* 通常情况，这边已经到了最后一层递归 */
                break;
            }
        }
    }

    /* 计算只含加减乘除数字的表达式 */
    int i=0, j=0;
    for(i=0; i<recs_topnum; i++) {
        /* 如果还有括号，赋0退出 */
        if(st_recs[i].type == 2 && ( st_recs[i].symbol == '(' || st_recs[i].symbol == ')' )) {
            st_res.val = 0;
            return st_res;
        }
        /* 如果是数字，先放回 */
        else if(st_recs[i].type == 1) {
            st_recs[j++] = st_recs[i];
        }
        /* 如果是加号或者减号，先放回 */
        if(st_recs[i].type == 2 && ( st_recs[i].symbol == '+' || st_recs[i].symbol == '-')) {
            st_recs[j++] = st_recs[i];
        }
        /* 如果是乘号或者除号，直接与后面一个数进行计算，然后将结果放回 */
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
    }

    /* 计算只含加减数字的表达式 */
    for(i=0; i<j; i++) {
        if(i==0) cval = st_recs[0].val;
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


char srcstr[50] = {"[#15 + #2*#3 -(#4-#5)]"};
char srcstr2[50] = {"[#1 + #2*#3 - (#2-#8)]"};
float exval[20] = {0};
void main(char argc, char* agrv[]) {
    printf("  second calc module! \n\n");

    int i=0;
    scl_stack_typedef calc_res = {0};
    
    /* 模拟采集点位数据 */
    for(i=0; i<20; i++) {
        exval[i] = i+1;
        if(i % 5 == 0) printf("\n");
        printf("#%d=%.3f ", i+1, exval[i]);
    }

    /* 计算前初始化 */
    scl_init();
    printf("\n\n src_str: %s \n", srcstr2);

    /* 字符串预处理，格式化入栈 */
    if(scl_presolv(srcstr2) < 0) {
        printf("calc string error: \"%s\"\n", srcstr2);
    }
    print_stack(scl_stack_pre);

    /* 颠倒字符串顺序，方便出栈 */
    reversal_stack(scl_stack_pre);

    /* 进行算术运算 */
    calc_res = scl_calc(scl_stack_pre, &pre_top);
    
    printf("\n calculate res: %.4f\n", calc_res.val);
}




