#include "BigNumber.h"
// ԭ��<=>����
BigInt* ToComplement(BigInt* src, BigInt* dst)
{
    int i;

    if (src->bit[SIGN_BIT] == NEGATIVE)  // ������
    {
        dst->bit[SIGN_BIT] = 1;

        for (i = 0; i < SIGN_BIT && src->bit[i] == 0; i++)
            dst->bit[i] = src->bit[i];

        if (i == SIGN_BIT)    // -0�Ĳ���
            dst->bit[i] = 0;
        else                  // ��0����
        {
            dst->bit[i] = src->bit[i];
            for (i++; i < SIGN_BIT; i++)
                dst->bit[i] = !src->bit[i];
        }
    }
    else  // �����󲹲���
    {
        for (i = 0; i < BIG_INT_BIT_LEN; i++)
            dst->bit[i] = src->bit[i];
    }

    return dst;
}

// תΪԭ��
BigInt* ToTrueForm(BigInt* src, BigInt* dst)
{
    return ToComplement(src, dst);
}

// תΪ�෴���Ĳ��� [x]�� => [-x]��, 
// ע�⣺���������8λ������������-128�෴���Ĳ���
// �㷨��˼������ͬ����λһ���󲹣�������λҲҪȡ������֤������ȷ��
BigInt* ToOppositeNumberComplement(BigInt* src, BigInt* dst)
{
    int i;

    for (i = 0; i < BIG_INT_BIT_LEN && src->bit[i] == 0; i++)
        dst->bit[i] = src->bit[i];

    // ���0�෴���Ĳ���
    if (i != BIG_INT_BIT_LEN)
    {
        dst->bit[i] = src->bit[i];

        // ������λҲҪȡ��
        for (i++; i < BIG_INT_BIT_LEN; i++)
            dst->bit[i] = !src->bit[i];
    }

    return dst;
}

// ����BigInt
BigInt* CopyBigInt(BigInt* src, BigInt* dst)
{
    int i;
    for (i = 0; i < BIG_INT_BIT_LEN; i++)
        dst->bit[i] = src->bit[i];
    return dst;
}

// ��������
BigInt* ShiftArithmeticLeft(BigInt* src, int indent, BigInt* dst)
{
    int i, j;

    dst->bit[SIGN_BIT] = src->bit[SIGN_BIT];

    for (i = SIGN_BIT - 1, j = i - indent; j >= 0; i--, j--)
    {
        dst->bit[i] = src->bit[j];
    }

    while (i >= 0)
    {
        dst->bit[i--] = 0;
    }

    return dst;
}

//����(��������)
BigInt* ShiftLeft(BigInt* src, int indent, BigInt* dst)
{
    int i, j;

    for (i = SIGN_BIT, j = i - indent; j >= 0; i--, j--)
    {
        dst->bit[i] = src->bit[j];
    }

    while (i >= 0)
    {
        dst->bit[i--] = 0;
    }

    return dst;
}

// ��������
BigInt* ShiftArithmeticRight(BigInt* src, int indent, BigInt* dst)
{
    int i, j;

	for (i = 0, j = i + indent; j < BIG_INT_BIT_LEN; i++, j++)
    {
        dst->bit[i] = src->bit[j];
    }

	while (i < BIG_INT_BIT_LEN)
    {
		dst->bit[i++] = src->bit[SIGN_BIT];
    }

    return dst;
}


// �ӷ�ʵ��
BigInt* DoAdd(BigInt* a, BigInt* b, BigInt* result)
{
    int i, t, carryFlag;           // ��λ��־
    int aSign = a->bit[SIGN_BIT];  // a�ķ���
    int bSign = b->bit[SIGN_BIT];  // b�ķ���

    for (carryFlag = i = 0; i < BIG_INT_BIT_LEN; i++)
    {
        t = a->bit[i] + b->bit[i] + carryFlag;
        result->bit[i] = t % 2;
        carryFlag = t > 1 ? 1 : 0;
    }

    return result;
}

// ����ʵ��
BigInt* DoSub(BigInt* a, BigInt* b, BigInt* result)
{
    BigInt t;

    ToOppositeNumberComplement(b, &t);
    DoAdd(a, &t, result);

    return result;
}

// �˷�ʵ�� Booth�㷨[����1λ��] ת��Ϊ��λ�ͼӷ�
BigInt* DoMul(BigInt* a, BigInt* b, BigInt* result)
{
    int i;
    BigInt c, t;

    ToOppositeNumberComplement(a, &c);  // c=[-a]�Ĳ�

    memset(t.bit, 0, BIG_INT_BIT_LEN);  // ��ʼ��Ϊ0

    // �Ӹ�λ����ʼ��������ͬ��λ����Ϊ���Ϊ0
    for (i = SIGN_BIT; i > 0 && b->bit[i] == b->bit[i - 1]; i--);

    while (i > 0)
    {
        ShiftArithmeticLeft(&t, 1, &t);

        if (b->bit[i] != b->bit[i - 1])
        {
            DoAdd(&t, b->bit[i - 1] > b->bit[i] ? a : &c, &t);
        }

        i--;
    }

    // ���һ������λ
    ShiftArithmeticLeft(&t, 1, &t);
    if (b->bit[0] != 0)
    {
        DoAdd(&t, &c, &t);
    }

    return CopyBigInt(&t, result);
}
//����С����
BigInt* DoMulDec(BigInt* a, BigInt* b, BigInt* result)
{
	int i = 1;
	BigInt	c,t;
	ToOppositeNumberComplement(a,&c);

	memset(t.bit, 0, BIG_INT_BIT_LEN);  // ��ʼ��Ϊ0

	if (b->bit[0] != 0)
    {
            DoAdd(&t, &c, &t);
    }

	while (i < BIG_INT_BIT_LEN)
    {
        ShiftArithmeticRight(&t, 1, &t);

        if (b->bit[i] != b->bit[i - 1])
        {
            DoAdd(&t, b->bit[i - 1] > b->bit[i] ? a : &c, &t);
        }

        i++;
    }

    return CopyBigInt(&t, result);
}

BigFloat* DoAddFloat(BigFloat* a, BigFloat* b, BigFloat* result)
{
    BigInt cha, one, tmptail;
	int resnag = 0;
	
	if(a->tail.bit[SIGN_BIT] == 0 && b->tail.bit[SIGN_BIT] == 0)
		resnag = 1;
	if(a->tail.bit[SIGN_BIT] == 1 && b->tail.bit[SIGN_BIT] == 1)
		resnag = 2;

	
	memset(one.bit, 0, BIG_INT_BIT_LEN);
	one.bit[0] = 1;

	DoSub(&a->order, &b->order, &cha);
	if(cha.bit[SIGN_BIT] == 1)
	{
		CopyBigInt(&a->tail,&tmptail);
		while(IsZero(&cha) == 0)
		{
			DoAdd(&cha, &one, &cha);
			ShiftArithmeticRight(&tmptail, 1, &tmptail);
		}
		DoAdd(&tmptail,&b->tail, &result->tail);
		CopyBigInt(&b->order, &result->order);
	}
	else
	{
		CopyBigInt(&b->tail,&tmptail);
		while(IsZero(&cha) == 0)
		{		
			DoSub(&cha, &one, &cha);
			ShiftArithmeticRight(&tmptail, 1, &tmptail);
		}
		DoAdd(&tmptail,&a->tail, &result->tail);
		CopyBigInt(&a->order, &result->order);
	}
	if(resnag == 1 && result->tail.bit[SIGN_BIT] == 1)
	{
		ShiftArithmeticRight(&result->tail, 1, &result->tail);
		result->tail.bit[SIGN_BIT] = 0;
		DoAdd(&result->order, &one, &result->order);
	}
	if(resnag == 2 && result->tail.bit[SIGN_BIT] == 0)
	{
		ShiftArithmeticRight(&result->tail, 1, &result->tail);
		result->tail.bit[SIGN_BIT] = 1;
		DoAdd(&result->order, &one, &result->order);
	}

	ajustFloat(result,result);
    return result;
}

BigFloat * DoSubFloat(BigFloat *a, BigFloat *b, BigFloat *result)
{
	BigFloat c;
	ToOppositeNumberComplement(&b->tail,&c.tail);
	CopyBigInt(&b->order, &c.order);
	return DoAddFloat(a,&c, result);
}

BigFloat * DoMulFloat(BigFloat *a, BigFloat *b, BigFloat *result)
{
	DoAdd(&a->order, &b->order, &result->order);
	DoMulDec(&a->tail, &b->tail, &result->tail);
	ajustFloat(result, result);
	return result;
}

BigFloat * DoDivFloat(BigFloat *a, BigFloat *b, BigFloat *result)
{
	BigInt one;
	memset(one.bit, 0, BIG_INT_BIT_LEN);
	one.bit[0] = 1;
	bool resnag = false;
	if(a->tail.bit[SIGN_BIT] ^ b->tail.bit[SIGN_BIT] == 1)
		resnag = true;
	DoSub(&a->order, &b->order, &result->order);
	DoDivDec(&a->tail, &b->tail, &result->tail);
	if((resnag && result->tail.bit[SIGN_BIT] == 0) || (!resnag && result->tail.bit[SIGN_BIT] == 1))
	{
		DoAdd(&result->order,&one,&result->order);
		ShiftArithmeticRight(&result->tail,1,&result->tail);
		result->tail.bit[SIGN_BIT] = (result->tail.bit[SIGN_BIT] + 1)%2;
	}
	ajustFloat(result, result);
	return result;
}


//��񻯸�����
BigFloat *ajustFloat(BigFloat *src, BigFloat *dst)
{
	BigInt one;
	memset(one.bit, 0, BIG_INT_BIT_LEN);
	one.bit[0] = 1;
	
	CopyBigInt(&src->order, &dst->order);
	CopyBigInt(&src->tail, &dst->tail);
	if(IsZero(&src->tail) == 1)
	{
		return dst;
	}

	while(dst->tail.bit[SIGN_BIT] == dst->tail.bit[SIGN_BIT - 1])
	{
		ShiftArithmeticLeft(&dst->tail,1,&dst->tail);
		DoSub(&dst->order, &one, &dst->order);
	}
	return dst;

}



//����С������
BigInt* DoDivDec(BigInt* a, BigInt* b, BigInt* result)
{
	int i = 1;
	BigInt	c,t,r;
	ToOppositeNumberComplement(b,&c);

	memset(t.bit, 0, BIG_INT_BIT_LEN);  // ��ʼ��Ϊ0
	memset(r.bit, 0, BIG_INT_BIT_LEN);  // ��ʼ��Ϊ0


	DoAdd(a, a->bit[SIGN_BIT] == b->bit[SIGN_BIT] ? &c : b, &r);

	while (i < BIG_INT_BIT_LEN)
    {
		if(r.bit[SIGN_BIT] == b->bit[SIGN_BIT])
		{
			t.bit[0]	= 1;
			ShiftLeft(&t, 1, &t);
			ShiftLeft(&r, 1, &r);
			DoAdd(&r,&c,&r);
		}
		else
		{
			t.bit[0]	= 0;
			ShiftLeft(&t, 1, &t);
			ShiftLeft(&r, 1, &r);
			DoAdd(&r,b,&r);
		}
        i++;
    }
	t.bit[0]	= 1;
    return CopyBigInt(&t, result);
}
// �ڲ����������£���ȡ����������Ƶĳ���
int GetMaxLeftShiftLen(BigInt* a)
{
    int i, k;
    BigInt t;

    ToTrueForm(a, &t);

    for (i = SIGN_BIT - 1, k = 0; i >= 0 && t.bit[i] == 0; i--, k++);

    return k;
}

// �ж�Bigint�Ƿ�Ϊ0
int IsZero(BigInt* a)
{
    int i;
    for (i = 0; i < BIG_INT_BIT_LEN; i++)
    {
        if (a->bit[i] != 0)
            return 0;
    }
    return 1;
}

// ����ʵ�� ��2�ַ�ȥ���̵ĸ���Ϊ1��λ д�ò������><
BigInt* DoDiv(BigInt* a, BigInt* b, BigInt* result, BigInt* remainder)
{
    int low, high, mid;
    BigInt c, d, e, t;

    low = 0;                       // ��ʼ����������ֵ
    high = GetMaxLeftShiftLen(b);  // ��ȡ����������Ƶĳ���

    memset(t.bit, 0, BIG_INT_BIT_LEN);  // ��ʼ����Ϊ0
    CopyBigInt(a, &c);                  // ��ʼ��cΪ������a

    // ͬ���������
    if (a->bit[SIGN_BIT] == b->bit[SIGN_BIT])
    {
        t.bit[SIGN_BIT] = POSITIVE;

        while (1)
        {
            while (low <= high)
            {
                mid = (low + high) / 2;
                ShiftArithmeticLeft(b, mid, &d);
                DoSub(&c, &d, &e);  // e = c - d

                // e >= 0����ʾ����
                if (d.bit[SIGN_BIT] == e.bit[SIGN_BIT] || IsZero(&e))
                    low = mid + 1;
                else
                    high = mid - 1;
            }

            // high����󹻼�����λ��
            // high == -1 ��ʾ�Ѿ���1���ĳ�������������
            if (high != -1)
            {
                t.bit[high] = 1;

                // ����ͳһ�����ˣ��ɸĽ�
                ShiftArithmeticLeft(b, high, &d);
                DoSub(&c, &d, &c);  // c = c - d
                
                low = 0;
                high--;
            }
            else
            {
                // ��ʱc����ʾ�ı�������Ϊ��������
                CopyBigInt(&c, remainder);
                break;
            }
        }
    }

    // ����������
    else
    {
        t.bit[SIGN_BIT] = NEGATIVE;

        while (1)
        {
            while (low <= high)
            {
                mid = (low + high) / 2;
                ShiftArithmeticLeft(b, mid, &d);
                DoAdd(&c, &d, &e);  // e = c + d

                // e >= 0
                if (d.bit[SIGN_BIT] != e.bit[SIGN_BIT] || IsZero(&e))
                    low = mid + 1;
                else
                    high = mid - 1;
            }

            // high����󹻼�����λ��
            // high == -1 ��ʾ�Ѿ���1���ĳ�������������
            if (high != -1)
            {
                t.bit[high] = 1;

                // ����ͳһ�����ˣ��ɸĽ�
                ShiftArithmeticLeft(b, high, &d);
                DoAdd(&c, &d, &c);  // c = c + d

                low = 0;
                high--;
            }
            else
            {
                // ��ʱc����ʾ�ı�������Ϊ��������
                CopyBigInt(&c, remainder);
                break;
            }
        }
    }

    return ToComplement(&t, result);
}



// ��ģʵ��
BigInt* DoMod(BigInt* a, BigInt* b, BigInt* remainder)
{
    BigInt c;

    DoDiv(a, b, &c, remainder);

    return remainder;
}

int GetTrueValueLen(BigInt* a)
{
    int i;
    BigInt t;

    ToTrueForm(a, &t);

    for (i = SIGN_BIT - 1; i >= 0 && t.bit[i] == 0; i--);

    return i + 1;
}



void printBigBinary(BigInt the)
{
	int i;
	for(i = SIGN_BIT; i>=0; i--)
	{
		printf("%d ", the.bit[i]);
	}
	printf("\n");

}

char *bigInt2strHex(BigInt *the)
{
	char *str;
	str = (char *)malloc(BIG_INT_BIT_LEN/4 + 1);
	int i, num=0, j=0;
	char tmp = 0;
	for(i=SIGN_BIT; i>=0; i--)
	{
		tmp = tmp * 2 + the->bit[i];
		num++;
		if(num == 4)
		{
			if(tmp > 9)
				str[j] = tmp - 10 + 'A';
			if(tmp <= 9 && tmp >= 0)
				str[j] = tmp + '0';
			j++;
			num = 0;
		}

	}
	str[BIG_INT_BIT_LEN/4] = '\0';
	return str;
}

char *bigInt2str(BigInt *bi)
{
	char *str;
	str = (char *)malloc(BIG_INT_BIT_LEN + 1);
	char tmp[BIG_INT_BIT_LEN + 1] = {0};
	char add[BIG_INT_BIT_LEN + 1] = {0};

	tmp[0] = 1;
	if(bi->bit[0] == 1)
		add[0] = 1;
	
	bool nag = false;
	BigInt bigtmp;
	CopyBigInt(bi, &bigtmp);
	
	if(bi->bit[SIGN_BIT] == 1)
	{
		nag = true;
		ToOppositeNumberComplement(&bigtmp, &bigtmp);
	}
	int i, j, the, carry = 0;
	for(i = 1; i < SIGN_BIT; i++)
	{
		carry = 0;
		for(j = 0; j< SIGN_BIT; j++)
		{
			the = tmp[j] * 2 + carry;
			carry	= the / 10;
			tmp[j]	= the % 10;
		}
		
		if(bigtmp.bit[i] == 0)
			continue;
		
		carry = 0;
		for(j = 0; j< SIGN_BIT; j++)
		{
			the		= tmp[j] + add[j] + carry;
			carry	= the / 10;
			add[j]	= the % 10;
		}
		carry = 0;
	}

	for(j = SIGN_BIT - 1; add[j] == 0 && j >= 0; j--);

	if(nag)
		str[0] = '-';
	else
		str[0] = '+';
	for(i = 1; i <= j + 1; i++)
	{
		str[i] = add[j - i + 1] + '0';
	}
	
	str[i] = '\0';
	
	return str;
}

//����ֵ��С������һλ��ʼ�ģ�ֻ��ת����ֵ
char *bigDec2str(BigInt *bd)
{
	char *add = (char *)malloc(BIG_INT_BIT_LEN + 1);
	char tmp[BIG_INT_BIT_LEN + 1] = {0};

	memset(add, 0, BIG_INT_BIT_LEN + 1);
	tmp[0] = 5;

	if(bd->bit[SIGN_BIT - 1] == 1)
		add[0] = 5;

	int i, j, the, carry = 0;
	for(i = SIGN_BIT - 2; i >= 0; i--)
	{
		carry = 0;
		for(j = 0; j< SIGN_BIT; j++)
		{
			the		= tmp[j] + carry * 10;
			carry	= the % 2;
			tmp[j]	= the / 2;
		}
		
		if(bd->bit[i] == 0)
			continue;
		
		carry = 0;
		for(j = BIG_INT_BIT_LEN; j >= 0; j--)
		{
			the		= tmp[j] + add[j] + carry;
			carry	= the / 10;
			add[j]	= the % 10;
		}
		carry = 0;
	}
	for(i = BIG_INT_BIT_LEN; i >= 0 && add[i]==0; i--);

	add[i + 1] = '\0';
	for(;i >= 0; i--)
		add[i] = add[i] + '0';
	return add;
}

char *bigFloat2str(BigFloat *bf)
{
	int i, j, order = 0, tmpi = 1;
	
	//������
	BigInt border;
	bool bnag = false;
	if(bf->order.bit[SIGN_BIT] == 1)
	{
		bnag = true;
		ToOppositeNumberComplement(&bf->order,&border);
	}
	else
	{
		CopyBigInt(&bf->order, &border);
	}



	for(i = 0; i < 31  && i < SIGN_BIT; i++)
	{
		if(border.bit[i] == 1)
			order += tmpi;
		tmpi *= 2;
	}
	
	char tmp[BIG_INT_BIT_LEN + 1] = {0};
	char add[BIG_INT_BIT_LEN + 1] = {0};
	tmp[0] = 5;

	bool nag = false;
	BigInt bigbp;
	BigInt bigap;
	if(bf->tail.bit[SIGN_BIT] == 1)
	{
		nag = true;
		ToOppositeNumberComplement(&bf->tail, &bigap);
	}
	else
		CopyBigInt(&bf->tail, &bigap);
	
	char *strap;
	char *strbp;
	if(bnag)
	{
		strbp = (char *)malloc(2);
		strcpy(strbp, "+");
		ShiftArithmeticRight(&bigap, order, &bigap);
	}
	else
	{
		memset(bigbp.bit, 0, BIG_INT_BIT_LEN);
		for(i = 0; i < order; i++)
		{
			bigbp.bit[i] = bigap.bit[SIGN_BIT + i - order];
		}
	
		strbp = bigInt2str(&bigbp);
		ShiftArithmeticLeft(&bigap, order, &bigap);
	}
	strap = bigDec2str(&bigap);
	
	for(j = SIGN_BIT - 1; j >= 0 && add[j] == 0; j--);

	if(nag)
		strbp[0] = '-';
	else
		strbp[0] = '+';

	char *str = (char *)malloc(2*BIG_INT_BIT_LEN);
	
	strcpy(str,strbp);
	strcat(str,".");
	strcat(str,strap);
	free(strap);
	free(strbp);	
	
	return str;
}

//0��ʾ�������
int str2bigInt(char *str, BigInt *inum)
{
	BigInt itmp;
	BigInt ten;
	memset(ten.bit, 0 ,BIG_INT_BIT_LEN);
	ten.bit[1] = ten.bit[3] = 1;
	int wei, rem, i;
	bool nag = false;
	
	if(*str == '-')
	{
		nag = true;
		str++;
	}
	else if(*str == '+')
		str++;

	memset(inum->bit, 0, BIG_INT_BIT_LEN);

	while(*str != '\0')
	{
		memset(itmp.bit, 0, BIG_INT_BIT_LEN);
		wei = *str - '0';
			
		if(wei > 9 || wei < 0)
			return 0;
			
		i = 0;
		while(wei > 0)
		{
			rem = wei % 2;
			itmp.bit[i] = rem;
			wei /= 2;
			i++;
		}
		DoMul(inum, &ten, inum);
		DoAdd(inum, &itmp, inum);
		str++;
	}
	if(nag)
		ToOppositeNumberComplement(inum,inum);
	return 1;

}

void str2bigDec(char *str,BigInt *dec)
{
	int i,len = 0, carry = 0;
	char *p = str;
	while(*p != '\0')
	{
		len++;
		p++;
	}

	char *suse = (char *)malloc(len + 1);
	strcpy(suse,str);
	int the;

	for(i = 0; i < len; i++)
	{
		*(suse + i) = *(suse + i) - '0';
	}

	for(i = SIGN_BIT - 1;i >= 0; i--)
	{
		p = suse + len - 1;
		while(p-suse > 0)
		{
			
			the		= 2 * *p + carry;
			carry	=  the / 10;
			*p		= the % 10;
			p--;
		}
		dec->bit[i] = (2 * *p + carry) / 10;
		*p          = (2 * *p + carry) % 10;
	}
}

void str2bigFloat(char *str, BigFloat *fnum)
{
	memset(fnum->order.bit, 0, BIG_INT_BIT_LEN);
	memset(fnum->tail.bit, 0, BIG_INT_BIT_LEN);
	bool nag = false;
	int start = 0;
	
	if(*str == '-')
	{
		nag = true;
		start++;
	}
	else if(*str == '+')
		start++;

	int len = 0;
	char *p = str;
	while(*p != '\0')
	{
		len++;
		p++;
	}
	char *suse = (char *)malloc(len + 1);
	strcpy(suse,str);
	char *point = strchr(suse,'.');
	*point = '\0';
	point++;

	BigInt bp;
	BigInt ap;
	memset(bp.bit, 0, BIG_INT_BIT_LEN);
	memset(ap.bit, 0, BIG_INT_BIT_LEN);
	str2bigInt(suse + start ,&bp);
	str2bigDec(point,&ap);

	int i = SIGN_BIT;
	while(bp.bit[i] == 0 && i >= 0)
		i--;
	int num = i + 1;

	
	ShiftArithmeticRight(&ap, num, &fnum->tail);
	for(;i>=0;i--)
	{
		fnum->tail.bit[SIGN_BIT - num + i] = bp.bit[i];
	}
	
	BigSetInt(&fnum->order, num);

	if(nag)
		ToOppositeNumberComplement(&fnum->tail,&fnum->tail);
	
}


void BigSetInt(BigInt *big, int n)
{
	memset(big->bit, 0, BIG_INT_BIT_LEN);

	int i = 0;
	while(n > 0)
	{
		big->bit[i]	= n % 2;
		n /= 2;
		i++;
	}
}

char *doIntAdd(char *num1, char *num2)
{
	char   *res;
	BigInt inum1;
	BigInt inum2;
	BigInt ires;

	str2bigInt(num1, &inum1);
	str2bigInt(num2, &inum2);
	DoAdd(&inum1, &inum2, &ires);
	
	res = bigInt2str(&ires);
	return res;

}

char *doIntSub(char *num1, char *num2)
{
	char *res;
	BigInt inum1;
	BigInt inum2;
	BigInt ires;

	str2bigInt(num1, &inum1);
	str2bigInt(num2, &inum2);
	DoSub(&inum1, &inum2, &ires);
	
	res = bigInt2str(&ires);
	return res;
}
char *doIntMul(char *num1, char *num2)
{
	char *res;
	BigInt inum1;
	BigInt inum2;
	BigInt ires;

	str2bigInt(num1, &inum1);
	str2bigInt(num2, &inum2);
	DoMul(&inum1, &inum2, &ires);
	
	res = bigInt2str(&ires);
	return res;
}
char *doIntDiv(char *num1, char *num2)
{
	char *res;
	BigInt inum1;
	BigInt inum2;
	BigInt inum3;
	BigInt ires;

	str2bigInt(num1, &inum1);
	str2bigInt(num2, &inum2);
	DoDiv(&inum1, &inum2, &ires, &inum3);
	
	res = bigInt2str(&ires);
	return res;
}
char *doFloatAdd(char *num1, char *num2)
{
	char *res;
	BigFloat	fnum1;
	BigFloat	fnum2;
	BigFloat	fres;

	str2bigFloat(num1, &fnum1);
	str2bigFloat(num2, &fnum2);
	DoAddFloat(&fnum1, &fnum2, &fres);

	res = bigFloat2str(&fres);
	return res;
}
char *doFloatSub(char *num1, char *num2)
{
	char *res;
	BigFloat	fnum1;
	BigFloat	fnum2;
	BigFloat	fres;

	str2bigFloat(num1, &fnum1);
	str2bigFloat(num2, &fnum2);
	DoSubFloat(&fnum1, &fnum2, &fres);

	res = bigFloat2str(&fres);
	return res;
}
char *doFloatMul(char *num1, char *num2)
{
	char *res;
	BigFloat	fnum1;
	BigFloat	fnum2;
	BigFloat	fres;

	str2bigFloat(num1, &fnum1);
	str2bigFloat(num2, &fnum2);
	DoMulFloat(&fnum1, &fnum2, &fres);

	res = bigFloat2str(&fres);
	return res;
}
char *doFloatDiv(char *num1, char *num2)
{
	char *res;
	BigFloat	fnum1;
	BigFloat	fnum2;
	BigFloat	fres;

	str2bigFloat(num1, &fnum1);
	str2bigFloat(num2, &fnum2);
	DoDivFloat(&fnum1, &fnum2, &fres);

	res = bigFloat2str(&fres);
	return res;
}