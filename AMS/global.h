#pragma once

#define FILENOTOPEN -1		//文件打开错误
#define FALSE 0		//表示操作失败
#define	TRUE 1		//表示操作成功
#define UNUSE 2		//卡无法使用
#define ENOUGHMONEY 3		//余额不足

#define UNIT 15		//最小收费单元（分钟）
#define CHARGE 0.5		//最小收费单元收费（RMB:元）

#define CARDPATH "data\\card.ams"		//卡信息保存路径
#define BILLINGPATH "data\\billing.ams"		//计费信息保存路径
#define MONEYPATH "data\\money.ams"		//充值退费信息保存路径
