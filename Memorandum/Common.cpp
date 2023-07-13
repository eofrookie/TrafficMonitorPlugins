#include "pch.h"
#include "Common.h"
//#include "DataManager.h"
//#include <sstream>
#include <algorithm>




std::wstring CCommon::StrToUnicode(const char* str, bool utf8)
{
    if (str == nullptr)
        return std::wstring();
    std::wstring result;
    int size;
    size = MultiByteToWideChar((utf8 ? CP_UTF8 : CP_ACP), 0, str, -1, NULL, 0);
    if (size <= 0) return std::wstring();
    wchar_t* str_unicode = new wchar_t[size + 1];
    MultiByteToWideChar((utf8 ? CP_UTF8 : CP_ACP), 0, str, -1, str_unicode, size);
    result.assign(str_unicode);
    delete[] str_unicode;
    return result;
}

std::string CCommon::UnicodeToStr(const wchar_t* wstr, bool utf8)
{
    if (wstr == nullptr)
        return std::string();
    std::string result;
    int size{ 0 };
    size = WideCharToMultiByte((utf8 ? CP_UTF8 : CP_ACP), 0, wstr, -1, NULL, 0, NULL, NULL);
    if (size <= 0) return std::string();
    char* str = new char[size + 1];
    WideCharToMultiByte((utf8 ? CP_UTF8 : CP_ACP), 0, wstr, -1, str, size, NULL, NULL);
    result.assign(str);
    delete[] str;
    return result;
}

/**
 * 将日志写入文件.
 * 
 * \param str_test
 * \param filepath
 */
void CCommon::WriteLog(const char* str_text, LPCTSTR file_path) {
    SYSTEMTIME cur_time;
    GetLocalTime(&cur_time);
    char buff[32] = {};

    //safe版本，防止格式化字符串
    sprintf_s(buff, "%d/%02d/%02d %02d:%02d:%02d.%03d", cur_time.wYear,
        cur_time.wMonth, cur_time.wDay, cur_time.wHour, cur_time.wMinute,
        cur_time.wSecond, cur_time.wMilliseconds);
    std::ofstream out_put_file(file_path, std::ios::app);
    

    out_put_file << buff;
    out_put_file << str_text << std::endl;
    out_put_file.close();
}

void CCommon::WriteLog(const wchar_t* str_text, LPCTSTR file_path) {
    SYSTEMTIME cur_time;
    GetLocalTime(&cur_time);
    char buff[32] = {};

    //safe版本，防止格式化字符串
    sprintf_s(buff, "%d/%02d/%02d %02d:%02d:%02d.%03d", cur_time.wYear,
        cur_time.wMonth, cur_time.wDay, cur_time.wHour, cur_time.wMinute,
        cur_time.wSecond, cur_time.wMilliseconds);
    std::ofstream out_put_file(file_path, std::ios::app);
    out_put_file << buff;

    out_put_file << UnicodeToStr(str_text).c_str() << std::endl;
    out_put_file.close();
}

/**
 * 模式拆分.
 * 
 * \param str
 * \param pattern
 * \return 
 */
std::vector<std::string> CCommon::split(const std::string& str, const char pattern) {
    std::vector<std::string> res;
    if (str.find(pattern) == -1) {
        res.push_back(str);
        return res;
    }
    auto e = str.end();
    auto i = str.begin();
    while (i != e) {
        i = find_if_not(i, e, [pattern](char c) { return c == pattern; });
        if (i == e) break;
        auto j = find_if(i, e, [pattern](char c) { return c == pattern; });
        res.push_back(std::string(i, j));
        i = j;
    }
    return res;
}

/**
 * 以字符串作为分隔符.
 * 
 * \param str
 * \param pattern
 * \return 
 */
std::vector<std::string> CCommon::split(std::string str, std::string pattern)
{
    std::string::size_type pos;
    std::vector<std::string> result;
    str += pattern;//扩展字符串以方便操作
    int size = str.size();
    for (int i = 0; i < size; i++)
    {
        pos = str.find(pattern, i);
        if (pos < size)
        {
            if (pos == 0)
            {
                i = pos + pattern.size() - 1;
                continue;
            }
            std::string s = str.substr(i, pos - i);
            result.push_back(s);
            i = pos + pattern.size() - 1;
        }
    }
    return result;
}

CString CCommon::vectorJoinString(const std::vector<Time>& data, const wstring& pattern) {
    std::wstring res{};
    int n = data.size();
    for (int i = 0; i < n; ++i) {
        res += data[i].toString();
        if (i != n - 1) {
            res += pattern;
        }
    }
    return res.c_str();
}



