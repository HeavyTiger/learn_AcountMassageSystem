#include<stdio.h>
#include<time.h>
#include<windows.h>
#include"tool.h"
#include"global.h"
#include"model.h"
#include"billing_service.h"


//[函数名]		statisticMonth
//[功能]		统计每月上机获取金额
//[参数]		void
//[返回值]		void
void statisticMonth() {
	int nMonth,nYear;
	float fTotalUse = 0.0;
	time_t tNow = time(nullptr);	//当前时间

	printf("请输入需要统计的年份月份(使用情况以结束时间为准，中间用空格隔开)：");
	scanf_s("%d%d",&nYear,&nMonth);
	rewind(stdin);
	system("cls");
	
	//判断输入时间是否符合要求
	if (nMonth > 12 || nMonth < 1 || nYear > timeToYear(tNow)) {
		printf("输入的需统计时间错误！\n");
		Sleep(1000);
		system("cls");
		return;
	}

	if (FALSE == doStatisticMonth(nYear, nMonth, &fTotalUse)) {
		printf("统计失败！\n");
		system("pause");
		system("cls");
		return;
	}

	//输出
	printf("查询到的信息如下：\n%d年%d月的总使用金额为：%-12.2f\n", nYear, nMonth, fTotalUse);
	system("pause");
	system("cls");
	return;
}

//[函数名]		statisticUser
//[功能]		统计一个用户上机数据
//[参数]		void
//[返回值]		void
void statisticUser() {
	char aName[18];		//用于存放用户名称；
	char aStartTime[30] = {0};		//存放开始时间
	char aLastTime[30] = {0};		//存放结束时间
	int nCount = 0;		//统计数目
	lpBillingNode node = NULL;
	printf("请输入需要查询的用户卡号：");
	scanf_s("%s", aName, 18);
	system("cls");

	if (FALSE == doStatisticUser(aName, &node)) {
		printf("查询错误！\n");
		system("pause");
		system("cls");
		return;
	}

	if (node == NULL) {
		printf("未查询到该卡号的使用数据！\n");
		system("pause");
		system("cls");
		return;
	}
	else {
		printf("卡号%s的使用信息如下：\n", aName);		//输出表头
		printf("--------------------------------------------------------------------------------------------\n");
		printf("上机时间                      下机时间                      使用金额       是否结算  是否删除\n");
		while (node->next != NULL) {
			node = node->next;
			timeToString(node->data.tStart, aStartTime);
			printf("%-30s", aStartTime);
			
			if (node->data.tEnd == 0) {		//如果tEnd为0，证明未下机，执行输出未下机操作
				printf("未下机                        ");
			}
			else {
				timeToString(node->data.tEnd, aLastTime);
				printf("%-30s", aLastTime);
			}
			printf("%-15.2f", node->data.fAmount);
			printf("%-10d%-10d\n", node->data.nStatus, node->data.nDel);
			nCount++;
		}
		printf("---------------------------------------共%-3d个数据-------------------------------------------\n",nCount);
		system("pause");
		system("cls");
		return;
	}
}