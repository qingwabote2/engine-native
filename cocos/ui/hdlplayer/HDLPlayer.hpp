#pragma once

#include <string>
#include "ijkplayer_cc.hpp"

class HDLPlayer {
private:
    IjkMediaPlayer* _mediaPlayer = nullptr;
    ImageInfo       _imageInfo;

public:
    HDLPlayer();
    void       init(const std::string& url);
    void       play();
    ImageInfo* takeImageInfo();
    ~HDLPlayer();
};
