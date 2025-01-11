#pragma once
#include "vlc.h"
#include <string>

class VlcSize {
public:
    int nWidth;
    int nHeight;
    VlcSize(int w = 0, int h = 0) : nWidth(w), nHeight(h) {};
    VlcSize(const VlcSize& size) : nWidth(size.nWidth), nHeight(size.nHeight) {};
    VlcSize& operator=(const VlcSize& size) {
        if (this != &size)
        {
            nWidth = size.nWidth;
            nHeight = size.nHeight;
        }
        return *this;
    }
};

class EVlc
{
public:
    EVlc();
    ~EVlc();
    int SetMedia(const std::string& url); // url路径如果有中文，需要转码
    int SetHwnd(HWND hwnd);
    int Play();
    int Pause();
    int Stop();
    float GetPositon();
    void SetPositon(float pos);
    int GetVolume();
    int SetVolume(int volume);
    VlcSize GetMediaInfo();
    // Unicode -> utf-8
    std::string UnicodeToUtf8(const std::wstring& strIn);

protected:
    libvlc_instance_t* m_vlcInstance;
    libvlc_media_player_t* m_mediaPlayer;
    libvlc_media_t* m_media;

};

