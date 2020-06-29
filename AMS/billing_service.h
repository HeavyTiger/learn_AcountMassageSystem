#pragma once
#include"model.h"
int addBilling(const char* pName, Billing* pBilling);
void initBillingList();
void releaseBillingList();
Billing* queryBilling(const char* pName, int* pIndex);
int doStatisticMonth(const int nYear, const int nMonth, float* fTotalUse);
int doStatisticUser(const char* aName, lpBillingNode* curHead);