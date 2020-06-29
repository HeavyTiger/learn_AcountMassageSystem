#pragma once
#include"model.h"
void releaseList();

int addCardInfo(Card card);
int annulCard(Card* pCard);
int doSettle(const char* pName, const char* pPwd, SettleInfo* pInfo);
int doLogon(const char* pName, const char* pPwd, LogonInfo* pInfo);
int doAddMoney(const char* pName, const char* pPwd, MoneyInfo* pMoneyInfo);
int doRefundMoney(const char* pName, const char* pPwd, MoneyInfo* pMoneyInfo);

double getAmount(time_t tStart);

Card* queryCardsInfo(const char* pName, int* pIndex);
Card* queryCardInfo(const char* pName);
Card* queryAllCardsInfo(int* pIndex);