#include "pch.h"
#include "EVlc.h"

EVlc::EVlc()
{
    m_vlcInstance = libvlc_new(0, NULL);
    m_mediaPlayer = NULL;
    m_media = NULL;
    m_hwnd = NULL;
}

EVlc::~EVlc()
{
    if (m_mediaPlayer != NULL)
    {
        libvlc_media_player_t* temp = m_mediaPlayer;
        m_mediaPlayer = NULL;
        libvlc_media_player_release(temp);
    }
    if (m_media != NULL)
    {
        libvlc_media_t* temp = m_media;
        m_media = NULL;
        libvlc_media_release(temp);
    }
    if (m_vlcInstance != NULL)
    {
        libvlc_instance_t* temp = m_vlcInstance;
        m_vlcInstance = NULL;
        libvlc_release(temp);
    }
}

int EVlc::SetMedia(const std::string& url)
{
    if (m_vlcInstance == NULL || m_hwnd == NULL)
    {
        return -1;
    }
    if (m_url == url)
    {
        return 0;
    }
    m_url = url;
    if (m_media != NULL)
    {
        libvlc_media_release(m_media);
        m_media = NULL;
    }
    m_media = libvlc_media_new_location(m_vlcInstance, url.c_str());
    if (!m_media)
    {
        return -2;
    }
    if (m_mediaPlayer != NULL)
    {
        libvlc_media_player_t* temp = m_mediaPlayer;
        m_mediaPlayer = NULL;
        libvlc_media_player_release(temp);
    }
    m_mediaPlayer = libvlc_media_player_new_from_media(m_media);
    if (!m_mediaPlayer)
    {
        return -3;
    }

    //调整比例
    CRect rect;
    GetWindowRect(m_hwnd, rect);
    std::string strRatio = "";
    strRatio.resize(32);
    sprintf((char*)strRatio.c_str(), "%d:%d", rect.Width(), rect.Height());
    libvlc_video_set_aspect_ratio(m_mediaPlayer, strRatio.c_str()); //设置视频的宽高比
    libvlc_media_player_set_hwnd(m_mediaPlayer, m_hwnd);
    return 0;
}

int EVlc::SetHwnd(HWND hwnd)
{
    m_hwnd = hwnd;

    return 0;
}

int EVlc::Play()
{
    if (!m_mediaPlayer || !m_media || !m_vlcInstance)
    {
        return -1;
    }
    return libvlc_media_player_play(m_mediaPlayer);
}

int EVlc::Pause()
{
    if (!m_mediaPlayer || !m_media || !m_vlcInstance)
    {
        return -1;
    }
    libvlc_media_player_pause(m_mediaPlayer);
    return 0;
}

int EVlc::Stop()
{
    if (!m_mediaPlayer || !m_media || !m_vlcInstance)
    {
        return -1;
    }
    libvlc_media_player_stop(m_mediaPlayer);
    return 0;
}

float EVlc::GetPositon()
{
    if (!m_mediaPlayer || !m_media || !m_vlcInstance)
    {
        return -1.0;
    }
    return libvlc_media_player_get_position(m_mediaPlayer);
}

void EVlc::SetPositon(float pos)
{
    if (!m_mediaPlayer || !m_media || !m_vlcInstance)
    {
        return;
    }
    libvlc_media_player_set_position(m_mediaPlayer, pos);
}

int EVlc::GetVolume()
{

    if (!m_mediaPlayer || !m_media || !m_vlcInstance)
    {
        return -1;
    }

    return libvlc_audio_get_volume(m_mediaPlayer);
}

int EVlc::SetVolume(int volume)
{
    if (!m_mediaPlayer || !m_media || !m_vlcInstance)
    {
        return -1;
    }
    return libvlc_audio_set_volume(m_mediaPlayer, volume);
}

float EVlc::GetLength()
{
    if (!m_mediaPlayer || !m_media || !m_vlcInstance)
    {
        return -1.0f;
    }
    auto tm = libvlc_media_player_get_length(m_mediaPlayer);
    float ret = tm / 1000.0f; // ms -> s
    return ret;
}

VlcSize EVlc::GetMediaInfo()
{
    if (!m_mediaPlayer || !m_media || !m_vlcInstance)
    {
        return VlcSize(-1, -1);
    }

    return VlcSize(libvlc_video_get_width(m_mediaPlayer), libvlc_video_get_height(m_mediaPlayer));
}

std::string EVlc::UnicodeToUtf8(const std::wstring& strIn)
{
    std::string strOut;
    int nLen = WideCharToMultiByte(CP_UTF8, 0, strIn.c_str(), strIn.size(), NULL, 0, NULL, NULL);
    strOut.resize(nLen + 1);
    WideCharToMultiByte(CP_UTF8, 0, strIn.c_str(), strIn.size(), (LPSTR)strOut.c_str(), nLen, NULL, NULL);
    return strOut;

}
