#include<stdio.h>				/*������Ҫ�õ��ĸ��ְ�*/
#include<stdlib.h>
#include<string.h>
#include <ctype.h>
#include <assert.h>


#define MAX_INPUT_SIZE        (1 << 12)
#define MAX_STACK_SIZE        (MAX_INPUT_SIZE / 2 + 64)
 
enum {
    kET_Atom,
    kET_High,
    kET_Mid,
    kET_Low
};
 
typedef struct
{
    int        kExprType, kExprLen;
    char    kExprValue[2*MAX_INPUT_SIZE - 2*sizeof(int)];
} ST_expr;
 
int solve(const char* input_expr, ST_expr* stack)
{
    int top = 0, c;
    ST_expr *curr, *left, *right;
 
    while(0 != (c = *input_expr++))
    {
        assert(top >= 0);
 
        if(top >= MAX_STACK_SIZE - 1)
            return -2;
 
        switch(c)
        {
        case ' ':
        case '\t':
        case '\r':
        case '\n':
        case '\f':
            /* ignore space */
            break;
 
        case '+':
        case '-':
            if(top < 2)    return -1;
            left  = curr = &stack[top - 2];
            right = &stack[top - 1];
            --top;
 
            curr->kExprValue[curr->kExprLen++] = c;
            if(right->kExprType == kET_Low)
            {
                curr->kExprValue[curr->kExprLen++] = '(';
                memcpy(curr->kExprValue + curr->kExprLen, right->kExprValue, right->kExprLen);
                curr->kExprLen += right->kExprLen;
                curr->kExprValue[curr->kExprLen++] = ')';
            }
            else
            {
                memcpy(curr->kExprValue + curr->kExprLen, right->kExprValue, right->kExprLen);
                curr->kExprLen += right->kExprLen;
            }
            curr->kExprValue[curr->kExprLen] = 0;
            curr->kExprType = kET_Low;
            break;
 
        case '*':
        case '/':
            if(top < 2)    return -1;
            left  = curr = &stack[top - 2];
            right = &stack[top - 1];
            --top;
 
            if(curr->kExprType == kET_Low)
            {
                memmove(curr->kExprValue + 1, curr->kExprValue, curr->kExprLen);
                curr->kExprValue[0] = '(';
                curr->kExprValue[1 + curr->kExprLen] = ')';
                curr->kExprLen += 2;
            }
 
            curr->kExprValue[curr->kExprLen++] = c;
            if(right->kExprType >= kET_Mid)
            {
                curr->kExprValue[curr->kExprLen++] = '(';
                memcpy(curr->kExprValue + curr->kExprLen, right->kExprValue, right->kExprLen);
                curr->kExprLen += right->kExprLen;
                curr->kExprValue[curr->kExprLen++] = ')';
            }
            else
            {
                memcpy(curr->kExprValue + curr->kExprLen, right->kExprValue, right->kExprLen);
                curr->kExprLen += right->kExprLen;
            }
            curr->kExprValue[curr->kExprLen] = 0;
            curr->kExprType = kET_Mid;
            break;
 
        case '!':
            if(top < 1)    return -1;
            curr = &stack[top - 1];
 
            if(curr->kExprType >= kET_Mid)
            {
                memmove(curr->kExprValue + 1, curr->kExprValue, curr->kExprLen);
                curr->kExprValue[0] = '(';
                curr->kExprValue[1 + curr->kExprLen] = ')';
                curr->kExprValue[2 + curr->kExprLen] = '!';
                curr->kExprLen += 3;
            }
            else
            {
                curr->kExprValue[curr->kExprLen++] = '!';
            }
 
            curr->kExprValue[curr->kExprLen]   = 0;
            curr->kExprType = kET_High;
            break;
 
        default:
            stack[top].kExprLen   = 1;
            stack[top].kExprType  = kET_Atom;
            stack[top].kExprValue[0] = c;
            stack[top].kExprValue[1] = 0;
            ++top;
            break;
        }
    }
 
    return top;
}



typedef struct                  /*����ṹ�������洢������*/
{
	char op;                        /*�洢�ַ�*/
	int level;						/*�洢���ȼ�*/
}OpNode;



typedef struct
{
	OpNode op[100];
	int top;
	int size;						/*��ʾջ��Ԫ�صĸ���*/
}stack;


							/*�������ջ*/
void init(stack*st)				/*��ʼ��ջ*/
{
	st->size=0;
	st->top=0;
}



OpNode pop(stack*a)				/*��ջ*/
{
	if(a->size==0)					/*���ջΪ�ս�������*/
	{
		exit(-1);
	}
	a->size--;
	return a->op[--(a->top)];		/*ȡ��ջ��Ԫ��*/
}



void push(stack*a,OpNode op)		/*��ջ����*/
{
	a->size++;
	a->op[(a->top)++]=op;
}



OpNode top(stack*a)				/*�۲�ջ������*/
{
	if(a->size==0)					/*���ջΪ�ս�������*/
	{
		printf("stack is empty\n");
		exit(-1);
	}
	return a->op[(a->top)-1];			/*ֻ�õ�ջ����ֵ������ջ*/
}



typedef struct						/*������ֵջ*/
{
	double num[100];
	int top;						/*ջ��ָ��*/
	int size;
}numstack;



void init2(numstack *st)				/*��ʼ����ֵջ*/
{
	st->size=0;
	st->top=0;}
	double pop2(numstack *a)				/*��ֵջ��ջ*/
{
	if(a->size==0)						/*��ջǰ���п�*/
	{
		exit(-1);
	}
	a->size--;
	return a->num[--(a->top)];			/*�õ�ջ����ֵ*/
}



void push2(numstack*a,double num)	/*��ջ*/
{
	a->size++;
	a->num[(a->top)++]=num;
}



int main()							/*������*/
{
	char ch='y';
	void change(char str[],char exp[]);		/*����Ҫ�õ��ĸ�������*/
	double CalResult(char exp[]);			/*������׺���ʽ�ļ��㺯��*/
	double Directcalresult(char str[]);
	int check(char str[],char chestr[100]);
	char str[100],exp[100],chestr[100];			/*str�洢ԭ�������ʽ��exp�洢��Ӧ�ĺ�׺���ʽ,chestr�洢�ݴ��ַ�'^'*/
	do
	{
	printf("�������ʽΪ:\n");
	gets(str);
	if(check(str,chestr))						/*�����ݴ���*/
		{
			printf("���ʽ���ڣ�\n");
			printf("%s\n",str);
			printf(chestr);							/*�����������ָ������ĵط�*/
			printf("\n");
			printf("Try agian<y/n>::");
			while(ch=getchar(),ch!='n'&&ch!='y');
		if(ch=='y')
			{
			system("pause");
			continue;
			}
		if(ch=='n')
			{
				system("pause");
				exit(-1);
			}
		}
	change(str,exp);									/*���ú�������׺ת��Ϊ��׺*/
	printf("��׺���ʽΪ:%s\n",exp);
	printf("������Ϊ��%f\n",CalResult(exp));			/*���ú��������׺���ʽ*/
	printf("Try agian<y/n>::");
	while(ch=getchar(),ch!='n'&&ch!='y');
	system("pause");
	}
	while(ch!='n');






	int err = 0, r;
    char*        input_expr;
    ST_expr*    stack;
 
    input_expr = (char*)calloc(1, MAX_INPUT_SIZE + 16);
    stack       = (ST_expr*)malloc(sizeof(ST_expr)*MAX_STACK_SIZE);
 
    while(NULL != fgets(input_expr, MAX_INPUT_SIZE, stdin))
    {
        r = solve(input_expr, stack);
        switch(r)
        {
        case 0:
            /* empty line */
            break;
 
        case 1:
            printf("%s\n", stack[0].kExprValue);
            break;
 
        default:
            fprintf(stderr, "%d, ERROR(%d): %s\n", ++err, r, input_expr);
        }
    }
 
    free(input_expr);
    free(stack);
 
    return err;
}




void change(char str[],char ch[])						/*��ǰ׺���ʽת��Ϊ��׺���ʽ*/
{
	int i=0;											/*str������*/
	int k=0;
	char c;											/*�ַ�����ȡ���ķ���C��*/
	stack st;/*�������ջ*/
	OpNode op;
	OpNode ops;
	init(&st);									/*��ʼ������ջ*/
	c=str[i++];
	while(c!='\0')				/*���ַ�������ɨ��*/
	{
	if((c>='0'&&c<='9')||c=='.')		/*����ַ�Ϊ���ֻ�С����*/
		{
		while((c>='0'&&c<='9')||c=='.')
			{
			ch[k++]=c;										/*���ַ�ֱ�ӷ���������*/
			c=str[i++];
			}
				ch[k++]='|';			/*����������һ���ָ���*/
		}
	if(c=='(')				/*����ַ���������*/
	{
	op.op='(';
	op.level=-1;			/*���������ȼ�Ϊ-1*/
	push(&st,op);			/*��������ֱ����ջ*/
	}
	if(c==')')				/*����ַ�Ϊ������*/
		{
		op=top(&st);							/*���ȹ۲�ջ��*/
		while(st.size!=0&&op.op!='(')			/*������������Ų���ջ��Ϊ��*/
			{
				op=pop(&st);				/*��ջ������������*/
				ch[k++]=op.op;
				if(st.size>0)				/*�ٴμ��ջ�Ƿ�Ϊ�գ�*/
				op=top(&st);
				else 
				break;				/*Ϊ�վͽ���*/
			}
		pop(&st);				/*ȥ��������*/
		}
		if(c=='+'||c=='-')		/*�����+-��*/
		{
			op.op=c;
			op.level=1;			/*���ȼ�Ϊ1*/
			if(st.size==0)
			{
				push(&st,op);		/*�����ʱջΪ��ֱ����ջ*/
			}
				else
				{
				ops=top(&st);					/*�۲�ջ��*/
				while(ops.level>=op.level)		/*���ջ�����ȼ���*/
					{
					ops=pop(&st);
					ch[k++]=ops.op;					/*��ջ��Ԫ��ȡ������������*/
					if(st.size>0)
					ops=top(&st);							/*�����пղ�����ջΪ�ս���*/
					else
					break;
					}
			push(&st,op);						/*��ʱջ�����ȼ��ͣ���ջ*/
				}
		}
		if(c=='*'||c=='/'||c=='%')				/*����ǽ���*/
		{
			op.op=c;
			op.level=2;									/*���ȼ�Ϊ1*/
			if(st.size==0)
			{
			push(&st,op);								/*�����ʱջΪ��ֱ����ջ*/
			}
			else
			{
				ops=top(&st);								/*�۲�ջ��*/
				while(ops.level>=op.level)					/*���ջ�����ȼ���*/
				{
				ops=pop(&st);							/*��ջ��Ԫ��ȡ������������*/
				ch[k++]=ops.op;
				if(st.size>0)
				ops=top(&st);							/*�����пղ�����ջΪ�ս���*/
				else
				break;
				}
				push(&st,op);								/*��ʱջ�����ȼ��ͣ���ջ*/
			}
		}
			c=str[i++];							/*�����ԼӼ�����һ���ַ�*/
	}
	while(st.size!=0)							/*����ж�ջ�����Ϊ��*/
	{
		ops=pop(&st);							/*ȡ��ջ��Ԫ�ش���������*/
		ch[k++]=ops.op;
	}
	ch[k]='\0';									/*��\0��Ϊ��β��������*/
}
	double CalResult(char exp[])					/*��׺���ʽ�ļ���*/
	{
	char c;
	numstack numst;								/*������ֵջ*/
	double d1,d2,dr;
	int k=0;					/*��׺���ʽ������*/
	int i=0;					/*���ַ�ת��Ϊ������������*/
	char*s;
	char trans[100];			/*���ַ���ʾ��һ������*/
	init2(&numst);				/*ʵ����ֵջ*/
	c=exp[k++];
	while(c!='\0')				/*��ʼɨ���׺���ʽ*/
	{
		if(c=='+'||c=='-'||c=='*'||c=='/'||c=='%')		/*����ǲ�����*/
		{
	switch(c)
	{
		case'+':			/*����Ǽӷ�����*/
			d2=pop2(&numst);
			d1=pop2(&numst);
			dr=d1+d2;		/*��Ӻ���ջ*/
			push2(&numst,dr);
			break;
		case'-':			/*����Ǽ�������*/
			d2=pop2(&numst);
			d1=pop2(&numst);
			dr=d1-d2;		/*�������ջ*/
			push2(&numst,dr);
			break;
		case'*':			/*����ǳ˷�����*/
			d2=pop2(&numst);
			d1=pop2(&numst);
			dr=d1*d2;			/*��˺���ջ*/
			push2(&numst,dr);
			break;
		case'/':				/*����ǳ�������*/
			d2=pop2(&numst);
			d1=pop2(&numst);
			dr=d1/d2;			/*�������ջ*/
			push2(&numst,dr);
			break;
		case'%':						/*�����ȡ�����*/
			d2=pop2(&numst);
			d1=pop2(&numst);
			dr=(double)((int)d1%(int)d2);	/*����ת����ȡ�����ջ*/
			push2(&numst,dr);
			break;
	}
		}
		if(c>='0'&&c<='9'||c=='.')				/*������ַ���ʾ������*/
		{
		while(c>='0'&&c<='9'||c=='.')
		{
		trans[i++]=c;					/*���ַ��������������һ����ɨ��*/
		c=exp[k++];
		}
		trans[i++]='\0';					/*����ʾ���ֵ��ַ�������*/
		i=0;
		s=trans;							/*��ָ��ָ�������*/
		d1=atof(s);			/*���ú������ַ���ת��Ϊ������*/
		push2(&numst,d1);
		}
		c=exp[k++];
	}
	return pop2(&numst);		/*�����������ֵջ�У�ȡ����Ϊ����ֵ*/
	}
	double Directcalresult(char str[])				/*���ʽ��ֱ�Ӽ�������*/
	{
	stack ms;											/*��������ջ*/
	numstack mns;										/*������ֵջ*/
	double calculate(double od1,double od2,OpNode op);
	int index=0;											/*str������*/
	int len=strlen(str);char	c;
	char trans[100];					/*�����ֵ��һ���ַ�*/
	int i=0;						/*trans������*/
	char *s;double d;
	OpNode tempn;					/*��ŵ�ǰɨ��Ĳ�����*/
	OpNode templn;
	double oda,odb,odr;
	double result;					/*��Ϊ����ֵ���ؽ��*/
	init(&ms);						/*ʵ������ջ*/
	init2(&mns);
		while(index<len)			/*��ʼ���û�����ı��ʽ����ɨ��*/
	{
	c=str[index++];
	if(c>='0'&&c<='9'||c=='.')			/*����������ַ���С����*/
	{
		while(c>='0'&&c<='9'||c=='.')
		{
			trans[i++]=c;				/*�����������ɨ����һ��*/
			c=str[index++];
		}
			trans[i++]='\0';			/*ɨ����һ������������*/
			i=0;						/*������0*/
			s=trans;
			d=atof(s);
			push2(&mns,d);				/*ת��Ϊ��������ջ*/
	}
			if(c=='+'||c=='-')			/*�����+-*/
			{
			tempn.level=1;				/*���ȼ���Ϊ1*/
			tempn.op=c;
			if(ms.size==0)
			{
				push(&ms,tempn);		/*ջΪ��ֱ����ջ*/
			}
			else
			{
			templn=top(&ms);
			while(templn.level>=tempn.level)		/*ջ�����ȼ���*/
			{
				templn=pop(&ms);					/*ȡ���������Ͳ���������*/
				odb=pop2(&mns);
				oda=pop2(&mns);
				odr=calculate(oda,odb,templn);
				push2(&mns,odr);					/*��������ջ*/
				if(ms.size>0)
				{
				templn=top(&ms);				/*���ջ�ս���*/
				}
				else
					break;
			}
	push(&ms,tempn);								/*��������ջ*/
			}
			}
		if(c=='*'||c=='/'||c=='%')					/*�����%����*/
		{
	tempn.level=2;					/*�������ȼ�Ϊ2*/
	tempn.op=c;if(ms.size==0)
	{
	push(&ms,tempn);			/*ջ��ֱ����ջ*/
	}
	else
	{
	templn=top(&ms);
		while(templn.level>=tempn.level)		/*ջ�����ȼ���*/
		{
		templn=pop(&ms);		/*ȡ���������Ͳ���������*/
		odb=pop2(&mns);oda=pop2(&mns);
		odr=calculate(oda,odb,templn);
		push2(&mns,odr);		/*��������ջ*/
		if(ms.size>0)
		{
		templn=top(&ms);
		}
		else
		break;					/*���ջ�ս���*/
		templn=top(&ms);
		}
		push(&ms,tempn);		/*��������ջ*/
		}
		}
		if(c=='(')				/*�����������*/
		{
		tempn.level=-1;
		tempn.op=c;									/*ֱ����ջ���ȼ���λ-1*/
		push(&ms,tempn);
		}
			if(c==')')							/*�����������*/{
			while(tempn.op!='(')							/*���������Ž���*/
			{
			templn=pop(&ms);
			odb=pop2(&mns);				/*����ջ��ȡ���������ӷ���ջ��ȡ������*/
			oda=pop2(&mns);
			odr=calculate(oda,odb,templn);			/*����������ջ*/	
			push2(&mns,odr);
			if(ms.size>0)
			tempn=top(&ms);
			else
			break;						/*���ջ�ս���*/
			}
		pop(&ms);				/*ȡ��������*/
		}
		}
		tempn=top(&ms);
		while(1)
		{
			templn=pop(&ms);
			odb=pop2(&mns);							/*����ջ��ȡ���������ӷ���ջ��ȡ������*/
			oda=pop2(&mns);
			odr=calculate(oda,odb,templn);			/*����������ջ*/
			push2(&mns,odr);
			if(ms.size>0)
			tempn=top(&ms);							/*���ջ�ս���*/
			else
			break;
		}
			result=pop2(&mns);						/*���Ľ������ֵջ�з���*/
			return result;
}
		double calculate(double od1,double od2,OpNode op)						/*��֪�������Ͳ������ļ���*/
		{
		switch(op.op)
		{
		case'+':
			return od1+od2;
		case'-':
			return od1-od2;				/*�жϲ��������ĸ�ִ����Ӧ����*/
		case'*':
			return od1*od2;
		case'/':
			return od1/od2;
		case'%':
			return(double)((int)od1%(int)od2);
		}
	return 0;									/*�������Ķ�û��ִ�з���0*/
		}
	int check(char str[],char chestr[100])			/*�ݴ���*/
	{
		char c;
		char cdivide;
		int i=0;									/*str������*/
		stack che;									/*����ƥ���õ���ջ*/
		OpNode temp;
		int k=0;									/*chestr������*/
		int isinteger(char integer[100]);			/*%�������ж��Ƿ�������*/
		char s1[10];								/*%����ʱ�洢%���ҵ�����*/
		char s2[10];
		int indexs1=0;								/*s1s2������*/
		int indexs2=0;
		int j;										/*����chestr����*/
		int flag=0;									/*0--û�г���1--�д�*/
		int tag=0;
		init(&che);
		c=str[i];									/*��ʼɨ��*/
			for(j=0;j<99;j++)
			{
			chestr[j]=' ';								/*�����ʼ�����Ժ����'^'*/
			}
		chestr[j]='\0';
		while(c!='\0')
		{
		if(c=='(')							/*����������ž���ջ*/
		{
			temp.op=c;
			push(&che,temp);
		}
		if(c==')')								/*�����������*/
		{
		if(che.size>0)
		{
		pop(&che);								/*ջ��Ϊ�վ�ȡ��һ��������*/
		}
		else
		{
		flag=1;
		printf("ȱ��������\n");					/*������ʾ�д�*/
		chestr[i]='^';							/*�������¼�'^'*/
		}
	}
		if(c=='/')								/*�жϳ����Ƿ�Ϊ0*/
		{
		j=0;
		cdivide=str[i+1+j];						/*ȡ�����ź����*/
		while(cdivide>='0'&&cdivide<='9'||cdivide=='.')			/*���������С�����һֱ��*/
		{
		s1[j++]=cdivide;
		if(cdivide!='0'&&cdivide!='.')					/*�������0����ȷ������*/
		{
			tag=1;
			break;
		}
			cdivide=str[i+j+1];
		}
			if(!tag)						/*���tagΪ0����ڴ������Ϊ0*/
			{
			chestr[i+1]='^';
			flag=1;										/*flagΪ1��ʾ�д�*/
		}
		}
		if(c=='%')								/*ȡ��������ݴ�*/
		{
		while(str[i-indexs1-1]>='0'&&str[i-indexs1-1]<='9'||str[i-indexs1-1]=='.')					/*��%Ϊ������ǰɨ��*/
	{
		s1[indexs1++]=str[i-indexs1-1];																/*���������С����*/}/*����s1��*/
		while(str[i+indexs2+1]>='0'&&str[i+indexs2+1]<='9'||str[i+indexs2+1]=='.')					/*��%Ϊ�������ɨ��*/
		{																							/*���������С����*/
			s2[indexs2++]=str[i+indexs2+1];															/*����s1��*/
		}
			if(isinteger(s1))																		/*���ú����ж�s1�ڴ浽�Ƿ�������*/
			{
				printf("ȡ���㷨��һ����ӦΪ��������\n");
				flag=1;																				/*��¼Ϊ�д�*/
				chestr[i-indexs1]='^';
			}
			if(isinteger(s2))										/*���ú����ж�s2�ڴ浽�Ƿ�������*/
			{
			printf("ȡ���㷨�ڶ�����ӦΪ��������\n");
			flag=1;													/*��¼Ϊ�д�*/
			chestr[i+indexs2]='^';
			}
		}i++;
		c=str[i];								/*������һ���ַ�*/
		}
		if(che.size>0)
		{										/*������ջ��Ϊ��*/
			printf("ȱ��������\n");				/*ջ�л���û��Ե������ű���*/
		}
			return flag;						/*�����Ƿ��д�*/
	}
	int isinteger(char integer[100])			/*�ж��������Ƿ�������*/
	{
	int i=0;									/*�����������������*/
	char c;
	c=integer[i++];
	while(c!='\0')								/*ֱ���ַ������ɨ�����*/
	{
	if(c=='.')									/*ֻҪ��һ���ַ�ΪС����Ͳ�������*/
		return 1;
	else
	c=integer[i++];								/*ɨ����һ��*/
	}
	return 0;
}


