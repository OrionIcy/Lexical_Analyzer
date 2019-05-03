#pragma once
//Õ»Í·ÎÄ¼þ
typedef int Status;
typedef char SElemType;
typedef struct SNode
{
	SElemType data;
	struct SNode* next;
} LinkStack;

LinkStack* InitStack();
void Push(LinkStack** p, SElemType e);
SElemType Pop(LinkStack** p);
Status StackEmpty(LinkStack* S);
SElemType GetTop(LinkStack* S);
void Destroy(LinkStack** p);