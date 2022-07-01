﻿#include "pch.h"
#include "TextReader.h"
#include "DataManager.h"
#include "OptionsDlg.h"
#include "OptionsContainerDlg.h"
#include <sstream>
#include <iomanip>

CTextReader CTextReader::m_instance;

CTextReader::CTextReader()
{
}

CTextReader& CTextReader::Instance()
{
    return m_instance;
}

IPluginItem* CTextReader::GetItem(int index)
{
    switch (index)
    {
    case 0:
        return &m_item;
    default:
        break;
    }
    return nullptr;
}

const wchar_t* CTextReader::GetTooltipInfo()
{
    static std::wstring tooltip_info;
    tooltip_info.clear();
    if (g_data.m_setting_data.show_in_tooltips)
    {
        std::wstringstream stream;
        stream << g_data.StringRes(IDS_OPEN_FILE).GetString() << L": " << g_data.m_setting_data.file_path << L"\r\n";
        std::wstring cur_chapter = g_data.GetChapter().GetCurrentChapterTitle();
        if (!cur_chapter.empty())
            stream << g_data.StringRes(IDS_CHAPTER).GetString() << L": " << cur_chapter << L"\r\n";
        stream << g_data.StringRes(IDS_READ_POSITION).GetString() << L": " << std::setiosflags(std::ios::fixed) << std::setprecision(2) << static_cast<double>(g_data.m_setting_data.current_position) * 100 / g_data.GetTextContexts().size() << "%";
        tooltip_info = stream.str();
    }
    return tooltip_info.c_str();
}

void CTextReader::DataRequired()
{
    //TODO: 在此添加获取监控数据的代码
}

ITMPlugin::OptionReturn CTextReader::ShowOptionsDialog(void* hParent)
{
    CWnd* pParent = CWnd::FromHandle((HWND)hParent);
    int result = ShowOptionsDlg(pParent);
    return (result == IDOK ? ITMPlugin::OR_OPTION_CHANGED : ITMPlugin::OR_OPTION_UNCHANGED);
}

const wchar_t* CTextReader::GetInfo(PluginInfoIndex index)
{
    static CString str;
    switch (index)
    {
    case TMI_NAME:
        return g_data.StringRes(IDS_PLUGIN_NAME).GetString();
    case TMI_DESCRIPTION:
        return g_data.StringRes(IDS_PLUGIN_DESCRIPTION).GetString();
    case TMI_AUTHOR:
        return L"zhongyang219";
    case TMI_COPYRIGHT:
        return L"Copyright (C) by Zhong Yang 2022";
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

void CTextReader::OnExtenedInfo(ExtendedInfoIndex index, const wchar_t* data)
{
    switch (index)
    {
    case ITMPlugin::EI_CONFIG_DIR:
        //从配置文件读取配置
        g_data.LoadConfig(std::wstring(data));
        SetAutoReadTimer();
        break;
    default:
        break;
    }
}


void CTextReader::ShowContextMenu(CWnd* pWnd)
{
    LoadContextMenu();
    CMenu* context_menu = m_menu.GetSubMenu(0);
    if (context_menu != nullptr)
    {
        CPoint point1;
        GetCursorPos(&point1);
        DWORD id = context_menu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD, point1.x, point1.y, pWnd);
        //点击了“选项”
        if (id == ID_OPTIONS)
        {
            ShowOptionsDlg(pWnd);
        }
        //点击了“章节”
        else if (id == ID_CHAPTERS)
        {
            ShowOptionsDlg(pWnd, 1);
        }
        //点击了“书签”
        else if (id == ID_BOOKMARKS)
        {
            ShowOptionsDlg(pWnd, 2);
        }
        //点击了“上一页”
        else if (id == ID_PREVIOUS)
        {
            g_data.PageUp();
        }
        //点击了“下一页”
        else if (id == ID_NEXT)
        {
            g_data.PageDown();
        }
        //点击了“添加书签”
        else if (id == ID_ADD_BOOKMARK)
        {
            g_data.AddBookmark();
        }
        //点击了“开启自动翻页”
        else if (id == ID_START_AUTO_READ)
        {
            if (!g_data.m_setting_data.auto_read)
            {
                g_data.m_setting_data.auto_read = true;
                SetAutoReadTimer();
            }
        }
        //点击了“停止自动翻页”
        else if (id == ID_STOP_AUTO_READ)
        {
            if (g_data.m_setting_data.auto_read)
            {
                g_data.m_setting_data.auto_read = false;
                SetAutoReadTimer();
            }
        }
    }

}

void CTextReader::SetAutoReadTimer()
{
    if (m_timer_id != 0)
        KillTimer(NULL, m_timer_id);
    if (g_data.m_setting_data.auto_read)
    {
        m_timer_id = SetTimer(NULL, 1008, g_data.m_setting_data.auto_read_timer_interval, CTextReader::TimerCallBack);
    }
}


int CTextReader::ShowOptionsDlg(CWnd* pParent, int cur_tab /*= 0*/)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    COptionsContainerDlg dlg(cur_tab, pParent);
    dlg.m_options_dlg.m_data = g_data.m_setting_data;
    int rtn = dlg.DoModal();
    if (rtn == IDOK)
    {
        bool auto_read_changed{ g_data.m_setting_data.auto_read != dlg.m_options_dlg.m_data.auto_read || g_data.m_setting_data.auto_read_timer_interval != dlg.m_options_dlg.m_data.auto_read_timer_interval };

        g_data.m_setting_data = dlg.m_options_dlg.m_data;
        int chapter_position = dlg.m_chapter_dlg.GetSelectedPosition();
        if (chapter_position >= 0)
            g_data.m_setting_data.current_position = chapter_position;
        int boolmark_position = dlg.m_bookmark_dlg.GetSelectedPosition();
        if (boolmark_position >= 0)
            g_data.m_setting_data.current_position = boolmark_position;
        
        if (auto_read_changed)
        {
            SetAutoReadTimer();
        }

        g_data.SaveConfig();
    }
    return rtn;
}

void CTextReader::LoadContextMenu()
{
    if (m_menu.m_hMenu == NULL)
    {
        AFX_MANAGE_STATE(AfxGetStaticModuleState());
        m_menu.LoadMenu(IDR_MENU1);
    }

}


void CTextReader::TimerCallBack(HWND, UINT, UINT_PTR, DWORD)
{
    g_data.PageDown();
}

ITMPlugin* TMPluginGetInstance()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return &CTextReader::Instance();
}
