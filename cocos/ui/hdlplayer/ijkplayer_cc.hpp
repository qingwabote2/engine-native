#pragma once

extern "C" {
#include "ijkmedia/ijkplayer/ijkplayer.h"
}

#include "ijksdl_vout_cc_imageinfo.hpp"

// ref_count is 1 after open
IjkMediaPlayer *ijkmp_cc_create(int (*msg_loop)(void *));

void ijkmp_cc_takeImageInfo(IjkMediaPlayer *mp, ImageInfo &info, bool &ok);
