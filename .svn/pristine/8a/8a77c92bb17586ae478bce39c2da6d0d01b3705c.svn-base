//
//  DateTimeUtil.cpp
//  Ranotan2D
//
//  Created by Takahashi Hiroyuki on 2014/01/12.
//
//

#include "DateTimeUtil.h"
#include <iostream>
#include <algorithm>
#include <string>

static const char* DateFormat = "%2d-%02d-%02d";

CCString* DateTimeUtil::getNowDateTime()
{
    time_t now = time(NULL);
    struct tm *pnow = localtime(&now);
    
    return CCString::createWithFormat(DateFormat,pnow->tm_year+1900,
                                      pnow->tm_mon + 1,
                                      pnow->tm_mday);
}

CCString* DateTimeUtil::getDateTime(int days)
{
    time_t now = time(NULL) + 86400 * days;
    struct tm *pnow = localtime(&now);
    
    return CCString::createWithFormat(DateFormat,pnow->tm_year+1900,
                                      pnow->tm_mon + 1,
                                      pnow->tm_mday);
}

bool DateTimeUtil::isBefore(CCString *baseDate, CCString *checkDate)
{
    return( doComparate(baseDate, checkDate)==BEFORE );
}

bool DateTimeUtil::isAfter(CCString *baseDate, CCString *checkDate)
{
    return( doComparate(baseDate, checkDate)==AFTER );
}


bool DateTimeUtil::isSame(CCString *baseDate, CCString *checkDate)
{
    return( doComparate(baseDate, checkDate)==SAME );
}

unsigned long long DateTimeUtil::getNow()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return t.tv_sec * 1000ull + t.tv_usec / 1000ull;
}

ChechRresult DateTimeUtil::doComparate(CCString *baseDate, CCString *checkDate)
{
    const std::string base(baseDate->getCString());
    const std::string target(checkDate->getCString());
    std::vector<char> v1(base.begin(), base.end());
    std::vector<char> v2(target.begin(), target.end());
    replace(v1.begin(), v1.end(), '-', '0');
    replace(v2.begin(), v2.end(), '-', '0');
    const std::string s11(v1.begin(), v1.end());
    const std::string s21(v2.begin(), v2.end());
    int date1 = CCString::create(s11.c_str())->intValue();
    int date2 = CCString::create(s21.c_str())->intValue();
    if ( date1<date2 ) {
        return BEFORE;
    }
    if ( date1==date2 ) {
        return SAME;
    }
    return AFTER;
}
