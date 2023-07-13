#pragma once
#include <string>
#include <iostream>
#include <cstring>
#include <string.h>
#include <atltime.h>
#include <vector>

using namespace std;

class Time {
public:

    Time(int hour, int minute) :hour(hour), minute(minute) {

    }
    Time() {
        this->hour = -1;
        this->minute = -1;
    }
    ~Time() {}

    Time(const Time& rhs) {
        this->hour = rhs.getHour();
        this->minute = rhs.getMinute();
    }

    Time& operator=(const Time& rhs) {
        this->hour = rhs.getHour();
        this->minute = rhs.getMinute();
        return *this;
    }

    bool operator>(const Time& rhs) {
        if (hour != rhs.getHour()) {
            return hour > rhs.getHour();
        }
        else {
            return minute > rhs.getMinute();
        }
    }

    bool operator<(const Time& rhs) {
        if (hour != rhs.getHour()) {
            return hour < rhs.getHour();
        }
        else {
            return minute < rhs.getMinute();
        }
    }

    bool operator>=(const Time& rhs) {
        if (hour != rhs.getHour()) {
            return hour > rhs.getHour();
        }
        else {
            return minute >= rhs.getMinute();
        }
    }

    bool operator<=(const Time& rhs) {
        if (hour != rhs.getHour()) {
            return hour < rhs.getHour();
        }
        else {
            return minute <= rhs.getMinute();
        }
    }

    std::wstring toString() const {
        wchar_t buff[6] = {};
        swprintf_s(buff, L"%02d:%02d", hour, minute);
        return std::wstring(buff);
    }

    int getHour() const {
        return hour;
    }

    int getMinute() const {
        return minute;
    }
private:
    int hour = -1;
    int minute = -1;
};

static void LOGX(LPCTSTR pStrFormat, ...) {
    ATLTRACE(_T("现在的函数：%s...\n"), __FUNCTIONW__);
    ATLTRACE(_T("现在的行数：%d...\n"), __LINE__);
    CTime curTime;
    curTime = CTime::GetCurrentTime();
    CString str = curTime.Format(_T("%d %b %y %H:%M:%S - "));
    ATLTRACE(str);
    
    va_list args;
    va_start(args, pStrFormat);
    CString new_str;
    new_str.FormatV(pStrFormat, args);
    ATLTRACE(new_str);
    va_end(args);

}

class CCommon
{
public:
    //将const char*字符串转换成宽字符字符串
    static std::wstring StrToUnicode(const char* str, bool utf8 = false);

    static std::string UnicodeToStr(const wchar_t* wstr, bool utf8 = false);
    //将日志信息写入file_path
    //static void WriteLog(const WORD w, LPCTSTR file_path);
    static void WriteLog(const char* str_text, LPCTSTR file_path);
    static void WriteLog(const wchar_t* str_text, LPCTSTR file_path);

    //字符串拆分
    static std::vector<std::string> split(const std::string& str, const char pattern);
    static std::vector <std::string> split(std::string str, string pattern);
    //将字符串合并
    static CString vectorJoinString(const std::vector<Time>& data, const wstring& pattern);
};


