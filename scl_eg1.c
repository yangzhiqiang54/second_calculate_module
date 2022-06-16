// HuiDongExpression.cpp : �������̨Ӧ�ó������ڵ㡣
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
	int who; //�洢�洢�ڵ����ṹ���ڵ�����������int����char,
				//0ΪInt,1Ϊchar

	int num; //�洢����
	char word; //�洢�ַ�
} BOWL;

//ȫ�ֱ���
BOWL temp_Stack[512];		//������ʱ��ţ�Ԥ�����ʱ���õ�
int temp_top;				//�洢����ջ��
BOWL Stack[512];			//���������õ�ջ
int top;					//�洢����ջ��
char chInPut[512] = {0};	//��ȡ�û�����
int Result;					//���ս��

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

void init_Stack()//��ʼ��ջ�����û������Ԥ����
{
	//���Stack
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

	//��ʼ���û����룬����Ԥ����
	int j;
	if(chInPut[0] == '-')//��ͷ�Ǹ���
	{
		push(TEMP_STACK,0,0,0);
	}

	int Converge = 0;		//�����ۼ��Դ�Ŷ�λ��
	for(j=0;j<512;j++)
	{
		if('0'<=chInPut[j]&&chInPut[j]<='9')//���������
		{
			if('0'<=chInPut[j+1]&&chInPut[j+1]<='9')//���������λ���ϣ���Ϊ��һ��Ҳ����
			{
				if(!('0'<=chInPut[j-1]&&chInPut[j-1]<='9'))//��ֻ�������λ���ϵ����Ŀ�ͷ
				{
					char ch[2];
					ch[0] = chInPut[j];
					Converge = atoi(ch);//�Ⱦ�ֱ�ӷŽ�ȥ
					continue;
				}
				int n;
				char ch[2];
				ch[0] = chInPut[j];
				n = atoi(ch);
				Converge*=10;
				Converge+=n;
			}
			else if(('0'<=chInPut[j-1]&&chInPut[j-1]<='9')&&!('0'<=chInPut[j+1]&&chInPut[j+1]<='9'))//���������λ���ϵ�һ�����Ľ�β
			{
				int n;
				char ch[2];
				ch[0] = chInPut[j];
				n = atoi(ch);
				Converge*=10;
				Converge+=n;
				push(TEMP_STACK,0,Converge,0);
				Converge = 0;//����
			}
			else//��ֻ�Ǹ�λ��
			{
				char ch[2];
				ch[0] = chInPut[j];
				int temp_into = atoi(ch);
				push(TEMP_STACK,0,temp_into,0);
			}
		}
		else if((chInPut[j] == '+')||(chInPut[j] == '-')||(chInPut[j] == '*')
			||(chInPut[j] == '/')||(chInPut[j] == ')'))//�Ƿ���
		{
			push(TEMP_STACK,1,0,chInPut[j]);
		}
		//һ�е����Ŷ�������С���ţ������д����ţ�ת��ΪС����
		else if(chInPut[j] == '('||chInPut[j] == '['||chInPut[j] == '{')//�������ţ�Ҫȷ��һ���ǲ����Ը��ſ�ͷ
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

void count()	//��ʼ����
{
	int progress;//ָ�����ڼ��㵽�ĵط�
	for(progress=0;progress<temp_top;progress++)
	{
		if((temp_Stack[progress].who == 1)&&(temp_Stack[progress].word == '*'))//���ϳ˺�
		{
			if(temp_Stack[progress+1].who == 0)//�˺ź���������
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
		else if((temp_Stack[progress].who == 1)&&(temp_Stack[progress].word == '/'))//���ϳ���
		{
			if(temp_Stack[progress+1].who == 0)//���ź���������
			{
				int C1 = Stack[top-1].num;
				int C2 = temp_Stack[progress+1].num;
				int J = C1/C2;

				//���ڿ��ܻ���������������������Ҫ��ʾ
				int ys = C1%C2;
				if(ys != 0)
				{
					printf("�����Ѻ���");
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
		else if((temp_Stack[progress].who == 1)&&(temp_Stack[progress].word == ')'))//����������
		{
			int J;
			if((Stack[top-1].who == 0)&&(Stack[top-2].who == 1 && Stack[top-2].word == '('))//��һ����������������������Ѿ�ֻʣ������
			{
				//���������ֻʣ�����֣���ô�Ͱ����������ȥ�����ţ��ٽ����ַŻ�
				int shift;
				shift = Stack[top-1].num;
				pop(MAIN_STACK);
				pop(MAIN_STACK);
				push(MAIN_STACK,0,shift,0);
				goto Checkpoints;//ֱ����ת���˳��ż���
			}

			int k;
			BOWL result;//����Ľ��
			int nPopNum = 0;//��ջ����
			BOWL pNode[4];//�����ջ������
			//��ʼ��
			for(k=0;k<4;k++)
			{
				pNode[k].who = 0;
				pNode[k].num = 0;
				pNode[k].word = 0;
			}
			result.who = 0;
			result.num = 0;
			result.word = 0;

			while(!((Stack[top - 1].who == 1)&&(Stack[top - 1].word == '(')))//����������֮ǰ��whileѭ������
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
									//���ʽ����
									break;
								}
							}
							else
							{
								printf("MISS BUG!\n");
								//���ʽ����
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
									//���ʽ����
									break;
								}
							}
							else
							{
								printf("MISS BUG!\n");
								//���ʽ����
								break;
							}
						}
						else
						{
							printf("MISS BUG!\n");
							//���ʽ����
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
				//������
				printf("MISS BUG!\n");
			}
Checkpoints:
			if(Stack[top-2].word == '*' || Stack[top-2].word == '/')//�ٴ�ȷ��һ��ǰ���ǲ��ǳ˳���
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
		else//��������ֺͼӼ��ţ�ֱ�ӷŽ�ջ��ͺã����һ����㡣
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

	//����һ�����
	int k;
	BOWL result;//����Ľ��
	int nPopNum = 0;//��ջ����
	BOWL pNode[4];//�����ջ������
	//��ʼ��
	for(k=0;k<4;k++)
	{
		pNode[k].who = 0;
		pNode[k].num = 0;
		pNode[k].word = 0;
	}
	result.who = 0;
	result.num = 0;
	result.word = 0;

	//���ֻʣ��һ����
	if(top == 1 && Stack[top].who == 0)
	{
		goto last;
	}

	while (top >= 1)
	{
		//�˴�Ҫ�����м䴦�����ŵķ�����д
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
							//���ʽ����
							printf("MISS BUG!\n");
							break;
						}
					}
					else
					{
						//���ʽ����
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
							//���ʽ����
							printf("MISS BUG!\n");
							break;
						}
					}
					else
					{
						//���ʽ����
						printf("MISS BUG!\n");
						break;
					}
				}
				else
				{
					//���ʽ����
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
		//������
		printf("MISS BUG!\n");
	}

last:
	Result = Stack[0].num;
}

int _tmain()
{
	printf("��������ı��ʽ(Ҫ����ʽҪ��ȷ):\n");
	gets(chInPut);
	init_Stack();
	count();
	printf("\nԭ��ʽ��");
	puts(chInPut);
	printf("\n�������ʽ��");
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
	printf("\n��������%d\n",Result);
	
}


