#pragma once
#include <afxwin.h>
#include "DataManager.h"

class CSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CSettingDlg);
public:
	//CSettingDlg();
	//CSettingDlg(CWnd* pParent = nullptr/*=nullptr*/);
	CSettingDlg(const Time& start_time = Time(), const Time& end_time = Time(), const std::wstring& event = std::wstring(), CWnd* pParent = nullptr);

	virtual ~CSettingDlg();

	/*SettingData m_data;*/

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETTING_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	
	DECLARE_MESSAGE_MAP()
private:
	Time start_time;
	Time end_time;
	CString event;
	std::wstring str;

	CEdit m_start_time_edit;
	CEdit m_end_time_edit;
	CEdit m_event_edit;

public:
	virtual BOOL OnInitDialog();
	/**
	 * 判断时字符串是否符合要求.
	 *
	 * \param time
	 * \return
	 */
	bool isValidTime(const CString& time);
	Time ParseString(const CString& time);

	Time GetStartTime() const{
		return start_time;
	}

	Time GetEndTime() const{
		return end_time;
	}

	CString GetEvent() const{
		return event;
	}

	std::wstring GetString() {
		return str;
	}
	//DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};

