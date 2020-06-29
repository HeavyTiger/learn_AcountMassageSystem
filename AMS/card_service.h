#pragma once
#include "model.h"
int addCard(Card crd);
Card* queryCard(const char* pName);
Card* queryCards(const char* pName, int* pIndex);
Card* queryAllCards(int* pIndex);
int getCard();
int initCardList();
void releaseCardList();
Card* checkCard(const char* pName, const char* pPwd, int* pIndex);