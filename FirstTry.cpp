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

//Author��Shan Zhihan 2020211453
//Encoding��GBK(chinese simplified)
//IDE:vs2019

typedef struct
{
	unsigned weight;//Ȩֵ
	unsigned parent;//���ڵ�
	unsigned lchild, rchild;//���Һ���
}huffTree_node,*huffpt;//���������ڵ㣬�öѴ洢

typedef struct
{
	unsigned symbol;//��Ӧ��charֵ��Ҳ�ǹ�����������±�
	unsigned weight;//Ȩֵ
}sort_node;//���ڿ��ŵ�����
typedef struct n
{
	Elemtype* top;
	Elemtype* base;
	int stacksize;
}SqStack;//ջ�Ľṹ�嶨��

typedef struct node
{
	Elemtype data;
	struct node* next;
}Qnode, * QP;//���нڵ�

typedef struct
{
	QP rear;
	QP front;
	int queue_size;
}LinkQueue;//���нṹ��


typedef struct
{
	unsigned char symbol;//��Ӧ��charֵ��Ҳ�ǹ�����������±�
	unsigned char parent;//�±�һ����128~255֮��
	unsigned char lchild, rchild;//���Һ��ӣ��±�һ����0~127֮�䣬char �����ˡ�
}other_node;//�浽�ļ��е������ڵ�Ĵ洢�ṹ

Status read_in(huffpt HT, sort_node sort[], FILE* in_fp);
void create_huffman(huffpt& HT, FILE* in_fp);
int cmp(const void* a, const void* b);
Status encode(huffpt HT, FILE* in_fp, FILE* &out_fp);
void find_father(huffpt HT, FILE* &out_fp, SqStack& s, LinkQueue& Q, unsigned char c);
Status write_carrier(FILE* &out_fp, LinkQueue& Q); 
Status save_huffman(FILE* &out_fp, huffpt HT);
//�����Ƕ��еĲ�������
Status init_queue(LinkQueue* Q);//��ʼ������
Status enqueue(LinkQueue& Q, Elemtype x);//��Ӳ���
Status dlqueue(LinkQueue& Q);//ɾ������Ԫ��
Status empty_queue(LinkQueue* Q);//�����п�
Status gethead_queue(LinkQueue Q, Elemtype* e);//ȡ����Ԫ��
//������ջ�Ĳ�������
Status StackEmpty(SqStack s);
Status init_stack(SqStack& s);
Status push(SqStack& s, Elemtype e);
Status pop(SqStack& s, Elemtype& e);
int main(void)
{
	char* inputfile = (char*)calloc(1, 51);//�ļ�����������50���ַ�
								           //calloc���뵽��ʼ��ȫ��Ϊ0���ڴ�
	cout << "Please enter the input file name,and make sure it is no longer than 50!\n" << endl;
	fgets(inputfile, 50, stdin);//���벻����50���ַ�����ֹ���������
	inputfile[strlen(inputfile) - 1] = 0;//����\n
	FILE* in_fp = fopen(inputfile, "r");

	char* outputfile = (char*)calloc(1, 51);//�ļ�����������50���ַ�
										   //calloc���뵽��ʼ��ȫ��Ϊ0���ڴ�
	cout << "Please enter the output file name,and make sure it is no longer than 50!\n" << endl;
	fgets(outputfile, 50, stdin);//���벻����50���ַ�����ֹ���������
	outputfile[strlen(outputfile) - 1] = 0;//����\n
	FILE* out_fp = fopen(outputfile, "wb");//�Զ�����д�ķ�ʽ������ļ�

	huffpt HT = NULL;//������������ָ��
	create_huffman(HT,in_fp);//������������
	save_huffman(out_fp, HT);//�洢��������
	encode(HT,in_fp,out_fp);//���뺯��
	fclose(in_fp);
	fclose(out_fp);
	return 0;
}

/**
* ����һ���ļ������Ȩֵ
*/
Status read_in(huffpt HT, sort_node sort[],FILE*in_fp)
{
	
	if (in_fp==NULL)//���ļ�ʧ��
	{
		cout << "Can't open the file properly!\n" << endl;
		exit(-1);
	}
	unsigned char c = fgetc(in_fp);
	int i = 1;
	while (c!=(unsigned char)EOF)
	{
		(HT[c].weight)++;//��Ӧ�±�Ȩֵ++
		sort[c].symbol = c;
		sort[c].weight++;
		c = fgetc(in_fp);//�����ַ�
		i++;
	}
	(HT[0].weight)++;//EOF��255����Ϊ0
	sort[0].symbol = 0;
	sort[0].weight++;//EOF��weightӦ����1
	//::fclose(in_fp);
	return OK;
}
void create_huffman(huffpt & HT,FILE* in_fp)
{
	HT = (huffpt)calloc(sizeof(huffTree_node), (2 * N +5));//���д洢��������,callocȫ����ʼ��Ϊ0
	sort_node* sort = (sort_node*)calloc(sizeof(sort_node), (2 * N + 5));
	if (!HT || !sort)
	{
		cout << "Can't get necessary memory for HT or sort!\n" << endl;
		exit(-1);
	}
	int i = 0;
	read_in(HT,sort,in_fp);//���ļ�����sort��HT��
	for (i = N; i < 2 * N; i++)
	{
		//1.qsort��
		qsort(sort, i, sizeof(sort_node), cmp);
		/*for (int j = 0; j < N; j++)
		{
			printf("%d:   %d\n", sort[j].symbol,sort[j].weight);
		}*/
		if (sort[1].weight == 0)//�ٽ�����������cmpĬ��0����󣬹������Ѿ�ȫ��������
			break;
		HT[sort[0].symbol].parent = i;//��¼���ӵĸ��ڵ�
		HT[sort[1].symbol].parent = i;//��¼���ӵĸ��ڵ�
		HT[i].weight = sort[0].weight + sort[1].weight;//����weight
		HT[i].lchild = sort[0].symbol;
		HT[i].rchild = sort[1].symbol;//��¼�ӽڵ�
		sort[0].weight = 0;
		sort[1].weight = 0;//ɾ���Ѿ����ӽڵ�Ľڵ��Ȩֵ
		sort[i].weight = HT[i].weight;//�����µĽڵ�������������
		sort[i].symbol = i;//��¼�½ڵ�Ķ�Ӧ�±�
		//2.������
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
* Qsort�ıȽϺ���
*/
int cmp(const void* a, const void* b)
{
	sort_node _a= *(sort_node*)a;
	sort_node _b= *(sort_node*)b;
	if (_a.weight == 0)//ȨֵΪ0�Ľڵ㲻������룬Ĭ��0Ϊ���ֵ
		_a.weight = MAX_int;
	if (_b.weight == 0)
		_b.weight = MAX_int;
	return _a.weight - _b.weight;//��0������������
}

/**
* �ѹ���������Ȩֵ��Ϊ0�Ľڵ㰴��ASCIIģʽ����
*/
Status save_huffman(FILE* &out_fp,huffpt HT)
{
	
	unsigned char i = 0; unsigned char father = 0;
	unsigned char lchild=0, rchild=0;
	unsigned char leaves = 0, others = 0;//��¼Ҷ�Ӻ������ڵ�����Ŀ
	for (i=0; i < 255; i++)//leaves
	{
		if (HT[i].weight)
		{
			if(i < 128)
			leaves++;//ͳ��Ҷ�ӵ���Ŀ
			else
			others++;//ͳ�������ڵ����Ŀ
		}
	}
	fwrite(&leaves, 1, 1, out_fp);//�ڿ�ͷ��һ���ֽ�д������Ҷ�ӽڵ���Ŀ
	fwrite(&others, 1, 1, out_fp);//�ڿ�ͷ�ڶ����ֽ�д�����������ڵ���Ŀ
	for (i = 0; i < N; i++)
	{
		if (HT[i].weight)
		{
			fwrite(&i, 1, 1, out_fp);//дsymbol
			father = (unsigned char)HT[i].parent;
			fwrite(&father, 1, 1, out_fp);//дparent
		}
	}
	for (i = N; i <255; i++)
	{
		if (HT[i].weight)
		{
			fwrite(&i, 1, 1, out_fp);//дsymbol
			father = (unsigned char)HT[i].parent;
			fwrite(&father, 1, 1, out_fp);//дparent
			lchild = (unsigned char)HT[i].lchild;
			rchild = (unsigned char)HT[i].rchild;
			fwrite(&lchild, 1, 1, out_fp);//дlchild
			fwrite(&rchild, 1, 1, out_fp);//дrchild
		}
	}
	char a = EOF;
	fwrite(&a, 1, 1, out_fp);//д��eof
	return 1;
}

/**
*���뺯�������ڱ��벢��д��������ļ�\
* 
* 
* 
* 
* д�Ĳ���GG
* 
* Ŀǰ˼·��
* ��ջ���һ���ַ��ı���
* Ȼ�����һ���ַ�֮�����
* Ȼ��Ӷ�ͷ��8��һ��λ
* ��λ���
* 
*/
Status encode(huffpt HT, FILE* in_fp, FILE* &out_fp)
{
	fseek(in_fp, 0, SEEK_SET);
	unsigned char c = fgetc(in_fp);//����
	int count = 0;
	SqStack s;

	Elemtype* stack = (Elemtype*)malloc(stack_size * sizeof(Elemtype));//ջ�Ŀռ�����
	s.base = stack;
	s.top = stack;
	s.stacksize = stack_size;//��ʼ��
	if (!stack) {//�ռ����ʧ��
		printf("ERROR when allocating memory for the stack!\n");
		exit(-1);
	}
	LinkQueue Q;//���нṹ��
	init_queue(&Q);//���г�ʼ��
	while (c != (unsigned char)EOF)//��EOF֮����ַ�����
	{
		find_father(HT, out_fp, s, Q, c);//count�����ã�trans��ָ�룬HT�Ǵ�ֵ��cҲ��
		while (Q.queue_size >= 8) {//���г��ȴ���8
			write_carrier(out_fp, Q);
		}//8λת��һ��unsigned charд��
		c = fgetc(in_fp);//������һ��Ԫ�صı���
	}
	//��EOF����дһ���룬ִ�е������ʱ��c=eof 255
	find_father(HT, out_fp, s, Q, 0);
	while (Q.queue_size >= 8) {//���г��ȴ���8
		write_carrier(out_fp, Q);
	}//8λת��һ��unsigned charд��
	if (Q.queue_size)//�����룬����д��
	{
		for (; Q.queue_size <=8; (Q.queue_size)++) {
			enqueue(Q, 0);
		}
		write_carrier(out_fp, Q);
	}
	//��������

	//fclose(in_fp);
	//fclose(out_fp);
	return OK;
}
void find_father(huffpt HT, FILE* &out_fp, SqStack& s, LinkQueue& Q, unsigned char c)
{
	unsigned parent = HT[c].parent;//���ְ֡�
	int code_size = 0;
	while (parent)
	{

		if (HT[parent].lchild == c)//���ӣ�0
			push(s, 0);
		else
			push(s, 1);//���ӣ�1

		c = parent;
		parent = HT[parent].parent;//�����Ұְ�
		code_size++;//�����±�++
	}//��ĳһ���ַ�����
	char e = 0;
	while(!StackEmpty(s))//ջ����ʱ
	{
		pop(s, e);//��ջ
		enqueue(Q, e);//���
		(Q.queue_size)++;//���г���++
	}
}
Status write_carrier(FILE* &out_fp, LinkQueue& Q)
{
	unsigned char carrier = 0;//���ڴչ��˸��ַ��˵�����ļ���ȥ
	int count = 0;
	char e = 0;
	// if(Q.queue_size<8)
	// return ERROR;//���ܴչ�����
	for (; count < 8; count++)
	{
		gethead_queue(Q, &e);
		carrier = (carrier << 1) ^ e;//λ�����һ��unsigned char
		dlqueue(Q);
		(Q.queue_size)--;
	}
	fwrite(&carrier, 1, 1, out_fp);
	return OK;
}

/**
ջ�ĺ���
*/
//ջ���п�
Status StackEmpty(SqStack s)
{
	if (s.base == s.top)
		return EMPTY;
	else return NOTEMPTY;
}
//��ջ
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

//��ջ
Status pop(SqStack& s, Elemtype& e)//��������e��ɾ����Ԫ�ط���ȥ
{
	if (s.top == s.base)//��ջ��error
		return ERROR;
	else
	{
		e = *--s.top;//����topָ����һ���հ�
		return OK;
	}
}


/**
�����Ƕ��еĺ���
*/
//�����ն���
Status init_queue(LinkQueue* Q)
{
	Q->front = NULL;
	Q->rear = NULL;
	Q->queue_size = 0;
	return OK;
}
//�жӿշ�
Status empty_queue(LinkQueue* Q)
{
	if (Q->front == NULL)
		return OK;
	else
		return ERROR;
}

//Ԫ����Ӻ���en_queue(q,x)
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

//Ԫ�س��Ӻ���dl_queue(q)
Status dlqueue(LinkQueue& Q)
{
	if (Q.front == NULL)
		return ERROR;
	QP p = Q.front;
	Q.front = Q.front->next;

	if (Q.front == NULL)
		Q.rear = NULL;//����ǳ��ǳ���Ҫ������ά������ָ��Ĺ����к����ױ����ԣ����Ұָ�룬ָ���Ѿ����ͷŵĿռ䡣
	free(p);
	return OK;
}
//ȡ��ͷԪ��
Status gethead_queue(LinkQueue Q, Elemtype* e)
{
	if (Q.front == NULL)
		return ERROR;
	else
		*e = Q.front->data;
	return OK;
}