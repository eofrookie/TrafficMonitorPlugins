#include "pch.h"
#include "MemorandumTimeItem.h"
#include "DataManager.h"
#include "Memorandum.h"

const wchar_t* CMemorandumTimeItem::GetItemName() const
{
    return g_data.StringRes(IDS_PLUGIN_ITEM_NAME1);
}

const wchar_t* CMemorandumTimeItem::GetItemId() const
{
	//TODO: 在此返回插件的唯一ID，建议只包含字母和数字
    return L"6buE6ZSQ5p2w";
}

const wchar_t* CMemorandumTimeItem::GetItemLableText() const
{
    return L"";
}

const wchar_t* CMemorandumTimeItem::GetItemValueText() const
{
    return L"";
}


const wchar_t* CMemorandumTimeItem::GetItemValueSampleText() const
{
    return L"Memorandum Time";
}

bool CMemorandumTimeItem::IsCustomDraw() const
{
	//TODO: 根据是否由插件自绘返回对应的值
    return true;
}

int CMemorandumTimeItem::GetItemWidthEx(void * hDC) const
{
    //绘图句柄
    CDC* pDC = CDC::FromHandle((HDC)hDC);
    //TODO: 如果插件需要自绘，则在此修改显示区域的宽度
    return 0;
}

int CMemorandumTimeItem::GetItemWidth() const{
    return g_data.m_setting_data.m_window_width;
}


void CMemorandumTimeItem::DrawItem(void* hDC, int x, int y, int w, int h, bool dark_mode)
{
    //绘图句柄
    CDC* pDC = CDC::FromHandle((HDC)hDC);
    //矩形区域
    CRect rect(CPoint(x, y), CSize(w, h));
    //TODO: 在此添加绘图代码
    //UINT flag = DT_NOPREFIX | DT_CALCRECT | DT_CENTER | DT_TOP;
    UINT flag = DT_NOPREFIX | DT_CENTER | DT_WORDBREAK;
    /*if (g_data.m_setting_data.m_cur_index >= g_data.m_setting_data.m_num_of_events) {
        g_data.m_setting_data.m_cur_index = g_data.m_setting_data.m_num_of_events - 1;
    }*/

    int index = g_data.m_setting_data.m_cur_index;
    std::wstring start = L"00:00";
    std::wstring end = L"00:00";
    if (index != -1 && g_data.m_setting_data.m_start_times.size() > 0) {
        start = g_data.m_setting_data.m_start_times[index].toString();
    }
    if (index != -1 && g_data.m_setting_data.m_end_times.size() > 0) {
        end = g_data.m_setting_data.m_end_times[index].toString();
    }

    //std::string event = g_data.m_setting_data.m_events_to_do[index];

    std::wstring str = start + L"-" + end;

    pDC->DrawText(str.c_str(), rect, flag);
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
int CMemorandumTimeItem::OnMouseEvent(MouseEventType type, int x, int y, void* hWnd, int flag) {
    //CWnd是窗口类，用于管理窗口的对象，而HWND类似于窗口的标识符，与一个CWnd对象绑定
    CWnd* pWnd = CWnd::FromHandle((HWND)hWnd);
    if (type == IPluginItem::MT_RCLICKED) {
        //显示菜单
        CMemorandum::Instance().ShowContextMenu(pWnd);
        return 1;
    }
    return 0;
}
