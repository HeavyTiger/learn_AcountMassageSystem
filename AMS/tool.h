#pragma once
#include<time.h>
void timeToString(time_t t, char* pBuf);
time_t stringToTime(char* pTime);
int timeToYear(time_t t);
int timeToMonth(time_t t);