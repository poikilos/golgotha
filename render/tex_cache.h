/********************************************************************** <BR>
   This file is part of Crack dot Com's free source code release of
   Golgotha. <a href="http://www.crack.com/golgotha_release"> <BR> for
   information about compiling & licensing issues visit this URL</a>
   <PRE> If that doesn't help, contact Jonathan Clark at
   golgotha_source@usa.net (Subject should have "GOLG" in it)
 ***********************************************************************/

#ifndef TEX_CACHE_HH
#define TEX_CACHE_HH


#include "palette/pal.h"

class i4_file_class;

//very similar to mipheader.
//color,base_*,flags,num_mip_levels
//should match corresponding mipheader_t's

//the format of the cache file is like this:
//the header
//the entries (the size and the lowest miplevel in 16 and 32bit color modes)
//the entry lumps (a directory, basically), sorted after increasing ids
//a sw32 that should == the size of the file
//(the last sw32 is to protect against corrupt cache files)

inline w32 tex_cache_entry_disk_size()
{
	return (5*4) + (2*2) + (2*1); //5 words, 2 shorts and 2 bytes
}

class tex_cache_entry_t
{
public:
	w32 id;
	w32 lowmipoffset; //for 16bit
	w32 ext_lowmipoffset; //for 24/32bit
	w32 average_color;
	w32 last_modified;

	sw16 base_width;
	sw16 base_height;

	w8 flags;
	sw8 num_mip_levels;

	void write(i4_file_class * f);
	void read(i4_file_class * f);
};

inline sw32 tex_cache_header_disk_size()
{
	i4_pixel_format blah;

	return (blah.write(0) * 3 + 4*4);
}

class tex_cache_header_t
{
	enum {
		SQUARE_TEXTURES=1
	};


public:
	enum {
		TEX_CACHE_DATA_VERSION=2
	};
	w32 version;          //not written from here, but returned after read
	i4_pixel_format regular_format;
	i4_pixel_format chroma_format;
	i4_pixel_format alpha_format;
	w32 max_mip_dimention;
	w32 num_entries;
	w32 flags;

	//not written
	tex_cache_entry_t * entries;
	void read(i4_file_class * f);
	void write(i4_file_class * f);
};

#endif
