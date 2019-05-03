//Lexical analysis program. 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "stack.h"

#define _KEY_WORD_END "waiting for your expanding..."

//����ĵ��ʶ�Ԫ��Ľṹ
typedef struct WORD
{
    int typenum; //�ֱ���
    char* word; //ֵ
} WORD;
//������
typedef struct Node
{
    char data[1024];
    struct Node* next;
} ListNode;
ListNode* F, * R, * Fint, * Rint; //ͷβָ��

char token[1024]; //���ʻ�����
char* lineRead; //�����л�����
char* line; //ɨ���л�����

int p_token; //���ʻ�����ָʾ��
int p_line, pe_line; //ɨ���л�����ָʾ��
int p_symtab = 1; //��������ָʾ��

FILE* fp, * fpdtup, * fpsymtab, * fperr, * fpchseq; //�����ļ�ָ��

char ch; //�����ж�ȡ�ַ�

int linenum = 0; //�к�
int isRwtab = 0; //�Ƿ��ڹؼ��ֱ���

//ע�⣬���������������C++�б�����ȷд��const����ΪC++�����е���˫�������������ַ����� const char* ����
//��C��û������涨��ֻ�ᱻ��Ϊ����ͨ���ַ����飨��Ȼ������constҲ�У��Ҿ�ֱ�Ӱ������ϵ�д�ˣ�
//����C�е�const��ֻ������������ͨ��ָ���޸���ֵ������C++��const�����������ϵĳ�����û���޸ĵİ취
/*const */char* rwtab[] = { "and","array","begin","call","case","char","constant","do","else","end",
							"for","if","input","integer","not","of","or","output","procedure","program",
							"read","real","repeat","set","then","to","until","var","while","write",_KEY_WORD_END }; //�ؼ��ֱ�

inline void Press_Any_Key()
{
    printf("\nPress any key to continue...");
	getche();
	return;
}

void scanner(); //ɨ�躯��
void preprocessor(); //Ԥ���������������line��
char* getline(); //���ļ�����һ��

int main()
{	
    int over = 1;
    int scanfCorr; //���ڼ��scanf()�ķ���ֵ����Ϊscanf()�ǲ���ȫ�ģ�
    char filename[260], filedtup[260], filesymtab[260], fileerr[260], filechseq[260];
    
    //���ڷ��ű���ȥ�����ظ��ĵ��ʣ�����Ҫ����һ�������������Ա����
    ListNode* p, * pint;
    F = (ListNode*)malloc(sizeof(ListNode));
    R = (ListNode*)malloc(sizeof(ListNode));
    Fint = (ListNode*)malloc(sizeof(ListNode));
    Rint = (ListNode*)malloc(sizeof(ListNode));
    F->next = NULL;
    Fint->next = NULL;
    R = F;
    Rint = Fint;

    line = (char*)malloc(sizeof(char)*1024); //����C���޷�ʵ�ֶ�̬�������鳤�ȣ�����һ�мٶ�Ϊ������1024���ַ�
    lineRead = (char*)malloc(sizeof(char)*1024);

    printf("[Please enter your file name below: ]\n>>>");
    setbuf(stdin, NULL);
    scanfCorr = scanf("%[^\n]s", filename); 
    while ((fp = fopen(filename, "r+")) == NULL) //Visual Studio��������������C6054���������ùܣ�scanf()�������Զ����\0
    {
        printf("[Cannot find the file \"%s\". Please check your file name and try again. ]\n", filename);
        printf("[Please Enter your file name below: ]\n>>>");
        scanfCorr = scanf("%s", filename);
    }
	printf("[Your file has been read. ]\n\n[%s: ]\n",filename);

    sprintf(filedtup, "%s_Two-tuples.txt", filename); //��Ԫ���ļ�
	if ((fpdtup = fopen(filedtup, "w")) == NULL)
    {
        printf("Cannot create the file \"%s\". ", filedtup);
        Press_Any_Key();
        return -1;
    }

    sprintf(filechseq, "%s_Character_sequence.txt", filename); //�ַ������ļ�
    if ((fpchseq = fopen(filechseq, "w")) == NULL)
    {
        printf("Cannot create the file \"%s\". ", filedtup);
        Press_Any_Key();
        return -1;
    }

    sprintf(filesymtab, "%s_Symbol_table.txt", filename); //���ű��ļ�
    if ((fpsymtab = fopen(filesymtab, "w")) == NULL)
    {
        printf("Cannot create the file \"%s\". ", filedtup);
        Press_Any_Key();
        return -1;
    }
    fprintf(fpsymtab, "NAME\t\tTYPE\tVALUE\t\t\t\n");

    sprintf(fileerr, "%s_Errors.txt", filename); //������Ϣ�ļ�
    if ((fperr = fopen(fileerr, "w")) == NULL)
    {
        printf("[Cannot create the file \"%s\". ]", fileerr);
        Press_Any_Key();
        return -1;
	}

    while (!feof(fp)) //ִ�дʷ�����
    {
        getline();
        preprocessor();
        scanner();
	}

    R = NULL;
    while (F->next != NULL) //����ַ����в��ͷű����ַ�������
    {
        p = F;
        fprintf(fpchseq, "%s", F->next->data);
        F = F->next;
        free(p);
    }
    free(F);

    Rint = NULL;
    while (Fint->next != NULL) //�ͷű�������������
    {
        pint = Fint;
        Fint = Fint->next;
        free(pint);
    }
    free(Fint);

	printf("\n[Lexical analysis completed. ]\n");
	printf("\n[Two-touples will be saved in a file named \"%s\". ]\n", filedtup);
	printf("[Symbol table will be saved in a file named \"%s\". ]\n", filesymtab);
	printf("[Character sequence will be saved in a file named \"%s\". ]\n", filechseq);
	printf("[Errors will be saved in a file named \"%s\". ]\n", fileerr);
	
    fclose(fp);
    fclose(fpdtup);
    fclose(fperr);
    fclose(fpchseq);
    fclose(fpsymtab);
    free(line);
    free(lineRead);
    Press_Any_Key();
    return 0;
}

//���ļ�����һ��
char* getline()
{
    linenum++; //�˺���ÿ����һ���кż�һ
    int len;
    if (!feof(fp))
    {
        fgets(lineRead, 1024, fp);
        len = strlen(lineRead);
        if (lineRead[len - 1] != '\n')
        {
            lineRead[len + 1] = lineRead[len];
            lineRead[len] = '\n';
        }
    }
    else
        lineRead = "";
    return lineRead;
}

//��Ԥ����������ж�ȡһ���ַ���ch��
char m_getch()
{
    if (p_line < strlen(line))
    {
        ch = line[p_line];
        p_line++;
        return ch;
    }
    else
        ch = '\0';
    return ch;
}

//ȥ���հ׷���
void getbc()
{
	while (ch == ' ' && p_line < strlen(line))
	{
		ch = line[p_line];
		p_line++;
	}
	return;
}

//ƴ�ӵ���
void concat()
{
	token[p_token] = ch;
	p_token++;
	token[p_token] = '\0';
	return;
}

//�ж��Ƿ�����ĸ
int letter()
{
	if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
		return 1;
	else
		return 0;
}

//�ж��Ƿ�������
int digit()
{
	if (ch >= '0' && ch <= '9')
		return 1;
	else
		return 0;
}

//�����ؼ��ֱ��
int reserve()
{
	int i = 0;
	while (strcmp(rwtab[i], _KEY_WORD_END))
	{
		if (!strcmp(rwtab[i], token))
		{
			isRwtab = 1;
			return i + 1;
		}
		i++;
	}
	isRwtab = 0;
	return 34;
}

//����һ���ַ�
void retract()
{
	p_line--;
	return;
}

//Ԥ���������������line��
void preprocessor()
{
	printf("%d	%s",linenum, lineRead);
	int i = 0;
	int len = 0;;
	int is_brar = 0;
	int pbra = 0;
	LinkStack* S = InitStack(); //ƥ��ջ���������ź�����
	LinkStack** p = &S;
	pe_line = p_line = 0;
	strcpy(line, lineRead);

	for (p_line = 0; p_line < strlen(line); p_line++) //���Ƿ��ַ������š�����ƥ��
	{
		len = strlen(line);
		if ((line[p_line] >= '0' && line[p_line] <= '9') && ((line[p_line + 1] >= 'a' && line[p_line] + 1 <= 'z') || (line[p_line + 1] >= 'A' && line[p_line + 1] <= 'Z')))
		{
			fprintf(fperr, "Line %d, column %d: an incorrect identifier. \n", linenum, p_line);
			for (int j = len; j > p_line + 1; j--)
				line[j] = line[j - 1];
			line[len + 1] = '\0';
			line[p_line + 1] = ' ';
			p_line++;
		}
		else
			switch (line[p_line])
			{
			case '/':
			{
				if (line[p_line + 1] == '*')
				{
					for (i = p_line; i < strlen(line) - 1; i++)
					{
						if (line[i] == '*' && line[i + 1] == '/')
						{
							line[i] = ' ';
							line[i + 1] = ' ';
							p_line = i + 1;
							break;
						}
						line[i] = ' ';
					}
					if (i == strlen(line) - 1)
					{
						fprintf(fperr, "Line %d, column %d: commenting delimiters are not matched. \n", linenum, p_line + 1);
						p_line = i - 1; //ָ��ָ��ĩβ�س���֮ǰ
					}
				}
				break;
			}
			case '*':
			{
				if (line[p_line + 1] == '/')
					fprintf(fperr, "Line %d, column %d: commenting delimiters are not matched. \n", linenum, p_line);
				p_line++;
				break;
			}
			case '#':
			{
				fprintf(fperr, "Line %d, column %d: an invalid symbol. \n", linenum, p_line + 1);
				line[p_line] = ' ';
				break;
			}
			case '!':
			{
				fprintf(fperr, "Line %d, column %d: an invalid symbol. \n", linenum, p_line + 1);
				line[p_line] = ' ';
				break;
			}
			case '$':
			{
				fprintf(fperr, "Line %d, column %d: an invalid symbol. \n", linenum, p_line + 1);
				line[p_line] = ' ';
				break;
			}
            case '%':
            {
                fprintf(fperr, "Line %d, column %d: an invalid symbol. \n", linenum, p_line + 1);
                line[p_line] = ' ';
                break;
            }
            case '^':
            {
                fprintf(fperr, "Line %d, column %d: an invalid symbol. \n", linenum, p_line + 1);
                line[p_line] = ' ';
                break;
            }
            case '&':
            {
                fprintf(fperr, "Line %d, column %d: an invalid symbol. \n", linenum, p_line + 1);
                line[p_line] = ' ';
                break;
            }
            case '~':
            {
                fprintf(fperr, "Line %d, column %d: an invalid symbol. \n", linenum, p_line + 1);
                line[p_line] = ' ';
                break;
            }
            case '?':
            {
                fprintf(fperr, "Line %d, column %d: an invalid symbol. \n", linenum, p_line + 1);
                line[p_line] = ' ';
                break;
            }
            case '\\':
            {
                fprintf(fperr, "Line %d, column %d: an invalid symbol. \n", linenum, p_line + 1);
                line[p_line] = ' ';
                break;
            }
            case '|':
            {
                fprintf(fperr, "Line %d, column %d: an invalid symbol. \n", linenum, p_line + 1);
                line[p_line] = ' ';
                break;
            }
			case '\t':
			{
				line[p_line] = ' ';
				break;
			}
			case '\n':
			{
				line[p_line] = ' ';
				break;
			}
			case '"':
			{
				for (pe_line = p_line + 1; pe_line < strlen(line) - 1; pe_line++)
				{
					if (line[pe_line] == '"')
					{
						p_line = pe_line;
						break;
					}
				}
				if (pe_line == strlen(line) - 1)
				{
					fprintf(fperr, "Line %d, column %d: an incorrect string constant. Quotation marks are not mached. \n", linenum, p_line + 1);
					line[pe_line] = '"';
					line[pe_line + 1] = '\0';
					p_line = pe_line + 1;
				}
				break;
			}
			case '(':
			{
				Push(p, '(');
				is_brar++; //���ֹ�������
				pbra = p_line;
				break;
			}
			case '[':
			{
				Push(p, '[');
				is_brar++;
				pbra = p_line;
				break;
			}
			case ')':
			{
				if (GetTop(S) == '(')
					Pop(p);
				else
					fprintf(fperr, "Line %d, column %d: brackets are not matched. \n", linenum, p_line + 1);
				break;
			}
			case ']':
			{
				if (GetTop(S) == '[')
					Pop(p);
				else
					fprintf(fperr, "Line %d, column %d: brackets are not matched. \n", linenum, p_line + 1);
				break;
			}
			default:
				break;
			}
	}
	if (!StackEmpty(S) && (GetTop(S) == '[' || GetTop(S) == '(') && is_brar == 0) //��û���ֹ�������
		fprintf(fperr, "Line %d, column %d: brackets are not matched. \n", linenum, pbra);
	//printf("%s\n", line);
	strcpy(lineRead, "");
    Destroy(p);
}

//ɨ�躯��
void scanner()
{
    ListNode* p, * pint;
    WORD* myword = (WORD*)malloc(sizeof(WORD));
	if (!myword)
		return NULL;
	p_line = 0;
	do
	{
        int inlist = 0, inlistint = 0;
        myword->typenum = 10;
		myword->word = "";
		strcpy(token, "");
		p_token = 0;
		m_getch();
		getbc(); //��������˿հ׾ͽ������¶�
		if (letter())
		{
			while (letter() || digit())
			{
				concat();
				m_getch();
			}
			retract();
			myword->typenum = reserve();
			myword->word = token;
			if (isRwtab)
				fprintf(fpdtup, "(%d, -)", myword->typenum);
			else
			{
				fprintf(fpdtup, "(%d, %d)", myword->typenum, linenum);
                if (F->next != NULL) //����
                {
                    p = F->next;
                    while (p != NULL)
                    {
                        if (!strcmp(p->data, myword->word))
                        {
                            inlist = 1;
                            break;
                        }
                        p = p->next;
                    }
                    if (!inlist)
                    {
                        R->next = (ListNode*)malloc(sizeof(ListNode));
                        strcpy(R->next->data, myword->word);
                        R->next->next = NULL;
                        R = R->next;
                        fprintf(fpsymtab, "%d\t%d\t\t\t\t\t\n", p_symtab, strlen(myword->word));
                        p_symtab += strlen(myword->word);
                    }
                }
                else
                {
                    R->next = (ListNode*)malloc(sizeof(ListNode));
                    strcpy(R->next->data, myword->word);
                    R->next->next = NULL;
                    R = R->next;
                    fprintf(fpsymtab, "%d\t%d\t\t\t\t\t\n", p_symtab, strlen(myword->word));
                    p_symtab += strlen(myword->word);
                }
			}
			//return myword;
		}
		else if (digit())
		{
			while (digit())
			{
				concat();
				m_getch();
			}
			retract();
			myword->typenum = 36;
			myword->word = token;
			fprintf(fpdtup, "(%d, %d)", myword->typenum, linenum);
            //fprintf(fpsymtab, "\t\tN\t%s\t\t\t\n", myword->word);
            if (Fint->next != NULL)
            {
                pint = Fint->next;
                while (pint != NULL) ////����
                {
                    if (!strcmp(pint->data, myword->word))
                    {
                        inlistint = 1;
                        break;
                    }
                    pint = pint->next;
                }
                if (!inlistint)
                {
                    Rint->next = (ListNode*)malloc(sizeof(ListNode));
                    strcpy(Rint->next->data, myword->word);
                    Rint->next->next = NULL;
                    Rint = Rint->next;
                    fprintf(fpsymtab, "\t\tN\t%s\t\t\t\n", myword->word);
                }
            }
            else
            {
                Rint->next = (ListNode*)malloc(sizeof(ListNode));
                strcpy(Rint->next->data, myword->word);
                Rint->next->next = NULL;
                Rint = Rint->next;
                fprintf(fpsymtab, "%d\t%d\t\t \t\t\t\n", p_symtab, strlen(myword->word));
                p_symtab += strlen(myword->word);
            }
			//return myword;
		}
		else
			switch (ch)
			{
			case '=':
			{
				myword->typenum = 43;
				myword->word = "-";
				fprintf(fpdtup, "(%d, %s)", myword->typenum, myword->word);
				//return myword;
				break;
			}
			case '+':
			{
				myword->typenum = 39;
				myword->word = "-";
				fprintf(fpdtup, "(%d, %s)", myword->typenum, myword->word);
				//return myword;
				break;
			}
			case '-':
			{
				myword->typenum = 40;
				myword->word = "-";
				fprintf(fpdtup, "(%d, %s)", myword->typenum, myword->word);
				//return myword; 
				break;
			}
			case '*':
			{
				myword->typenum = 41;
				myword->word = "-";
				fprintf(fpdtup, "(%d, %s)", myword->typenum, myword->word);
				//return myword;
				break;
			}
			case '/':
			{
				myword->typenum = 42;
				myword->word = "-";
				fprintf(fpdtup, "(%d, %s)", myword->typenum, myword->word);
				//return myword;
				break;
			}
			case '(':
			{
				myword->typenum = 46;
				myword->word = "-";
				fprintf(fpdtup, "(%d, %s)", myword->typenum, myword->word);
				//return myword;
				break;
			}
			case ')':
			{
				myword->typenum = 47;
				myword->word = "-";
				fprintf(fpdtup, "(%d, %s)", myword->typenum, myword->word);
				//return myword;
				break;
			}
			case '[':
			{
				myword->typenum = 48;
				myword->word = "-";
				fprintf(fpdtup, "(%d, %s)", myword->typenum, myword->word);
				//return myword;
				break;
			}
			case ']':
			{
				myword->typenum = 49;
				myword->word = "-";
				fprintf(fpdtup, "(%d, %s)", myword->typenum, myword->word);
				//return myword;
				break;
			}
			case '.':
			{
				m_getch();
				if (ch == '.')
				{
					myword->typenum = 58;
					myword->word = "-";
					fprintf(fpdtup, "(%d, %s)", myword->typenum, myword->word);
					break;
					//return myword;
				}
				retract();
				myword->typenum = 51;
				myword->word = "-";
				fprintf(fpdtup, "(%d, %s)", myword->typenum, myword->word);
				//return myword;
				break;
			}
			case ',':
			{
				myword->typenum = 53;
				myword->word = "-";
				fprintf(fpdtup, "(%d, %s)", myword->typenum, myword->word);
				//return myword;
				break;
			}
			case ':':
			{
				m_getch();
				if (ch == '=')
				{
					myword->typenum = 57;
					myword->word = "-";
					fprintf(fpdtup, "(%d, %s)", myword->typenum, myword->word);
					//return myword;
					break;
				}
				retract();
				myword->typenum = 50;
				myword->word = "-";
				fprintf(fpdtup, "(%d, %s)", myword->typenum, myword->word);
				//return myword;
				break;
			}
			case ';':
			{
				myword->typenum = 52;
				myword->word = "-";
				fprintf(fpdtup, "(%d, %s)", myword->typenum, myword->word);
				//return myword;
				break;
			}
			case '>':
			{
				m_getch();
				if (ch == '=')
				{
					myword->typenum = 56;
					myword->word = "-";
					fprintf(fpdtup, "(%d, %s)", myword->typenum, myword->word);
					//return myword;
					break;
				}
				retract();
				myword->typenum = 45;
				myword->word = "-";
				fprintf(fpdtup, "(%d, %s)", myword->typenum, myword->word);
				//return myword;
				break;
			}
			case '<':
			{
				m_getch();
				if (ch == '=')
				{
					myword->typenum = 55;
					myword->word = "-";
					fprintf(fpdtup, "(%d, %s)", myword->typenum, myword->word);
					//return myword;
					break;
				}
				else if (ch == '>')
				{
					myword->typenum = 54;
					myword->word = "-";
					fprintf(fpdtup, "(%d, %s)", myword->typenum, myword->word);
					//return myword;
					break;
				}
				retract();
				myword->typenum = 44;
				myword->word = "-";
				fprintf(fpdtup, "(%d, %s)", myword->typenum, myword->word);
				//return myword;
				break;
			}
			case -1:
			{
				myword->typenum = 1000;
				myword->word = "OVER";
				fprintf(fpdtup, "(%d, %s)", myword->typenum, myword->word);
				//return myword;
				break;
			}
			case '"':
			{
				m_getch();
				while (line[p_line] != '"')
				{
					concat();
					m_getch();
				}
				concat();
				p_line++;
				//printf("%s\n", token);
				myword->typenum = 38;
				myword->word = token;
				fprintf(fpdtup, "(%d, %d)", myword->typenum, linenum);
				//fprintf(fpchseq, "%s", myword->word);
                if (F->next != NULL)
                {
                    p = F->next;
                    while (p != NULL)
                    {
                        if (!strcmp(p->data, myword->word))
                        {
                            inlist = 1;
                            break;
                        }
                        p = p->next;
                    }
                    if (!inlist)
                    {
                        R->next = (ListNode*)malloc(sizeof(ListNode));
                        strcpy(R->next->data, myword->word);
                        R->next->next = NULL;
                        R = R->next;
                        fprintf(fpsymtab, "%d\t%d\tC\t\t\t\t\n", p_symtab, strlen(myword->word));
                        p_symtab += strlen(myword->word);
                    }
                }
                else
                {
                    R->next = (ListNode*)malloc(sizeof(ListNode));
                    strcpy(R->next->data, myword->word);
                    R->next->next = NULL;
                    R = R->next;
                    fprintf(fpsymtab, "%d\t%d\tC\t\t\t\t\n", p_symtab, strlen(myword->word));
                }
				break;
			}
			case ' ':
				break;
			default:
			{
				myword->typenum = -1;
				myword->word = "ERROR";
				fprintf(fpdtup, "(%d, %s)", myword->typenum, myword->word);
				//return myword;
				break;
			}
			}
		//printf("%d\n", p_line);
	}
	while (line[p_line] != '\0');
	free(myword);
	strcpy(line, "");
}
