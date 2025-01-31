#include "pch.h"

/********************************************************************** <BR>
   This file is part of Crack dot Com's free source code release of
   Golgotha. <a href="http://www.crack.com/golgotha_release"> <BR> for
   information about compiling & licensing issues visit this URL</a>
   <PRE> If that doesn't help, contact Jonathan Clark at
   golgotha_source@usa.net (Subject should have "GOLG" in it)
 ***********************************************************************/

/*
 * Mpeg Layer-1,2,3 audio decoder
 * ------------------------------
 * copyright (c) 1995,1996,1997 by Michael Hipp, All rights reserved.
 * See also 'README'
 *
 * slighlty optimized for machines without autoincrement/decrement.
 * The performance is highly compiler dependend. Maybe
 * the decode.c version for 'normal' processor may be faster
 * even for Intel processors.
 */

#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "loaders/mp3/mpg123.h"

#define WRITE_SAMPLE(samples,sum,clip)                             \
	if( (sum) > 32767.0) { *(samples) = 0x7fff; (clip)++; }        \
	else if( (sum) < -32768.0) { *(samples) = -0x8000; (clip)++; } \
	else { *(samples) = sum; }

static int bo = 1;

void init_decode_i386(void)
{
	bo = 1;
}

int synth_1to1_8bit(real * bandPtr,int channel,unsigned char * samples)
{
	short samples_tmp[64];
	short * tmp1 = samples_tmp + channel;
	int i,ret;

	samples += channel;
	ret = synth_1to1(bandPtr,channel,(unsigned char *)samples_tmp);

	for(i=0; i<32; i++)
	{
		*samples = conv16to8[*tmp1>>4];
		samples += 2;
		tmp1 += 2;
	}

	return ret;
}

int synth_1to1_8bit_mono(real * bandPtr,unsigned char * samples)
{
	short samples_tmp[64];
	short * tmp1 = samples_tmp;
	int i,ret;

	ret = synth_1to1(bandPtr,0,(unsigned char *)samples_tmp);

	for(i=0; i<32; i++)
	{
		*samples++ = conv16to8[*tmp1>>4];
		tmp1+=2;
	}

	return ret;
}

int synth_1to1_8bit_mono2stereo(real * bandPtr,unsigned char * samples)
{
	short samples_tmp[64];
	short * tmp1 = samples_tmp;
	int i,ret;

	ret = synth_1to1(bandPtr,0,(unsigned char *)samples_tmp);

	for(i=0; i<32; i++)
	{
		*samples++ = conv16to8[*tmp1>>4];
		*samples++ = conv16to8[*tmp1>>4];
		tmp1 += 2;
	}

	return ret;
}

int synth_1to1_mono(real * bandPtr,unsigned char * samples)
{
	short samples_tmp[64];
	short * tmp1 = samples_tmp;

	int i,ret;

	ret = synth_1to1(bandPtr,0,(unsigned char *) samples_tmp);

	for(i=0; i<32; i++)
	{
		*( (short *) samples) = *tmp1; //Korrekt?
		*((short *)samples)=(*((short *)samples))+1;
		tmp1 += 2;
	}
	return ret;
}


int synth_1to1_mono2stereo(real * bandPtr,unsigned char * samples)
{
	int i,ret = synth_1to1(bandPtr,0,samples);

	for(i=0; i<32; i++)
	{
		((short *)samples)[1] = ((short *)samples)[0];
		samples+=4;
	}
	return ret;
}

#ifndef PENTIUM_OPT
int synth_1to1(real * bandPtr,int channel,unsigned char * out)
{
	static real buffs[2][2][0x110];
	static const int step = 2;
	short * samples = (short *) out;

	real * b0,(* buf)[0x110];
	int clip = 0;
	int bo1;

	if(flags.equalizer)
	{
		do_equalizer(bandPtr,channel);
	}

	if(!channel)
	{
		bo--;
		bo &= 0xf;
		buf = buffs[0];
	}
	else
	{
		samples++;
		buf = buffs[1];
	}

	if(bo & 0x1)
	{
		b0 = buf[0];
		bo1 = bo;
		dct64(buf[1]+((bo+1)&0xf),buf[0]+bo,bandPtr);
	}
	else
	{
		b0 = buf[1];
		bo1 = bo+1;
		dct64(buf[0]+bo,buf[1]+bo+1,bandPtr);
	}

	{
		register int j;
		real * window = decwin + 16 - bo1;

		for (j=16; j; j--,b0+=0x10,window+=0x20,samples+=step)
		{
			real sum;
			sum  = window[0x0] * b0[0x0];
			sum -= window[0x1] * b0[0x1];
			sum += window[0x2] * b0[0x2];
			sum -= window[0x3] * b0[0x3];
			sum += window[0x4] * b0[0x4];
			sum -= window[0x5] * b0[0x5];
			sum += window[0x6] * b0[0x6];
			sum -= window[0x7] * b0[0x7];
			sum += window[0x8] * b0[0x8];
			sum -= window[0x9] * b0[0x9];
			sum += window[0xA] * b0[0xA];
			sum -= window[0xB] * b0[0xB];
			sum += window[0xC] * b0[0xC];
			sum -= window[0xD] * b0[0xD];
			sum += window[0xE] * b0[0xE];
			sum -= window[0xF] * b0[0xF];

			WRITE_SAMPLE(samples,sum,clip);
		}

		{
			real sum;
			sum  = window[0x0] * b0[0x0];
			sum += window[0x2] * b0[0x2];
			sum += window[0x4] * b0[0x4];
			sum += window[0x6] * b0[0x6];
			sum += window[0x8] * b0[0x8];
			sum += window[0xA] * b0[0xA];
			sum += window[0xC] * b0[0xC];
			sum += window[0xE] * b0[0xE];
			WRITE_SAMPLE(samples,sum,clip);
			b0-=0x10,window-=0x20,samples+=step;
		}
		window += bo1<<1;

		for (j=15; j; j--,b0-=0x10,window-=0x20,samples+=step)
		{
			real sum;
			sum = -window[-0x1] * b0[0x0];
			sum -= window[-0x2] * b0[0x1];
			sum -= window[-0x3] * b0[0x2];
			sum -= window[-0x4] * b0[0x3];
			sum -= window[-0x5] * b0[0x4];
			sum -= window[-0x6] * b0[0x5];
			sum -= window[-0x7] * b0[0x6];
			sum -= window[-0x8] * b0[0x7];
			sum -= window[-0x9] * b0[0x8];
			sum -= window[-0xA] * b0[0x9];
			sum -= window[-0xB] * b0[0xA];
			sum -= window[-0xC] * b0[0xB];
			sum -= window[-0xD] * b0[0xC];
			sum -= window[-0xE] * b0[0xD];
			sum -= window[-0xF] * b0[0xE];
			sum -= window[-0x0] * b0[0xF];

			WRITE_SAMPLE(samples,sum,clip);
		}
	}
	return clip;
}
#endif
