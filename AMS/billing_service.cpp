#include<stdio.h>
#include<time.h>
#include<string.h>
#include<stdlib.h>
#include"billing_file.h"
#include"global.h"
#include"model.h"
#include"tool.h"

lpBillingNode billingList = NULL;

//[函数名]		addBilling
//[功能]		上机时添加消费信息到文件
//[参数]		pName：上机卡号；pBilling：指向消费信息结构体
//[返回值]		TRUE：保存成功	FALSE：保存失败
int addBilling(const char* pName, Billing* pBilling) {
	//将消费信息写入结构体
	strcpy_s(pBilling->aCardName, pName);		//上机卡号
	pBilling->tStart = time(nullptr);		//上机时间
	pBilling->tEnd = 0;		//下机时间		若未下机，则标注为0；
	pBilling->fAmount = 0;		//消费金额
	pBilling->nStatus = 0;		//消费状态	0未结算1结算
	pBilling->nDel = 0;		//删除状态 0未删1删

	//消费信息结构体写入文件
	if (saveBilling(pBilling, BILLINGPATH) == FALSE) {
		return FALSE;
	}
	return TRUE;
}

//[函数名]		intiBillingList
//[功能]		初始化计费信息链表
//[参数]		void
//[返回值]		void
void initBillingList() {
	lpBillingNode head = NULL;
	if (billingList == NULL) {
		head = (lpBillingNode)malloc(sizeof(BillingNode));
		head->next = NULL;
		billingList = head;
	}
}


//[函数名]		releaseBillingList
//[功能]		释放计费信息量表
//[参数]		void
//[返回值]		void
void releaseBillingList() {
	lpBillingNode cur = billingList;
	lpBillingNode next = NULL;
	//销毁链表
	while (cur != NULL) {
		next = cur->next;		//结点内存释放前，next保存其后继
		free(cur);
		cur = next;
	}
	billingList = NULL;
}

//[函数名]		getBilling
//[功能]		从计费信息文件中，获取计费信息，保存到链表中
//[参数]		void
//[返回值]		读取成功返回TRUE，否则返回FALSE
int getBilling() {
	int i = 0;
	int nCount = 0;		//实际的计费信息数
	Billing* pBilling = NULL;		// 读取到的一条计费信息
	lpBillingNode node = NULL;		//当前的尾节点
	lpBillingNode cur = NULL;		//添加到链表中的节点

	//清空链表中已经存在的数据
	if (billingList != NULL) {
		releaseBillingList();
	}
	//初始化链表
	initBillingList();

	//获取文件中计费信息个数
	nCount = getBillingCount(BILLINGPATH);
	
	//动态分配内存用来保存所有的计费信息，相当于结构体数组，pCard相当于数组名
	pBilling = (Billing*)malloc(sizeof(Billing) * nCount);
	if (pBilling != NULL) {
		//如果返回FALSE则读取失败
		if (readBilling(pBilling, BILLINGPATH) == FALSE) {
			free(pBilling);
			return FALSE;
		}

		//将读取的信息，保存到链表中
		for (i = 0, node = billingList; i < nCount; i++) {
			//为节点分配内存
			cur = (lpBillingNode)malloc(sizeof(BillingNode));
			//如果分配内存失败，则返回
			if (cur == NULL) {
				free(pBilling);
				return FALSE;
			}
			//初始化新的空间，全部赋值为0；
			memset(cur, 0, sizeof(BillingNode));

			//将数据保存到链表中
			cur->data = pBilling[i];
			cur->next = NULL;

			//将结点添加到链表的尾部
			node->next = cur;
			node = cur;
		}
		//释放内存
		free(pBilling);
		return TRUE;
	}
	return FALSE;
}

//[函数名]		queryBilling
//[功能]		在计费信息链表中，查询对应的卡计费信息，并获取其在链表中的索引号
//[参数]		pName:指向用户输入的要查询的卡号 pIndex：指向查询到的卡信息在链表中的索引号
//[返回值]		指向查询到的计费信息指针
Billing* queryBilling(const char* pName, int* pIndex) {
	lpBillingNode node = NULL;
	int nIndex = 0;

	//从计费信息文件中读取计费信息列表，失败返回NULL
	if (getBilling() == FALSE) {
		return NULL;
	}

	//node指向第一个数据，准被开始遍历
	node = billingList;		//第一个数据

	//遍历链表，指向表尾NULL时结束；
	while (node != NULL) {
		//判断在遍历的卡号中，查找是否包含pName字符串；
		if (strcmp(node->data.aCardName, pName) == 0 && node->data.nStatus == 0) {
			//卡号相同并且没有结算计费信息
			return &node->data;
		}

		//移动到下一个结点
		node = node->next;
		nIndex++;
		*pIndex = nIndex;
	}
	return NULL;
}

//[函数名]		doStatisticMonth
//[功能]		按月统计全体使用情况
//[参数]		nYear:年份           nMonth：月份		fTotalUse：全体使用金额
//[返回值]		TRUE:成功	FALSE：失败
int doStatisticMonth(const int nYear, const int nMonth, float* fTotalUse) {
	lpBillingNode node = NULL;
	*fTotalUse = 0.0;	//初始化金额

	//从计费信息文件中读取计费信息列表，失败返回NULL
	if (getBilling() == FALSE) {
		return NULL;
	}

	//node指向第一个数据，准被开始遍历
	node = billingList;		//第一个数据
	while (node != NULL) {
		
		if ((node->data.nStatus == 1) && (timeToYear(node->data.tEnd) == nYear) && (timeToMonth(node->data.tEnd) == nMonth)) {
			//已缴费并且以最后下机时间来计算消费金额
			(*fTotalUse) = (*fTotalUse) + node->data.fAmount;
		}
		node = node->next;
	}
	return TRUE;
}

//[函数名]		doStatisticUser
//[功能]		统计一位用户的使用数据
//[参数]		aName：用户名 curHead：指向数据的头指针的地址；
//[返回值]		TRUE:成功	FALSE：失败
int doStatisticUser(const char* aName,lpBillingNode* curHead) {

	int i = 0;
	int nCount = 0;		//实际的计费信息数
	Billing* pBilling = NULL;		// 读取到的一条计费信息
	lpBillingNode node = NULL;		//当前的尾节点
	lpBillingNode cur = NULL;		//添加到链表中的节点

	//清空链表中已经存在的数据
	if (billingList != NULL) {
		releaseBillingList();
	}
	//初始化链表
	initBillingList();

	//获取文件中计费信息个数
	nCount = getBillingCount(BILLINGPATH);

	//动态分配内存用来保存所有的计费信息，相当于结构体数组，pCard相当于数组名
	pBilling = (Billing*)malloc(sizeof(Billing) * nCount);
	if (pBilling != NULL) {
		//如果返回FALSE则读取失败
		if (readBilling(pBilling, BILLINGPATH) == FALSE) {
			free(pBilling);
			return FALSE;
		}

		//将读取的信息，保存到链表中
		for (i = 0, node = billingList; i < nCount; i++) {
			if (strcmp(pBilling[i].aCardName, aName) == 0) {		//如果匹配成功即将数据保存到链表
				//为节点分配内存
				cur = (lpBillingNode)malloc(sizeof(BillingNode));
				//如果分配内存失败，则返回
				if (cur == NULL) {
					free(pBilling);
					return FALSE;
				}
				//初始化新的空间，全部赋值为0；
				memset(cur, 0, sizeof(BillingNode));

				//将数据保存到链表中
				cur->data = pBilling[i];
				cur->next = NULL;
				
				//将结点添加到链表的尾部
				node->next = cur;
				node = cur;
			}
		}
		*curHead = billingList;
		//释放内存
		free(pBilling);
		return TRUE;
	}
	return FALSE;
}