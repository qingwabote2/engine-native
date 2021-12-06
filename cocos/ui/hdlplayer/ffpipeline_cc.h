#pragma once

#include "ijkplayer/ff_ffpipeline.h"

struct FFPlayer;

IJKFF_Pipeline *ffpipeline_create_from_cc(struct FFPlayer *ffp);
