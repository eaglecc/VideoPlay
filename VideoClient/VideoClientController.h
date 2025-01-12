#pragma once
#include "VideoClientDlg.h"
#include "EVlc.h"


enum EVLCCommand
{
    EVLC_PLAY = 0,
    EVLC_PAUSE,
    EVLC_STOP,
    EVLC_GET_VOLUM,
    EVLC_GET_POSITION,
    EVLC_GET_LENGTH,
};

class CVideoClientController
{
public:
    CVideoClientController();
    ~CVideoClientController();
    int Init(CWnd*& pWnd);
    int Invoke();
    int SetMedia(const std::string& strUrl);
    float VideoCtrl(EVLCCommand nCmd);
    void SetPosition(float pos);
    void SetWnd(HWND hwnd);
    int SetVolume(int volume);
    VlcSize GetMediaInfo();
    // Unicode -> utf-8
    std::string UnicodeToUtf8(const std::wstring& strIn);

private:
    CVideoClientDlg m_dlg;
    EVlc m_vlc;
};

