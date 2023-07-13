#include "pch.h"
#include "SettingDlg.h"
#include "Memorandum.h"
#include "Common.h"
#include "DataManager.h"

IMPLEMENT_DYNAMIC(CSettingDlg, CDialog)

//CSettingDlg::CSettingDlg(CWnd* pParent) 
//	:CDialog(IDD_SETTING_DIALOG, pParent){
//}


CSettingDlg::CSettingDlg(const Time& start_time, const Time& end_time, const std::wstring& event, CWnd* pParent)
	:CDialog(IDD_SETTING_DIALOG, pParent),
	start_time(start_time),
	end_time(end_time),
	event(event.c_str())
{
}

CSettingDlg::~CSettingDlg() {

}

/**
 * 将控件与变量进行绑定
 * UpdateData(true)可以将控件的值赋给变量
 * UpdateData(false)将变量的值赋给控件.
 * 
 * \param pDX
 */
void CSettingDlg::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_START_TIME_EDIT, m_start_time_edit);
	DDX_Control(pDX, IDC_END_TIME_EDIT, m_end_time_edit);
	DDX_Control(pDX, IDC_EVENT_EDIT, m_event_edit);
}

BOOL CSettingDlg::OnInitDialog() {
	CDialog::OnInitDialog();
	SetDlgItemText(IDC_START_TIME_EDIT, start_time.toString().c_str());
	SetDlgItemText(IDC_END_TIME_EDIT, end_time.toString().c_str());
	SetDlgItemText(IDC_EVENT_EDIT, event);

	//UpdateData(FALSE);
	return TRUE;
}

BEGIN_MESSAGE_MAP(CSettingDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CSettingDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSettingDlg::OnBnClickedCancel)
END_MESSAGE_MAP()



bool CSettingDlg::isValidTime(const CString& time) {
	int index = time.Find(':');
	if (index == -1) {
		return false;
	}
	//_ttoi()可以用于两种不同编码的CString转换
	CString h = time.Left(index).Trim();
	CString m = time.Right(time.GetLength() - index - 1).Trim();
	CString h1 = h.SpanIncluding(_T("0123456789"));
	CString m1 = m.SpanIncluding(_T("0123456789"));
	if (h1.IsEmpty() || m1.IsEmpty()) {
		return false;
	}

	if (h1 != h || m1 != m) {
		return false;
	}

	int hour = _ttoi(time.Left(index));
	int minute = _ttoi(time.Right(time.GetLength() - index - 1));
	LOGX(L"hour: %d, minute: %d", hour, minute);
	if (hour >= 0 && hour < 24 && minute >= 0 && minute < 60) {
		return true;
	}
	return false;
}

Time CSettingDlg::ParseString(const CString& time) {
	int index = time.Find(':');
	int hour = _ttoi(time.Left(index));
	int minute = _ttoi(time.Right(time.GetLength() - index - 1));
	return Time(hour, minute);
}

void CSettingDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString time1, time2;
	GetDlgItemText(IDC_START_TIME_EDIT, time1);
	GetDlgItemText(IDC_END_TIME_EDIT, time2);
	GetDlgItemText(IDC_EVENT_EDIT, event);

	if (!isValidTime(time1) || !isValidTime(time2)) {
		MessageBox(L"错误的时间格式，正确的是xx:xx", L"", MB_ICONWARNING | MB_OK);
		return;
	}
	start_time = ParseString(time1);
	end_time = ParseString(time2);
	str = start_time.toString() + L"-" + end_time.toString() + L"      " + event.GetString();
	CDialog::OnOK();
}


void CSettingDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnCancel();
}
