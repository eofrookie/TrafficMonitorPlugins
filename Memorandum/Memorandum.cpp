#include "pch.h"
#include "Memorandum.h"
#include "DataManager.h"
#include "OptionsDlg.h"

CMemorandum CMemorandum::m_instance;

CMemorandum::CMemorandum()
{
}

CMemorandum& CMemorandum::Instance()
{
    return m_instance;
}

IPluginItem* CMemorandum::GetItem(int index)
{
    switch (index)
    {
    case 0:
        return &m_time_item;
    case 1:
        return &m_event_item;
    default:
        break;
    }
    return nullptr;
}

const wchar_t* CMemorandum::GetTooltipInfo()
{
    return m_tooltip_info.c_str();
}

void CMemorandum::DataRequired()
{
    //TODO: 在此添加获取监控数据的代码
    //更新cur_index;
    SYSTEMTIME cur_time;
    GetLocalTime(&cur_time);
    Time cur(cur_time.wHour, cur_time.wMinute);
    for (size_t i = 0; i < g_data.m_setting_data.m_num_of_events; ++i) {
        if (cur >= g_data.m_setting_data.m_start_times[i] && cur <= g_data.m_setting_data.m_end_times[i]) {
            g_data.m_setting_data.m_cur_index = i;
            break;
        }
    }
}

ITMPlugin::OptionReturn CMemorandum::ShowOptionsDialog(void* hParent)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    CWnd* pParent = CWnd::FromHandle((HWND)hParent);
    COptionsDlg dlg(pParent);
    dlg.m_data = g_data.m_setting_data;
    if (dlg.DoModal() == IDOK)
    {
        g_data.m_setting_data = dlg.m_data;
        return ITMPlugin::OR_OPTION_CHANGED;
    }
    return ITMPlugin::OR_OPTION_UNCHANGED;
}

const wchar_t* CMemorandum::GetInfo(PluginInfoIndex index)
{
    static CString str;
    switch (index)
    {
    case TMI_NAME:
        return g_data.StringRes(IDS_PLUGIN_NAME).GetString();
    case TMI_DESCRIPTION:
        return g_data.StringRes(IDS_PLUGIN_DESCRIPTION).GetString();
    case TMI_AUTHOR:
        //TODO: 在此返回作者的名字
        return L"hrjray";
    case TMI_COPYRIGHT:
        //TODO: 在此返回版权信息
        return L"Copyright (C) by hrjray 2023";
    case ITMPlugin::TMI_URL:
        //TODO: 在此返回URL
        return L"";
        break;
    case TMI_VERSION:
        //TODO: 在此修改插件的版本
        return L"1.00";
    default:
        break;
    }
    return L"";
}

void CMemorandum::OnExtenedInfo(ExtendedInfoIndex index, const wchar_t* data)
{
    switch (index)
    {
    case ITMPlugin::EI_CONFIG_DIR:
        //从配置文件读取配置
        g_data.LoadConfig(std::wstring(data));
        break;
    default:
        break;
    }
}

ITMPlugin* TMPluginGetInstance()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return &CMemorandum::Instance();
}

/**
 * 从rc文件中动态加载菜单内容.
 * 
 */
void CMemorandum::LoadContextMenu() {
    if (m_menu.m_hMenu == NULL) {
        AFX_MANAGE_STATE(AfxGetStaticModuleState());
        m_menu.LoadMenu(IDR_MENU1);
    }
}

void CMemorandum::ShowContextMenu(CWnd* pWnd) {
    LoadContextMenu();

    CMenu* context_menu = m_menu.GetSubMenu(0);

    if (context_menu != nullptr) {
        CPoint point;
        ::GetCursorPos(&point);

        //确定当前选择的选项
        DWORD id = context_menu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD, point.x, point.y, pWnd);

        if (id == ID_OPTIONS) {
            AFX_MANAGE_STATE(AfxGetStaticModuleState());
            COptionsDlg dlg(pWnd);

            dlg.m_data = g_data.m_setting_data;
            auto rtn = dlg.DoModal();
            if (rtn == IDOK) {
                g_data.m_setting_data = dlg.m_data;
            }
        }

    }


}
