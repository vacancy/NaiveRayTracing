/**
 * File   : config.h
 * Author : Jiayuan Mao
 * Email  : mjy14@mails.tsinghua.edu.cn
 * Date   : 2015-12-23 23:29:19
 * This file is part of the school project RayTracing of course
 * ``Advanced Computational Geometry''.
 **/

#ifndef _INCLUDE_CONFIG_H_
#define _INCLUDE_CONFIG_H_

const int SAMPLE_X = 1;
const int SAMPLE_Y = 1;
const int SAMPLE = 25;
const int SAMPLE_XY = SAMPLE_X * SAMPLE_Y;
const int SAMPLE_ALL = SAMPLE * SAMPLE_XY;

const int PHOTON_SAMPLE = 1000000;
const int PHOTON_GLOBAL  = 10000000;
const int PHOTON_CAUSTIC = 0;

const int    PHOTON_GLOBAL_N = 64;
const double PHOTON_GLOBAL_R = 32.0;
const int    PHOTON_CAUSTIC_N = 64;
const double PHOTON_CAUSTIC_R = 0.5;
const double PHOTON_GLOBAL_MUL = 1.0;
const double PHOTON_CAUSTIC_MUL = 1.0;

const int h = 512, w = 512;
const int MAX_DEPTH = 32;
const int SCENE_ID = 4;
const int DEBUG = false;

#endif
