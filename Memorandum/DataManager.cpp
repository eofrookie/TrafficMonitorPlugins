#include "pch.h"
#include "DataManager.h"
#include "Common.h"
#include <vector>
#include <sstream>
#include <string>

CDataManager CDataManager::m_instance;

CDataManager::CDataManager()
{
    //初始化DPI
    HDC hDC = ::GetDC(HWND_DESKTOP);
    m_dpi = GetDeviceCaps(hDC, LOGPIXELSY);
    ::ReleaseDC(HWND_DESKTOP, hDC);
}

CDataManager::~CDataManager()
{
    SaveConfig();
}

CDataManager& CDataManager::Instance()
{
    return m_instance;
}

static void WritePrivateProfileInt(const wchar_t* app_name, const wchar_t* key_name, int value, const wchar_t* file_path)
{
    wchar_t buff[16];
    swprintf_s(buff, L"%d", value);
    WritePrivateProfileString(app_name, key_name, buff, file_path);
}

/**
 * 将配置文件中的字符串转化为多个时间.
 * 
 * \param m_str_of_time
 */
std::vector<Time> CDataManager::parseStrOfTime(CString& m_str_of_time){
    std::vector<Time> ret;
    //CString赋值之后需要通过Releasebuff更新长度
    m_str_of_time.ReleaseBuffer();
    if (m_str_of_time.GetLength() <= 0) {
        return ret;
    }
    std::vector<string> times = CCommon::split(CCommon::UnicodeToStr(m_str_of_time), ',');
    for (auto& t : times) {
        int pos = t.find(':');
        int hour = stoi(t.substr(0, pos));
        int minute = stoi(t.substr(pos + 1));
        ret.emplace_back(hour, minute);
    }
    return ret;
}

void CDataManager::LoadConfig(const std::wstring& config_dir)
{
    //获取模块的路径
    HMODULE hModule = reinterpret_cast<HMODULE>(&__ImageBase);
    wchar_t path[MAX_PATH];
    GetModuleFileNameW(hModule, path, MAX_PATH);
    std::wstring module_path = path;
    m_config_path = module_path;
    if (!config_dir.empty())
    {
        size_t index = module_path.find_last_of(L"\\/");
        //模块的文件名
        std::wstring module_file_name = module_path.substr(index + 1);
        m_config_path = config_dir + module_file_name;
    }
    m_config_path += L".ini";
    std::wstring m_file_path = config_dir + L"event.txt";
    //TODO: 在此添加载入配置的代码
    TCHAR buff[MAX_PATH];
    GetPrivateProfileString(_T("config"), _T("file_path"), m_file_path.c_str(), buff, MAX_PATH, m_config_path.c_str());
    //GetPrivateProfileString(_T("config"), _T("time_format"), _T("HH:mm:ss"), buff, 255, m_config_path.c_str());
    m_setting_data.m_file_path = buff;
    CString m_str_of_start_time{};
    CString m_str_of_end_time{};

    GetPrivateProfileString(L"config", L"start_times", L"", m_str_of_start_time.GetBuffer(MAX_PATH), MAX_PATH, m_config_path.c_str());
    GetPrivateProfileString(L"config", L"end_times", L"", m_str_of_end_time.GetBuffer(MAX_PATH), MAX_PATH, m_config_path.c_str());
    

    m_setting_data.m_start_times = parseStrOfTime(m_str_of_start_time);
    m_setting_data.m_end_times = parseStrOfTime(m_str_of_end_time);
    m_setting_data.m_start_times_str = m_str_of_start_time;
    m_setting_data.m_end_times_str = m_str_of_end_time;
    m_setting_data.m_num_of_events = m_setting_data.m_start_times.size();
    m_setting_data.m_cur_index = -1;
    m_setting_data.m_window_width = GetPrivateProfileInt(_T("config"), _T("window_width"), 180, m_config_path.c_str());

    SYSTEMTIME curtime;
    GetLocalTime(&curtime);
    Time cur(curtime.wHour, curtime.wMinute);

    //当前时间大于事件的结束时间则为下一个事件
    /*for (auto& end_time : m_setting_data.m_end_times) {
        if (end_time < cur) {
            m_setting_data.m_cur_index++;
        }
    }*/
    for (size_t i = 0; i < m_setting_data.m_num_of_events; ++i) {
        if (cur > m_setting_data.m_start_times[i] && cur < g_data.m_setting_data.m_end_times[i]) {
            m_setting_data.m_cur_index = i;
            break;
        }
    }

    //加载事件
    loadEvents(m_setting_data.m_file_path.c_str());

}

void CDataManager::SaveConfig() 
{
    if (!m_config_path.empty())
    {
        //TODO: 在此添加保存配置的代码
        WritePrivateProfileString(_T("config"), _T("file_path"), m_setting_data.m_file_path.c_str(), m_config_path.c_str());
        m_setting_data.updateAllCodeStr();
        ::WritePrivateProfileString(_T("config"), _T("start_times"), m_setting_data.m_start_times_str, m_config_path.c_str());
        ::WritePrivateProfileString(_T("config"), _T("end_times"), m_setting_data.m_end_times_str, m_config_path.c_str());
        WritePrivateProfileInt(_T("config"), _T("window_width"), m_setting_data.m_window_width, m_config_path.c_str());
        std::ofstream out_put_file(m_setting_data.m_file_path, std::ios::out);
        if (out_put_file.fail()) {
            return;
        }
        for (auto& event : m_setting_data.m_events_to_do) {
            string line = CCommon::UnicodeToStr(event, true);
            out_put_file << line <<endl;
        }
        out_put_file.close();
    }
}

const CString& CDataManager::StringRes(UINT id)
{
    auto iter = m_string_table.find(id);
    if (iter != m_string_table.end())
    {
        return iter->second;
    }
    else
    {
        AFX_MANAGE_STATE(AfxGetStaticModuleState());
        m_string_table[id].LoadString(id);
        return m_string_table[id];
    }
}

void CDataManager::DPIFromWindow(CWnd* pWnd)
{
    CWindowDC dc(pWnd);
    HDC hDC = dc.GetSafeHdc();
    m_dpi = GetDeviceCaps(hDC, LOGPIXELSY);
}

int CDataManager::DPI(int pixel)
{
    return m_dpi * pixel / 96;
}

float CDataManager::DPIF(float pixel)
{
    return m_dpi * pixel / 96;
}

int CDataManager::RDPI(int pixel)
{
    return pixel * 96 / m_dpi;
}

HICON CDataManager::GetIcon(UINT id)
{
    auto iter = m_icons.find(id);
    if (iter != m_icons.end())
    {
        return iter->second;
    }
    else
    {
        AFX_MANAGE_STATE(AfxGetStaticModuleState());
        HICON hIcon = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(id), IMAGE_ICON, DPI(16), DPI(16), 0);
        m_icons[id] = hIcon;
        return hIcon;
    }
}

void SettingData::updateAllCodeStr(){
    if (m_start_times.empty()) {
        m_start_times_str.ReleaseBuffer();  //更新长度
        int len = m_start_times_str.GetLength();
        m_start_times_str.Delete(0, len);
    }
    if (m_end_times.empty()) {
        m_end_times_str.ReleaseBuffer();  //更新长度
        int len = m_end_times_str.GetLength();
        m_end_times_str.Delete(0, len);
    }

    m_start_times_str = CCommon::vectorJoinString(m_start_times, L",");
    m_end_times_str = CCommon::vectorJoinString(m_end_times, L",");
}

void SettingData::updateCurIndex() {
    SYSTEMTIME cur_time;
    GetLocalTime(&cur_time);
    Time cur(cur_time.wHour, cur_time.wMinute);
    m_cur_index = -1;

    for (size_t i = 0; i < m_num_of_events; ++i) {
        if (cur >= m_start_times[i] && cur <= m_end_times[i]) {
            m_cur_index = i;
            break;
        }
    }
}

bool CDataManager::loadEvents(LPCTSTR file_path) {
    std::ifstream file{ file_path, std::ios::binary };  //二进制读取
    if (file.fail()) {
        return false;
    }

    file.seekg(0, ios::end);
    int len = file.tellg();
    file.seekg(0, ios::beg);

    if (len > MAX_FILE_SIZE) {
        len = MAX_FILE_SIZE;
    }
    else if (len < 0) {
        return false;
    }

    char* buff = new char[len + 1];
    file.read(buff, len);
    file.close();
    buff[len] = '\0';

    std::string str(buff);
    delete[] buff;

    //一行代表一个事件
    std::istringstream iss(str);
    
    std::string line = "";
    while (iss >> line) {
        std::wstring str = CCommon::StrToUnicode(line.c_str(), true);
        m_setting_data.m_events_to_do.push_back(str.c_str());
    }
    return true;
}

/**
 * 按照格式输出wstring.
 *
 * \param _str
 * \param _Format
 * \param
 * \return
 */
std::wstring wstring_format(std::wstring& _str, const wchar_t* _Format1, ...) {
    std::wstring tmp;

    va_list marker = NULL;
    va_start(marker, _Format1);

    size_t num_of_chars = _vscwprintf(_Format1, marker);

    if (num_of_chars > tmp.capacity()) {
        tmp.resize(num_of_chars + 1);
    }

    vswprintf_s((wchar_t*)tmp.data(), tmp.capacity(), _Format1, marker);

    va_end(marker);

    _str = tmp.c_str();
    return _str;
}


