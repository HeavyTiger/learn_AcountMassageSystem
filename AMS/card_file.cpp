#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <stdlib.h>
#include"card_file.h"
#include "model.h"
#include "global.h"
#include "tool.h"
//#define CARDCHARNUM 256		//从文件中读取的字符数最大值

//[函数名]		saveCard
//[功能]		将卡信息保存到文件中
//[参数]		pCard:指向要保存的结构体； pPath：保存的文件路径
//[返回值]		保存成功返回TRUE,失败返回FALSE
int saveCard(const Card* pCard, const char* pPath) {
	//以追加方式按二进制写入
	FILE* fp;
	fopen_s(&fp, pPath, "ab+");
	if (fp == NULL) {
		printf("文件无法正确打开！不能写入卡信息！\n");
		return FALSE;
	}
	//将卡信息保存到文件中
	fwrite(pCard, sizeof(Card), 1, fp);
	//关闭文件
	fclose(fp);
	printf("-----******-----卡信息成功存入文件！-----******-----\n\n");
	return TRUE;
}

//[函数名]		readCard
//[功能]		从文件中读取卡信息到结构体数组
//[参数]		pCard：指向结构体数组；	pPath：文件路径
//[返回值]		正确读取返回TRUE 错误为FALSE
int readCard(Card* pCard, const char* pPath) {
	int nIndex = 0;		//卡信息数量

	//以二进制方式读取
	FILE* fp;
	fopen_s(&fp, pPath, "rb");
	if (fp == NULL) {
		printf("文件无法正确打开！不能读取卡信息！\n");
		return FALSE;
	}
	else {
		//遍历文件，读取文件内容
		while (!feof(fp)) {
			if (fread(&pCard[nIndex], sizeof(Card), 1, fp) != 0) {
				nIndex++;
			}
		}
		//关闭文件
		fclose(fp);
		return TRUE;
	}
}


//[函数名]		getCardCount
//[功能]		从文件中读取卡信息计算卡数量
//[参数]		pPath：文件路径
//[返回值]		文件数量；
int getCardCount(const char* pPath) {
	int nCount = 0;		//卡信息数量
	Card* pCard = (Card*)malloc(sizeof(Card));
	//以二进制方式读取
	FILE* fp;
	fopen_s(&fp, pPath, "rb");
	if (fp == NULL) {
		printf("文件无法正确打开！不能统计卡信息数量！\n");
		return -1;		//-1表示文件无法正确打开
	}
	else {
		//遍历文件，读取文件内容
		while (!feof(fp)) {
			if (fread(pCard, sizeof(Card), 1, fp) != 0) {
				nCount++;
			}
		}
	}
	//关闭文件
	fclose(fp);
	free(pCard);
	return nCount;

}

//[函数名]		updateCard
//[功能]		更新卡信息文件中对应的一条卡信息
//[参数]		pCard：更新后的卡内容		pPath：卡信息文件路径
//			nIndex：需要更新的卡信息文件在文件中的卡序号
//[返回值]		TRUE更新成功		FALSE更新失败
int updateCard(const Card* pCard, const char* pPath, int nIndex) {
	int nCount = 0;		//文件中的当前卡序号
	long lPosition = 0;		//文件标记位置
	Card pcBuf;

	//以二进制读写模式打开文件，如果失败，返回FALSE
	FILE* fp;
	fopen_s(&fp, pPath, "rb+");
	if (fp == NULL) {
		printf("文件无法正确打开！不能更新卡信息！\n");
		return FALSE;
	}
	//遍历文件，获取消费信息在文件中的位置
	while ((!feof(fp)) && (nCount < nIndex)) {
		if (fread(&pcBuf, sizeof(Card), 1, fp) != 0) {
			//获取文件标识位置，最后一次是找到的位置
			lPosition = ftell(fp);
		}
		nCount++;
	}
	//移动到文件标识位置
	fseek(fp, lPosition - sizeof(Card), SEEK_SET);		//将文件指针从需要替换的行尾向前一行
	//将更新信息写入文件
	fwrite(pCard, sizeof(Card), 1, fp);
	printf("-----******-----卡信息更新到文件成功！-----******-----\n\n");

	fclose(fp);
	return TRUE;
	
}