#include "pch.h"

/********************************************************************** <BR>
   This file is part of Crack dot Com's free source code release of
   Golgotha. <a href="http://www.crack.com/golgotha_release"> <BR> for
   information about compiling & licensing issues visit this URL</a>
   <PRE> If that doesn't help, contact Jonathan Clark at
   golgotha_source@usa.net (Subject should have "GOLG" in it)
 ***********************************************************************/

#include "sound_man.h"
#include "objs/model_id.h"
#include "objs/model_draw.h"
#include "objs/rocktank.h"
#include "objs/base_launcher.h"
#include "input.h"
#include "math/pi.h"
#include "math/angle.h"
#include "math/trig.h"
#include "objs/buster_rocket.h"
#include "resources.h"
#include "saver.h"
#include "object_definer.h"
#include "objs/fire.h"
#include "lisp/lisp.h"

static g1_object_type stank;

void g1_base_launcher_init()
{
	//buster = g1_get_object_type("buster_rocket");
	stank = g1_get_object_type("stank");
}

g1_object_definer<g1_base_launcher_class>
g1_base_launcher_def("base_launcher",
					 g1_object_definition_class::EDITOR_SELECTABLE,
					 g1_base_launcher_init);


g1_base_launcher_class::g1_base_launcher_class(g1_object_type id,
											   g1_loader_class * fp)
	: g1_map_piece_class(id,fp)
{
	defaults = g1_base_launcher_def.defaults;

	draw_params.setup("rocket_truck_top","rocket_truck_shadow");
	first_time=i4_F;
	w16 ver,data_size;
	if (fp)
	{
		fp->get_version(ver,data_size);
		if (ver==DATA_VERSION)
		{
			fp->end_version(I4_LF);
		}
		else
		{
			fp->seek(fp->tell() + data_size);
		}
	}
	else
	{
		first_time=i4_T;
	}

	fire_delay = defaults->fire_delay;
	health = 1000;

	//no sound for now
	radar_type=G1_RADAR_WEAPON;
	set_flag(AERIAL        |
			 HIT_GROUND    |
			 SHADOWED      |
			 DANGEROUS, 1);
}

i4_bool g1_base_launcher_class::can_attack(g1_object_class * who)
{
//bad: was attacking ANY stank.
	return (who->id == stank && who->player_num!=player_num);
}

void g1_base_launcher_class::save(g1_saver_class * fp)
{
	g1_map_piece_class::save(fp);

	fp->start_version(DATA_VERSION);
	fp->end_version();
}


void g1_base_launcher_class::fire()
{
	if (attack_target.valid())
	{
		i4_transform_class o2w;
		calc_world_transform(1,&o2w);

		i4_3d_vector local_fire_point;
		i4_3d_vector world_fire_point, world_fire_dir;
		o2w.transform(i4_3d_vector(0.4f, 0, 0.2f), world_fire_point);
		o2w.transform(i4_3d_vector(1,0,0.2f), world_fire_dir);
		world_fire_dir-=world_fire_point;
		fire_delay=defaults->fire_delay;
		//g1_fire(li_get_symbol("buster_rocket"), this, attack_target.get(),
		//                      world_fire_point, world_fire_dir);
		g1_fire(defaults->fire_type,this,attack_target.get(),
				world_fire_point,world_fire_dir);
	}
}

i4_bool g1_base_launcher_class::occupy_location()
{
	if (first_time)
	{
		h += 0.9f; //this alone is dangerous: moving the object in
		//the editor would increase its height beyond limit without
		//the user actually dragging anything.
		lh = h;
	}

	first_time=i4_F;


	return g1_map_piece_class::occupy_location();
}



void g1_base_launcher_class::think()
{
	find_target();

	// can't hurt me.... I'm invincible! (no more)
	//health = 1000;

	pitch = 0; //groundpitch;//*cos(theta) - groundroll *sin(theta);
	roll  = 0; //groundroll;// *cos(theta) + groundpitch*sin(theta);

	if (fire_delay)
	{
		fire_delay--;
	}
	request_think();
	//aim the turet
	if (attack_target.valid())
	{


		i4_float dx,dy,angle,dangle;

		//this will obviously only be true if attack_target.ref != NULL
		dx = (attack_target->x - x);
		dy = (attack_target->y - y);

		//aim the turet

		angle = i4_atan2(dy,dx);

		//snap it
		i4_normalize_angle(angle);

		dangle = i4_rotate_to(theta,angle,defaults->turn_speed);
		if (dangle<defaults->turn_speed && dangle>-defaults->turn_speed)
		{
			if (!fire_delay)
			{
				fire();
			}
		}
	}
}
