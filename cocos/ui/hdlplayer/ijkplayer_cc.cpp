#include "ijkplayer_cc.hpp"

extern "C"
{
#include "ijkmedia/ijkplayer/ijkplayer_internal.h"
#include "ffpipeline_cc.h"
}

IjkMediaPlayer *ijkmp_cc_create(int (*msg_loop)(void*))
{
    IjkMediaPlayer *mp = ijkmp_create(msg_loop);
    if (!mp)
        goto fail;

    mp->ffplayer->vout = SDL_VoutCC_CreateForImageInfo();
    if (!mp->ffplayer->vout)
        goto fail;

    mp->ffplayer->pipeline = ffpipeline_create_from_cc(mp->ffplayer);
    if (!mp->ffplayer->pipeline)
        goto fail;

    return mp;

fail:
    ijkmp_dec_ref_p(&mp);
    return NULL;
}

void ijkmp_cc_takeImageInfo_l(IjkMediaPlayer *mp, ImageInfo &info, bool &ok)
{
    assert(mp);
    assert(mp->ffplayer);
    assert(mp->ffplayer->vout);

    SDL_VoutCC_TakeImageInfo(mp->ffplayer->vout, info, ok);
}

void ijkmp_cc_takeImageInfo(IjkMediaPlayer *mp, ImageInfo &info, bool &ok)
{
    assert(mp);
    pthread_mutex_lock(&mp->mutex);
    ijkmp_cc_takeImageInfo_l(mp, info, ok);
    pthread_mutex_unlock(&mp->mutex);
}
