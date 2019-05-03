# Lexical_Analyzer
A simple lexical analyzer using DFA(a part of a compiler).  
The acceptable language looks like Pascal(XD  
Maybe the whole compiler could be accomplished(but it can't be in a high probability  
Here are the details.  
1. 待分析的简单语言的定义  
1）字符集  
<字符集>→<字母> | <数字> | <符号>  
<字母>→A | B | … | Z | a | b | … | z  
<数字>→0 | 1 | 2 | … | 9  
<符号>→+ | - | * | / | = | < | > | ( | ) | [ | ] | : | . | ; | , | “  
2）单词集  
<单词集>→<保留字> | <标识符> | <常数> | <单界符> | <双界符>  
<保留字>→and | array | begin | call | case | char | constant | do | else | end | for | if | input | integer |  
not | of | or | output | procedure | program | read | real | repeat | set | then | to | until |  
var | while | write  
<标识符>→<字母> | <标识符><字母> | <标识符><数字>  
<常数>→<整数> | <字符常数>  
<整数>→<数字> | <整数><数字>  
<字符常数>→用双引号引起来的任意字符串  
<单界符>→+ | - | * | / | = | < | > | ( | ) | [ | ] | : | . | ; | , | “  
<双界符>→<> | <= | >= | := | .. | /* | */  
  
2. 各种单词符号对应的种别码  
保留字：一字一种，其种别码依字典顺序取 1-30；  
标识符：种别码为 34，值为该符号串在符号表中的位置（行号）；  
整数：种别码为 36，值为该常数在符号表中的位置（行号）；  
字符常数：种别码为 38，值为该常数在符号表中的位置（行号）；  
单界符：一符一种，其种别码依次取 39-53（“无种别码）；  
双界符：一符一种，其种别码依次取 54-60。  
  
3. 词法分析程序的功能  
输入：源程序文件，每行最多1024个字符。  
输出：  
1）二元式文件；  
2）符号表文件；  
3）出错信息文件。  
要求：词法分析程序做一遍扫描。  
主要任务：  
扫描源程序；  
识别单词；  
转换成二元式形式；  
填写或查询符号表；  
错误处理。  
  
4. 程序中用到的自编函数说明：  
char* getline()：从文件读入一行；  
void preprocessor()：预处理函数；  
void scanner()：扫描函数；  
char m_getch()：从与处理完的结果中读取一个字符放在ch中，同时指针（p_line）后移；  
void getba()：去掉空白符号；  
void retract()：指针回退一个字符；  
void concat()：拼接字符；  
int reserve()：搜索关键字表格，并自动修改是否在关键字表格里的标志。  
int letter()：判断ch是否是字母；  
int digit()：判断ch是否是数字；  
LinkStack* InitStack()：初始化栈；  
void Push(LinkStack**, SElemType)：入栈；  
SElemType Pop(LinkStack**)：出栈  
Status StackEmpty(LinkStack*)：判空栈；  
SElemType GetTop(LinkStack*)：获取栈顶元素；  
void Destroy(LinkStack**)：清空并销毁栈。  
  
5. 中三个重要函数的具体算法说明  
a) char* getline()：  
	此函数较短，所以这里直接附上代码：  
```C
char* getline()
{
    linenum++; //此函数每调用一次行号加一
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
```   
  其中lineRead是读入行缓冲区，大小为1024字符；linenum为行号，初始化为0；fp为输入文件的文件指针。以上三个数组或变量都是全局的。在第3行中，每调用一次此函数则行号将被加一。这个linenum变量将用于二元组中对标识符、整数和字符常数所在的行号进行输出（见2. 各种单词符号对应的种别码。关于读入一行的方法，因为C语言中没有可以直接将文件按行读入的库函数，所以我使用了fgets函数。将fgets函数的读入长度设为读入行缓冲区的长度，利用fgets函数遇到换行符自动返回的特点，实现将文件的一整行（包括换行符）读入。为了后面处理的方便，如果一行末尾不是换行符（一般最后一行）那么在读入之后在结束符“\0”之前加入换行符。  
  
b) void preprocessor()：  
	设扫描缓行冲区为line，读入行缓冲区为lineRead，扫描行的两个指示器分别为p_line和pe_line。以上四个变量均为全局变量。下面给出预处理函数的算法描述（假设匹配栈已建立并且各个变量初始化已完成）：  
1.	将lineRead复制到line中。  
2.	比较p_line和line的长度。如果p_line<strlen(line)则从步骤3开始继续执行，否则从5步骤开始继续执行。  
3.	对于line[p_line]，如果line[p_line]是数字并且line[p_line+1]是字母，则在line[p_line]和line[p_line+1]之间插入空格，并输出不正确的标识符的错误信息，将p_line加二并执行步骤2；否则执行步骤4。  
4.	对于line[p_line]，做如下操作（有穷状态机）：  
1）	如果line[p_line]是字符“/”并且line[p_line+1]是字符“*”，则向后寻找字符组合“*/”，并删除包括line[p_line]、line[p_line+1]和“*/”之间的所有字符。并将p_line至于“*/”中的“/”处。如果找不到字符组合“*/”则一直删除到行尾，输出注释符不匹配的错误信息并将p_line置于行尾。  
2）	如果line[p_line]是字符“*”并且line[p_line+1]是字符“/”，则输出注释符不匹配的错误信息并将p_line加一。  
3）	如果line[p_line]是不在字符集中，则输出非法字符的错误信息。  
4）	如果line[p_line]是换行符或制表符，则删除。  
5）	如果line[p_line]是字符“””，则向后寻找字符 “””，并将p_line置于后面的“””处；如果向后找不到“””，则在行尾添加“””，输出引号不匹配的错误信息并将p_line置于行尾。  
6）	如果line[p_line]是字符“(”或“[”，则将line[p_line]压栈，并且标记已经出现过左括号。  
7）	如果line[p_line]是字符“)”，则获取栈顶元素。如果栈顶数据是“(”，则弹栈；否则输出括号不匹配的错误信息。  
8）	如果line[p_line]是字符“]”，则获取栈顶元素。如果栈顶数据是“[”，则弹栈；否则输出括号不匹配的错误信息。  
最后将p_line加一并执行步骤2。  
5.	则检查匹配栈。如果栈不空并且栈顶为“[”或“(”并且左括号没有出现过，则输出括号不匹配的错误信息。否则什么都不做。  
6.	将line置空并销毁匹配栈，算法结束。  
上述算法完成后line内的内容就是预处理的结果。  
  
c) void scanner()：  
	设扫描行缓冲区为line，单词缓冲区为token[1024]，其指示器为p_token，二元组的结构体实例化为结构体变量指针myword，单链表结点结构体实例化为结构体指针p和pint。其中line，token和p_token为全局的。下面给出扫描函数的算法描述（假设各个变量初始化已完成）：  
1.	清空token并令p_token为0。执行m_getch函数和getbe函数。  
2.	对ch做如下操作（有穷状态机）：  
1）	如果ch是字母，则向后搜索，直到遇到既不是字母也不是数字的的字符为止。将搜索到的字符拼接起来放到token中（调用concat函数即可），同时将token赋值给myword->word。之后调用reserve函数搜索关键字表格，并将结果赋给myword->typemun。如果token在关键字表里，则输出二元组(myword->typemun, -)；否则输出二元组(myword->typemun, linenum)并填写符号表。  
2）	如果ch是数字，则向后搜索，直到遇到不是数字的的字符为止。将搜索到的字符拼接起来放到token中（调用concat函数即可），同时将token赋值给myword->word。将myword->typemun设为36。输出二元组(myword->typemun, linenum)并填写符号表。  
3）	如果ch是单界符“+”、“-”、“*”、“/”、“=”、“(”、“)”、“[”、“]”、“,”或“;”，则将myword->typemun设为对应的种别码，myword->word设为“-”，并输出二元组(myword->typemun, myword->word)。  
4）	如果ch是单界符“>”、“<”、“:”或“.”，则向后搜索一个字符。如果ch可以与后面一个字符组成双界符，则将myword->typemun设为对应的双界符的种别码；否则将myword->typemun设为该单界符的种别码。不论能不能组成双界符，统一设myword->word设为“-”。输出二元组(myword->typemun, myword->word)。  
5）	如果ch是字符“””，则向后搜索，直到再次遇到字符“””为止。将搜索到的字符拼接起来放到token中（调用concat函数即可），同时将token赋值给myword->word。将myword->typemun设为38。输出二元组(myword->typemun, linenum)并填写符号表。  
6）	如果ch是空格，则什么也不做。  
3.	判定line[p_line]的值。如果line[p_line]是字符串结尾“\0”，则执行步骤4，否则执行步骤2。  
4.	将line置空，算法结束。  
