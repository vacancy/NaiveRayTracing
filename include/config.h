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

const int SAMPLE_X = 2;
const int SAMPLE_Y = 2;
const int SAMPLE = 250;
const int SAMPLE_XY = SAMPLE_X * SAMPLE_Y;
const int SAMPLE_ALL = SAMPLE*SAMPLE_XY;

const int h = 512, w = 512;
const int MAX_DEPTH = 100;
const int SCENE_ID = 3;

#endif
