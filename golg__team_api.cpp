/********************************************************************** <BR>
   This file is part of Crack dot Com's free source code release of
   Golgotha. <a href="http://www.crack.com/golgotha_release"> <BR> for
   information about compiling & licensing issues visit this URL</a>
   <PRE> If that doesn't help, contact Jonathan Clark at
   golgotha_source@usa.net (Subject should have "GOLG" in it)
 ***********************************************************************/
#include "pch.h"
#include "team_api.h"
#include "player.h"
#include "map.h"
#include "objs/stank.h"
#include "objs/path_object.h"
#include "resources.h"
#include "map_man.h"
#include "lisp/lisp.h"
#include "objs/bases.h"
#include "li_objref.h"
#include "playback.h"
#include "file/file.h"
#include "tick_count.h"

#include "statistics.h"
#include "controller.h"
#include "time/time.h"

void read_playback_checker(i4_file_class * fp)
{
	int x=fp->read_32();

	if (x!=(int)g1_tick_counter)
	{
		i4_warning("demo check off is");
	}
}

void write_playback_checker(i4_file_class * fp)
{
	fp->write_32(g1_tick_counter);
}

g1_team_api_class::g1_team_api_class()
	: record(0),
	  playback(0),
	  name(0),
	  user_fire_at(0,0,0)
//{{{
{
	reload=0;
	lastnewpos=300;
	user_accel= user_angle= user_straferight=0;
	user_strafeup=0;
	user_lookx= user_looky=0;
	user_fire1= user_fire2= user_fire3=i4_F;
}
//}}}

g1_player_piece_class * g1_team_api_class::commander() const
//{{{
{
	return player->get_commander();
}
//}}}

g1_map_piece_class * g1_team_api_class::controlled() const
{
	if (!g1_current_controller.get())
	{
		return 0;
	}
	g1_map_piece_class * obj=
		g1_map_piece_class::cast(
		g1_global_id.checked_get(g1_current_controller->view.get_following_object()));
	if (obj==commander())
	{
		return 0;
	}
	return obj;
}

sw16 g1_team_api_class::health() const
//{{{
{
	if (!commander())
	{
		return 0;
	}

	return commander()->health;
}
//}}}

w16 g1_team_api_class::ammo0() const
//{{{
{
	if (!commander())
	{
		return 0;
	}

	return commander()->ammo[0].amount;
}
//}}}

w16 g1_team_api_class::ammo1() const
//{{{
{
	if (!commander())
	{
		return 0;
	}

	return commander()->ammo[1].amount;
}
//}}}

w16 g1_team_api_class::ammo2() const
//{{{
{
	if (!commander())
	{
		return 0;
	}

	return commander()->ammo[2].amount;
}
//}}}

i4_bool g1_team_api_class::full0() const
//{{{
{
	g1_player_piece_class * stank=player->get_commander();

	if (!stank ||stank->ammo[0].ammo_type)
	{
		return 0;
	}
	else
	{
		return stank->ammo[0].amount==stank->ammo[0].ammo_type->max_amount;
	}
}
//}}}

i4_bool g1_team_api_class::full1() const
//{{{
{
	g1_player_piece_class * stank=player->get_commander();

	if (!stank ||stank->ammo[1].ammo_type)
	{
		return 0;
	}
	else
	{
		return stank->ammo[1].amount==stank->ammo[1].ammo_type->max_amount;
	}
}
//}}}

i4_bool g1_team_api_class::full2() const
//{{{
{
	g1_player_piece_class * stank=player->get_commander();

	if (!stank ||stank->ammo[2].ammo_type)
	{
		return 0;
	}
	else
	{
		return stank->ammo[2].amount==stank->ammo[2].ammo_type->max_amount;
	}
}
//}}}

i4_bool g1_team_api_class::in_range0() const
//{{{
{
	g1_player_piece_class * stank=player->get_commander();

	if (!stank)
	{
		return i4_F;
	}
	return stank->in_range(0, stank->attack_target.get());
}
//}}}

i4_bool g1_team_api_class::in_range1() const
//{{{
{
	g1_player_piece_class * stank=player->get_commander();

	if (!stank)
	{
		return i4_F;
	}
	return stank->in_range(1, stank->attack_target.get());
}
//}}}

i4_bool g1_team_api_class::in_range2() const
//{{{
{
	g1_player_piece_class * stank=player->get_commander();

	if (!stank)
	{
		return i4_F;
	}
	return stank->in_range(2, stank->attack_target.get());
}
//}}}

void g1_team_api_class::turn(i4_float angle, g1_map_piece_class * forwho)
//{{{
{
	if (record)
	{
		write_playback_checker(record);
		record->write_8(G1_COMMAND_TURN);
		record->write_float(angle);
	}

	i4_float max = g1_resources.player_turn_speed;
	if (!forwho&&commander())
	{
		commander()->dtheta = (angle>max) ? max : (angle<-max) ? -max : angle;
	}
	if (forwho)
	{
		if (forwho->get_flag(g1_object_class::SELECTABLE)&&angle!=0)
		{
			forwho->unlink();
			user_angle=angle;
		}
	}

}
//}}}

void g1_team_api_class::accelerate(i4_float ratio,g1_map_piece_class * forwho)
//{{{
{
	if (record)
	{
		write_playback_checker(record);
		record->write_8(G1_COMMAND_ACCEL);
		record->write_float(ratio);
	}

	if (!forwho&&commander())
	{
		commander()->accel_ratio = (ratio>1.0f) ? 1.0f : (ratio<-1.0f) ? -1.0f : ratio;
	}
	if (forwho)
	{
		if (forwho->get_flag(g1_object_class::SELECTABLE)&&ratio!=0)
		{
			forwho->unlink();
			user_accel=ratio;
		}
	}
}
//}}}

void g1_team_api_class::strafe(i4_float strafe_right,
							   i4_float strafe_up,
							   g1_map_piece_class * forwho)
//{{{
{
	if (record)
	{
		write_playback_checker(record);
		record->write_8(G1_COMMAND_STRAFE);
		record->write_float(strafe_right);
		record->write_float(strafe_up);
	}

	if (!forwho&&commander())
	{
		commander()->strafe_accel_ratio = (strafe_right>1.0f) ? 1.0f : (strafe_right<-1.0f) ? -1.0f : strafe_right;
	}
	if (forwho)
	{
		if (forwho->get_flag(g1_object_class::SELECTABLE)&&
			(strafe_right!=0||strafe_up!=0))
		{
			forwho->unlink();
			user_straferight=strafe_right;
			user_strafeup=strafe_up;
		}
	}
}
//}}}

void g1_team_api_class::look(i4_float dax, i4_float day,g1_map_piece_class * forwho)
//{{{
{
	if (record)
	{
		write_playback_checker(record);
		record->write_8(G1_COMMAND_LOOK);
		record->write_float(dax);
		record->write_float(day);
	}

	if (!forwho&&commander())
	{
		commander()->mouse_look_increment_x = dax;
		commander()->mouse_look_increment_y = day;
	}

	if (forwho)
	{
		if (forwho->get_flag(g1_object_class::SELECTABLE))
		{
			//forwho->unlink();
			user_lookx=dax;
			user_looky=day;
		}
	}

}
//}}}

i4_bool g1_team_api_class::fire0(g1_map_piece_class * forwho)
//{{{
{
	if (record)
	{
		write_playback_checker(record);
		record->write_8(G1_COMMAND_FIRE0);
	}


	if (!forwho)
	{
		if (ammo0()==0)
		{
			return i4_F;
		}

		commander()->fire[0] = i4_T;
	}
	else
	{
		user_fire1=i4_T;
	}
	return i4_T;
}
//}}}

i4_bool g1_team_api_class::continue_game()
{
	//if (g1_player_man.get(team())->continue_wait)
	//{
	if (record)
	{
		write_playback_checker(record);
		record->write_8(G1_COMMAND_CONTINUE);
	}

	//g1_player_man.get(team())->continue_wait=i4_F;

	//}

	return i4_T;
}


i4_bool g1_team_api_class::fire1(g1_map_piece_class * forwho)
//{{{
{
	if (forwho)
	{
		return i4_F;
	}
	if (record)
	{
		write_playback_checker(record);
		record->write_8(G1_COMMAND_FIRE1);
	}

	if (ammo1()==0)
	{
		return i4_F;
	}

	commander()->fire[1] = i4_T;
	return i4_T;
}
//}}}


i4_bool g1_team_api_class::fire2(g1_map_piece_class * forwho)
//{{{
{
	if (forwho)
	{
		return i4_F;
	}
	if (record)
	{
		write_playback_checker(record);
		record->write_8(G1_COMMAND_FIRE2);
	}

	if (ammo2()==0)
	{
		return i4_F;
	}

	commander()->fire[2] = i4_T;
	return i4_T;
}
//}}}

g1_player_type g1_team_api_class::team() const
//{{{
{
	return player->player_num;
}
//}}}

sw32 g1_team_api_class::money() const
//{{{
{
	return player->money();
}
//}}}

g1_map_class * g1_team_api_class::map() const
//{{{
{
	//(OLI) should get rid of this one sometime... add real map interface
	return g1_get_map();
}
//}}}

w32 g1_team_api_class::map_height() const
//{{{
{
	return g1_get_map()->height();
}
//}}}

w32 g1_team_api_class::map_width() const
//{{{
{
	return g1_get_map()->width();
}
//}}}

i4_float g1_team_api_class::terrain_height(i4_float x, i4_float y, i4_float z) const
//{{{
{
	if (g1_map_is_loaded())
	{
		return g1_get_map()->map_height(x,y,z);
	}
	else
	{
		return 0;
	}
}
//}}}

i4_bool g1_team_api_class::is_building() const
//{{{
{
	return i4_F;
}
//}}}

w16 g1_team_api_class::object_type(const char * name) const
//{{{
{
	return g1_get_object_type(name);
}
//}}}

i4_bool g1_team_api_class::deploy_unit(w32 id, i4_float x, i4_float y)
// send convoy to location
//{{{
{
	if (record)
	{
		write_playback_checker(record);
		record->write_8(G1_COMMAND_DEPLOY);
		record->write_32(id);
		record->write_float(x);
		record->write_float(y);
	}

	i4_bool ret=0;
	;

	if (g1_global_id.check_id(id))
	{
		g1_object_class * obj=g1_global_id.get(id);

		if (obj->player_num==team())
		{
			ret = obj->deploy_to(x,y,0);
		}
	}

	return ret;
}
//}}}

w32 g1_path_time_counter=0;

i4_bool g1_team_api_class::set_current_target(w32 global_id)
// select target for convoy/object
//{{{
{
	if (record)
	{
		write_playback_checker(record);
		record->write_8(G1_COMMAND_TARGET);
		record->write_32(global_id);
	}

	g1_path_object_class * target=g1_path_object_class::cast(g1_global_id.get(global_id));

	if (!target)
	{
		return i4_F;
	}

	int player_num=team();

	for (g1_factory_class * f=g1_factory_list.first(); f; f=f->next)
	{
		if (f->player_num==player_num)
		{
			g1_path_object_class * o=f->get_start();
			if (o)
			{
				g1_path_object_class * path[2000];
				int t=o->find_path(player->get_team(), target, path, 2000);
				for (int j=0; j<t; j++)
				{
					path[j]->last_selected_tick[player->get_team()]=g1_path_time_counter+t-j;
				}

				g1_path_time_counter+=t;
			}
		}
	}
	return i4_T;
}
//}}}

int g1_team_api_class::build_unit(g1_object_type type)
{
	if (record)
	{
		write_playback_checker(record);
		record->write_8(G1_COMMAND_BUILD);
		record->write_16(type);
	}
	int ret=G1_BUILD_INVALID_OBJECT;
	int player_num=team();
	for (g1_factory_class * f=g1_factory_list.first(); f; f=f->next)
	{
		if (f->player_num==player_num)
		{
			if (f->build(type))
			{
				return G1_BUILD_OK;
			}
		}
	}

	return ret;
}

int g1_team_api_class::build_unit(g1_object_type type, g1_object_class * factory)
{
	g1_factory_class * f=g1_factory_class::cast(factory);

	if (f==NULL)
		return G1_BUILD_INVALID_OBJECT;


	if (record)
	{
		write_playback_checker(record);
		record->write_8(G1_COMMAND_BUILD_AT_FACTORY);
		record->write_16(type);
		//Since the global id may have changed uppon load,
		//the only thing we can really do is lookup based on the factory position.
		//(Actually, record and playback isn't tested and most probably doesn't work anyway)
		record->write_32(factory->global_id);
		record->write_float(factory->x);
		record->write_float(factory->y);
	}
	int ret=G1_BUILD_INVALID_OBJECT;
	if (factory!=NULL)
		ret=factory->build(type);


	return ret;
}

i4_bool g1_team_api_class::record_start(char * name)
{
	record = i4_open(name, I4_WRITE);
	return (record!=0);
}

void g1_team_api_class::record_end()
{
	if (record)
	{
		delete record;
	}
	record = 0;
}




static int demo_start_time=i4_F;
static i4_time_class demo_start;

i4_bool g1_team_api_class::playback_start(i4_file_class * fp)
{

	playback = fp;
	demo_start_time = i4_T;
	g1_stat_counter.set_value(g1_statistics_counter_class::FRAMES,0);
	g1_stat_counter.set_value(g1_statistics_counter_class::AISTEPS,0);

	return (playback!=0);
}

void g1_team_api_class::playback_end()
{
	if (playback)
	{
		delete playback;
	}
	playback = 0;

	if (g1_current_controller.get())
	{
		i4_time_class now;

		//char buff[512];
		sw32
		frames = g1_stat_counter.get_value(g1_statistics_counter_class::FRAMES);
		i4_float
		time = i4_float(now.milli_diff(demo_start))/1000.0f,
		fps = i4_float(frames)/time;

		//sprintf(buff, "Demo Frames: %d  Time: %7.3f FPS: %5.2f", frames, time, fps);
		//g1_current_controller->scroll_message(buff, 0xff00ff);
	}
}


i4_bool g1_team_api_class::playback_think()
{
	if (!playback)
	{
		return i4_F;
	}

	if (demo_start_time)
	{
		demo_start_time = i4_F;
		demo_start.get();
	}

	if (playback->eof())
	{
		playback_end();
		return i4_F;
	}

	w8 com = 0;
	do
	{
		read_playback_checker(playback);
		com = playback->read_8();
		//Todo: Fix playback for other units than stank.
		//But playback is currently not working anyway, so ignore this
		//for the moment
		switch (com)
		{
			case G1_COMMAND_TURN:
				{
					i4_float angle = playback->read_float();
					turn(angle,0);
				} break;
			case G1_COMMAND_ACCEL:
				{
					i4_float ratio = playback->read_float();
					accelerate(ratio,0);
				} break;
			case G1_COMMAND_STRAFE:
				{
					i4_float ratio = playback->read_float();
					strafe(ratio,0,0);
				} break;
			case G1_COMMAND_LOOK:
				{
					i4_float dx,dy;
					dx = playback->read_float();
					dy = playback->read_float();
					look(dx,dy,0);
				} break;
			case G1_COMMAND_DEPLOY:
				{
					w32
					id = playback->read_32();
					i4_float px,py;
					px = playback->read_float();
					py = playback->read_float();
					deploy_unit(id,px,py);
				} break;
			case G1_COMMAND_BUILD:
				{
					w16 t = playback->read_16();
					build_unit(t);
				} break;
			case G1_COMMAND_TARGET:
				{
					w32 id = playback->read_32();
					set_current_target(id);
				} break;
			case G1_COMMAND_FIRE0:
				fire0(0);
				break;
			case G1_COMMAND_FIRE1:
				fire1(0);
				break;
			case G1_COMMAND_FIRE2:
				fire2(0);
				break;
			case G1_COMMAND_END:
				break;
			default:
				i4_warning("Invalid playback code received!");
				com = G1_COMMAND_END;
				break;
		}
	}
	while (com!=G1_COMMAND_END);

	return i4_T;
}

void g1_team_api_class::post_think()
{
	if (record)
	{
		write_playback_checker(record);
		record->write_8(G1_COMMAND_END);
	}
}

g1_team_api_class::~g1_team_api_class()
{
	record_end();
	playback_end();
	if (name)
	{
		delete name;
	}
	name=0;
}


//  Team API definition class

g1_team_api_definition_class * g1_team_api_definition_class::first=0;

g1_team_api_definition_class::g1_team_api_definition_class(const char * name)
	: _name(name)
//{{{
{
	next = first;
	first = this;
}
//}}}

g1_team_api_definition_class::~g1_team_api_definition_class()
//{{{
{
	g1_team_api_definition_class * p = first;
	g1_team_api_definition_class * * ref = &first;

	while (p && p!=this)
	{
		ref = &p->next;
		p = p->next;
	}

	if (p)
	{
		*ref = p->next;
	}

	g1_player_man.unload_ai(this);
}
//}}}

g1_team_api_class * g1_team_api_definition_class::create(const char * name,
														 g1_loader_class * f)
//{{{
{
	g1_team_api_definition_class * p=first;

	while (p && strcmp(name, p->name()))
		p = p->next;



	if (!p)
	{
		i4_error("ERROR: No ai called '%s' installed. Using default.",name);
		p=first;
	}

	g1_team_api_class * r=p->create(f);
	if (r->name)
	{
		delete r->name;
	}                         //if we just created the human api, it already has a name
	r->name=new i4_str(name); //because we want copy-semantics!
	//the name is lost next time we access it.
	return r;
}
//}}}

//{{{ Emacs Locals
// Local Variables:
// folded-file: t
// End:
//}}}
