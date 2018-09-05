
// ScreenShare-RemoteAssistantDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ScreenShare-RemoteAssistant.h"
#include "ScreenShare-RemoteAssistantDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#include "AgoraConfig.h"
#include "AgoraMediaWrapper.h"
#include "AgoraSignalWrapper.h"
#include "AGEngineEventHandler.h"
#include "RemoteAssistantDlg.h"
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CScreenShareRemoteAssistantDlg dialog



CScreenShareRemoteAssistantDlg::CScreenShareRemoteAssistantDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CScreenShareRemoteAssistantDlg::IDD, pParent),
	m_pAgoraMediaWrapper(nullptr),
	m_pAgoraConfig(nullptr),
	m_hScreenWnd(nullptr)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CScreenShareRemoteAssistantDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_CHANNELNAME, m_editChannelName);
	DDX_Control(pDX, IDC_EDIT_UID, m_editUID);
	DDX_Control(pDX, IDC_BUTTON_JOIN, m_btnJoin);
	DDX_Control(pDX, IDC_BUTTON_LEAVE, m_btnLeave);
	DDX_Control(pDX, IDC_BUTTON_REMOTE_ASSISTANT, m_btnRemoteAssistant);
	DDX_Control(pDX, IDC_LIST_UserList, m_ltUserList);
}

BEGIN_MESSAGE_MAP(CScreenShareRemoteAssistantDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_JOIN, &CScreenShareRemoteAssistantDlg::OnBnClickedButtonJoin)
	ON_BN_CLICKED(IDC_BUTTON_LEAVE, &CScreenShareRemoteAssistantDlg::OnBnClickedButtonLeave)
	ON_BN_CLICKED(IDC_BUTTON_REMOTE_ASSISTANT, &CScreenShareRemoteAssistantDlg::OnBnClickedButtonRemoteAssistant)
	ON_MESSAGE(WM_MSGID(EID_JOINCHANNEL_SUCCESS), &CScreenShareRemoteAssistantDlg::OnEIDJoinChannelSuccess)
	ON_MESSAGE(WM_MSGID(EID_REJOINCHANNEL_SUCCESS), &CScreenShareRemoteAssistantDlg::OnEIDReJoinChannelSuccess)
	ON_MESSAGE(WM_MSGID(EID_FIRST_LOCAL_VIDEO_FRAME), &CScreenShareRemoteAssistantDlg::OnEIDFirstLocalFrame)
	ON_MESSAGE(WM_MSGID(EID_FIRST_REMOTE_VIDEO_DECODED), &CScreenShareRemoteAssistantDlg::OnEIDFirstRemoteFrameDecoded)
	ON_MESSAGE(WM_MSGID(EID_USER_JOINED), &CScreenShareRemoteAssistantDlg::OnEIDUserJoined)
	ON_MESSAGE(WM_MSGID(EID_USER_OFFLINE), &CScreenShareRemoteAssistantDlg::OnEIDUserOffline)
	ON_MESSAGE(WM_MSGID(EID_REMOTE_VIDEO_STAT), &CScreenShareRemoteAssistantDlg::OnRemoteVideoStat)
	ON_MESSAGE(WM_MSGID(EID_START_RCDSRV), &CScreenShareRemoteAssistantDlg::OnStartRecordingService)
	ON_MESSAGE(WM_MSGID(EID_STOP_RCDSRV), &CScreenShareRemoteAssistantDlg::OnStopRecordingService)
END_MESSAGE_MAP()


// CScreenShareRemoteAssistantDlg message handlers

BOOL CScreenShareRemoteAssistantDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here	
	std::string strAppId = CAgoraConfig::getInstance()->getAppId();
	if ("" == strAppId) {
		AfxMessageBox(L"AppId is empty, please input .");
		PostQuitMessage(0);
		return FALSE;
	}

	initCtrl();
	initMediaResource();
	initSignalResource();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CScreenShareRemoteAssistantDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CScreenShareRemoteAssistantDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CScreenShareRemoteAssistantDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CScreenShareRemoteAssistantDlg::initCtrl()
{
	std::string strChannelName = CAgoraConfig::getInstance()->getChannelName();
	m_editChannelName.SetWindowTextW(CAgoraWrapperUtilc::s2cs(strChannelName));

	std::string strLoginUID = CAgoraConfig::getInstance()->getLoginUID();
	m_editUID.SetWindowTextW(CAgoraWrapperUtilc::s2cs(strLoginUID));

	m_pAgoraConfig = CAgoraConfig::getInstance();
}

void CScreenShareRemoteAssistantDlg::initMediaResource()
{
	m_pAgoraMediaWrapper = CAgoraMediaWrapper::getInstance();
	m_pAgoraMediaWrapper->getEngineEventHandle()->SetMsgReceiver(m_hWnd);
}

void CScreenShareRemoteAssistantDlg::initSignalResource()
{

}

void CScreenShareRemoteAssistantDlg::uninitResource()
{

}

void CScreenShareRemoteAssistantDlg::OnBnClickedButtonJoin()
{
	// TODO: Add your control notification handler code here
	m_pAgoraMediaWrapper->EnableSDKVideoCapture(FALSE);
	m_pAgoraMediaWrapper->setChannelProfile(CHANNEL_PROFILE_LIVE_BROADCASTING);
	UINT uid  = CAgoraWrapperUtilc::str2int(m_pAgoraConfig->getLoginUID());
	//m_pAgoraMediaWrapper->setLocalVideo(m_hWnd,uid);

	m_pAgoraMediaWrapper->enableVideo(TRUE);
	m_pAgoraMediaWrapper->enableAudio(TRUE);

	//m_pAgoraMediaWrapper->muteAllRemoteAudioStreams(TRUE);
	//m_pAgoraMediaWrapper->muteAllRemoteVideoStreams(TRUE);

	m_pAgoraMediaWrapper->setVideoProfile(VIDEO_PROFILE_TYPE::VIDEO_PROFILE_480P_10, FALSE);
	m_pAgoraMediaWrapper->setClientRole(CLIENT_ROLE_TYPE::CLIENT_ROLE_BROADCASTER,NULL);

	m_pAgoraMediaWrapper->getRtcEngine()->startPreview();

	std::string strChannelName = m_pAgoraConfig->getChannelName();
	m_pAgoraMediaWrapper->JoinChannel(NULL, strChannelName.data(), NULL, uid);

	HWND hMarkWnd = ::GetDesktopWindow();
	m_pAgoraMediaWrapper->EnableScreenCapture(hMarkWnd);
}

void CScreenShareRemoteAssistantDlg::OnBnClickedButtonLeave()
{
	// TODO: Add your control notification handler code here

	if (m_pAgoraMediaWrapper) {
		
		m_pAgoraMediaWrapper->leaveChannel();
		m_pAgoraMediaWrapper->getRtcEngine()->stopPreview();
		UINT uid = CAgoraWrapperUtilc::str2int(m_pAgoraConfig->getLoginUID());
		m_pAgoraMediaWrapper->setLocalVideo(NULL, uid);
		m_pAgoraMediaWrapper->EnableScreenCapture(NULL, 15, NULL, FALSE);
	}
}


void CScreenShareRemoteAssistantDlg::OnBnClickedButtonRemoteAssistant()
{
	// TODO: Add your control notification handler code here

	CString strSelectText;
	int nCurSel = m_ltUserList.GetCurSel();
	m_ltUserList.GetText(nCurSel, strSelectText);
	static UINT uSelectUID = 0;
	UINT uRemoteID = CAgoraWrapperUtilc::str2int(CAgoraWrapperUtilc::cs2s(strSelectText));

	std::map<UINT, BOOL>::iterator it = m_mapRemoteUserStatus.find(uRemoteID);
	if (m_mapRemoteUserStatus.end() != it) {

		if (uSelectUID != uRemoteID && uSelectUID != 0) {
			m_pAgoraMediaWrapper->muteRemoteAudioStream(uSelectUID, true);
			m_pAgoraMediaWrapper->muteRemoteVideoStream(uSelectUID, true);
			m_pAgoraMediaWrapper->muteRemoteVideoStream(uRemoteID, false);
			m_pAgoraMediaWrapper->muteRemoteAudioStream(uRemoteID, false);
		}
		else if (uSelectUID == 0) {
			m_pAgoraMediaWrapper->muteRemoteVideoStream(uRemoteID, false);
			m_pAgoraMediaWrapper->muteRemoteAudioStream(uRemoteID, false);
		}
		else
			AfxMessageBox(L"Already Remote Assisting..please Switch Remote User.");
	}

	if (uSelectUID != uRemoteID)
		uSelectUID = uRemoteID;
}

LRESULT CScreenShareRemoteAssistantDlg::OnEIDJoinChannelSuccess(WPARAM wParam, LPARAM lParam)
{
	return TRUE;
}

LRESULT CScreenShareRemoteAssistantDlg::OnEIDReJoinChannelSuccess(WPARAM wParam, LPARAM lParam)
{
	return TRUE;
}

LRESULT CScreenShareRemoteAssistantDlg::OnEIDFirstLocalFrame(WPARAM wParam, LPARAM lParam)
{
	return TRUE;
}

LRESULT CScreenShareRemoteAssistantDlg::OnEIDFirstRemoteFrameDecoded(WPARAM wParam, LPARAM lParam)
{
	LPAGE_FIRST_REMOTE_VIDEO_DECODED lpData = (LPAGE_FIRST_REMOTE_VIDEO_DECODED)wParam;
	if (lpData) {
		
		CRemoteAssistantDlg DlgRemoteAssistant(lpData->uid);
		INT_PTR nResponse = DlgRemoteAssistant.DoModal();
		if (IDOK == nResponse) {

		}
		else if (IDCANCEL == nResponse) {
			//Notify MainWnd RemoteAssistant Finished..

		}

		delete lpData; lpData = nullptr;


	}

	return TRUE;
}

LRESULT CScreenShareRemoteAssistantDlg::OnEIDUserJoined(WPARAM wParam, LPARAM lParam)
{
	LPAGE_USER_JOINED lpData = (LPAGE_USER_JOINED)wParam;
	if (lpData) {

		CString strRemoteID = CAgoraWrapperUtilc::s2cs(CAgoraWrapperUtilc::int2str(lpData->uid));
		m_ltUserList.AddString(strRemoteID);

		if (m_pAgoraMediaWrapper) {
			m_pAgoraMediaWrapper->muteRemoteVideoStream(lpData->uid, true);
			m_pAgoraMediaWrapper->muteRemoteAudioStream(lpData->uid, true);

			m_mapRemoteUserStatus[lpData->uid] = FALSE;
		}

		delete lpData; lpData = nullptr;
	}

	return TRUE;
}

LRESULT CScreenShareRemoteAssistantDlg::OnEIDUserOffline(WPARAM wParam, LPARAM lParam)
{
	return TRUE;
}

LRESULT CScreenShareRemoteAssistantDlg::OnEIDConnectionLost(WPARAM wParam, LPARAM lParam)
{
	return TRUE;
}

LRESULT CScreenShareRemoteAssistantDlg::OnEIDVideoDeviceChanged(WPARAM wParam, LPARAM lParam)
{
	return TRUE;
}

LRESULT CScreenShareRemoteAssistantDlg::OnRemoteVideoStat(WPARAM wParam, LPARAM lParam)
{
	return TRUE;
}
			   
LRESULT CScreenShareRemoteAssistantDlg::OnStartRecordingService(WPARAM wParam, LPARAM lParam)
{
	return TRUE;
}

LRESULT CScreenShareRemoteAssistantDlg::OnStopRecordingService(WPARAM wParam, LPARAM lParam)
{
	return TRUE;
}

LRESULT CScreenShareRemoteAssistantDlg::OnApiCallExecuted(WPARAM wParam, LPARAM lParam)
{
	return TRUE;
}