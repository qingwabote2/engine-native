#pragma once

extern "C"
{
#include "ijkmedia/ijksdl/ijksdl_vout.h"
}

struct ImageInfo {
    int width;
    int height;
    uint8_t *data;
};

SDL_Vout *SDL_VoutCC_CreateForImageInfo();
void SDL_VoutCC_TakeImageInfo(SDL_Vout *vout, ImageInfo &photo, bool &ok);
