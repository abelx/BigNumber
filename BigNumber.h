#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define BIG_INT_BIT_LEN 128           // ����BigInt��λ��
#define SIGN_BIT BIG_INT_BIT_LEN - 1   // ����λ��λ��
#define BUFFER_SIZE BIG_INT_BIT_LEN    // ��������С
#define POSITIVE 0                     // 0��ʾ����
#define NEGATIVE 1                     // 1��ʾ����

typedef struct    // ���������ͣ����ò����ʾ
{
    char bit[BIG_INT_BIT_LEN];
}BigInt;

typedef struct
{
	BigInt order;
	BigInt tail;
} BigFloat;

int IsZero(BigInt*);
BigFloat *ajustFloat(BigFloat *, BigFloat *);
BigInt* DoDivDec(BigInt*, BigInt*, BigInt*);
void BigSetInt(BigInt *, int);

BigInt* ToComplement(BigInt*, BigInt*);

BigInt* ToTrueForm(BigInt*, BigInt*);
BigInt* ToOppositeNumberComplement(BigInt*, BigInt*);
BigInt* CopyBigInt(BigInt*, BigInt*);
BigInt* ShiftArithmeticLeft(BigInt*, int, BigInt*);
BigInt* ShiftLeft(BigInt*, int, BigInt*);
BigInt* ShiftArithmeticRight(BigInt* src, int indent, BigInt* dst);
BigInt* DoAdd(BigInt* a, BigInt* b, BigInt* result);
BigInt* DoSub(BigInt* a, BigInt* b, BigInt* result);
BigInt* DoMul(BigInt* a, BigInt* b, BigInt* result);
BigInt* DoMulDec(BigInt* a, BigInt* b, BigInt* result);
BigFloat* DoAddFloat(BigFloat* a, BigFloat* b, BigFloat* result);
BigFloat * DoSubFloat(BigFloat *a, BigFloat *b, BigFloat *result);
BigFloat * DoMulFloat(BigFloat *a, BigFloat *b, BigFloat *result);
BigFloat * DoDivFloat(BigFloat *a, BigFloat *b, BigFloat *result);
BigFloat *ajustFloat(BigFloat *src, BigFloat *dst);
BigInt* DoDivDec(BigInt* a, BigInt* b, BigInt* result);
int GetMaxLeftShiftLen(BigInt* a);
int IsZero(BigInt* a);
BigInt* DoDiv(BigInt* a, BigInt* b, BigInt* result, BigInt* remainder);
BigInt* DoMod(BigInt* a, BigInt* b, BigInt* remainder);
int GetTrueValueLen(BigInt* a);
void printBigBinary(BigInt the);
char *bigInt2strHex(BigInt *the);
char *bigInt2str(BigInt *bi);
char *bigDec2str(BigInt *bd);
char *bigFloat2str(BigFloat *bf);
int str2bigInt(char *str, BigInt *inum);
void str2bigDec(char *str,BigInt *dec);
void str2bigFloat(char *str, BigFloat *fnum);
void BigSetInt(BigInt *big, int n);
char *doIntAdd(char *num1, char *num2);
char *doIntSub(char *num1, char *num2);
char *doIntMul(char *num1, char *num2);
char *doIntDiv(char *num1, char *num2);
char *doFloatAdd(char *num1, char *num2);
char *doFloatSub(char *num1, char *num2);
char *doFloatMul(char *num1, char *num2);
char *doFloatDiv(char *num1, char *num2);
