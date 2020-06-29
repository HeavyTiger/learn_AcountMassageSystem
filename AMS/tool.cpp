#include<stdio.h>
#include<time.h>
#include<malloc.h>

//[函数名]	timeToString
//[功能]		将time_t转换成字符串，字符串的格式为“年-月-日 时：分”
//[参数]		time_t t:需要转化的时间； char* pBuf:转换后的字符串
//[返回值]	void
void timeToString(time_t t, char* pBuf) {
	struct tm* timeinfo ;
	timeinfo = (struct tm*)malloc(sizeof(struct tm));
	localtime_s(timeinfo, &t);
	strftime(pBuf, 20, "%Y-%m-%d %H:%M", timeinfo);
}

//[函数名]	stringToTime
//[功能]		将字符串转换成time_t，字符串的格式为“年-月-日 时：分”
//[参数]		char* pTime；字符串的格式为“年-月-日 时：分
//[返回值]	time_t:时间类型
time_t stringToTime(char* pTime) {
	struct tm tm_1;
	time_t time_1;

	sscanf_s(pTime, "%d-%d-%d %d:%d", &tm_1.tm_year, &tm_1.tm_mon, &tm_1.tm_mday, &tm_1.tm_hour, &tm_1.tm_min);
	
	tm_1.tm_year -= 1900;
	tm_1.tm_mon -= 1;
	tm_1.tm_sec = 0;
	tm_1.tm_isdst = -1;

	time_1 = mktime(&tm_1);

	return time_1;
}

//[函数名]	timeToYear
//[功能]		得到time_t所表示的年份
//[参数]		time_t t:需要转化的时间； 
//[返回值]	int
int timeToYear(time_t t) {
	struct tm* timeinfo;
	timeinfo = (struct tm*)malloc(sizeof(struct tm));
	localtime_s(timeinfo, &t);
	return timeinfo->tm_year + 1900;
}

//[函数名]	timeToMonth
//[功能]		得到time_t所表示的月份
//[参数]		time_t t:需要转化的时间；
//[返回值]	int
int timeToMonth(time_t t) {
	struct tm* timeinfo;
	timeinfo = (struct tm*)malloc(sizeof(struct tm));
	localtime_s(timeinfo, &t);
	return timeinfo->tm_mon + 1;
}