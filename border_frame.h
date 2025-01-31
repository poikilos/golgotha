/********************************************************************** <BR>
   This file is part of Crack dot Com's free source code release of
   Golgotha. <a href="http://www.crack.com/golgotha_release"> <BR> for
   information about compiling & licensing issues visit this URL</a>
   <PRE> If that doesn't help, contact Jonathan Clark at
   golgotha_source@usa.net (Subject should have "GOLG" in it)
 ***********************************************************************/

#ifndef BORDER_FRAME_HH
#define BORDER_FRAME_HH


#include "window/window.h"
#include "string/string.h"
#include "memory/array.h"
#include "window/colorwin.h"

class i4_button_class;
class i4_image_class;
class i4_graphical_style_class;
class g1_amount_display_class;

// this window is responsible for drawing the border area around the game window's
// it also loads the board frame from disk when need.  My hope is that during the normal
// game it can free up the memory associated with the border.
class g1_border_frame_class :
	public i4_color_window_class
{
public:


private:

	enum
	{
		OPTIONS,
		BUILD_FIRST, BUILD_LAST=BUILD_FIRST+0x10000,
		ACTIVE_FIRST, ACTIVE_LAST=ACTIVE_FIRST+0x10000
	};

	// left justify draw number with commas if needed


	enum {
		REFRESH_FRAME=1,
		REFRESH_ALL=REFRESH_FRAME*2-1
	};

	/*
	   struct last_struct
	   {
	   int money, ammo[4], frame_upgrade_level;
	   int lives;
	   last_struct()
	   	  {
	   	  money=0;
	   	  ammo[0]=0;
	   	  ammo[1]=0;
	   	  ammo[2]=0;
	   	  ammo[3]=0;
	   	  frame_upgrade_level=0;
	   	  lives=0;
	   	  }
	   } last;
	 */

	int refresh;
	void reparent(i4_image_class * draw_area, i4_parent_window_class * parent);

public:
	int border_x();
	int border_y();

	void relocate(i4_parent_window_class * w, char * loc, int x, int y);

	void resize_controller(int shrink_add);

	int shrink;            // how many pixels the 3d view is shrinked in for speed-up
	i4_str frame_name;
	i4_image_class * frame;

	i4_bool mouse_grabbed;

	i4_window_class * strategy_window;
	i4_window_class * controller_window;
	i4_window_class * radar_window;
	enum
	{
		LIVES,
		MONEY,
		MAINGUN,
		MISSILES,
		CHAIN,
		HEALTH,
		MAX_AMOUNT_WINDOWS
	};
	g1_amount_display_class * amount_windows[MAX_AMOUNT_WINDOWS];

	void set_strategy_on_top(i4_bool v);
	i4_bool strategy_on_top;

	void update();     // check for changes in the game

	g1_border_frame_class();

	virtual void parent_draw(i4_draw_context_class &context);
	virtual void draw(i4_draw_context_class &context);

	virtual void receive_event(i4_event * ev);
	virtual void resize(w16 w,w16 h);
	~g1_border_frame_class();
	void name(char * buffer)
	{
		static_name(buffer,"border_frame");
	}

};


class g1_strategy_screen_class :
	public i4_color_window_class
{
	enum {
		OPTIONS, BUILD=1, ACTIVE=10000
	};
	i4_array<i4_button_class *> build_buttons;

public:
	int border_x();
	int border_y();
	int shrink;            // how many pixels the 3d view is shrinked in for speed-up
	i4_image_class * frame;
	i4_window_class * strategy_window;
	void name(char * buffer)
	{
		static_name(buffer,"strategy screen");
	}
	void create_build_buttons();

	virtual void parent_draw(i4_draw_context_class &context);
	g1_strategy_screen_class();
	virtual ~g1_strategy_screen_class();
	void receive_event(i4_event * ev);
	void resize(w16 new_width, w16 new_height);
};

class g1_amount_display_class :
	public i4_window_class
{
public:
	int last_amount, last_max;
	i4_image_class * im, * dark_im;
	i4_bool own_im,own_dark;
	i4_bool refresh_as_text;

	g1_amount_display_class(int w, int h)
		: i4_window_class(w,h)
	{
		refresh_as_text=i4_T;
		im=dark_im=0;
		own_im=own_dark=i4_F;
		last_amount=0;
		last_max=1;
	}

	g1_amount_display_class(char * image_name=0);

	void update(int new_amount, int max_amount);

	void draw(i4_draw_context_class &context);

	void change_icon(i4_image_class * new_im,i4_image_class * new_dark);

	~g1_amount_display_class()
	{
		if (im&&own_im)
		{
			delete im;
		}
		if (dark_im&&own_dark)
		{
			delete dark_im;
		}
	}

	void name(char * buffer)
	{
		static_name(buffer,"amount_display");
	}
};


extern i4_event_handler_reference_class<g1_border_frame_class> g1_border;
extern i4_event_handler_reference_class<g1_strategy_screen_class> g1_strategy_screen;

w32 g1_get_upgrade_color(int upgrade_level);  // used for font colors & border edges


class g1_help_screen_class :
	public i4_parent_window_class
{
	i4_image_class * help;
	w32 mess_id_to_send;
public:
	void name(char * buffer)
	{
		static_name(buffer,"help screen");
	}

	g1_help_screen_class(w16 w, w16 h,
						 i4_graphical_style_class * style,
						 const i4_const_str &image_name,
						 w32 mess_id_to_send);

	virtual void parent_draw(i4_draw_context_class &context);
	void receive_event(i4_event * ev);
	~g1_help_screen_class();
};

class g1_win_screen_class :
	public g1_help_screen_class
{
private:
	i4_str fname;
public:
	void name(char * buffer)
	{
		static_name(buffer,"win screen");
	}
	g1_win_screen_class(w16 w, w16 h,
						i4_graphical_style_class * style,
						const i4_const_str &image_name,
						i4_str * filename);
	virtual void parent_draw(i4_draw_context_class &context);
	void receive_event(i4_event * ev);
	~g1_win_screen_class();
};

#endif
