#include <iostream>
#include "vlc.h"
#include <Windows.h>

// Unicode -> utf-8
std::string UnicodeToUtf8(const std::wstring& strIn) {
    std::string strOut;
    int nLen = WideCharToMultiByte(CP_UTF8, 0, strIn.c_str(), strIn.size(), NULL, 0, NULL, NULL);
    strOut.resize(nLen + 1);
    WideCharToMultiByte(CP_UTF8, 0, strIn.c_str(), strIn.size(), (LPSTR)strOut.c_str(), nLen, NULL, NULL);
    return strOut;
}

int main()
{
    libvlc_instance_t* vlc_instance = libvlc_new(0, nullptr);
    std::string filePath = UnicodeToUtf8(L"C:\\Users\\24777\\Videos\\mp4素材\\car.mp4");  // 多字节-> Unicode -> utf-8
    libvlc_media_t* media = libvlc_media_new_path(vlc_instance, filePath.c_str());
    libvlc_media_player_t* player = libvlc_media_player_new_from_media(media);
    do
    {
        int ret = libvlc_media_player_play(player);
        if (ret == -1)
        {
            printf("error play!\r\n");
            break;
        }
        Sleep(200); // 只有 media 加载完成后才会有下面的参数
        libvlc_time_t tm = libvlc_media_player_get_length(player);
        printf("%02d:%02d:%02d.%03d\r\n", int(tm / 3600000), int((tm / 60000) % 60), int(tm / 1000) % 60, int(tm) % 1000); // 时:分:秒.毫秒
        int width = libvlc_video_get_width(player);
        int height = libvlc_video_get_height(player);
        printf("width:%d height:%d\r\n", width, height);
        getchar();
        libvlc_media_player_pause(player); // 暂停
        getchar();
        libvlc_media_player_play(player); // 播放
        getchar();
        libvlc_media_player_stop(player); // 停止
    } while (0);

    libvlc_media_player_release(player);
    libvlc_media_release(media);
    libvlc_release(vlc_instance);

    std::cout << "Hello World!\n";
}