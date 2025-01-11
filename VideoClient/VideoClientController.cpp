#include "pch.h"
#include "VideoClientController.h"

CVideoClientController::CVideoClientController()
{
    m_dlg.m_controller = this;
}

CVideoClientController::~CVideoClientController()
{
}

int CVideoClientController::Init(CWnd*& pWnd)
{
    pWnd = &m_dlg;
    return 0;
}

int CVideoClientController::Invoke()
{
    INT_PTR nResponse = m_dlg.DoModal();
    if (nResponse == IDOK)
    {
        // TODO: 在此放置处理何时用
        //  “确定”来关闭对话框的代码
    }
    else if (nResponse == IDCANCEL)
    {
        // TODO: 在此放置处理何时用
        //  “取消”来关闭对话框的代码
    }
    else if (nResponse == -1)
    {
        TRACE(traceAppMsg, 0, "警告: 对话框创建失败，应用程序将意外终止。\n");
        TRACE(traceAppMsg, 0, "警告: 如果您在对话框上使用 MFC 控件，则无法 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS。\n");
    }

    return nResponse;
}

int CVideoClientController::SetMedia(const std::string& strUrl)
{
    return m_vlc.SetMedia(strUrl);

}

float CVideoClientController::VideoCtrl(EVLCCommand nCmd)
{
    switch (nCmd)
    {
    case EVLC_PLAY:
        return m_vlc.Play();
    case EVLC_PAUSE:
        return m_vlc.Pause();
    case EVLC_STOP:
        return m_vlc.Stop();
    case EVLC_GET_VOLUM:
        return m_vlc.GetVolume();
    case EVLC_GET_POSITION:
        return m_vlc.GetPositon();
    default:
        break;
    }
    return -1.0f;
}

void CVideoClientController::SetPosition(float pos)
{
    m_vlc.SetPositon(pos);
}

void CVideoClientController::SetWnd(HWND hwnd)
{
    m_vlc.SetHwnd(hwnd);
}

int CVideoClientController::SetVolume(int volume)
{
    return m_vlc.SetVolume(volume);
}

VlcSize CVideoClientController::GetMediaInfo()
{
    return m_vlc.GetMediaInfo();
}

std::string CVideoClientController::UnicodeToUtf8(const std::wstring& strIn)
{
    return m_vlc.UnicodeToUtf8(strIn);
}
