#pragma once
#include "PluginInterface.h"
#include "MemorandumTimeItem.h"
#include "MemorandumEventItem.h"
#include <string>

class CMemorandum : public ITMPlugin
{
private:
    CMemorandum();

public:
    static CMemorandum& Instance();
    
    virtual IPluginItem* GetItem(int index) override;
    virtual const wchar_t* GetTooltipInfo() override;
    virtual void DataRequired() override;
    virtual OptionReturn ShowOptionsDialog(void* hParent) override;
    virtual const wchar_t* GetInfo(PluginInfoIndex index) override;
    virtual void OnExtenedInfo(ExtendedInfoIndex index, const wchar_t* data) override;
    void ShowContextMenu(CWnd* pWnd);
    void LoadContextMenu();


private:
    static CMemorandum m_instance;
    CMemorandumTimeItem m_time_item;
    CMemorandumEventItem m_event_item;
    std::wstring m_tooltip_info;
    CMenu m_menu;
};

#ifdef __cplusplus
extern "C" {
#endif
    __declspec(dllexport) ITMPlugin* TMPluginGetInstance();

#ifdef __cplusplus
}
#endif
