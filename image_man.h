/********************************************************************** <BR>
   This file is part of Crack dot Com's free source code release of
   Golgotha. <a href="http://www.crack.com/golgotha_release"> <BR> for
   information about compiling & licensing issues visit this URL</a>
   <PRE> If that doesn't help, contact Jonathan Clark at
   golgotha_source@usa.net (Subject should have "GOLG" in it)
 ***********************************************************************/

#ifndef G1_IMAGE_MAN_HH
#define G1_IMAGE_MAN_HH

#include "g1_limits.h"

class i4_image_class;

class g1_image_ref
{
public:
	static g1_image_ref * first;
	g1_image_ref * next;
	//i4_const_str image_name;//don't wan't to use this, because this
	//class is heavily used before the memory manager is available
	char image_name[256];
	i4_image_class * im;

	virtual void cleanup();
	virtual void load();

	g1_image_ref(const char * filename);
	virtual ~g1_image_ref();

	// this will delete the old image (if present)
	// and load up a new one
	void set_image(const char * filename);
	i4_image_class *get()
	{
		return im;
	}
};

class g1_team_icon_ref :
	public g1_image_ref
{
public:
	void load();

	i4_image_class * tinted_icons[G1_MAX_PLAYERS];
	g1_team_icon_ref(const char * filename);
	~g1_team_icon_ref();
	void cleanup();
};


void g1_load_images();
void g1_unload_images();

#endif
