/*
    BigInt: 大整型
    update: 2014.06.09 by xkuga

    程序实现了这几种大数运算：+, -, *, /, ^, %, ^%
    采用的是二进制的方式实现, BigInt的存储与运算均采用补码
    乘法：采用的是Booth一位乘
    除法：用二分法去做减法
    幂运算与模幂运算：都是采用二进制的方式实现的，并非一个一个乘
    具体实现原理可@C程序设计语言

    虽然可能不是很快，不过还是能用><
    而且程序比较易懂，对于理解计算机如何运算有一定的帮助
    如果有任何错误或改进可@C程序设计语言，谢谢!
*/


#include "BigNumber.h"

#define FILE_NAME_LEN 32               //文件名长度
#define ACU_PI_LEN    2000

int checkNum(char *str);
BigFloat doPiOperate(int num);
void doOperate();

int checkNum(char *str)
{
	if(str == NULL)
		return 1;

	int res = 0;
	
	while(*str != '\0')
	{
		if(res == 2 && *str == '.')
			return 1;
		if(*str > '9' || *str < '0' && *str != '.')
			return 1;
		if(*str == '.')
			res = 2;
		str++;
	}
	
	return res;

}

BigFloat doPiOperate(int num)
{
	BigFloat pi;
	BigFloat tmp;
	BigFloat fens;
	BigFloat fenx;
	BigFloat ftwo;
	BigFloat fone;
	str2bigFloat("2.0", &tmp);
	str2bigFloat("1.0", &fone);
	str2bigFloat("2.0", &pi);
	str2bigFloat("2.0",&ftwo);
	str2bigFloat("1.0",&fens);
	str2bigFloat("3.0",&fenx);
	
	
	int i;
	for(i = 1;i <= num; i++)
	{
		DoMulFloat(&tmp, &fens, &tmp);
		DoDivFloat(&tmp, &fenx, &tmp);
		DoAddFloat(&pi, &tmp, &pi);
		DoAddFloat(&fens, &fone, &fens);
		DoAddFloat(&fenx, &ftwo, &fenx);
	}

	return pi;
}


void doOperate()
{
	char opType		= '+';
	char numType	= 'i';
	char fromFile	= 'n';
	char num1[BIG_INT_BIT_LEN];
	char num2[BIG_INT_BIT_LEN];
	char filename[FILE_NAME_LEN];
	FILE *fp;
	char *res;
	while(1)
	{
		printf("Hello, please input the operate type(+|-|*|/|p|q):\n");
		scanf("%c",&opType);
		if(opType == 'q')
			return;
		if(opType == 'p')
		{
			BigFloat pi;
			pi = doPiOperate(ACU_PI_LEN);
			res = bigFloat2str(&pi);
			printf("PI:%s\n", res);
			continue;
		}

		fflush(stdin);
		printf("Now, please input the number type(i-int,f-float):\n");
		scanf("%c", &numType);
		fflush(stdin);
		printf("Do you want to input the numbers from file?( y or n)");
		scanf("%c", &fromFile);
		fflush(stdin);
		if(fromFile == 'n')
		{
			printf("Now, please input the first number:\n");
			gets(num1);
			fflush(stdin);
			printf("Now, please input the second number:\n");
			gets(num2);
			fflush(stdin);
		}
		else if(fromFile == 'y')
		{
			printf("Now, please put the file with numbers by lines in the root path and input the file name:\n");
			gets(filename);
			fp = fopen(filename,"r+");
			fgets(num1, FILE_NAME_LEN - 1, fp);
			printf("num1:%s", num1);
			fgets(num2, FILE_NAME_LEN - 1, fp);
			printf("num2:%s\n", num2);

			num1[strlen(num1) - 1] = '\0';
			num2[strlen(num2) - 1] = '\0';
		}
		else
		{
			printf("Input error:Please input y or n");
		}

		int check1 = checkNum(num1);
		int check2 = checkNum(num2);
		if(check1 == 1 || check2 == 1)
		{
			printf("You have input wrong numbers!\n");
		}
		else if(opType == '+' && numType == 'i' && check1 == 0 && check2 == 0)
		{
			res = doIntAdd(num1, num2);
			printf("The result is:%s\n", res);
		}
		else if(opType == '-' && numType == 'i' && check1 == 0 && check2 == 0)
		{
			res = doIntSub(num1, num2);
			printf("The result is:%s\n", res);
		}
		else if(opType == '*' && numType == 'i' && check1 == 0 && check2 == 0)
		{
			res = doIntMul(num1, num2);
			printf("The result is:%s\n", res);
		}
		else if(opType == '/' && numType == 'i' && check1 == 0 && check2 == 0)
		{
			res = doIntDiv(num1, num2);
			printf("The result is:%s\n", res);
		}
		else if(opType == '+' && numType == 'f' && check1 == 2 && check2 == 2)
		{
			res = doFloatAdd(num1, num2);
			printf("The result is:%s\n", res);
		}
		else if(opType == '-' && numType == 'f' && check1 == 2 && check2 == 2)
		{
			res = doFloatSub(num1, num2);
			printf("The result is:%s\n", res);
		}
		else if(opType == '*' && numType == 'f' && check1 == 2 && check2 == 2)
		{
			res = doFloatMul(num1, num2);
			printf("The result is:%s\n", res);
		}
		else if(opType == '/' && numType == 'f' && check1 == 2 && check2 == 2)
		{
			res = doFloatDiv(num1, num2);
			printf("The result is:%s\n", res);
		}
		else
		{
			printf("Input error!\n");
		}

		if(fromFile == 'y')
		{
			fclose(fp);
		}
	}
}

int main()
{
	doOperate();
	return 0;
}
