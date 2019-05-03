#ifndef __BLIZZARD_HASH_H
#define __BLIZZARD_HASH_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXMPQHASHTABLELEN 8192

typedef struct
{
    long nHashA;
    long nHashB;
    unsigned int bExists;
} MPQHASHTABLE;

unsigned long cryptTable[0x500];
int MPQHashLen;
MPQHASHTABLE* MyHashTable;

unsigned long HashString(char *lpszString, unsigned long dwHashType);
unsigned int MPQHashTableInit(MPQHASHTABLE **ppHashTable, long nTableLength);
void MPQHashTableFree(MPQHASHTABLE *pHashTable);
unsigned int MPQHashTableAdd(char *lpszString, MPQHASHTABLE *pHashTable);
long MPQHashTableIsExist(char *lpszString, MPQHASHTABLE *pHashTable);

#endif
