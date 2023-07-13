// OptionsDlg.cpp: 实现文件
//

#include "pch.h"
#include "Memorandum.h"
#include "OptionsDlg.h"
#include "afxdialogex.h"
#include "DataManager.h"
//#include "Common.h"
#include "SettingDlg.h"

// COptionsDlg 对话框

IMPLEMENT_DYNAMIC(COptionsDlg, CDialog)

COptionsDlg::COptionsDlg(CWnd* pParent /*=nullptr*/)
    : CDialog(IDD_OPTIONS_DIALOG, pParent)
{
}


COptionsDlg::~COptionsDlg()
{
}

void COptionsDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_MGR_LIST, m_events_list);
}


BEGIN_MESSAGE_MAP(COptionsDlg, CDialog)
    //ON_STN_CLICKED(IDC_CODE_LABEL, &COptionsDlg::OnStnClickedCodeLabel)   //静态资源点击的动作
    //ON_EN_CHANGE(IDC_CODE_EDIT, &COptionsDlg::OnEnChangeCodeEdit)           //监控到发生变化时
    ON_LBN_SELCHANGE(IDC_MGR_LIST, &COptionsDlg::OnLbnSelchangeMgrList)
    ON_BN_CLICKED(IDC_MGR_ADD_BTN, &COptionsDlg::OnBnClickedMgrAddBtn)
    ON_BN_CLICKED(IDC_MGR_DEL_BTN, &COptionsDlg::OnBnClickedMgrDelBtn)
    ON_BN_CLICKED(IDOK, &COptionsDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &COptionsDlg::OnBnClickedCancel)
    ON_LBN_DBLCLK(IDC_MGR_LIST, &COptionsDlg::OnLbnDblclkMgrList)
END_MESSAGE_MAP()


// COptionsDlg 消息处理程序


BOOL COptionsDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  在此添加额外的初始化
    for (size_t i = 0; i < m_data.m_events_to_do.size(); ++i) {
        std::wstring start =m_data.m_start_times[i].toString();
        std::wstring end = m_data.m_end_times[i].toString();
        CString event = m_data.m_events_to_do[i];
        std::wstring str = start + L"-" + end + L"      " + event.GetString();

        m_events_list.AddString(str.c_str());
    }

    //设置窗口长度
    CString str;
    str.Format(_T("%d"), m_data.m_window_width);
    SetDlgItemText(IDC_WINDOW_WIDTH_EDIT, str);
    
    return TRUE;  // return TRUE unless you set the focus to a control
                  // 异常: OCX 属性页应返回 FALSE
}


void COptionsDlg::OnStnClickedCodeLabel()
{
    // TODO: 在此添加控件通知处理程序代码
}


void COptionsDlg::OnEnChangeCodeEdit()
{
    // TODO:  如果该控件是 RICHEDIT 控件，它将不
    // 发送此通知，除非重写 CDialog::OnInitDialog()
    // 函数并调用 CRichEditCtrl().SetEventMask()，
    // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

    // TODO:  在此添加控件通知处理程序代码
}


void COptionsDlg::OnLbnSelchangeMgrList()
{
    // TODO: 在此添加控件通知处理程序代码
    CString str;
    int curSelPos = 0;

    curSelPos = m_events_list.GetCurSel();

    m_events_list.GetText(curSelPos, str);

    LOGX(_T("CurEvent: %s\n"), str);
    LPCTSTR a = _T("sss");          //项目使用的是Unicode字符集，所以LP指针指向的应该是Unicode 字符串
}




void COptionsDlg::OnBnClickedMgrAddBtn()
{
    // TODO: 在此添加控件通知处理程序代码
    CSettingDlg dlg;
    auto rtn = dlg.DoModal();
    if (rtn == IDOK) {
        Time start = dlg.GetStartTime();
        Time end = dlg.GetEndTime();
        //判断时间是否有重叠的
        for (size_t i = 0; i < m_data.m_num_of_events; ++i) {
            if (start > m_data.m_start_times[i] && start < m_data.m_end_times[i] || 
                end > m_data.m_start_times[i] && end < m_data.m_end_times[i]) {
                MessageBox(L"当前时间段已有安排", TEXT(""), MB_ICONWARNING | MB_OK);
                return;
            }
        }
        m_data.m_start_times.push_back(start);
        m_data.m_end_times.push_back(end);

        CString event = dlg.GetEvent();
        m_data.m_events_to_do.push_back(event);
        m_data.m_num_of_events = m_data.m_events_to_do.size();
        m_data.updateAllCodeStr();
        m_data.updateCurIndex();
        isChanged = true;

        m_events_list.AddString(dlg.GetString().c_str());
    }
}


void COptionsDlg::OnBnClickedMgrDelBtn()
{
    // TODO: 在此添加控件通知处理程序代码
    int curSel = m_events_list.GetCurSel();
    if (curSel < 0 || curSel >= m_data.m_start_times.size()) {
        return;
    }
    m_data.m_start_times.erase(m_data.m_start_times.begin() + curSel);
    m_data.m_end_times.erase(m_data.m_end_times.begin() + curSel);
    m_data.m_events_to_do.erase(m_data.m_events_to_do.begin() + curSel);
    m_data.m_num_of_events = m_data.m_events_to_do.size();
    m_events_list.DeleteString(curSel);
    m_data.updateAllCodeStr();
    m_data.updateCurIndex();
    isChanged = true;
}


void COptionsDlg::OnBnClickedOk()
{
    // TODO: 在此添加控件通知处理程序代码
    CString window_width_str;
    GetDlgItemText(IDC_WINDOW_WIDTH_EDIT, window_width_str);
    //isChanged = window_width_str == m_data.m_window_width;
    m_data.m_window_width = _ttoi(window_width_str);
    g_data.m_setting_data = m_data;
    g_data.SaveConfig();
    /*if (isChanged) {
        m_data.m_window_width = _ttoi(window_width_str);
        g_data.m_setting_data = m_data;
        g_data.SaveConfig();
    }*/
    CDialog::OnOK();
}


void COptionsDlg::OnBnClickedCancel()
{
    // TODO: 在此添加控件通知处理程序代码
    CDialog::OnCancel();
}


void COptionsDlg::OnLbnDblclkMgrList()
{
    // TODO: 在此添加控件通知处理程序代码
    int curSel = m_events_list.GetCurSel();
    if (curSel < 0 || curSel > m_data.m_start_times.size()) {
        return;
    }
    CSettingDlg dlg(m_data.m_start_times[curSel], m_data.m_end_times[curSel], m_data.m_events_to_do[curSel].GetString());
    if (dlg.DoModal() == IDOK) {
        if (!dlg.GetEvent().IsEmpty()) {
            m_data.m_start_times[curSel] = dlg.GetStartTime();
            m_data.m_end_times[curSel] = dlg.GetEndTime();
            m_data.m_events_to_do[curSel] = dlg.GetEvent();
            m_events_list.DeleteString(curSel);
            m_events_list.InsertString(curSel, dlg.GetString().c_str());
            m_data.updateAllCodeStr();
        }
    }
}
