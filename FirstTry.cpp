#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<iostream>
#ifndef TRUE
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define Status int
#define Elemtype char
#define EMPTY 1
#define NOTEMPTY 0
//#define OVERFLOW -1
#define MAX_int 0x7fffffff
#define N 128
#define stack_size 100
#endif
using namespace std;

//Author：Shan Zhihan 2020211453
//Encoding：GBK(chinese simplified)
//IDE:vs2019

typedef struct
{
	unsigned weight;//权值
	unsigned parent;//父节点
	unsigned lchild, rchild;//左右孩子
}huffTree_node,*huffpt;//哈夫曼树节点，用堆存储

typedef struct
{
	unsigned symbol;//对应的char值，也是哈夫曼数组的下标
	unsigned weight;//权值
}sort_node;//用于快排的数组
typedef struct n
{
	Elemtype* top;
	Elemtype* base;
	int stacksize;
}SqStack;//栈的结构体定义

typedef struct node
{
	Elemtype data;
	struct node* next;
}Qnode, * QP;//队列节点

typedef struct
{
	QP rear;
	QP front;
	int queue_size;
}LinkQueue;//队列结构体


typedef struct
{
	unsigned char symbol;//对应的char值，也是哈夫曼数组的下标
	unsigned char parent;//下标一定在128~255之间
	unsigned char lchild, rchild;//左右孩子，下标一定在0~127之间，char 够用了。
}other_node;//存到文件中的其他节点的存储结构

Status read_in(huffpt HT, sort_node sort[], FILE* in_fp);
void create_huffman(huffpt& HT, FILE* in_fp);
int cmp(const void* a, const void* b);
Status encode(huffpt HT, FILE* in_fp, FILE* &out_fp);
void find_father(huffpt HT, FILE* &out_fp, SqStack& s, LinkQueue& Q, unsigned char c);
Status write_carrier(FILE* &out_fp, LinkQueue& Q); 
Status save_huffman(FILE* &out_fp, huffpt HT);
//下面是队列的操作函数
Status init_queue(LinkQueue* Q);//初始化队列
Status enqueue(LinkQueue& Q, Elemtype x);//入队操作
Status dlqueue(LinkQueue& Q);//删除队首元素
Status empty_queue(LinkQueue* Q);//队列判空
Status gethead_queue(LinkQueue Q, Elemtype* e);//取队首元素
//下面是栈的操作函数
Status StackEmpty(SqStack s);
Status init_stack(SqStack& s);
Status push(SqStack& s, Elemtype e);
Status pop(SqStack& s, Elemtype& e);
int main(void)
{
	char* inputfile = (char*)calloc(1, 51);//文件名，不超过50个字符
								           //calloc申请到初始化全部为0的内存
	cout << "Please enter the input file name,and make sure it is no longer than 50!\n" << endl;
	fgets(inputfile, 50, stdin);//读入不超过50个字符，防止缓冲区溢出
	inputfile[strlen(inputfile) - 1] = 0;//覆盖\n
	FILE* in_fp = fopen(inputfile, "r");

	char* outputfile = (char*)calloc(1, 51);//文件名，不超过50个字符
										   //calloc申请到初始化全部为0的内存
	cout << "Please enter the output file name,and make sure it is no longer than 50!\n" << endl;
	fgets(outputfile, 50, stdin);//读入不超过50个字符，防止缓冲区溢出
	outputfile[strlen(outputfile) - 1] = 0;//覆盖\n
	FILE* out_fp = fopen(outputfile, "wb");//以二进制写的方式打开输出文件

	huffpt HT = NULL;//哈夫曼树数组指针
	create_huffman(HT,in_fp);//建立哈夫曼树
	save_huffman(out_fp, HT);//存储哈夫曼树
	encode(HT,in_fp,out_fp);//编码函数
	fclose(in_fp);
	fclose(out_fp);
	return 0;
}

/**
* 遍历一遍文件，获得权值
*/
Status read_in(huffpt HT, sort_node sort[],FILE*in_fp)
{
	
	if (in_fp==NULL)//打开文件失败
	{
		cout << "Can't open the file properly!\n" << endl;
		exit(-1);
	}
	unsigned char c = fgetc(in_fp);
	int i = 1;
	while (c!=(unsigned char)EOF)
	{
		(HT[c].weight)++;//对应下标权值++
		sort[c].symbol = c;
		sort[c].weight++;
		c = fgetc(in_fp);//读入字符
		i++;
	}
	(HT[0].weight)++;//EOF是255，记为0
	sort[0].symbol = 0;
	sort[0].weight++;//EOF的weight应该是1
	//::fclose(in_fp);
	return OK;
}
void create_huffman(huffpt & HT,FILE* in_fp)
{
	HT = (huffpt)calloc(sizeof(huffTree_node), (2 * N +5));//堆中存储哈夫曼树,calloc全部初始化为0
	sort_node* sort = (sort_node*)calloc(sizeof(sort_node), (2 * N + 5));
	if (!HT || !sort)
	{
		cout << "Can't get necessary memory for HT or sort!\n" << endl;
		exit(-1);
	}
	int i = 0;
	read_in(HT,sort,in_fp);//从文件读入sort和HT中
	for (i = N; i < 2 * N; i++)
	{
		//1.qsort版
		qsort(sort, i, sizeof(sort_node), cmp);
		/*for (int j = 0; j < N; j++)
		{
			printf("%d:   %d\n", sort[j].symbol,sort[j].weight);
		}*/
		if (sort[1].weight == 0)//临界条件，由于cmp默认0是最大，故现在已经全部排完了
			break;
		HT[sort[0].symbol].parent = i;//记录左子的父节点
		HT[sort[1].symbol].parent = i;//记录右子的父节点
		HT[i].weight = sort[0].weight + sort[1].weight;//更新weight
		HT[i].lchild = sort[0].symbol;
		HT[i].rchild = sort[1].symbol;//记录子节点
		sort[0].weight = 0;
		sort[1].weight = 0;//删除已经是子节点的节点的权值
		sort[i].weight = HT[i].weight;//插入新的节点继续构造哈夫曼
		sort[i].symbol = i;//记录新节点的对应下标
		//2.遍历版
		/*int minp = HT[].weight;
		int minq;
		if(HT[0].weight>HT[1].weight)
		for (int j = 0; j < N; j++)
		{
			if (HT[j].parent)
				continue;
			if (HT[j].weight < minp)
			{
				minp = HT[j].weight;
			}
		}*/
	}
	if (i == 2 * N+1)
	{
		printf("The Qsort didn't do well,sth Wrong!\n");
		exit(-1);
	}
	return;
}

/**
* Qsort的比较函数
*/
int cmp(const void* a, const void* b)
{
	sort_node _a= *(sort_node*)a;
	sort_node _b= *(sort_node*)b;
	if (_a.weight == 0)//权值为0的节点不参与编码，默认0为最大值
		_a.weight = MAX_int;
	if (_b.weight == 0)
		_b.weight = MAX_int;
	return _a.weight - _b.weight;//除0以外升序排列
}

/**
* 把哈夫曼树中权值不为0的节点按照ASCII模式存入
*/
Status save_huffman(FILE* &out_fp,huffpt HT)
{
	
	unsigned char i = 0; unsigned char father = 0;
	unsigned char lchild=0, rchild=0;
	unsigned char leaves = 0, others = 0;//记录叶子和其他节点是数目
	for (i=0; i < 255; i++)//leaves
	{
		if (HT[i].weight)
		{
			if(i < 128)
			leaves++;//统计叶子的数目
			else
			others++;//统计其他节点的数目
		}
	}
	fwrite(&leaves, 1, 1, out_fp);//在开头第一个字节写入树的叶子节点数目
	fwrite(&others, 1, 1, out_fp);//在开头第二个字节写入树的其他节点数目
	for (i = 0; i < N; i++)
	{
		if (HT[i].weight)
		{
			fwrite(&i, 1, 1, out_fp);//写symbol
			father = (unsigned char)HT[i].parent;
			fwrite(&father, 1, 1, out_fp);//写parent
		}
	}
	for (i = N; i <255; i++)
	{
		if (HT[i].weight)
		{
			fwrite(&i, 1, 1, out_fp);//写symbol
			father = (unsigned char)HT[i].parent;
			fwrite(&father, 1, 1, out_fp);//写parent
			lchild = (unsigned char)HT[i].lchild;
			rchild = (unsigned char)HT[i].rchild;
			fwrite(&lchild, 1, 1, out_fp);//写lchild
			fwrite(&rchild, 1, 1, out_fp);//写rchild
		}
	}
	char a = EOF;
	fwrite(&a, 1, 1, out_fp);//写入eof
	return 1;
}

/**
*编码函数，用于编码并且写入二进制文件\
* 
* 
* 
* 
* 写的不对GG
* 
* 目前思路：
* 用栈完成一个字符的编码
* 然后编完一个字符之后入队
* 然后从队头以8个一单位
* 按位异或
* 
*/
Status encode(huffpt HT, FILE* in_fp, FILE* &out_fp)
{
	fseek(in_fp, 0, SEEK_SET);
	unsigned char c = fgetc(in_fp);//读入
	int count = 0;
	SqStack s;

	Elemtype* stack = (Elemtype*)malloc(stack_size * sizeof(Elemtype));//栈的空间申请
	s.base = stack;
	s.top = stack;
	s.stacksize = stack_size;//初始化
	if (!stack) {//空间分配失败
		printf("ERROR when allocating memory for the stack!\n");
		exit(-1);
	}
	LinkQueue Q;//队列结构体
	init_queue(&Q);//队列初始化
	while (c != (unsigned char)EOF)//对EOF之外的字符编码
	{
		find_father(HT, out_fp, s, Q, c);//count是引用，trans是指针，HT是传值，c也是
		while (Q.queue_size >= 8) {//队列长度大于8
			write_carrier(out_fp, Q);
		}//8位转成一个unsigned char写入
		c = fgetc(in_fp);//继续下一个元素的编码
	}
	//给EOF单独写一个码，执行到这里的时候c=eof 255
	find_father(HT, out_fp, s, Q, 0);
	while (Q.queue_size >= 8) {//队列长度大于8
		write_carrier(out_fp, Q);
	}//8位转成一个unsigned char写入
	if (Q.queue_size)//还有码，补零写入
	{
		for (; Q.queue_size <=8; (Q.queue_size)++) {
			enqueue(Q, 0);
		}
		write_carrier(out_fp, Q);
	}
	//结束编码

	//fclose(in_fp);
	//fclose(out_fp);
	return OK;
}
void find_father(huffpt HT, FILE* &out_fp, SqStack& s, LinkQueue& Q, unsigned char c)
{
	unsigned parent = HT[c].parent;//“爸爸”
	int code_size = 0;
	while (parent)
	{

		if (HT[parent].lchild == c)//左子，0
			push(s, 0);
		else
			push(s, 1);//右子，1

		c = parent;
		parent = HT[parent].parent;//继续找爸爸
		code_size++;//计数下标++
	}//对某一个字符编码
	char e = 0;
	while(!StackEmpty(s))//栈不空时
	{
		pop(s, e);//弹栈
		enqueue(Q, e);//入队
		(Q.queue_size)++;//队列长度++
	}
}
Status write_carrier(FILE* &out_fp, LinkQueue& Q)
{
	unsigned char carrier = 0;//用于凑够八个字符运到输出文件中去
	int count = 0;
	char e = 0;
	// if(Q.queue_size<8)
	// return ERROR;//不能凑够出队
	for (; count < 8; count++)
	{
		gethead_queue(Q, &e);
		carrier = (carrier << 1) ^ e;//位运算凑一个unsigned char
		dlqueue(Q);
		(Q.queue_size)--;
	}
	fwrite(&carrier, 1, 1, out_fp);
	return OK;
}

/**
栈的函数
*/
//栈的判空
Status StackEmpty(SqStack s)
{
	if (s.base == s.top)
		return EMPTY;
	else return NOTEMPTY;
}
//入栈
Status push(SqStack& s, Elemtype e)
{
	if (s.top - s.base >= s.stacksize)
		return OVERFLOW;
	else
	{
		*s.top++ = e;
		return OK;
	}
}

//出栈
Status pop(SqStack& s, Elemtype& e)//这里是用e把删除的元素返回去
{
	if (s.top == s.base)//满栈，error
		return ERROR;
	else
	{
		e = *--s.top;//这里top指向顶上一个空白
		return OK;
	}
}


/**
下面是队列的函数
*/
//创建空队列
Status init_queue(LinkQueue* Q)
{
	Q->front = NULL;
	Q->rear = NULL;
	Q->queue_size = 0;
	return OK;
}
//判队空否
Status empty_queue(LinkQueue* Q)
{
	if (Q->front == NULL)
		return OK;
	else
		return ERROR;
}

//元素入队函数en_queue(q,x)
Status enqueue(LinkQueue& Q, Elemtype x)
{
	QP s = (QP)malloc(sizeof(Qnode));
	if (!s)
		return ERROR;
	s->data = x;
	s->next = NULL;
	if (Q.front == NULL)
	{
		Q.front = s;
		Q.rear = s;
	}
	Q.rear->next = s;
	Q.rear = s;
	return OK;
}

//元素出队函数dl_queue(q)
Status dlqueue(LinkQueue& Q)
{
	if (Q.front == NULL)
		return ERROR;
	QP p = Q.front;
	Q.front = Q.front->next;

	if (Q.front == NULL)
		Q.rear = NULL;//这里非常非常重要，这在维护两个指针的过程中很容易被忽略，造成野指针，指向已经被释放的空间。
	free(p);
	return OK;
}
//取队头元素
Status gethead_queue(LinkQueue Q, Elemtype* e)
{
	if (Q.front == NULL)
		return ERROR;
	else
		*e = Q.front->data;
	return OK;
}