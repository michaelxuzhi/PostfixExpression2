#include<stdio.h>				/*导入需要用到的各种包*/
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



typedef struct                  /*定义结构体用来存储操作符*/
{
	char op;                        /*存储字符*/
	int level;						/*存储优先级*/
}OpNode;



typedef struct
{
	OpNode op[100];
	int top;
	int size;						/*表示栈内元素的个数*/
}stack;


							/*定义符号栈*/
void init(stack*st)				/*初始化栈*/
{
	st->size=0;
	st->top=0;
}



OpNode pop(stack*a)				/*出栈*/
{
	if(a->size==0)					/*如果栈为空结束操作*/
	{
		exit(-1);
	}
	a->size--;
	return a->op[--(a->top)];		/*取出栈顶元素*/
}



void push(stack*a,OpNode op)		/*入栈函数*/
{
	a->size++;
	a->op[(a->top)++]=op;
}



OpNode top(stack*a)				/*观察栈顶函数*/
{
	if(a->size==0)					/*如果栈为空结束操作*/
	{
		printf("stack is empty\n");
		exit(-1);
	}
	return a->op[(a->top)-1];			/*只得到栈顶的值而不出栈*/
}



typedef struct						/*定义数值栈*/
{
	double num[100];
	int top;						/*栈顶指针*/
	int size;
}numstack;



void init2(numstack *st)				/*初始化数值栈*/
{
	st->size=0;
	st->top=0;}
	double pop2(numstack *a)				/*数值栈出栈*/
{
	if(a->size==0)						/*出栈前的判空*/
	{
		exit(-1);
	}
	a->size--;
	return a->num[--(a->top)];			/*得到栈顶的值*/
}



void push2(numstack*a,double num)	/*入栈*/
{
	a->size++;
	a->num[(a->top)++]=num;
}



int main()							/*主函数*/
{
	char ch='y';
	void change(char str[],char exp[]);		/*声明要用到的各个函数*/
	double CalResult(char exp[]);			/*声明后缀表达式的计算函数*/
	double Directcalresult(char str[]);
	int check(char str[],char chestr[100]);
	char str[100],exp[100],chestr[100];			/*str存储原算术表达式，exp存储对应的后缀表达式,chestr存储容错字符'^'*/
	do
	{
	printf("算术表达式为:\n");
	gets(str);
	if(check(str,chestr))						/*调用容错函数*/
		{
			printf("表达式错在：\n");
			printf("%s\n",str);
			printf(chestr);							/*根据输入情况指出错误的地方*/
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
	change(str,exp);									/*调用函数将中缀转化为后缀*/
	printf("后缀表达式为:%s\n",exp);
	printf("运算结果为：%f\n",CalResult(exp));			/*调用函数计算后缀表达式*/
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




void change(char str[],char ch[])						/*将前缀表达式转化为后缀表达式*/
{
	int i=0;											/*str的索引*/
	int k=0;
	char c;											/*字符串中取出的放在C中*/
	stack st;/*定义符号栈*/
	OpNode op;
	OpNode ops;
	init(&st);									/*初始化符号栈*/
	c=str[i++];
	while(c!='\0')				/*对字符串进行扫描*/
	{
	if((c>='0'&&c<='9')||c=='.')		/*如果字符为数字或小数点*/
		{
		while((c>='0'&&c<='9')||c=='.')
			{
			ch[k++]=c;										/*将字符直接放入数组中*/
			c=str[i++];
			}
				ch[k++]='|';			/*在其后面放入一个分隔符*/
		}
	if(c=='(')				/*如果字符是左括号*/
	{
	op.op='(';
	op.level=-1;			/*定义其优先级为-1*/
	push(&st,op);			/*将左括号直接入栈*/
	}
	if(c==')')				/*如果字符为右括号*/
		{
		op=top(&st);							/*首先观察栈顶*/
		while(st.size!=0&&op.op!='(')			/*如果不是左括号并且栈不为空*/
			{
				op=pop(&st);				/*出栈并存入数组中*/
				ch[k++]=op.op;
				if(st.size>0)				/*再次检查栈是否为空，*/
				op=top(&st);
				else 
				break;				/*为空就结束*/
			}
		pop(&st);				/*去掉左括号*/
		}
		if(c=='+'||c=='-')		/*如果是+-号*/
		{
			op.op=c;
			op.level=1;			/*优先级为1*/
			if(st.size==0)
			{
				push(&st,op);		/*如果此时栈为空直接入栈*/
			}
				else
				{
				ops=top(&st);					/*观察栈顶*/
				while(ops.level>=op.level)		/*如果栈顶优先级高*/
					{
					ops=pop(&st);
					ch[k++]=ops.op;					/*将栈顶元素取出存入数组中*/
					if(st.size>0)
					ops=top(&st);							/*进行判空操作，栈为空结束*/
					else
					break;
					}
			push(&st,op);						/*此时栈顶优先级低，入栈*/
				}
		}
		if(c=='*'||c=='/'||c=='%')				/*如果是进行*/
		{
			op.op=c;
			op.level=2;									/*优先级为1*/
			if(st.size==0)
			{
			push(&st,op);								/*如果此时栈为空直接入栈*/
			}
			else
			{
				ops=top(&st);								/*观察栈顶*/
				while(ops.level>=op.level)					/*如果栈顶优先级高*/
				{
				ops=pop(&st);							/*将栈顶元素取出存入数组中*/
				ch[k++]=ops.op;
				if(st.size>0)
				ops=top(&st);							/*进行判空操作，栈为空结束*/
				else
				break;
				}
				push(&st,op);								/*此时栈顶优先级低，入栈*/
			}
		}
			c=str[i++];							/*索引自加检索下一个字符*/
	}
	while(st.size!=0)							/*最后判断栈如果不为空*/
	{
		ops=pop(&st);							/*取出栈内元素存入数组中*/
		ch[k++]=ops.op;
	}
	ch[k]='\0';									/*将\0作为结尾存入数组*/
}
	double CalResult(char exp[])					/*后缀表达式的计算*/
	{
	char c;
	numstack numst;								/*建立数值栈*/
	double d1,d2,dr;
	int k=0;					/*后缀表达式的索引*/
	int i=0;					/*将字符转化为浮点数的索引*/
	char*s;
	char trans[100];			/*存字符表示的一段数字*/
	init2(&numst);				/*实现数值栈*/
	c=exp[k++];
	while(c!='\0')				/*开始扫描后缀表达式*/
	{
		if(c=='+'||c=='-'||c=='*'||c=='/'||c=='%')		/*如果是操作符*/
		{
	switch(c)
	{
		case'+':			/*如果是加法操作*/
			d2=pop2(&numst);
			d1=pop2(&numst);
			dr=d1+d2;		/*相加后入栈*/
			push2(&numst,dr);
			break;
		case'-':			/*如果是减法操作*/
			d2=pop2(&numst);
			d1=pop2(&numst);
			dr=d1-d2;		/*相减后入栈*/
			push2(&numst,dr);
			break;
		case'*':			/*如果是乘法操作*/
			d2=pop2(&numst);
			d1=pop2(&numst);
			dr=d1*d2;			/*相乘后入栈*/
			push2(&numst,dr);
			break;
		case'/':				/*如果是除法操作*/
			d2=pop2(&numst);
			d1=pop2(&numst);
			dr=d1/d2;			/*相除后入栈*/
			push2(&numst,dr);
			break;
		case'%':						/*如果是取余操作*/
			d2=pop2(&numst);
			d1=pop2(&numst);
			dr=(double)((int)d1%(int)d2);	/*类型转化并取余后入栈*/
			push2(&numst,dr);
			break;
	}
		}
		if(c>='0'&&c<='9'||c=='.')				/*如果是字符表示的数字*/
		{
		while(c>='0'&&c<='9'||c=='.')
		{
		trans[i++]=c;					/*将字符存入数组进行下一个的扫描*/
		c=exp[k++];
		}
		trans[i++]='\0';					/*将表示数字的字符串结束*/
		i=0;
		s=trans;							/*将指针指向该数组*/
		d1=atof(s);			/*利用函数将字符串转化为浮点数*/
		push2(&numst,d1);
		}
		c=exp[k++];
	}
	return pop2(&numst);		/*最后结果将在数值栈中，取出作为返回值*/
	}
	double Directcalresult(char str[])				/*表达式的直接计算出结果*/
	{
	stack ms;											/*建立符号栈*/
	numstack mns;										/*建立数值栈*/
	double calculate(double od1,double od2,OpNode op);
	int index=0;											/*str的索引*/
	int len=strlen(str);char	c;
	char trans[100];					/*存放数值的一段字符*/
	int i=0;						/*trans的索引*/
	char *s;double d;
	OpNode tempn;					/*存放当前扫描的操作符*/
	OpNode templn;
	double oda,odb,odr;
	double result;					/*作为返回值返回结果*/
	init(&ms);						/*实现两个栈*/
	init2(&mns);
		while(index<len)			/*开始对用户输入的表达式进行扫描*/
	{
	c=str[index++];
	if(c>='0'&&c<='9'||c=='.')			/*如果是数字字符或小数点*/
	{
		while(c>='0'&&c<='9'||c=='.')
		{
			trans[i++]=c;				/*将其存入数组扫描下一个*/
			c=str[index++];
		}
			trans[i++]='\0';			/*扫描完一个数结束数组*/
			i=0;						/*索引归0*/
			s=trans;
			d=atof(s);
			push2(&mns,d);				/*转化为浮点数入栈*/
	}
			if(c=='+'||c=='-')			/*如果是+-*/
			{
			tempn.level=1;				/*优先级设为1*/
			tempn.op=c;
			if(ms.size==0)
			{
				push(&ms,tempn);		/*栈为空直接入栈*/
			}
			else
			{
			templn=top(&ms);
			while(templn.level>=tempn.level)		/*栈顶优先级高*/
			{
				templn=pop(&ms);					/*取出操作数和操作符计算*/
				odb=pop2(&mns);
				oda=pop2(&mns);
				odr=calculate(oda,odb,templn);
				push2(&mns,odr);					/*结算结果入栈*/
				if(ms.size>0)
				{
				templn=top(&ms);				/*如果栈空结束*/
				}
				else
					break;
			}
	push(&ms,tempn);								/*操作符入栈*/
			}
			}
		if(c=='*'||c=='/'||c=='%')					/*如果是%操作*/
		{
	tempn.level=2;					/*定义优先级为2*/
	tempn.op=c;if(ms.size==0)
	{
	push(&ms,tempn);			/*栈空直接入栈*/
	}
	else
	{
	templn=top(&ms);
		while(templn.level>=tempn.level)		/*栈顶优先级高*/
		{
		templn=pop(&ms);		/*取出操作数和操作符计算*/
		odb=pop2(&mns);oda=pop2(&mns);
		odr=calculate(oda,odb,templn);
		push2(&mns,odr);		/*结算结果入栈*/
		if(ms.size>0)
		{
		templn=top(&ms);
		}
		else
		break;					/*如果栈空结束*/
		templn=top(&ms);
		}
		push(&ms,tempn);		/*操作符入栈*/
		}
		}
		if(c=='(')				/*如果是左括号*/
		{
		tempn.level=-1;
		tempn.op=c;									/*直接入栈优先级定位-1*/
		push(&ms,tempn);
		}
			if(c==')')							/*如果是右括号*/{
			while(tempn.op!='(')							/*遇到左括号结束*/
			{
			templn=pop(&ms);
			odb=pop2(&mns);				/*从数栈中取两个数，从符号栈里取操作符*/
			oda=pop2(&mns);
			odr=calculate(oda,odb,templn);			/*计算出结果入栈*/	
			push2(&mns,odr);
			if(ms.size>0)
			tempn=top(&ms);
			else
			break;						/*如果栈空结束*/
			}
		pop(&ms);				/*取出左括号*/
		}
		}
		tempn=top(&ms);
		while(1)
		{
			templn=pop(&ms);
			odb=pop2(&mns);							/*从数栈中取两个数，从符号栈里取操作符*/
			oda=pop2(&mns);
			odr=calculate(oda,odb,templn);			/*计算出结果入栈*/
			push2(&mns,odr);
			if(ms.size>0)
			tempn=top(&ms);							/*如果栈空结束*/
			else
			break;
		}
			result=pop2(&mns);						/*最后的结果在数值栈中返回*/
			return result;
}
		double calculate(double od1,double od2,OpNode op)						/*已知操作符和操作数的计算*/
		{
		switch(op.op)
		{
		case'+':
			return od1+od2;
		case'-':
			return od1-od2;				/*判断操作符是哪个执行相应计算*/
		case'*':
			return od1*od2;
		case'/':
			return od1/od2;
		case'%':
			return(double)((int)od1%(int)od2);
		}
	return 0;									/*如果上面的都没有执行返回0*/
		}
	int check(char str[],char chestr[100])			/*容错函数*/
	{
		char c;
		char cdivide;
		int i=0;									/*str的索引*/
		stack che;									/*括号匹配用到的栈*/
		OpNode temp;
		int k=0;									/*chestr的索引*/
		int isinteger(char integer[100]);			/*%计算是判断是否是整数*/
		char s1[10];								/*%操作时存储%左右的数字*/
		char s2[10];
		int indexs1=0;								/*s1s2的索引*/
		int indexs2=0;
		int j;										/*数组chestr索引*/
		int flag=0;									/*0--没有出错1--有错*/
		int tag=0;
		init(&che);
		c=str[i];									/*开始扫描*/
			for(j=0;j<99;j++)
			{
			chestr[j]=' ';								/*数组初始化待以后加入'^'*/
			}
		chestr[j]='\0';
		while(c!='\0')
		{
		if(c=='(')							/*如果是左括号就入栈*/
		{
			temp.op=c;
			push(&che,temp);
		}
		if(c==')')								/*如果是右括号*/
		{
		if(che.size>0)
		{
		pop(&che);								/*栈不为空就取出一个左括号*/
		}
		else
		{
		flag=1;
		printf("缺少左括号\n");					/*否则提示有错*/
		chestr[i]='^';							/*右括号下加'^'*/
		}
	}
		if(c=='/')								/*判断除数是否为0*/
		{
		j=0;
		cdivide=str[i+1+j];						/*取出除号后的数*/
		while(cdivide>='0'&&cdivide<='9'||cdivide=='.')			/*如果是数或小数点就一直存*/
		{
		s1[j++]=cdivide;
		if(cdivide!='0'&&cdivide!='.')					/*如果不是0则正确并结束*/
		{
			tag=1;
			break;
		}
			cdivide=str[i+j+1];
		}
			if(!tag)						/*如果tag为0则存在错误除数为0*/
			{
			chestr[i+1]='^';
			flag=1;										/*flag为1表示有错*/
		}
		}
		if(c=='%')								/*取余操作的容错*/
		{
		while(str[i-indexs1-1]>='0'&&str[i-indexs1-1]<='9'||str[i-indexs1-1]=='.')					/*以%为中心向前扫描*/
	{
		s1[indexs1++]=str[i-indexs1-1];																/*如果是数或小数点*/}/*放在s1中*/
		while(str[i+indexs2+1]>='0'&&str[i+indexs2+1]<='9'||str[i+indexs2+1]=='.')					/*以%为中心向后扫描*/
		{																							/*如果是数或小数点*/
			s2[indexs2++]=str[i+indexs2+1];															/*放在s1中*/
		}
			if(isinteger(s1))																		/*调用函数判断s1内存到是否是整数*/
			{
				printf("取余算法第一个数应为整数运算\n");
				flag=1;																				/*记录为有错*/
				chestr[i-indexs1]='^';
			}
			if(isinteger(s2))										/*调用函数判断s2内存到是否是整数*/
			{
			printf("取余算法第二个数应为整数运算\n");
			flag=1;													/*记录为有错*/
			chestr[i+indexs2]='^';
			}
		}i++;
		c=str[i];								/*检索下一个字符*/
		}
		if(che.size>0)
		{										/*如果最后栈不为空*/
			printf("缺少右括号\n");				/*栈中还有没配对的左括号报错*/
		}
			return flag;						/*返回是否有错*/
	}
	int isinteger(char integer[100])			/*判断数组内是否是整数*/
	{
	int i=0;									/*传过来的数组的索引*/
	char c;
	c=integer[i++];
	while(c!='\0')								/*直到字符串最后扫描结束*/
	{
	if(c=='.')									/*只要有一个字符为小数点就不是整数*/
		return 1;
	else
	c=integer[i++];								/*扫描下一个*/
	}
	return 0;
}


