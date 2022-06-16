// HuiDongExpression.cpp : 定义控制台应用程序的入口点。
//

#include <stdio.h>
#include "stdint.h"
#include <tchar.h>
#include <stdlib.h>
#include <conio.h>
#include <memory.h>
#include <string.h>

#define TEMP_STACK 0
#define MAIN_STACK 1

typedef struct bowl
{
	int who; //存储存储在单个结构体内的数据类型是int还是char,
				//0为Int,1为char

	int num; //存储数字
	char word; //存储字符
} BOWL;

//全局变量
BOWL temp_Stack[512];		//用于临时存放，预处理的时候用的
int temp_top;				//存储上述栈顶
BOWL Stack[512];			//真正计算用的栈
int top;					//存储上述栈顶
char chInPut[512] = {0};	//获取用户输入
int Result;					//最终结果

void push(int NAME,int in_who,int in_num,char in_word)
{
	if(NAME == TEMP_STACK)
	{
		temp_Stack[temp_top].who = in_who;
		temp_Stack[temp_top].num = in_num;
		temp_Stack[temp_top].word = in_word;
		temp_top++;
	}
	else if(NAME == MAIN_STACK)
	{
		Stack[top].who = in_who;
		Stack[top].num = in_num;
		Stack[top].word = in_word;
		top++;
	}
}

BOWL pop(int NAME)
{
	BOWL node;
	if(NAME == TEMP_STACK)
	{
		node = temp_Stack[temp_top-1];

		temp_Stack[temp_top-1].who = 0;
		temp_Stack[temp_top-1].num = 0;
		temp_Stack[temp_top-1].word = 0;
		temp_top--;
	}
	else if(NAME == MAIN_STACK)
	{
		node = Stack[top-1];

		Stack[top-1].who = 0;
		Stack[top-1].num = 0;
		Stack[top-1].word = 0;
		top--;
	}
	return node;
}

void init_Stack()//初始化栈及对用户输入的预处理
{
	//清空Stack
	int i;
	temp_top = 0;
	top = 0;
	for(i=0;i<512;i++)
	{
		Stack[i].num = 0;
		Stack[i].who = 0;
		Stack[i].word = 0;
		temp_Stack[i].num = 0;
		temp_Stack[i].who = 0;
		temp_Stack[i].word = 0;
	}

	//开始读用户输入，进行预处理
	int j;
	if(chInPut[0] == '-')//开头是负号
	{
		push(TEMP_STACK,0,0,0);
	}

	int Converge = 0;		//用于累计以存放多位数
	for(j=0;j<512;j++)
	{
		if('0'<=chInPut[j]&&chInPut[j]<='9')//如果是数字
		{
			if('0'<=chInPut[j+1]&&chInPut[j+1]<='9')//这个数是两位以上，因为下一个也是数
			{
				if(!('0'<=chInPut[j-1]&&chInPut[j-1]<='9'))//这只是这个两位以上的数的开头
				{
					char ch[2];
					ch[0] = chInPut[j];
					Converge = atoi(ch);//先就直接放进去
					continue;
				}
				int n;
				char ch[2];
				ch[0] = chInPut[j];
				n = atoi(ch);
				Converge*=10;
				Converge+=n;
			}
			else if(('0'<=chInPut[j-1]&&chInPut[j-1]<='9')&&!('0'<=chInPut[j+1]&&chInPut[j+1]<='9'))//这个数是两位以上的一个数的结尾
			{
				int n;
				char ch[2];
				ch[0] = chInPut[j];
				n = atoi(ch);
				Converge*=10;
				Converge+=n;
				push(TEMP_STACK,0,Converge,0);
				Converge = 0;//清零
			}
			else//这只是个位数
			{
				char ch[2];
				ch[0] = chInPut[j];
				int temp_into = atoi(ch);
				push(TEMP_STACK,0,temp_into,0);
			}
		}
		else if((chInPut[j] == '+')||(chInPut[j] == '-')||(chInPut[j] == '*')
			||(chInPut[j] == '/')||(chInPut[j] == ')'))//是符号
		{
			push(TEMP_STACK,1,0,chInPut[j]);
		}
		//一切的括号都必须是小括号，如遇中大括号，转换为小括号
		else if(chInPut[j] == '('||chInPut[j] == '['||chInPut[j] == '{')//是左括号，要确认一下是不是以负号开头
		{
			push(TEMP_STACK,1,0,'(');
			if(chInPut[j+1] == '-')
			{
				push(TEMP_STACK,0,0,0);
			}
		}
		else if(chInPut[j] == ']'||chInPut[j] == '}')
		{
			push(TEMP_STACK,1,0,')');
		}
	}
	
}

void count()	//开始计算
{
	int progress;//指向现在计算到的地方
	for(progress=0;progress<temp_top;progress++)
	{
		if((temp_Stack[progress].who == 1)&&(temp_Stack[progress].word == '*'))//碰上乘号
		{
			if(temp_Stack[progress+1].who == 0)//乘号后面是数字
			{
				int C1 = Stack[top-1].num;
				int C2 = temp_Stack[progress+1].num;
				int J = C1*C2;
				pop(MAIN_STACK);
				push(MAIN_STACK,0,J,0);
				progress++;
				continue;
			}
			else
			{
				push(MAIN_STACK,1,0,'*');
			}
		}
		else if((temp_Stack[progress].who == 1)&&(temp_Stack[progress].word == '/'))//碰上除号
		{
			if(temp_Stack[progress+1].who == 0)//除号后面是数字
			{
				int C1 = Stack[top-1].num;
				int C2 = temp_Stack[progress+1].num;
				int J = C1/C2;

				//由于可能会出现余数，有余数的情况要提示
				int ys = C1%C2;
				if(ys != 0)
				{
					printf("余数已忽略");
				}

				pop(MAIN_STACK);
				push(MAIN_STACK,0,J,0);
				progress++;
				continue;
			}
			else
			{
				push(MAIN_STACK,1,0,'/');
			}
		}
		else if((temp_Stack[progress].who == 1)&&(temp_Stack[progress].word == ')'))//碰上右括号
		{
			int J;
			if((Stack[top-1].who == 0)&&(Stack[top-2].who == 1 && Stack[top-2].word == '('))//有一个特殊情况，就是括号内已经只剩下数字
			{
				//如果括号内只剩下数字，那么就把数字提出，去掉括号，再将数字放回
				int shift;
				shift = Stack[top-1].num;
				pop(MAIN_STACK);
				pop(MAIN_STACK);
				push(MAIN_STACK,0,shift,0);
				goto Checkpoints;//直接跳转到乘除号检查点
			}

			int k;
			BOWL result;//计算的结果
			int nPopNum = 0;//出栈计数
			BOWL pNode[4];//保存出栈的数据
			//初始化
			for(k=0;k<4;k++)
			{
				pNode[k].who = 0;
				pNode[k].num = 0;
				pNode[k].word = 0;
			}
			result.who = 0;
			result.num = 0;
			result.word = 0;

			while(!((Stack[top - 1].who == 1)&&(Stack[top - 1].word == '(')))//碰到左括号之前，while循环成立
			{
				pNode[3-nPopNum] = pop(MAIN_STACK);
				nPopNum++;
				if(nPopNum == 4)
				{
					if( 1 == pNode[0].who )
					{
						if('-' == pNode[0].word)
						{
							if((1 == pNode[2].who) &&(0 == pNode[1].who) && (0 == pNode[3].who))
							{
								if('+' == pNode[2].word)
								{
									result.who = 0;
									result.num = pNode[1].num - pNode[3].num;
								}
								else if('-' == pNode[2].word)
								{
									result.who = 0;
									result.num = pNode[1].num + pNode[3].num;
								}
								else
								{
									printf("MISS BUG!\n");
									//表达式错误
									break;
								}
							}
							else
							{
								printf("MISS BUG!\n");
								//表达式错误
								break;
							}
						}
						else if('+' == pNode[0].word)
						{
							if((1 == pNode[2].who) &&(0 == pNode[1].who) && (0 == pNode[3].who))
							{
								if('+' == pNode[2].word)
								{
									result.who = 0;
									result.num = pNode[1].num + pNode[3].num;
								}
								else if('-' == pNode[2].word)
								{
									result.who = 0;
									result.num = pNode[1].num - pNode[3].num;
								}
								else
								{
									printf("MISS BUG!\n");
									//表达式错误
									break;
								}
							}
							else
							{
								printf("MISS BUG!\n");
								//表达式错误
								break;
							}
						}
						else
						{
							printf("MISS BUG!\n");
							//表达式错误
							break;
						}
					}
					push(MAIN_STACK,1,0,pNode[0].word);
					push(MAIN_STACK,0,result.num,0);
					nPopNum = 0;
				}
			}
			if(nPopNum == 3)
			{
				if(pNode[2].word == '+')
				{
					result.num = pNode[1].num + pNode[3].num;
				}
				if(pNode[2].word == '-')
				{
					result.num = pNode[1].num - pNode[3].num;
				}
				pop(MAIN_STACK);
				push(MAIN_STACK,0,result.num,0);
			}
			else
			{
				//有问题
				printf("MISS BUG!\n");
			}
Checkpoints:
			if(Stack[top-2].word == '*' || Stack[top-2].word == '/')//再次确认一下前面是不是乘除号
			{
				if(Stack[top-2].word == '*')
				{
					J = Stack[top-3].num * Stack[top-1].num;
				}
				else if(Stack[top-2].word == '/')
				{
					J = Stack[top-3].num / Stack[top-1].num;
				}
				pop(MAIN_STACK);
				pop(MAIN_STACK);
				pop(MAIN_STACK);
				push(MAIN_STACK,0,J,0);
			}
		}
		else//如果是数字和加减号，直接放进栈里就好，最后一起计算。
		{
			if (temp_Stack[progress].who == 0)
			{
				push(MAIN_STACK,0,temp_Stack[progress].num,0);
			}
			else if (temp_Stack[progress].who == 1)
			{
				push(MAIN_STACK,1,0,temp_Stack[progress].word);
			}
		}
	}

	//最后的一点计算
	int k;
	BOWL result;//计算的结果
	int nPopNum = 0;//出栈计数
	BOWL pNode[4];//保存出栈的数据
	//初始化
	for(k=0;k<4;k++)
	{
		pNode[k].who = 0;
		pNode[k].num = 0;
		pNode[k].word = 0;
	}
	result.who = 0;
	result.num = 0;
	result.word = 0;

	//如果只剩下一个数
	if(top == 1 && Stack[top].who == 0)
	{
		goto last;
	}

	while (top >= 1)
	{
		//此处要参照中间处理括号的方法重写
		pNode[3-nPopNum] = pop(MAIN_STACK);
		nPopNum++;
		if(nPopNum == 4)
		{
			if( 1 == pNode[0].who )
			{
				if('-' == pNode[0].word)
				{
					if((1 == pNode[2].who) &&(0 == pNode[1].who) && (0 == pNode[3].who))
					{
						if('+' == pNode[2].word)
						{
							result.who = 0;
							result.num = pNode[1].num - pNode[3].num;
						}
						else if('-' == pNode[2].word)
						{
							result.who = 0;
							result.num = pNode[1].num + pNode[3].num;
						}
						else
						{
							//表达式错误
							printf("MISS BUG!\n");
							break;
						}
					}
					else
					{
						//表达式错误
						printf("MISS BUG!\n");
						break;
					}
				}
				else if('+' == pNode[0].word)
				{
					if((1 == pNode[2].who) &&(0 == pNode[1].who) && (0 == pNode[3].who))
					{
						if('+' == pNode[2].word)
						{
							result.who = 0;
							result.num = pNode[1].num + pNode[3].num;
						}
						else if('-' == pNode[2].word)
						{
							result.who = 0;
							result.num = pNode[1].num - pNode[3].num;
						}
						else
						{
							//表达式错误
							printf("MISS BUG!\n");
							break;
						}
					}
					else
					{
						//表达式错误
						printf("MISS BUG!\n");
						break;
					}
				}
				else
				{
					//表达式错误
					printf("MISS BUG!\n");
					break;
				}
			}
			push(MAIN_STACK,1,0,pNode[0].word);
			push(MAIN_STACK,0,result.num,0);
			nPopNum = 0;
		}
	}
	if(nPopNum == 3)
	{
		if(pNode[2].word == '+')
		{
			result.num = pNode[1].num + pNode[3].num;
		}
		if(pNode[2].word == '-')
		{
			result.num = pNode[1].num - pNode[3].num;
		}
		//pop(MAIN_STACK);
		push(MAIN_STACK,0,result.num,0);
	}
	else
	{
		//有问题
		printf("MISS BUG!\n");
	}

last:
	Result = Stack[0].num;
}

int _tmain()
{
	printf("请输入你的表达式(要求表达式要正确):\n");
	gets(chInPut);
	init_Stack();
	count();
	printf("\n原算式：");
	puts(chInPut);
	printf("\n处理后算式：");
	for(int i=0;i<temp_top;i++)
	{
		if(temp_Stack[i].who == 0)
		{
			printf("%d",temp_Stack[i].num);
		}
		else if(temp_Stack[i].who == 1)
		{
			printf("%c",temp_Stack[i].word);
		}
	}
	printf("\n计算结果：%d\n",Result);
	
}


