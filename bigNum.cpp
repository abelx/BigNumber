#include <stdio.h>
#include "BigNum.h"


//判断是否为负数
bool is_nag(const bigNum the)
{
	if(the[BIG_LENGTH-1] >= 128)
		return true;
	else
		return false;
}

int add_big_int(const bigNum add1,const bigNum add2,bigNum result)
{
	int carry	= 0;
	int tmp		= 0;

	
	int i = 0;
	for( ; i<BIG_LENGTH; i++)
	{
		tmp			= add1[i] + add2[i] + carry;
		result[i]	= (unsigned char) tmp%256;
		carry		= tmp/256;
	}
	
	if((is_nag(add1) &&  is_nag(add2) && !is_nag(result)) || (!is_nag(add1) && !is_nag(add2) && is_nag(result)))
		return 1; //溢出

	return 0;
}

//取反
void inv_big_int(const bigNum theNum,bigNum toNum)
{
	int carry	= 1;
	int tmp		= 0;

	
	int i = 0;
	for( ; i<BIG_LENGTH; i++)
	{
		tmp					= (unsigned char)~theNum[i] + carry;
		toNum[i]			= tmp % 256;
		carry				= tmp/256;
	}
}

//减法
int sub_big_int(const bigNum beSubed, const bigNum theSub, bigNum result)
{
	bigNum tmp;
	inv_big_int(theSub,tmp);
	return add_big_int(tmp, beSubed,result);
}


void print_big_int(const bigNum theNum, char mod = 'a')
{
	int i;
	bigNum tmp;
	if(mod == 'a')
	{
		if(is_nag(theNum))
		{
			inv_big_int(theNum,tmp);
			printf("- ");
			for(i = BIG_LENGTH - 1; i >= 0; i-- )
				printf("%d ",tmp[i]);
		}
		else
		for(i = BIG_LENGTH - 1; i >= 0; i-- )
			printf("%d ",theNum[i]);
	}
}

//大数右移一位
void shr_big_int(bigNum sbig, bigNum dbig)
{
	int i;
	for(i = 0; i < BIG_LENGTH; i++)
	{
		dbig[i] = sbig[i] >> 1;
		
		if(i < BIG_LENGTH - 1)
			(sbig[i+1] & 1) == 1 ? (dbig[i] |= (1 << 7)):(dbig[i] &= 127);
	}

}

void copy(bigNum sbig, bigNum dbig)
{
	int i;
	for(i = 0; i < BIG_LENGTH; i++)
	{
		dbig[i] = sbig[i];
	}

}

//初始化大数
void init_big_int(bigNum num, unsigned char value = 0)
{
	int i;
	for(i = 0; i < BIG_LENGTH; i++)
	{
		num[i] = value;
	}

}

//比较大小，返回0相等，1表示num1 > num2，2表示num1 < num2
int cmp_big_int(const bigNum num1, const bigNum num2)
{
	bigNum	tmp;
	bool	flag = false;
	sub_big_int(num1, num2, tmp);

	int i;
	for(i = 0; i < BIG_LENGTH; i++)
	{
		if(tmp[i] != 0)
		{
			flag = true;
			break;
		}

	}

	if(flag){
		if(is_nag(tmp))
			return 2;
		else
			return 1;
	}
	else
		return 0;

}

int muti_big_int(bigNum beMuted, bigNum theMut, bigNum result)
{
	bigNum pos1, pos2, tmp,tmp2;
	init_big_int(tmp,0);
	init_big_int(tmp2,0);
	init_big_int(result,0);
	int flag = 0;
	
	if(is_nag(beMuted))
	{
		inv_big_int(beMuted,pos1);
		flag += 1;
	}
	else
		copy(beMuted,pos1);

	if(is_nag(theMut))
	{
		inv_big_int(theMut,pos2);
		flag += 1;
	}
	else
		copy(theMut,pos2);

	int i, j, carry;
	for(i = 0; i < BIG_LENGTH; i++)
	{
		carry = 0;
		
		if(pos2[i] == 0)
			result[i]	= tmp[0];
		else
		{
			result[i]	= (pos2[i] * pos1[0] + tmp[0]) % 256;
			carry		= (pos2[i] * pos1[0] + tmp[0]) / 256;
		}
		for(j = 1; j < BIG_LENGTH; j++)
		{
			if(pos2[i] == 0)
				tmp[j-1]	= tmp[j];
			else
			{
				tmp[j-1]	= (pos2[i] * pos1[j] + carry + tmp[j]) % 256;
				carry		= (pos2[i] * pos1[j] + carry + tmp[j]) / 256;
			}
		}
		tmp[j]		= carry;
	}
	if(cmp_big_int(tmp,tmp2) != 0)
		return 1; //溢出
	else
	{
		if(flag == 1)
			inv_big_int(result,result);
	}


	return 0;
}

int div_big_int(bigNum deDived, bigNum theDiv, bigNum reslut)
{
	return 0;
}


int main()
{
	bigNum a = {123,234};
	bigNum b = {3};
	bigNum c = {7};
	bigNum d;
	sub_big_int(b,c,d);
	muti_big_int(d,a,c);
	print_big_int(c);
	//int d = cmp_big_int(c,b);

	return 0;
}


