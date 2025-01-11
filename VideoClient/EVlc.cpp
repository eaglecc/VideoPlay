#include "pch.h"
#include "EVlc.h"

EVlc::EVlc()
{
    m_vlcInstance = libvlc_new(0, NULL);
    m_mediaPlayer = NULL;
    m_media = NULL;
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
    if (m_vlcInstance == NULL)
    {
        return -1;
    }
    m_media = libvlc_media_new_location(m_vlcInstance, url.c_str());
    if (!m_media)
    {
        return -2;
    }
    m_mediaPlayer = libvlc_media_player_new_from_media(m_media);
    if (!m_mediaPlayer)
    {
        return -3;
    }
    return 0;
}

int EVlc::SetHwnd(HWND hwnd)
{
    if (!m_mediaPlayer || !m_media || !m_vlcInstance)
    {
        return -1;
    }
    libvlc_media_player_set_hwnd(m_mediaPlayer, hwnd);
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

VlcSize EVlc::GetMediaInfo()
{
    if (!m_mediaPlayer || !m_media || !m_vlcInstance)
    {
        return VlcSize(-1, -1);
    }
    
    return VlcSize(libvlc_video_get_width(m_mediaPlayer), libvlc_video_get_height(m_mediaPlayer));
}
