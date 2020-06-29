#include<stdio.h>
#include"global.h"
#include"model.h"
int saveMoney(const Money* pMoney, const char* pPath) {
	//以追加方式按二进制写入
	FILE* fp;
	fopen_s(&fp, pPath, "ab+");
	if (fp == NULL) {
		printf("文件无法正确打开！不能写入充值退费信息！\n");
		return FALSE;
	}
	//将计费信息保存到文件中
	fwrite(pMoney, sizeof(*pMoney), 1, fp);
	//关闭文件
	fclose(fp);
	printf("-----******-----充值退费信息成功存入文件中！-----******-----\n\n");
	return TRUE;
}
