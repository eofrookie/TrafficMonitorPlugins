#include "pch.h"
#include "MemorandumEventItem.h"
#include "DataManager.h"
#include "Memorandum.h"

const wchar_t* CMemorandumEventItem::GetItemName() const
{
    return g_data.StringRes(IDS_PLUGIN_ITEM_NAME2);
}

const wchar_t* CMemorandumEventItem::GetItemId() const
{
    //TODO: 在此返回插件的唯一ID，建议只包含字母和数字
    return L"cmF5Cg";
}

const wchar_t* CMemorandumEventItem::GetItemLableText() const
{
    return L"";
}

const wchar_t* CMemorandumEventItem::GetItemValueText() const
{
    return L"";
}


const wchar_t* CMemorandumEventItem::GetItemValueSampleText() const
{
    return L"Memorandum Event";
}

bool CMemorandumEventItem::IsCustomDraw() const
{
    //TODO: 根据是否由插件自绘返回对应的值
    return true;
}

int CMemorandumEventItem::GetItemWidthEx(void* hDC) const
{
    //绘图句柄
    CDC* pDC = CDC::FromHandle((HDC)hDC);
    //TODO: 如果插件需要自绘，则在此修改显示区域的宽度
    return 0;
}

int CMemorandumEventItem::GetItemWidth() const {
    return g_data.m_setting_data.m_window_width;
}


void CMemorandumEventItem::DrawItem(void* hDC, int x, int y, int w, int h, bool dark_mode)
{
    //绘图句柄
    CDC* pDC = CDC::FromHandle((HDC)hDC);
    //矩形区域
    CRect rect(CPoint(x, y), CSize(w, h));
    //TODO: 在此添加绘图代码
    UINT flag = DT_NOPREFIX | DT_CENTER | DT_WORDBREAK | DT_CALCRECT;
    //UINT flag = DT_NOPREFIX | DT_WORDBREAK;
    //为了单例模式的线程安全还是在删除时就更新cur_index
    /*if (g_data.m_setting_data.m_cur_index >= g_data.m_setting_data.m_num_of_events) {
        g_data.m_setting_data.m_cur_index = g_data.m_setting_data.m_num_of_events - 1;
    }*/
    int index = g_data.m_setting_data.m_cur_index;
    
    /*std::string start = g_data.m_setting_data.m_start_times[index].toString();
    std::string end = g_data.m_setting_data.m_end_times[index].toString();*/
    wstring event = L"默认事件";
    if (index != -1 && g_data.m_setting_data.m_events_to_do.size() > 0) {
        event = g_data.m_setting_data.m_events_to_do[index].GetString();
    }

    
    pDC->DrawText(event.c_str(), rect, flag);
}

/**
     * @brief   当插件显示区域有鼠标事件时由主程序调用
     * @param   MouseEventType type 鼠标事件的类型
     * @param   int x 鼠标指针所在的x坐标
     * @param   int y 鼠标指针所在的y坐标
     * @param   void* hWnd 产生此鼠标事件的窗口的句柄（主窗口或任务栏窗口）
     * @param   int flag 为若干MouseEventFlag枚举常量的组合
     * @return  int 如果返回非0，则主程序认为插件已经对此鼠标事件作出了全部的响应，主程序将不会再对此鼠标事件做额外的响应。
     *   例如当type为MT_RCLICKED时，如果程序返回0，则会弹出主程序提供的右键菜单；而返回非0时，主程序不会再做任何处理。
     */
int CMemorandumEventItem::OnMouseEvent(MouseEventType type, int x, int y, void* hWnd, int flag) {
    //CWnd是窗口类，用于管理窗口的对象，而HWND类似于窗口的标识符，与一个CWnd对象绑定
    CWnd* pWnd = CWnd::FromHandle((HWND)hWnd);
    if (type == IPluginItem::MT_RCLICKED) {
        //显示菜单
        CMemorandum::Instance().ShowContextMenu(pWnd);
        return 1;
    }
    return 0;
}
