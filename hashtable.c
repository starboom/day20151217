#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define MAXSIZE 30

typedef enum{False,True} BOOL;
typedef enum{NULLKEY,HAVEKEY,DELKEY} HAVEORNOT;

typedef struct 
{
	int elem[MAXSIZE];
	HAVEORNOT elemflag[MAXSIZE];
	int count;
}HashTable;

typedef struct 
{
	int keynum;	
}Record;

void InitialHash(HashTable *);
void CreateHash(HashTable *);
void PrintHash(HashTable);
BOOL SearchHash(HashTable,int,int *);
BOOL InsertHash(HashTable *,Record);
BOOL DeleteHash(HashTable *,Record);
int Hash(int);

int main(int argc, char const *argv[])
{
	HashTable H;
	int ch;
	char j = 'y';


	int position;
	Record R;
	BOOL temp;

	InitialHash(&H);
	CreateHash(&H);

	printf("This program will show how to operate to a HashTable.\n");
	printf("You can display all elems,serach a elem,/ninsert a elem,delete a elem\n");


	while(j != 'n')
	{
		printf("1.display\n");
		printf("2.serach\n");
		printf("3.insert\n");
		printf("4.delete\n");
		printf("5.exit\n");
		printf("请如上五个选项输入索要提供的服务项\n");
		scanf("%d",&ch);


		// while(!ch);
		// printf("%c\n",ch );
		// while(1);

		switch(ch)
		{
			case 1:
				if(H.count)
					PrintHash(H);
				else
					printf("The HashTable has no elems!\n");
				break;
			case 2:
				if(!H.count)
					printf("The HashTable has no elem\n");	
				else
					{
						printf("Please input the keynum(int) of the elem to serach:\n");
						scanf("%d",&R.keynum);
						temp = SearchHash(H,R.keynum,&position);
						if(temp)
							printf("The position of the elem is %d\n",position );
						else
							printf("The elem is not exist!\n");
					}
				break;
			case 3:
				if(H.count == MAXSIZE)
				{
					printf("The HashTable is full\n");
					break;
				}
				printf("Please input the elem(int) to insert:\n");
				scanf("%d",&R.keynum);
				temp = InsertHash(&H,R);
				if(temp)
					printf("Success to insert the elem!\n");
				else
					printf("Fail to insert the elem.The same elem has been exist\n");
				break;
			case 4:
				printf("Please input the keynum of the elem(int) to delete:");
				scanf("%d",&R.keynum);
				temp = DeleteHash(&H,R);
				if(temp)
					printf("Success to delete the elem!\n");
				else
					printf("The elem is not exist in the HashTable!\n");
				break;
			default:
			    j = 'n';

		}
	}
	printf("The program is over!\npress any key to shut off the window!\n");

	getchar();
	return 0;
}
/*
*	函数	：void InitialHash(HashTable *H)
	参数	：哈希表结构体
	返回值	：无返回值
	功能描述：初始化哈希表，计数清零，元素标志位清零
*/
void InitialHash(HashTable *H)
{
	int i;
	(*H).count = 0; 
	for(i = 0;i < MAXSIZE;i++)
		(*H).elemflag[i] = NULLKEY;
}
/*
*	函数	：void CreateHash(HashTable * H)
	参数	：哈希表结构体
	返回值	：无返回值
	功能描述：将哈希数放入struct Record的keynum中。
*/
void CreateHash(HashTable * H)
{
	Record e;
	printf("请输入一系列整数，不超过12个，并键入 -1 结束\n");
	printf("所建立哈希表如下\n");

	scanf("%d",&e.keynum); //输入哈希数
	while(e.keynum != -1)
	{
		if(InsertHash(H,e))
			scanf("%d",&e.keynum);
		else
		{
			printf("请输入不重复的数据！");
			return;
		}
	}
}
/*
*	函数	：void PrintHash(HashTable H)
	参数	：哈希表结构体
	返回值	：无返回值
	功能描述：打印哈希表中元素标志位 以及 各哈希数，
				并打印计数器现值。
*/
void PrintHash(HashTable H)
{
	int i;
	for(i = 0;i < MAXSIZE;i++)
		if(H.elemflag[i] == HAVEKEY)
			printf("%-4d\t",i );
			printf("\n");
	for(i = 0;i < MAXSIZE;i++)
		if(H.elemflag[i] == HAVEKEY)
			printf("%-4d\t",H.elem[i] );
	printf("\ncount: %d\n", H.count);
}
/*
*	函数	：BOOL SearchHash(HashTable H,int k,int *p)
	参数	：哈希表结构体，哈希数k，存放哈希数的指针p
	返回值	：
				return 0:
					查找失败
				return 1：
					查找成功，找到匹配值
	功能描述：在开放定址哈希表H中查找关键字为K的数据元素，若
	查找成功，以p指示待查数据在表中的位置，并返回True；否则，以p
	指示插入位置，并且返回False。
*/

BOOL SearchHash(HashTable H,int k,int *p)
{
	int p1;
	(*p) = Hash(k); //求得哈希地址
	p1 = (*p);
	while(H.elemflag[(*p)] == HAVEKEY && k != H.elem[(*p)])
	{//该位置中填有记录并且关键字不相等
		(*p)++;//哈希冲突处理方法：线性探测再散列
		if((*p) >= MAXSIZE)
			(*p) = (*p) % MAXSIZE;//循环搜索
		if((*p) == p1)//整个表已经搜索完毕，没有待查找的元素
			return False;
	}
	if(k == H.elem[(*p)] && H.elemflag[(*p)] == HAVEKEY)
	//查找成功，p指示待查找元素位置
		return True;
	else
		return False;//查找失败
}
/*
*	函数	：BOOL InsertHash(HashTable *H,Record e)
	参数	：哈希表结构体，记录哈希数结构体
	返回值	：
				return 0:
					发生哈希冲突
				return 1：
					插入哈希表成功
	功能描述：先查询是否与已有数据发生哈希冲突，若冲突，直接
	返回0，否则哈希表结构体中的元素标志数组的相应位置置为HAVEKEY
	哈希表结构体中的哈希数组放入所要插入的值。
	哈希表结构的计数器加1；
*/
BOOL InsertHash(HashTable *H,Record e)
{
	int p;//存放hash地址
	if(SearchHash((*H),e.keynum,&p))
		return False;
	else
	{
		(*H).elemflag[p] = HAVEKEY;
		(*H).elem[p] = e.keynum;
		(*H).count++;
		return True;
	}
}
/*
*	函数	：BOOL DeleteHash(HashTable *H,Record e)
	参数	：哈希表结构体，记录哈希数结构体
	返回值	：
				return 0:
					查找失败，并没有这个值
				return 1：
					删除成功
	功能描述：首先查找是否存在想要删除的元素，不存在，查找无，返回0；
			  否则哈希表结构体元素标志数组相应位置置为DELKEY，
			  计数器减1；返回1；
*/
BOOL DeleteHash(HashTable *H,Record e)
{
	int p;//存放hash地址
	if(!SearchHash((*H),e.keynum,&p))
		return False;
	else
	{
		(*H).elemflag[p] = DELKEY;
		(*H).count--;
		return True;
	}
}
/*
*	函数	：int Hash(int kn)
	参数	：要存放的hash数
	返回值	：
				return kn % 11
	功能描述：
			hash函数 H(key) = key mod 11
*/
int Hash(int kn)
{
	return (kn % 11); 
}
