#include "ijksdl_vout_cc_imageinfo.hpp"
#include <functional>

extern "C" {
#include "ijksdl/ffmpeg/ijksdl_vout_overlay_ffmpeg.h"
#include "ijksdl/ijksdl_vout_internal.h"
#include "ijksdl/ios/ijksdl_vout_overlay_videotoolbox.h"
}

struct SDL_Vout_Opaque {
    SDL_VoutOverlay *overlay;
};

static SDL_VoutOverlay *vout_create_overlay_l(int width, int height, int frame_format, SDL_Vout *vout) {
    switch (frame_format) {
        case IJK_AV_PIX_FMT__VIDEO_TOOLBOX:
            return SDL_VoutVideoToolBox_CreateOverlay(width, height, vout);
        default:
            return SDL_VoutFFmpeg_CreateOverlay(width, height, frame_format, vout);
    }
}

static SDL_VoutOverlay *vout_create_overlay(int width, int height, int frame_format, SDL_Vout *vout) {
    SDL_LockMutex(vout->mutex);
    SDL_VoutOverlay *overlay = vout_create_overlay_l(width, height, frame_format, vout);
    SDL_UnlockMutex(vout->mutex);
    return overlay;
}

static void vout_free_l(SDL_Vout *vout) {
    if (!vout)
        return;

    SDL_Vout_Opaque *opaque = vout->opaque;
    if (opaque) {
        //        if (opaque->gl_view) {
        //            // TODO: post to MainThread?
        //            [opaque->gl_view release];
        //            opaque->gl_view = nil;
        //        }
    }

    SDL_Vout_FreeInternal(vout);
}

static int vout_display_overlay_l(SDL_Vout *vout, SDL_VoutOverlay *overlay) {
    SDL_Vout_Opaque *opaque = vout->opaque;
    opaque->overlay         = overlay;
    //    const auto w = overlay->w;
    //    const auto h = overlay->h;
    //    const auto pixels = overlay->pixels;
    //    opaque->updateTexture = [w, h, pixels](cocos2d::Texture2D *texture){
    //        texture->initWithData(pixels[0], w * h * 4, cocos2d::Texture2D::PixelFormat::RGBA8888, w, h, cocos2d::Size(w, h));
    //    };

    return 0;
}

static int vout_display_overlay(SDL_Vout *vout, SDL_VoutOverlay *overlay) {
    SDL_LockMutex(vout->mutex);
    int retval = vout_display_overlay_l(vout, overlay);
    SDL_UnlockMutex(vout->mutex);
    return retval;
}

SDL_Vout *SDL_VoutCC_CreateForImageInfo() {
    SDL_Vout *vout = SDL_Vout_CreateInternal(sizeof(SDL_Vout_Opaque));
    if (!vout)
        return NULL;

    SDL_Vout_Opaque *opaque = vout->opaque;
    opaque->overlay         = nullptr;
    vout->create_overlay    = vout_create_overlay;
    vout->free_l            = vout_free_l;
    vout->display_overlay   = vout_display_overlay;

    return vout;
}

static void SDL_VoutCC_TakeImageInfo_l(SDL_Vout *vout, ImageInfo &info, bool &ok) {
    SDL_Vout_Opaque *opaque  = vout->opaque;
    SDL_VoutOverlay *overlay = opaque->overlay;
    if (!overlay) {
        ok = false;
        return;
    }
    
    info.width = overlay->w;
    info.height = overlay->h;
    info.data = overlay->pixels[0];

    opaque->overlay = nullptr;
}

void SDL_VoutCC_TakeImageInfo(SDL_Vout *vout, ImageInfo &info, bool &ok)
{
    SDL_LockMutex(vout->mutex);
    SDL_VoutCC_TakeImageInfo_l(vout, info, ok);
    SDL_UnlockMutex(vout->mutex);
}
