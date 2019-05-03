//��ջ��Ԫ������Ϊchar
#include <stdlib.h>
#include "stack.h"
#define FALSE 0
#define TRUE 1

//ջ��ʼ��
LinkStack* InitStack()
{
	LinkStack* S = NULL;
	return S;
}

//��ջ
void Push(LinkStack **p, SElemType e) //����ָ���ָ�������޸�ջָ�루Ҫ����ָ���ָ������Ϊֻ���������ܸı�ָ���λ�ã�����Cû�����ã���
{
	LinkStack* sp;
	sp = (LinkStack*)malloc(sizeof(LinkStack));
	if (sp)
	{
		sp->data = e;
		sp->next = *p; //��ʼд����p������������ŷ���
	}
	LinkStack** ssp = &sp;
	*p = *ssp;
}

//��ջ
SElemType Pop(LinkStack** p)
{
	LinkStack* sp = &**p; //ָ��ջ����ָ��
	if ((*p) == NULL)
		return NULL;
	SElemType e;
	e = (*p)->data;
	(*p) = (*p)->next; //���ջ��ֻ��һ��Ԫ�����ջ��ջ��
	free(sp);
	return e;
}

//�п�
Status StackEmpty(LinkStack* S)
{
	if (S == NULL)
		return TRUE;
	else
		return FALSE;
}

//��ȡջ��Ԫ��
SElemType GetTop(LinkStack* S)
{
	if (S)
		return S->data;
	else
		return NULL;
}

//����ջ�����ڵ�������Ԫ�غ�ջָ���Զ��ÿգ�����ֱ�ӵ�������Ԫ�أ������ú�������ʱ�Զ�����
void Destroy(LinkStack **p)
{
    if (!StackEmpty(*p))
    {
        Pop(p);
    }
}