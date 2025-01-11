
// VideoClientDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "VideoClient.h"
#include "VideoClientDlg.h"
#include "afxdialogex.h"
#include "VideoClientController.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVideoClientDlg 对话框



CVideoClientDlg::CVideoClientDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_VIDEOCLIENT_DIALOG, pParent), m_status(false)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVideoClientDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_PLAY, m_video);
    DDX_Control(pDX, IDC_SLIDER_POS, m_pose);
    DDX_Control(pDX, IDC_SLIDER_VOLUME, m_volume);
    DDX_Control(pDX, IDC_EDIT_URL, m_url);
    DDX_Control(pDX, IDC_BTN_PLAY, m_btnPlay);
}

BEGIN_MESSAGE_MAP(CVideoClientDlg, CDialogEx)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_TIMER()
    ON_BN_CLICKED(IDC_BTN_PLAY, &CVideoClientDlg::OnBnClickedBtnPlay)
    ON_BN_CLICKED(IDC_BTN_STOP, &CVideoClientDlg::OnBnClickedBtnStop)
    ON_WM_DESTROY()
    ON_NOTIFY(TRBN_THUMBPOSCHANGING, IDC_SLIDER_POS, &CVideoClientDlg::OnTRBNThumbPosChangingSliderPos)
    ON_NOTIFY(TRBN_THUMBPOSCHANGING, IDC_SLIDER_VOLUME, &CVideoClientDlg::OnTRBNThumbPosChangingSliderVolume)
    ON_WM_HSCROLL()
    ON_WM_VSCROLL()
END_MESSAGE_MAP()


// CVideoClientDlg 消息处理程序

BOOL CVideoClientDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
    //  执行此操作
    SetIcon(m_hIcon, TRUE);			// 设置大图标
    SetIcon(m_hIcon, FALSE);		// 设置小图标

    // TODO: 在此添加额外的初始化代码
    SetTimer(0, 500, NULL);
    m_pose.SetRange(0, 100);
    m_volume.SetRange(0, 100);
    m_volume.SetTic(10);
    m_volume.SetTicFreq(20);
    SetDlgItemText(IDC_STATIC_VOLUME, L"100%");
    SetDlgItemText(IDC_STATIC_TIME, L"--:--:--/--:--:--");

    return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CVideoClientDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // 用于绘制的设备上下文

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // 使图标在工作区矩形中居中
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // 绘制图标
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialogEx::OnPaint();
    }
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CVideoClientDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}



void CVideoClientDlg::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent == 0)
    {
        // 控制层，获取视频状态、进度信息
        // IDC_STATIC_TIME 更新时间
        auto pos = m_controller->VideoCtrl(EVLC_GET_POSITION);
        if (pos != -1.0f)
        {
            CString strPos;
            strPos.Format(L"%02d:%02d:%02d/%02d:%02d:%02d",
                (int)(pos * 3600) / 60, (int)(pos * 3600) % 60, (int)(pos * 3600 * 60) % 60,
                0, 0, 0);
            SetDlgItemText(IDC_STATIC_TIME, strPos);
        }
        // IDC_STATIC_VOLUME 更新音量
        auto volume = m_controller->VideoCtrl(EVLC_GET_VOLUM);
        CString strVolume;
        strVolume.Format(L"%d%%", volume);
        SetDlgItemText(IDC_STATIC_VOLUME, strVolume);

    }
    CDialogEx::OnTimer(nIDEvent);
}


void CVideoClientDlg::OnBnClickedBtnPlay()
{
    if (m_status)
    {
        // 暂停
        m_controller->VideoCtrl(EVLC_PAUSE);
        m_btnPlay.SetWindowTextW(L"播放");
        m_status = false;
    }
    else
    {
        // 播放
        CString url;
        m_url.GetWindowTextW(url);
        // unicode->utf-8
        m_controller->SetMedia(m_controller->UnicodeToUtf8((LPCTSTR)url));
        // TODO：判断当前是否为恢复
        m_controller->VideoCtrl(EVLC_PLAY);
        m_btnPlay.SetWindowTextW(L"暂停");
        m_status = true;
    }
}


void CVideoClientDlg::OnBnClickedBtnStop()
{
    m_controller->VideoCtrl(EVLC_STOP);
    m_btnPlay.SetWindowTextW(L"播放");
    m_status = false;
}


void CVideoClientDlg::OnDestroy()
{
    CDialogEx::OnDestroy();
    KillTimer(0);
}


void CVideoClientDlg::OnTRBNThumbPosChangingSliderPos(NMHDR* pNMHDR, LRESULT* pResult)
{
    // 此功能要求 Windows Vista 或更高版本。
    // _WIN32_WINNT 符号必须 >= 0x0600。
    NMTRBTHUMBPOSCHANGING* pNMTPC = reinterpret_cast<NMTRBTHUMBPOSCHANGING*>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码
    *pResult = 0;
}


void CVideoClientDlg::OnTRBNThumbPosChangingSliderVolume(NMHDR* pNMHDR, LRESULT* pResult)
{
    // 此功能要求 Windows Vista 或更高版本。
    // _WIN32_WINNT 符号必须 >= 0x0600。
    NMTRBTHUMBPOSCHANGING* pNMTPC = reinterpret_cast<NMTRBTHUMBPOSCHANGING*>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码
    *pResult = 0;
}

// 拖动进度条
void CVideoClientDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    if (nSBCode == 5)
    {
        CString strPos;
        strPos.Format(L"%d%%", nPos);
        SetDlgItemText(IDC_STATIC_TIME, strPos);
        m_controller->SetPosition(nPos);
        //m_controller->SetPosition(nPos / 100.0f);
    }
    CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


// 拖动音量
void CVideoClientDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    if (nSBCode == 5)
    {
        CString strVolume;
        strVolume.Format(L"%d%%", 100 - nPos);
        SetDlgItemText(IDC_STATIC_VOLUME, strVolume);
        m_controller->SetVolume(100 - nPos);
    }
    CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}
