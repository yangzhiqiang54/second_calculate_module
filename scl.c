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
}scl_stack_typedef;
/***** end 结构体自定义 ****/

/**** start 声明变量 ****/
static scl_stack_typedef scl_stack_pre[50];   //预处理堆栈
static scl_stack_typedef scl_stack_main[50];  //主堆栈
static scl_stack_typedef scl_stack_temp[50];  //临时堆栈
static uint16_t pre_top = 0, main_top = 0, temp_top = 0;
/**** end 声明变量 ****/

/**** start 声明函数 ****/

/**** end 声明函数 ****/

/* 获取入栈数 */
static uint16_t get_stack_top(scl_stack_typedef *ps) {
    if(ps == scl_stack_pre) return pre_top;
    else if(ps == scl_stack_main) return main_top;
    else if(ps == scl_stack_temp) return temp_top;
    else return 0;
}

/* 入栈函数 */
static void in_stack(scl_stack_typedef *pstack, float v, uint8_t t, char s) {
    uint16_t num = get_stack_top(pstack);
    pstack[num].val = v;
    pstack[num].type = t;
    pstack[num].symbol = s;
    num++;
}

/* 出栈函数 */
static scl_stack_typedef out_stack(scl_stack_typedef *pstack) {
    uint16_t num = get_stack_top(pstack);
    if(num > 0) return pstack[num-1];
    else pstack[0];
}

/* 打印栈全部内容 */
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
    对输入字符串进行预处理 
    1.判断必须以[]头尾括起来
    2.删除空格等无关字符
    @str:输入的运算字符串
    @return:预处理结果 0成功 -1失败
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

    /* 将输入字符格式化到预处理栈中 */
    for(i=1; i<stlen-1; i++) { //跳过'['和']'
        if(str[i] >= '0' && str[i] <= '9') {

        }
    }

}








char srcstr[100] = {"#1+#2*#3-(#4-#5)"};
float exval[20] = {0};
void main(char argc, char* agrv[]) {

    int i=0;

    printf("  second calc module! \n\n");

    /* 模拟采集点位数据 */
    for(i=0; i<20; i++) {
        exval[i] = i+1;
        printf("#%d=%f ", i+1, exval[i]);
    }

    printf("\n\n src_str: %s \n", srcstr);
    scl_prosolv(srcstr);
    print_stack(scl_stack_pre);
}




