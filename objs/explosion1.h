/********************************************************************** <BR>
   This file is part of Crack dot Com's free source code release of
   Golgotha. <a href="http://www.crack.com/golgotha_release"> <BR> for
   information about compiling & licensing issues visit this URL</a>
   <PRE> If that doesn't help, contact Jonathan Clark at
   golgotha_source@usa.net (Subject should have "GOLG" in it)
 ***********************************************************************/

#ifndef G1_EXPLOSION1_HH
#define G1_EXPLOSION1_HH

#include "g1_object.h"

class g1_light_object_class;
class g1_particle_emitter_class;

class g1_explosion1_class :
	public g1_object_class
{
protected:
	w16 num_exp_frames, exp_frame;
	sw16 start_delay;

public:
	float zv;               // z velocity
	r1_texture_handle han;

	w16 model_id;
	void destroy_light();
	void create_light();

	g1_typed_reference_class<g1_light_object_class> light;
	g1_typed_reference_class<g1_particle_emitter_class> emitter;

	virtual i4_float occupancy_radius() const
	{
		return 0.4f;
	}

	g1_explosion1_class(g1_object_type id, g1_loader_class * fp);
	~g1_explosion1_class();

	enum
	{
		HIT_GROUND  = 1,
		HIT_OBJECT  = 2,
		MAKE_SPHERE = 128
	};

	virtual void setup(i4_float sx, i4_float sy, i4_float sh, w32 type=HIT_OBJECT);

	virtual void draw(g1_draw_context_class * context, i4_3d_vector& viewer_position);
	virtual void think();
};

class g1_inv_explosion_class :
	public g1_object_class
{
public:
	void think()
	{
		unoccupy_location();
		request_remove();
	};
	g1_inv_explosion_class(g1_object_type id,g1_loader_class * fp)
		: g1_object_class(id,fp)
	{
		//no flags
	}
	void draw(g1_draw_context_class * context)
	{
	};
	void setup()
	{
		occupy_location();
	}

	~g1_inv_explosion_class()
	{
	};
};

#endif
