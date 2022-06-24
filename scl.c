/*
    二次计算模块
    1.支持的运算符号优先级：
        最高：#取值 
        较高：()括号 b(n)取bit值
        中等：~取反 f(n)符号位(0正1负) ^指数
        较低：*乘 /除
        最低：+加 -减 
    2.对于输入的小数取3位有效小数位
*/

#include "stdio.h"
#include "stdint.h"
#include "string.h"

#include "scl.h"

/**** start 结构体自定义 ****/
typedef struct scl_stack {
    float val;      //数值
    uint8_t type;   //0-NULL 1-值 2-运算符
    char symbol;    //运算符号
    uint8_t sybval; //运算符数值
    uint8_t en;     //有效标记位
}scl_stack_typedef;
/***** end 结构体自定义 ****/

/**** start 宏定义 ****/
#define STACK_MAXNUM   30
/**** end 宏定义 ****/

/**** start 声明变量 ****/
static scl_stack_typedef scl_stack_pre[50];   //预处理堆栈
static scl_stack_typedef scl_stack_main[30];  //主堆栈
// static scl_stack_typedef scl_stack_temp[50];  //临时堆栈
static uint16_t pre_top = 0;
static uint16_t pre_bot = 0;
static uint16_t main_top = 0;
static uint16_t main_bot = 0;
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


/* 入栈函数 从栈顶入 @t:1值 2运算符*/
static void in_stack_top(scl_stack_typedef *pstack, float v, uint8_t t, char s) {
    
}

/* 入栈函数 从栈底入 @t:1值 2运算符*/
static void in_stack_top(scl_stack_typedef *pstack, float v, uint8_t t, char s) {

}

/* 出栈函数 从栈顶出 */
static scl_stack_typedef* out_stack_top(scl_stack_typedef *pstack) {
    for(int i=0; i<STACK_MAXNUM; i++) {
        if(pstack[STACK_MAXNUM-1-i].en != 0) {
            pstack[STACK_MAXNUM-1-i].en = 0;
            return &pstack[STACK_MAXNUM-1-i];
        }
    }
    return NULL;
}

/* 出栈函数 从栈底出 */
static scl_stack_typedef* out_stack_bot(scl_stack_typedef *pstack) {
    for(int i=0; i<STACK_MAXNUM; i++) {
        if(pstack[i].en != 0) {
            pstack[i].en = 0;
            return &pstack[i];
        }
    }
    return NULL;
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
        
    /* 将输入字符格式化到预处理栈中 */
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
                in_stack(scl_stack_pre, basedata[numid-1], 1, 0);
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
                in_stack(scl_stack_pre, 0, 2, str[i]);
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
static scl_stack_typedef scl_calc(scl_stack_typedef *tar_stack) {
    int i=0;
    scl_stack_typedef st_infun[20];
    scl_stack_typedef *st_cur, *st_cur2;
    float cval=0; 

    /* 从预处理栈中逐个取出进行计算 */
    while(st_cur = out_stack(tar_stack)) {
        /* 取出是数值 */
        if(st_cur->type == 1) {
            /* 数值先入栈，等待运算符号 */
            in_stack(scl_stack_main, st_cur->val, st_cur->type, st_cur->symbol);
        }
        /* 去除是符号 */
        else if(st_cur->type == 2) {
            /* +和- 先入栈，最后统一计算 */
            if(st_cur->symbol == '+' || st_cur->symbol == '-') {
                in_stack(scl_stack_main, st_cur->val, st_cur->type, st_cur->symbol);
            }
            /* 乘除运算 */
            else if(st_cur->symbol == '*' || st_cur->symbol == '/') {
                /* *和/后面是数值，直接运算 */
                if((tar_stack+1)->type == 1) {
                    st_cur2 = out_stack(scl_stack_main);
                    if(st_cur->symbol == '*') {
                        cval = st_cur2->val * (tar_stack+1)->val;
                    }else if(st_cur->symbol == '/') {
                        cval = st_cur2->val / (tar_stack+1)->val;
                    }
                    /* 将*和/计算后的结果入栈 */
                    in_stack(scl_stack_main, cval, 1, 0);
                }
                /* *和/后面是运算符，根据符号分别处理 */
                else {
                    /* *和/后面是（ */
                    if( (tar_stack+1)->symbol == '(' ) {
                        /* 补入一个*号 */
                        in_stack(scl_stack_main, 0, 2, '*');
                        /* 递归 */
                        // scl_calc(st_temp);
                    }



                    in_stack(scl_stack_main, st_cur->val, st_cur->type, st_cur->symbol);
                }
            }
            /* 遇到左括号先入临时栈 */
            else if( st_cur->symbol == '(' ) {
                
            }
            /* 遇到右括号计算临时栈里面的内容 */
            else if( st_cur->symbol == ')' ) {
                
            }
        }
    }

#if 1
    /* 判断是否只剩数字和加减 */
    uint8_t num_add_reduce_flag = 0;
    for(i=0; i<main_top; i++) {
        if(scl_stack_main[i].type == 2) {
            if(scl_stack_main[i].symbol == '(') {
                num_add_reduce_flag = 1;
            }
        }
    }
#endif

    /* 如果还存在括号，进行递归运算 */
    // if(num_add_reduce_flag) {

    // }

    /* 计算剩余的数字和加减符号，从左往右 */


    print_stack(scl_stack_main);

}


char srcstr[50] = {"[#15 + #2*#3 -(#4-#5)]"};
char srcstr2[50] = {"[#1 + #2 * (#3-#5)]"};
float exval[20] = {0};
void main(char argc, char* agrv[]) {
    printf("  second calc module! \n\n");

    int i=0;
    
    /* 模拟采集点位数据 */
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

    out_stack_bot(scl_stack_pre);
    out_stack_bot(scl_stack_main);

    // scl_calc(scl_stack_pre);
    
}




