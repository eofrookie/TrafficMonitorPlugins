#pragma once
#include <string>
#include <map>
#include "resource.h"
#include <vector>
#include "Common.h"


#define g_data CDataManager::Instance()


///**
// * 按照格式输出wstring.
// *
// * \param _str
// * \param _Format
// * \param
// * \return
// */
//int test(std::string& _str1, const wchar_t* _Format1, ...) {
//    /*std::wstring tmp;*/
//
//    /*va_list marker = NULL;
//    va_start(marker, _Format1);
//
//    size_t num_of_chars = _vscwprintf(_Format1, marker);
//
//    if (num_of_chars > tmp.capacity()) {
//        tmp.resize(num_of_chars + 1);
//    }
//
//    vswprintf_s((wchar_t*)tmp.data(), tmp.capacity(), _Format1, marker);
//
//    va_end(marker);
//
//    _str = tmp.c_str();*/
//    return 0;
//}

struct SettingData
{
    //TODO: 在此添加选项设置的数据
    //std::map<CString, CString> time2things;
    std::vector<Time> m_start_times;       //事件开始时间
    std::vector<Time> m_end_times;         //事件结束时间
    std::vector<CString> m_events_to_do;   //事件
    int m_num_of_events{};                     //事件个数 
    int m_cur_index{};                       //当前应该做的事情
    int m_window_width;                     //窗口的宽度
    CString m_start_times_str{};            //开始时间序列化字符串
    CString m_end_times_str{};              //结束时间序列化字符串
    std::wstring m_file_path{};                    //存储事件的文件路径

    void updateAllCodeStr();
    void updateCurIndex();
 };


class CDataManager
{
private:
    CDataManager();
    ~CDataManager();

public:
    static CDataManager& Instance();

    void LoadConfig(const std::wstring& config_dir);
    void SaveConfig();
    const CString& StringRes(UINT id);      //根据资源id获取一个字符串资源
    void DPIFromWindow(CWnd* pWnd);
    int DPI(int pixel);
    float DPIF(float pixel);
    int RDPI(int pixel);
    HICON GetIcon(UINT id);

    bool loadEvents(LPCTSTR file_path);

    std::vector<Time> parseStrOfTime(CString& m_str_of_time);
    SettingData m_setting_data;

private:
    static CDataManager m_instance;
    std::wstring m_config_path;
    std::map<UINT, CString> m_string_table;
    std::map<UINT, HICON> m_icons;
    int m_dpi{ 96 };
};
