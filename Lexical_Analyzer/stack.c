//链栈，元素类型为char
#include <stdlib.h>
#include "stack.h"
#define FALSE 0
#define TRUE 1

//栈初始化
LinkStack* InitStack()
{
	LinkStack* S = NULL;
	return S;
}

//入栈
void Push(LinkStack **p, SElemType e) //传递指针的指针用于修改栈指针（要传递指针的指针是因为只有这样才能改变指针的位置（都怪C没有引用））
{
	LinkStack* sp;
	sp = (LinkStack*)malloc(sizeof(LinkStack));
	if (sp)
	{
		sp->data = e;
		sp->next = *p; //开始写成了p，调试了两天才发现
	}
	LinkStack** ssp = &sp;
	*p = *ssp;
}

//出栈
SElemType Pop(LinkStack** p)
{
	LinkStack* sp = &**p; //指向栈顶的指针
	if ((*p) == NULL)
		return NULL;
	SElemType e;
	e = (*p)->data;
	(*p) = (*p)->next; //如果栈中只有一个元素则出栈后栈空
	free(sp);
	return e;
}

//判空
Status StackEmpty(LinkStack* S)
{
	if (S == NULL)
		return TRUE;
	else
		return FALSE;
}

//获取栈顶元素
SElemType GetTop(LinkStack* S)
{
	if (S)
		return S->data;
	else
		return NULL;
}

//销毁栈。由于弹出所有元素后栈指针自动置空，所以直接弹出所有元素，当调用函数结束时自动销毁
void Destroy(LinkStack **p)
{
    if (!StackEmpty(*p))
    {
        Pop(p);
    }
}