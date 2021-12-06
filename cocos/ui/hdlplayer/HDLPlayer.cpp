#include "HDLPlayer.hpp"
#include "platform/FileUtils.h"

// NOTE: could be called from multiple thread
static int ijkff_inject_callback(void *opaque, int message, void *data, size_t data_size) {
    return 0;
}

static int media_player_msg_loop(void *arg) {
    IjkMediaPlayer *mp = (IjkMediaPlayer *)arg;
    while (1) {
        AVMessage msg;

        int retval = ijkmp_get_msg(mp, &msg, 1);
        if (retval < 0)
            break;

        // block-get should never return 0
        assert(retval > 0);

        switch (msg.what) {
            case FFP_MSG_FLUSH:
                MPTRACE("FFP_MSG_FLUSH:\n");
                break;
            case FFP_MSG_ERROR:
                MPTRACE("FFP_MSG_ERROR: %d\n", msg.arg1);
                break;
            case FFP_MSG_PREPARED:
                MPTRACE("FFP_MSG_PREPARED:\n");
                break;
            case FFP_MSG_COMPLETED:
                MPTRACE("FFP_MSG_COMPLETED:\n");
                break;
            case FFP_MSG_VIDEO_SIZE_CHANGED:
                MPTRACE("FFP_MSG_VIDEO_SIZE_CHANGED: %d, %d\n", msg.arg1, msg.arg2);
                break;
            case FFP_MSG_SAR_CHANGED:
                MPTRACE("FFP_MSG_SAR_CHANGED: %d, %d\n", msg.arg1, msg.arg2);
                break;
            case FFP_MSG_VIDEO_RENDERING_START:
                MPTRACE("FFP_MSG_VIDEO_RENDERING_START:\n");
                break;
            case FFP_MSG_AUDIO_RENDERING_START:
                MPTRACE("FFP_MSG_AUDIO_RENDERING_START:\n");
                break;
            case FFP_MSG_VIDEO_ROTATION_CHANGED:
                MPTRACE("FFP_MSG_VIDEO_ROTATION_CHANGED: %d\n", msg.arg1);
                break;
            case FFP_MSG_AUDIO_DECODED_START:
                MPTRACE("FFP_MSG_AUDIO_DECODED_START:\n");
                break;
            case FFP_MSG_VIDEO_DECODED_START:
                MPTRACE("FFP_MSG_VIDEO_DECODED_START:\n");
                break;
            case FFP_MSG_OPEN_INPUT:
                MPTRACE("FFP_MSG_OPEN_INPUT:\n");
                break;
            case FFP_MSG_FIND_STREAM_INFO:
                MPTRACE("FFP_MSG_FIND_STREAM_INFO:\n");
                break;
            case FFP_MSG_COMPONENT_OPEN:
                MPTRACE("FFP_MSG_COMPONENT_OPEN:\n");
                break;
            case FFP_MSG_BUFFERING_START:
                MPTRACE("FFP_MSG_BUFFERING_START:\n");
                break;
            case FFP_MSG_BUFFERING_END:
                MPTRACE("FFP_MSG_BUFFERING_END:\n");
                break;
            case FFP_MSG_BUFFERING_UPDATE:
                // MPTRACE("FFP_MSG_BUFFERING_UPDATE: %d, %d", msg.arg1, msg.arg2);
                break;
            case FFP_MSG_BUFFERING_BYTES_UPDATE:
                break;
            case FFP_MSG_BUFFERING_TIME_UPDATE:
                break;
            case FFP_MSG_SEEK_COMPLETE:
                MPTRACE("FFP_MSG_SEEK_COMPLETE:\n");
                break;
            case FFP_MSG_ACCURATE_SEEK_COMPLETE:
                MPTRACE("FFP_MSG_ACCURATE_SEEK_COMPLETE:\n");
                break;
            case FFP_MSG_PLAYBACK_STATE_CHANGED:
                break;
            case FFP_MSG_TIMED_TEXT:
                break;
            case FFP_MSG_GET_IMG_STATE:
                break;
            case FFP_MSG_VIDEO_SEEK_RENDERING_START:
                MPTRACE("FFP_MSG_VIDEO_SEEK_RENDERING_START:\n");
                break;
            case FFP_MSG_AUDIO_SEEK_RENDERING_START:
                MPTRACE("FFP_MSG_AUDIO_SEEK_RENDERING_START:\n");
                break;
            default:
                ALOGE("unknown FFP_MSG_xxx(%d)\n", msg.what);
                break;
        }
        msg_free_res(&msg);
    }
    return 0;
}

HDLPlayer::HDLPlayer() {
}

void HDLPlayer::init(const std::string &url) {
    ijkmp_global_init();
    ijkmp_global_set_inject_callback(ijkff_inject_callback);

    _mediaPlayer = ijkmp_cc_create(media_player_msg_loop);

    //FIXME
    //    ijkmp_set_weak_thiz(_mediaPlayer, (__bridge_retained void *) self);
    //    ijkmp_set_inject_opaque(_mediaPlayer, (__bridge_retained void *) weakHolder);
    //    ijkmp_set_ijkio_inject_opaque(_mediaPlayer, (__bridge_retained void *)weakHolder);
    //    ijkmp_set_option_int(_mediaPlayer, IJKMP_OPT_CATEGORY_PLAYER, "start-on-prepared", _shouldAutoplay ? 1 : 0);

    ijkmp_set_weak_thiz(_mediaPlayer, this);
    ijkmp_set_inject_opaque(_mediaPlayer, this);
    ijkmp_set_ijkio_inject_opaque(_mediaPlayer, this);
    ijkmp_set_option_int(_mediaPlayer, IJKMP_OPT_CATEGORY_PLAYER, "start-on-prepared", 0);

    //    ijkmp_set_option(_mediaPlayer, IJKMP_OPT_CATEGORY_PLAYER, "overlay-format", "fcc-_es2");
    ijkmp_set_option(_mediaPlayer, IJKMP_OPT_CATEGORY_PLAYER, "overlay-format", "fcc-rv32");

    ijkmp_set_data_source(_mediaPlayer, url.c_str());
    ijkmp_set_option(_mediaPlayer, IJKMP_OPT_CATEGORY_FORMAT, "safe", "0"); // for concat demuxer

    ijkmp_prepare_async(_mediaPlayer);
}

void HDLPlayer::play() {
    ijkmp_start(_mediaPlayer);
}

ImageInfo *HDLPlayer::takeImageInfo() {
    bool ok = true;
    ijkmp_cc_takeImageInfo(_mediaPlayer, _imageInfo, ok);
    if (ok) return &_imageInfo;

    return nullptr;
}

HDLPlayer::~HDLPlayer() {
}
