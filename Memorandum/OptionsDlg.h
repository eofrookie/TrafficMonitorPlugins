#pragma once
#include "DataManager.h"

// COptionsDlg 对话框

class COptionsDlg : public CDialog
{
    DECLARE_DYNAMIC(COptionsDlg)

public:
    COptionsDlg( CWnd* pParent = nullptr /*=nullptr*/);   // 标准构造函数
    virtual ~COptionsDlg();

    SettingData m_data;

    // 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_OPTIONS_DIALOG };
#endif

private:
    bool isChanged = false;
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    CListBox m_events_list;
    DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    afx_msg void OnStnClickedCodeLabel();
    afx_msg void OnEnChangeCodeEdit();
    afx_msg void OnLbnSelchangeMgrList();
    afx_msg void OnBnClickedMgrAddBtn();
    afx_msg void OnBnClickedMgrDelBtn();
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnLbnDblclkMgrList();
};
