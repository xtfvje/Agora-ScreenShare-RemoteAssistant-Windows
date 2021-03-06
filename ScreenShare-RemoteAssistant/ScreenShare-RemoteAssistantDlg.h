
// ScreenShare-RemoteAssistantDlg.h : header file
//

#pragma once


class CAgoraConfig;
class CAgoraMediaWrapper;
class CAgoraSignalWrapper;
class CRemoteAssistantDlg;
// CScreenShareRemoteAssistantDlg dialog
class CScreenShareRemoteAssistantDlg : public CDialogEx
{
// Construction
public:
	CScreenShareRemoteAssistantDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SCREENSHAREREMOTEASSISTANT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	//Media
	afx_msg LRESULT OnEIDJoinChannelSuccess(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEIDReJoinChannelSuccess(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEIDLeaveChannel(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnEIDFirstLocalFrame(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEIDFirstRemoteFrameDecoded(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEIDUserJoined(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEIDUserOffline(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEIDMuteVideo(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnEIDMuteAudio(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnEIDConnectionLost(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEIDVideoDeviceChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRemoteVideoStat(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnStartRecordingService(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnStopRecordingService(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnApiCallExecuted(WPARAM wParam, LPARAM lParam);

	//Signal
	afx_msg HRESULT onLoginSuccess(WPARAM wParam, LPARAM lParam);
	afx_msg HRESULT onLogout(WPARAM wParam, LPARAM lParam);
	afx_msg HRESULT onLogFailed(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT onError(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT onLog(WPARAM wParam, LPARAM lParam);

	afx_msg HRESULT onQueryUserStatusResult(WPARAM wParam, LPARAM lParam);
	afx_msg HRESULT onMessageSendSuccess(WPARAM wParam, LPARAM lParam);
	afx_msg HRESULT onMessageSendError(WPARAM wParam, LPARAM lParam);
	afx_msg HRESULT onMessageInstantReceive(WPARAM wParam, LPARAM lParam);
	afx_msg HRESULT onMessageChannelReceive(WPARAM wParam, LPARAM lParam);

	afx_msg HRESULT onChannelJoined(WPARAM wParam, LPARAM lParam);
	afx_msg HRESULT onChannelJoinFailed(WPARAM wParam, LPARAM lParam);
	afx_msg HRESULT onChannelLeaved(WPARAM wParam, LPARAM lParam);

protected:

	inline void initCtrl();
	inline void initMediaResource();
	inline void initSignalResource();
	inline void uninitResource();

	inline void parseMsg(const std::string &msg);
	inline void notifyLbtnDown(POINT &pt);
	inline void notifyLbtnUp(POINT &pt);
	inline void notifyLbtnDClick(POINT &pt);
	inline void notifyRbtnDown(POINT &pt);
	inline void notifyRbtnUp(POINT &pt);
	inline void notifyRbtnDClick(POINT &pt);
	inline void notifyMove(POINT &pt);
	inline void notifyChar(WPARAM wParam,char ch);
	inline void notifyCopy(const std::string &msg);
	inline void notifyPaste(const std::string &msg);
	inline void notifyStart();
	inline void notifyStop();
	inline void notifyWheel(WPARAM wParam,POINT &pt);

	static BOOL CALLBACK WndEnumProc(HWND hWnd, LPARAM lParam);

private:

	CEdit m_editChannelName;
	CEdit m_editUID;
	
	CButton m_btnJoin;
	CButton m_btnLeave;
	CButton m_btnRemoteAssistant;
	CListBox m_ltMediaUserList;
	CListBox m_ltSignalMsgInfo;
	CButton m_checkMedia;
	CButton m_checkSignal;

	UINT m_uLocalUID;

	CAgoraConfig* m_pAgoraConfig;
	CAgoraMediaWrapper* m_pAgoraMediaWrapper;
	CAgoraSignalWrapper *m_pSignalWrapper;

	HWND m_hScreenWnd;
	std::map<UINT, BOOL> m_mapRemoteUserStatus;

	static UINT uSelectUID;

	CList<HWND> m_listWnd;
	HWND m_hMarkWnd;
	CRemoteAssistantDlg* m_pScreenShareRemoteDlg;

public:
	afx_msg void OnBnClickedButtonJoin();
	afx_msg void OnBnClickedButtonLeave();
	afx_msg void OnBnClickedButtonRemoteAssistant();
	afx_msg void OnLbnDblclkListUserlist();
};
