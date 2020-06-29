#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#include<string.h>
#include"card_file.h"
#include"card_service.h"
#include"billing_file.h"
#include"billing_service.h"
#include"model.h"
#include"global.h"

lpCardNode cardList = NULL;		//链表头指针

//[函数名]		addCard
//[功能]		添加卡信息到结构体数组
//[参数]		卡信息结构体
//[返回值]		整数1：添加成功；整数0：不能添加
int addCard(Card crd) {

	if (saveCard(&crd, CARDPATH) == TRUE) {
		return TRUE;
	}
	return FALSE;
}


//[函数名]		queryCard
//[功能]		在结构体数组中查找卡信息
//[参数]		用户输入的要查询指定卡号的卡信息
//[返回值]		结构体数组中查询到的卡信息地址，没有找到返回NULL
Card* queryCard(const char* pName) {
	Card* pCard  = NULL;
	lpCardNode p;		//用于查找中迭代，依次指向链表中的每个节点

	//从卡信息文件中读取卡信息到链表，失败返回NULL；
	
	if (getCard() == FALSE){
		return NULL;
	}
	//指向链表头结点，用于迭代
	p = cardList;
	//在链表中查找指定卡号
	while (p != NULL) {
		if (strcmp(p->data.aName, pName) == 0) {
			pCard = &(p->data);
			return pCard;
		}
		else {
			p = p->next;
		}
	}
	//若未找到则返回NULL
	return pCard;
}

//[函数名]		getCard
//[功能]		从卡信息文件中，获取卡信息，保存到链表中
//[参数]		void
//[返回值]		读取成功返回TRUE，否则返回FALSE
int getCard() {
	int i = 0;
	Card* pCard = NULL;		// 读取到的一条卡信息
	int nCount = 0;		//实际的卡信息数
	lpCardNode node = NULL;		//当前的尾节点
	lpCardNode cur = NULL;		//添加到链表中的节点

	//清空链表中已经存在的数据
	if (cardList != NULL) {
		releaseCardList();
	}
	//初始化链表
	initCardList();

	//获取文件中卡信息个数
	nCount = getCardCount(CARDPATH);
	if (nCount == 0) {
		return FALSE;
	}
	else if (nCount == -1) {
		printf("文件无法打开！\n");
		return FALSE;
	}
	//动态分配内存用来保存所有的卡信息，相当于结构体数组，pCard相当于数组名
	pCard = (Card*)malloc(sizeof(Card) * nCount);
	if (pCard != NULL) {
		//如果返回FALSE则读取失败
		if (readCard(pCard, CARDPATH) == 0) {
			free(pCard);
			return FALSE;
		}

		//将读取的信息，保存到链表中
 		for (i = 0, node = cardList; i < nCount;i++) {
			//为节点分配内存
			cur = (lpCardNode)malloc(sizeof(CardNode));
			//如果分配内存失败，则返回
			if (cur == NULL) {
				free(pCard);
				return FALSE;
			}
			//初始化新的空间，全部赋值为0；
			memset(cur, 0, sizeof(CardNode));

			//将卡信息保存到节点中
			cur->data = pCard[i];
			cur->next = NULL;

			//将结点添加到链表的尾部
			node->next = cur;
			node = cur;
		}
		//释放内存
		free(pCard);
		return TRUE;
	}
	return FALSE;
}


//[函数名]		intiCardList
//[功能]		初始化卡信息链表
//[参数]		void
//[返回值]		int型，TRUE表示成功，FALSE表示失败
int initCardList() {
	lpCardNode head = NULL;
	if (cardList == NULL) {
		head = (lpCardNode)malloc(sizeof(CardNode));
		//如果分配失败，则将链表指向
		if (head != NULL) {
			head->next = NULL;
			cardList = head;	//将链表头指针赋值给全局变量cardList
			return TRUE;
		}
	}
	return FALSE;
}

//[函数名]		releaseCardList
//[功能]		释放卡信息量表
//[参数]		void
//[返回值]		void
void releaseCardList() {
	lpCardNode cur = cardList;
	lpCardNode next = NULL;

	while (cur != NULL) {
		next = cur->next;
		free(cur);
		cur = next;
	}
	cardList = NULL;
}

//[函数名]		queryCards
//[功能]		在卡信息链表中，模糊查询包含的所有卡信息
//[参数]		pName:指向用户输入的要查询的卡号 pIndex：指向查到的卡信息数变量
//[返回值]		指向卡信息结构体的指针
Card* queryCards(const char* pName, int* pIndex) {
	lpCardNode node = NULL;
	Card* pCard = NULL;		//保存查询到的符合条件的卡信息；

	//从卡信息文件中读取卡信息列表，失败返回NULL
	if (getCard() == FALSE) {
		return NULL;
	}
	//首先分配一个Card大小的空间；
	pCard = (Card*)malloc(sizeof(Card));
	if (pCard == NULL) {
		return NULL;
	}

	//node指向第一个数据，准被开始遍历
	node = cardList->next;		//第一个数据

	//遍历链表，指向表尾NULL时结束；
	while (node != NULL) {
		//判断在遍历的卡号中，查找是否包含pName字符串；
		if (strstr(node->data.aName, pName) != NULL) {
			//如果有，则保存结点中的数据
			pCard[*pIndex] = node->data;
			(*pIndex)++;

			//重新为指针分配内存，包含已有内容
			pCard = (Card*)realloc(pCard, ((*pIndex) + 1) * sizeof(Card));
		}

		//移动到下一个结点
		node = node->next;
	}
	if (*pIndex == 0) {
		return NULL;		//未查询到相关信息
	}
	return pCard;
}

Card* queryAllCards(int* pIndex) {
	Card* pCard = NULL;
	lpCardNode node;

	*pIndex = getCardCount(CARDPATH);
	
	//从卡信息文件中读取卡信息列表，失败返回NULL
	if (getCard() == FALSE) {
		return NULL;
	}

	pCard = (Card*)malloc((*pIndex)*sizeof(Card));		//分配内存空间
	if (pCard == NULL) {
		return NULL;
	}
	
	int i = 0;
	node = cardList->next;
	while (node != NULL){		//遍历链表保存到开辟的空间pCard中
		pCard[i++] = node->data;
		node = node->next;
	}
	return pCard;
}


//[函数名]		checkCard
//[功能]		从文件中读取卡信息到链表，在链表中查询卡信息，并获得其在年表中的位置
//[参数]		pName:上机卡号	pPwd：上机密码	pIndex：返回的索引号
//[返回值]		上机卡结构体
Card* checkCard(const char* pName, const char* pPwd, int* pIndex) {
	lpCardNode cardNode = NULL;		//当前结点
	int nIndex = 0;		//匹配的卡的信息结点再链表中的序号，用于更新卡信息
	//从卡信息中读取卡信息到链表。失败返回NULL
	if (getCard() == FALSE) {
		return FALSE;
	}
	//当前结点指向头结点
	if (cardList != NULL) cardNode = cardList;

	while (cardNode != NULL) {
		//在链表中查找是否有对应的卡信息
		if ((strcmp(cardNode->data.aName, pName) == 0) && (strcmp(cardNode->data.aPwd, pPwd) == 0)) {
			//返回卡信息节点数据地址
			*pIndex = nIndex;
			return &cardNode->data;
		}
		cardNode = cardNode->next;
		nIndex++;
	}
	return NULL;
}
