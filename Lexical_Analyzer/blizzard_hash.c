//��ѩMPQ��ϣ
#include "blizzard_hash.h"
#include <string.h>
#include <time.h>
#include <windows.h>

#define FALSE 0
#define TRUE 1
#define ERROR -1
#define _KEY_WORD_END "waiting for your expanding..."

//��ʼ�������ϣֵ���õ��ı�
static void InitCryptTable()
{
	unsigned long seed = 0x00100001, index1 = 0, index2 = 0, i;

	for (index1 = 0; index1 < 0x100; index1++)
	{
		for (index2 = index1, i = 0; i < 5; i++, index2 += 0x100)
		{
			unsigned long temp1, temp2;
			seed = (seed * 125 + 3) % 0x2AAAAB;
			temp1 = (seed & 0xFFFF) << 0x10;
			seed = (seed * 125 + 3) % 0x2AAAAB;
			temp2 = (seed & 0xFFFF);
			cryptTable[index2] = (temp1 | temp2);
		}
	}
}

//��ϣ����
//lpszString:�ַ����ĵ�ַ
//dwHashType:��ϣֵ����
//dwHashType = 0ʱ����Ĺ�ϣֵ����ȷ���ַ����ڹ�ϣ���е�λ�ã�
//dwHashType = 1��dwHashType = 2ʱ����Ĺ�ϣֵ������֤�ַ���
//�����ַ����Ĺ�ϣֵ
unsigned long HashString(char *lpszString, unsigned long dwHashType)
{
	unsigned char *key = (unsigned char *)lpszString;
	unsigned long seed1 = 0x7FED7FED, seed2 = 0xEEEEEEEE;
	int ch;

	while(*key != 0)
	{
		ch = toupper(*key++);
		seed1 = cryptTable[(dwHashType << 8) + ch] ^ (seed1 + seed2);
		seed2 = ch + seed1 + seed2 + (seed2 << 5) + 3;
	}
	return seed1;
}


//��ʼ����ϣ��
//*pHashTable�����ط���Ĺ�ϣ��ĵ�ַ
//nTableLength����ϣ��ĳ���
unsigned int MPQHashTableInit(MPQHASHTABLE **pHashTable, long nTableLength)
{
    MPQHashLen = nTableLength;
    if (MPQHashLen > MAXMPQHASHTABLELEN)
    {
        printf("Hash table is too long. \n");
        return 0;
    }
    long i = 0;
    MPQHASHTABLE* p = NULL;
	
	InitCryptTable();
	
	p = malloc(nTableLength * sizeof(MPQHASHTABLE));
	if (p == NULL)
	{
		printf("Allocation failed. \n");
		return 0;
	}
	*pHashTable = p;
		
	for (i = 0; i < nTableLength; i++)
	{
		(*pHashTable + i)->nHashA = -1;
		(*pHashTable + i)->nHashB = -1;
		(*pHashTable + i)->bExists = FALSE;
	}
	
	return TRUE;
}


//�ͷŹ�ϣ��
void MPQHashTableFree(MPQHASHTABLE *pHashTable)
{
	if (pHashTable != NULL)
	{
		free(pHashTable);
		pHashTable = NULL;
	}
}


//���ַ�������Ϣ�����ϣ��
//lpszString���ַ����ĵ�ַ
//pHashTable����ϣ��ĵ�ַ
unsigned int MPQHashTableAdd(char *lpszString, MPQHASHTABLE *pHashTable)
{
	const unsigned long HASH_OFFSET = 0, HASH_A = 1, HASH_B = 2;
	unsigned long nHash = HashString(lpszString, HASH_OFFSET);
	unsigned long nHashA = HashString(lpszString, HASH_A);
	unsigned long nHashB = HashString(lpszString, HASH_B);
	unsigned long nHashStart = nHash % MPQHashLen;
	unsigned long nHashPos = nHashStart;
		
	while ((pHashTable + nHashPos)->bExists)
	{
		nHashPos = (nHashPos + 1) % MPQHashLen;
		
		if (nHashPos == nHashStart)
		{
			return FALSE;
		}
	}
	
	(pHashTable + nHashPos)->nHashA = nHashA;
	(pHashTable + nHashPos)->nHashB = nHashB;
	(pHashTable + nHashPos)->bExists = TRUE;

	return TRUE;
}


//�ж�ĳ�ַ����ڹ�ϣ�����Ƿ����
//lpszString���ַ����ĵ�ַ
//pHashTable����ϣ��ĵ�ַ
//����-1��������
//����nHashPos�����ַ����ڹ�ϣ���е�����ֵ
long MPQHashTableIsExist(char *lpszString, MPQHASHTABLE *pHashTable)
{
	const unsigned long HASH_OFFSET = 0, HASH_A = 1, HASH_B = 2;
	unsigned long nHash = HashString(lpszString, HASH_OFFSET);
	unsigned long nHashA = HashString(lpszString, HASH_A);
	unsigned long nHashB = HashString(lpszString, HASH_B);
	unsigned long nHashStart = nHash % MPQHashLen;
	unsigned long nHashPos = nHashStart;
	//MPQHASHTABLE *_pHashTable = (MPQHASHTABLE *)pHashTable;
	
	while ((pHashTable + nHashPos)->bExists)
	{
		if (((pHashTable + nHashPos)->nHashA == nHashA) && 
			((pHashTable + nHashPos)->nHashB == nHashB))
		{
			return nHashPos;
		}
		else
		{
			nHashPos = (nHashPos +1) % MPQHashLen;
		}

		if (nHashPos == nHashStart)
		{
			break;
		}
	}

	return ERROR;
}
    
