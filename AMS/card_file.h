#pragma once
#include"model.h"
int saveCard(const Card* pCard,const char* pPath);
int readCard(Card* pCard, const char* pPath);
//Card praseCard(char* pBuf);
int getCardCount(const char* pPath);
int updateCard(const Card* pCard, const char* pPath, int nIndex);