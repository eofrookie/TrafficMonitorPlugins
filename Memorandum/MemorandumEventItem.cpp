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
    //TODO: �ڴ˷��ز����ΨһID������ֻ������ĸ������
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
    //TODO: �����Ƿ��ɲ���Ի淵�ض�Ӧ��ֵ
    return true;
}

int CMemorandumEventItem::GetItemWidthEx(void* hDC) const
{
    //��ͼ���
    CDC* pDC = CDC::FromHandle((HDC)hDC);
    //TODO: ��������Ҫ�Ի棬���ڴ��޸���ʾ����Ŀ��
    return 0;
}

int CMemorandumEventItem::GetItemWidth() const {
    return g_data.m_setting_data.m_window_width;
}


void CMemorandumEventItem::DrawItem(void* hDC, int x, int y, int w, int h, bool dark_mode)
{
    //��ͼ���
    CDC* pDC = CDC::FromHandle((HDC)hDC);
    //��������
    CRect rect(CPoint(x, y), CSize(w, h));
    //TODO: �ڴ���ӻ�ͼ����
    UINT flag = DT_NOPREFIX | DT_CENTER | DT_WORDBREAK | DT_CALCRECT;
    //UINT flag = DT_NOPREFIX | DT_WORDBREAK;
    //Ϊ�˵���ģʽ���̰߳�ȫ������ɾ��ʱ�͸���cur_index
    /*if (g_data.m_setting_data.m_cur_index >= g_data.m_setting_data.m_num_of_events) {
        g_data.m_setting_data.m_cur_index = g_data.m_setting_data.m_num_of_events - 1;
    }*/
    int index = g_data.m_setting_data.m_cur_index;
    
    /*std::string start = g_data.m_setting_data.m_start_times[index].toString();
    std::string end = g_data.m_setting_data.m_end_times[index].toString();*/
    wstring event = L"Ĭ���¼�";
    if (index != -1 && g_data.m_setting_data.m_events_to_do.size() > 0) {
        event = g_data.m_setting_data.m_events_to_do[index].GetString();
    }

    
    pDC->DrawText(event.c_str(), rect, flag);
}

/**
     * @brief   �������ʾ����������¼�ʱ�����������
     * @param   MouseEventType type ����¼�������
     * @param   int x ���ָ�����ڵ�x����
     * @param   int y ���ָ�����ڵ�y����
     * @param   void* hWnd ����������¼��Ĵ��ڵľ���������ڻ����������ڣ�
     * @param   int flag Ϊ����MouseEventFlagö�ٳ��������
     * @return  int ������ط�0������������Ϊ����Ѿ��Դ�����¼�������ȫ������Ӧ�������򽫲����ٶԴ�����¼����������Ӧ��
     *   ���統typeΪMT_RCLICKEDʱ��������򷵻�0����ᵯ���������ṩ���Ҽ��˵��������ط�0ʱ�������򲻻������κδ���
     */
int CMemorandumEventItem::OnMouseEvent(MouseEventType type, int x, int y, void* hWnd, int flag) {
    //CWnd�Ǵ����࣬���ڹ����ڵĶ��󣬶�HWND�����ڴ��ڵı�ʶ������һ��CWnd�����
    CWnd* pWnd = CWnd::FromHandle((HWND)hWnd);
    if (type == IPluginItem::MT_RCLICKED) {
        //��ʾ�˵�
        CMemorandum::Instance().ShowContextMenu(pWnd);
        return 1;
    }
    return 0;
}
