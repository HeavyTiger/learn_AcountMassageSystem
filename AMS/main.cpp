#include<stdio.h>
#include<Windows.h>
#include"menu.h"
#include"global.h"

//[函数名]	main
//[功能]		程序入口函数
//[参数]		void
//[返回值]	0:程序正常退出；非0：程序异常；
int main() {
	
	printf("------欢迎使用计费管理系统------\n");
	printf("---------制作：喵式重战--------\n\n");
	Sleep(1000);
	system("cls");
	//即将选择的菜单项目的编号；
	int nSelection = -1;

	do{
		//输出选择菜单；
		outputMenu();
		
		nSelection = -1;
		scanf_s("%d", &nSelection);
		system("cls");
		switch (nSelection) {
		case 0: {
			rewind(stdin);		//防止填入数据异常
			exitApp();
			break;
		}
		case 1: {
			printf("----------添加卡----------\n");
			rewind(stdin);		//防止填入数据异常
			add();
			break;
		}
		case 2: {
			printf("----------查询卡----------\n");
			rewind(stdin);		//防止填入数据异常
			query();
			break;
		}
		case 3: {
			printf("----------上机----------\n");
			rewind(stdin);		//防止填入数据异常
			logon();
			break;
		}
		case 4: {
			printf("----------下机----------\n");
			rewind(stdin);		//防止填入数据异常
			settle();
			break;
		}
		case 5: {
			printf("----------充值----------\n");
			rewind(stdin);		//防止填入数据异常
			addMoney();
			break;
		}
		case 6: {
			printf("----------退费----------\n");
			rewind(stdin);		//防止填入数据异常
			refundMoney();
			break;
		}
        case 7: {
			printf("----------统计----------\n");
			rewind(stdin);		//防止填入数据异常
			statistic();
			break;
		}		
		case 8: {
			printf("----------注销卡----------\n");
			rewind(stdin);		//防止填入数据异常
			annual();
			break;
		}
		default: {
			rewind(stdin);		//防止填入数据异常
			printf("您输入的选项编号有误，请检查！\n");
			Sleep(1000);
			system("cls");
		}
		}//检查功能是否存在；
	} while (nSelection != 0);
	printf("------欢迎下次使用------\n");
	return 0;
}