#include<stdio.h>
#include<Windows.h>
#include<malloc.h>
#include"model.h"
#include"card_service.h"
#include"global.h"
#include"tool.h"
#include"service.h"
#include"statistic.h"

//[函数名]		outputMenu
//[功能]		输出系统菜单
//[参数]		void
//[返回值]		void
void outputMenu() {
	//输出系统菜单
	printf(" ------计费系统菜单------\n");
	printf("|      1. 添加卡\t|\n");
	printf("|      2. 查询卡\t|\n");
	printf("|      3. 上机\t\t|\n");
	printf("|      4. 下机\t\t|\n");
	printf("|      5. 充值\t\t|\n");
	printf("|      6. 退费\t\t|\n");
	printf("|      7. 查询统计\t|\n");
	printf("|      8. 注销卡\t|\n");
	printf("|      0. 退出\t\t|\n");
	printf(" ------------------------\n\n");
	printf("请选择菜单编号：");
}

//[函数名]		add
//[功能]		添加用户卡信息到结构体变量，并在屏幕显示
//[参数]		void
//[返回值]		void
void add() {
	Card card;
	char name[30];		//临时存放用户名
	char pwd[20];		//临时存放密码
	struct tm* endTime;			//临时存放截止时间
	struct tm* startTime;		//临时存放开卡时间
	endTime = startTime = (struct tm*)malloc(sizeof(struct tm));
	getCard();		//便于查重

step1: 
	printf("请输入卡号（长度为1~17）：");
	scanf_s("%s", name,30);
	system("cls");
	//判断输入的卡号长度是否符合标准
	if (strlen(name) > 17) {
		printf("输入卡号长度超过最大值，请重新输入！\n");
		rewind(stdin);		//防止填入数据异常
		Sleep(1000);
		system("cls");
		goto step1;
	}
	//判断输入的卡号是否已经存在
	if (queryCardInfo(name) != NULL) {
		printf("输入的卡号已经存在！请重新输入！\n");
		rewind(stdin);		//防止填入数据异常
		Sleep(1000);
		system("cls");
		goto step1;
	}
	//将输入的卡号保存保存到结构体变量中
	strcpy_s(card.aName, strlen(name) + 1, name);
	system("cls");

step2: 
	printf("请输入密码（长度为1~7）：");
	scanf_s("%s", pwd,20);
	system("cls");
	//判断输入的密码长度是否符合标准
	if (strlen(pwd) > 7) {
		printf("输入密码长度超过最大值，请重新输入！\n");
		rewind(stdin);		//防止填入数据异常
		Sleep(1000);
		system("cls");
		goto step2;
	}
	//将输入的卡号保存保存到结构体变量中
	strcpy_s(card.aPwd, strlen(pwd) + 1,pwd);
	system("cls");
	
	printf("请输入开卡金额（RMB）：");
	scanf_s("%f", &card.fBalance);
	system("cls");

	card.fTotalUse = card.fBalance;		//累计金额等于开卡金额
	card.nDel = 0;						//删除标志
	card.nStatus = 0;					//卡状态
	card.nUseCount = 0;					//使用次数
	//开卡时间，截止时间，最后使用时间都默认为当前时间
	//time（NULL）获取当前绝对时间（日历时间），1970-01-01 00：00：00到现在起的秒数
	card.tStart = card.tEnd = card.tLastTime = time(nullptr);
	
	//根据开卡时间，计算截止时间，有效期均为一年
	localtime_s(startTime,&card.tStart);
	localtime_s(endTime,&card.tEnd);
	endTime->tm_year += 1;				//有效截止时间向后增加1年
	card.tEnd = mktime(endTime);		//将分解时间转换成日历时间

	printf("----------------添加的卡信息如下----------------\n");
	printf("卡号\t\t    密码      状态    开卡金额\n");
	printf("%-20s%-10s%-8d%-15.2f\n",card.aName,card.aPwd,card.nStatus,card.fBalance);

	//卡信息是否添加成功
	if (addCardInfo(card) == FALSE) {
		printf("------****------添加卡信息失败！------****------\n");
		system("pause");
		system("cls");
	}
	else {
		printf("------****------添加卡信息成功！------****------\n");
		system("pause");
		system("cls");
	}
}

//[函数名]		query
//[功能]		根据输入的卡号，判断有无该卡，有的话，输出卡的信息
//[参数]		void
//[返回值]		void
void query() {
	Card* pCard = NULL;
	char name[18];		//存放要查询的用户名；
	char aLastTime[30];		//存放指定格式的字符串的时间
	int icha = 0;		//选择查询方式
	int nIndex = 0;		//卡查询到的信息数量

	printf("1.精确查询\n2.模糊查询\n3.查询全部信息\n请选择查询方式：");
	scanf_s("%d", &icha);
	system("cls");
	
	if (icha == 1 || icha == 2) {
		printf("请输入要调查的卡号（长度为1~17）：");
		scanf_s("%s", name, 18);
		system("cls");
	}
	
	
	switch (icha) {
		case 1: {		//精确查询
			rewind(stdin);		//防止填入数据异常
			pCard = queryCardInfo(name);
			break;
		}
		case 2: {		//模糊查询
			rewind(stdin);		//防止填入数据异常
			pCard = queryCardsInfo(name, &nIndex);
			break;
		}
		case 3: {		//查询全体
			rewind(stdin);		//防止填入数据异常
			 pCard = queryAllCardsInfo(&nIndex);
			 break;
		}	
		default: {
			rewind(stdin);		//防止填入数据异常
			printf("您输入的选项编号有误，请检查！\n");
			Sleep(1000);
			system("cls");
			return;
		}
	}

	//pCard为NULL说明没有查询到卡信息
	if (pCard == NULL) {
		printf("------****------没有该卡的信息！------****------\n");
		Sleep(1000);
		system("cls");
	}
	else {
		printf("------------********------------查询到该卡信息如下！------------********------------\n");
		printf("卡号\t\t  状态\t  余额\t\t 累计金额\t使用次数    上次使用时间\n");

		if (icha == 1) {		//精确输出
			timeToString(pCard->tLastTime, aLastTime);
		
			printf("%-18s%-8d%-15.2f%-15.2f%-12d%-30s\n", pCard->aName, pCard->nStatus, pCard->fBalance, pCard->fTotalUse,pCard->nUseCount, aLastTime);
			system("pause");
			system("cls");
		}
		else {		//模糊输出，全体输出
			for (int i = 0; i < nIndex; i++) {
				timeToString(pCard[i].tLastTime, aLastTime);

				printf("%-18s%-8d%-15.2f%-15.2f%-12d%-30s\n", pCard[i].aName, pCard[i].nStatus, pCard[i].fBalance, pCard[i].fTotalUse, pCard[i].nUseCount, aLastTime);
			}
			printf("---------------------------------查询到%-3d个卡信息！---------------------------------\n",nIndex);
													
			system("pause");
			system("cls");
			//释放动态分配的内存；
			free(pCard);
		}
		pCard = NULL;
	}
}

//[函数名]		exitApp
//[功能]		退出应用程序
//[参数]		void
//[返回值]		void
void exitApp() {
	releaseList();
}

//[函数名]		logon
//[功能]		输入上机卡信息，若查询到则输出卡信息
//[参数]		void
//[返回值]		void
void logon() {
	char aName[30];		//上机卡号
	char aPwd[20];		//上机卡密码
//	Card* pCard = nullptr;
	char aLastTime[30];		//存放指定格式的字符串时间
	LogonInfo* pInfo = NULL;
	int nResult = 0;

	memset(aName, 0, sizeof(aName));
	memset(aPwd, 0, sizeof(aPwd));
	memset(aLastTime, 0, sizeof(aLastTime));

	//提示并接受上机卡号
	printf("请输入上机卡号（长度为1-17）：");
	scanf_s("%s", aName,30);
	rewind(stdin);		//防止填入数据异常

	//判断输入的卡号是否符合要求
	if (strlen(aName) >= 18) {
		printf("输入卡号长度超过最大值！\n");
		system("pause");
		system("cls");
		return;
	}

	//提示并接受上机密码；
	printf("请输入上机密码（长度为1-7）:");
	scanf_s("%s", aPwd,20);
	rewind(stdin);		//防止填入数据异常
	system("cls");

	//判断输入的卡号密码是否符合要求
	if (strlen(aPwd) >= 8) {
		printf("输入卡号长度超过最大值！\n");
		system("pause");
		system("cls");
		return;
	}

	//开始上机
//	pCard = doLogon(aName, aPwd);
	pInfo = (LogonInfo*)malloc(sizeof(LogonInfo));
	//根据上机的结果，提示输出不同信息
	nResult = doLogon(aName, aPwd, pInfo);


	//根据上机信息，提示不同的信息
	switch (nResult) {
		case 0:
			printf("------------上机失败！------------\n"); break;
		case 1:
			printf("-----******-----上机信息如下-----******-----\n");
			printf("卡号              余额              上机时间\n");		//18 18

			timeToString(pInfo->tLogon, aLastTime);

			//输出信息
			printf("%-18s", pInfo->aCardName);
			printf("%-18.2f", pInfo->fBalance);
			printf("%s\n", aLastTime);
			printf("上机成功！\n");
			break;
		case 2:
			printf("------------该卡不能使用！------------\n"); break;
		case 3:
			printf("------------余额不足！------------\n"); break;

			
	}
	system("pause");
	system("cls");
	free(pInfo);
}

//[函数名]		settle
//[功能]		输入下机卡信息，查询到，输出上机卡信息
//[参数]		void
//[返回值]		void
void settle() {
	char aName[18] = { 0 };		//卡号
	char aPwd[8] = { 0 };		//密码
	int nResult = -1;		//下机结果
	SettleInfo* pInfo = NULL;		//下机信息
	char aStartTime[30] = { 0 };	//上机时间
	char aEndTime[30] = { 0 };		//下机时间

	//为下机信息动态分配内存
	pInfo = (SettleInfo*)malloc(sizeof(SettleInfo));

	//提示并接受下机卡号
	printf("请输入下机卡号（长度为1-17）：");
	scanf_s("%s", aName, 18);
	rewind(stdin);		//防止填入数据异常

	//判断输入的卡号是否符合要求
	if (strlen(aName) >= 18) {
		printf("输入卡号长度超过最大值！\n");
		system("pause");
		system("cls");
		return;
	}

	//提示并接受下机密码；
	printf("请输入下机密码（长度为1-7）:");
	scanf_s("%s", aPwd, 8);
	rewind(stdin);		//防止填入数据异常
	system("cls");

	//判断输入的卡号密码是否符合要求
	if (strlen(aPwd) >= 8) {
		printf("输入卡号长度超过最大值！\n");
		system("pause");
		system("cls");
		return;
	}

	//进行下机
	nResult = doSettle(aName, aPwd, pInfo);

	//根据下机结果进行判断，提示不同信息
	printf("------------******------------下机信息如下------------******------------\n");
	switch (nResult) {
		case 0: {
			printf("下机失败！\n");
			system("pause");
			system("cls");
			break;
		}
		case 1: {
			//下机成功！
			printf("卡号              消费           余额           ");
			printf("上机时间                      下机时间                      \n");
			timeToString(pInfo->tStart, aStartTime);
			timeToString(pInfo->tEnd, aEndTime);

			//输出下机卡信息
			printf("%-18s", pInfo->aCardName);
			printf("%-15.2f", pInfo->fAmount);
			printf("%-15.2f", pInfo->fBalance);
			printf("%-30s", aStartTime);
			printf("%-30s\n\n", aEndTime);
			printf("---------下机成功---------\n");
			system("pause");
			system("cls");
			break;
		}
		case 2: {
			//未在使用或者已注销
			printf("该卡未在使用！\n");
			system("pause");
			system("cls");
			break;
		}
		case 3: {
			//卡余额不足
			printf("卡余额不足！\n");
			system("pause");
			system("cls");
			break;
		}
		default: {
			break;
		}
	}
	//释放动态分配的内存
	free(pInfo);
}

//[函数名]		addMoney
//[功能]		充值
//[参数]		void
//[返回值]		void
void addMoney() {
	char aName[18] = { 0 };		//卡号
	char aPwd[8] = { 0 };		//密码
	float fAmount = 0;		//充值金额
	MoneyInfo sMoneyInfo;		//充值信息

	printf("请输入需要充值的卡号（1-17）:");
	scanf_s("%s", aName, 18);
	
	printf("请输入密码（1-7）:");
	scanf_s("%s", aPwd,8);
	rewind(stdin);
	system("cls");

	printf("请输入需要充值的金额（RMB）:");
	scanf_s("%f", &fAmount);
	rewind(stdin);
	system("cls");

	//保存充值金额
	sMoneyInfo.fMoney = fAmount;

	//判断充值是否成功
	if (TRUE == doAddMoney(aName, aPwd, &sMoneyInfo)) {
		//提示充值信息
		printf("------****------充值信息如下------****------\n");

		//输出表头文件
		printf("卡号              充值金额    余额      \n");
		//输出充值卡信息
		printf("%-18s%-12.2f%-12.2f\n\n", sMoneyInfo.aCardName, sMoneyInfo.fMoney, sMoneyInfo.fBalance);
		system("pause");
		system("cls");
	}
	else {
		printf("--------充值失败--------");
		Sleep(1000);
		system("cls");
	}
}

//[函数名]		refundMoney
//[功能]		退费
//[参数]		void
//[返回值]		void
void refundMoney() {
	char aName[18] = { 0 };		//卡号
	char aPwd[8] = { 0 };		//密码
	int nResult = -1;		//退费结果
	MoneyInfo sMoneyInfo;		//退费信息

	printf("请输入需要充值的卡号（1-17）:");
	scanf_s("%s", aName, 18);
	
	printf("请输入密码（1-7）:");
	scanf_s("%s", aPwd, 8);
	rewind(stdin);
	system("cls");

	//进行退费
	nResult = doRefundMoney(aName, aPwd, &sMoneyInfo);
	//根据退费信息，提示不同结果
	switch (nResult) {
		case 0: { //退费失败
			printf("--------退费失败！--------\n");
			Sleep(1000);
			system("cls");
			break;
		}
		case 1: {	//退费成功
			//提示退费信息
			printf("----****----退费信息如下----****----\n");
			//输出表头
			printf("卡号              退费金额    余额\n");
			//输出退费卡信息
			printf("%-18s%-12.2f%-12.2f\n\n", sMoneyInfo.aCardName, sMoneyInfo.fMoney, sMoneyInfo.fBalance);
			system("pause");
			system("cls");
			break;
		}
		case 2: { //正在使用或已经注销
			printf("--------该卡正在使用或者已注销！--------\n");
			Sleep(1000);
			system("cls");
			break;
		}
		case 3: {	//卡余额不足
			printf("--------该卡余额不足！--------\n");
			Sleep(1000);
			system("cls");
			break;
		}
		default: {
			break;
		}
	}
}

//[函数名]		annual
//[功能]		注销
//[参数]		void
//[返回值]		void
void annual() {
	Card card;
	printf("请输入需要注销的卡号（1-17）:");
	scanf_s("%s", card.aName, 18);
	
	printf("请输入密码（1-7）:");
	scanf_s("%s", card.aPwd, 8);
	rewind(stdin);
	system("cls");

	if (FALSE == annulCard(&card)) {
		printf("--------注销卡失败！--------\n");
		Sleep(1000);
		system("cls");
		return;
	}
	else {
		//提示注销信息
		printf("----****----注销信息如下----****----\n");
		//输出表头
		printf("卡号              退款金额\n");
		//输出退费卡信息
		printf("%-18s%-12.2f\n\n", card.aName, card.fBalance);
		system("pause");
		system("cls");
		return;
	}
}

//[函数名]		statistic
//[功能]		进行统计
//[参数]		void
//[返回值]		void
void statistic() {
	int nSelection = -1;
	printf("1.按月份统计用户使用总营业额\n");
	printf("2.按用户卡号统计该用户使用金额\n");
	printf("0.返回\n");
	printf("请选择需要进行的统计操作：");

	//读取用户操作
	scanf_s("%d", &nSelection);
	system("cls");
	
	switch (nSelection) {
		case 0: {
			return;
		}
		case 1: {
			statisticMonth();
			break;
		}
		case 2: {
			statisticUser();
			break;
		}
		default: {
			printf("输入操作错误！\n");
			rewind(stdin);
			Sleep(1000);
			return;
		}
	}
}