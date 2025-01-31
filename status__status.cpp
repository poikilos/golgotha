/********************************************************************** <BR>
   This file is part of Crack dot Com's free source code release of
   Golgotha. <a href="http://www.crack.com/golgotha_release"> <BR> for
   information about compiling & licensing issues visit this URL</a>
   <PRE> If that doesn't help, contact Jonathan Clark at
   golgotha_source@usa.net (Subject should have "GOLG" in it)
 ***********************************************************************/
#include "pch.h"
#include "status/status.h"

#include "device/kernel.h"

#include "math/num_type.h"

#include "memory/array.h"

#include "string/string.h"

#include "time/time.h"

#include "window/window.h"
#include "window/style.h"
#include "window/wmanager.h"
#include "window/win_evt.h"

#include "gui/button.h"
#include "gui/text.h"
#include "gui/deco_win.h"


#ifdef _WINDOWS
#include <windows.h>
#include <commctrl.h>
#include "main/win_main.h"
#endif


i4_status_create_function_type i4_stat_fun=0;
void i4_set_status_create_function(i4_status_create_function_type fun)
{
	i4_stat_fun=fun;
}

i4_idle_class * i4_idle_class::first=0;


i4_idle_class::i4_idle_class()
{
	next=first;
	first=this;
}

i4_idle_class::~i4_idle_class()
{
	if (first==this)
	{
		first=first->next;
	}
	else
	{
		i4_idle_class * p=first;
		for (; p->next && p->next!=this; p=p->next)
		{
			;
		}
		p->next=next;
	}
}



#ifdef _WINDOWS
enum {
	MAX_UPDATES=50
};

static int stat_y=40;

class win32_status_class :
	public i4_status_class
{
public:
	HWND hwndPB;  // handle of progress bar
	i4_time_class start_time;
	int last_p;
	i4_bool win_created;
	char * description;

	virtual i4_bool update(float percent)
	{
		i4_time_class now;

		//if (now.milli_diff(start_time)>500)//show bar only if an operation takes more than half a second
		//{//this code is only used for the initialization-window. So no problem if always used.
		start_time.get();

		if (!win_created)
		{
			//InitCommonControls();

			//char buf[100];
			//i4_os_string(*description, buf, 100);

			hwndPB = CreateWindowEx(WS_EX_TOPMOST, PROGRESS_CLASS, description,
									WS_BORDER | WS_VISIBLE, 50, stat_y,
									500, 40,
									0, (HMENU) 0, i4_win32_instance, NULL);


			// Set the range and increment of the progress bar.
			SendMessage(hwndPB, PBM_SETRANGE, 0, MAKELPARAM(0, MAX_UPDATES+1));
			SendMessage(hwndPB, PBM_SETSTEP, (WPARAM) 1, 0);
			win_created=i4_T;
		}
		//}

		if (win_created)
		{
			int p=(int)(percent*MAX_UPDATES);
			BringWindowToTop(hwndPB); //Be shure we are seen
			RedrawWindow(hwndPB,NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW);
			while (last_p<=p)
			{
				last_p++;
				SendMessage(hwndPB, PBM_STEPIT, 0, 0);
			}
		}

		return i4_T;
	}

	i4_bool update(const i4_const_str &newtext, float percent)
	{
		char buf[300];

		i4_os_string(newtext,buf,300);
		free(description);
		description=(char *)malloc(newtext.ascii_length()+2);
		i4_os_string(newtext,description,newtext.ascii_length()+1);
		SendMessage(hwndPB,WM_SETTEXT,0,(LPARAM)&buf);
		return update(percent);
	};

	win32_status_class(const i4_const_str &d)
	{
		//description=new i4_const_str(d);
		if(d.ascii_length()<=1) //there's a problem with the resources
		{
			MessageBox(0,"The exe file doesn't contain valid resource information. Check your make process.",
					   "FATAL ERROR: CANNOT CONTINUE",MB_OK|MB_SYSTEMMODAL|MB_ICONSTOP);
			i4_current_app->fatalexit(82);
		}
		description=(char *)malloc(d.ascii_length()+2);
		i4_os_string(d,description,d.ascii_length()+1);
		win_created=i4_F;
		last_p=0;
		stat_y+=60;
	}

	virtual ~win32_status_class()
	{
		//delete description;
		free(description);

		if (win_created)
		{
			while (last_p<=MAX_UPDATES)
			{
				last_p++;
				SendMessage(hwndPB, PBM_STEPIT, 0, 0);
			}

			DestroyWindow(hwndPB);
		}

		stat_y-=60;

	}
};



// this is operating system dependant
i4_status_class *i4_create_status(const i4_const_str &description, int flags)
{
	if (i4_stat_fun)
	{
		return i4_stat_fun(description, flags);
	}
	else
	{
		return new win32_status_class(description);
	}
}


#else


class i4_null_status_class :
	public i4_status_class
{
public:
	i4_bool update(float percent)
	{
		for (i4_idle_class * p=i4_idle_class::first; p; p=p->next)
		{
			p->idle();
		}
		int iper=(int)(percent*100);
		if (iper<100)
		{
			printf("Initialisation in progress. %i percent complete.\n",iper);
		}
		else
		{
			printf("Initialisation completed.\n");
		}
		return i4_T;
	}
	i4_bool update(const i4_const_str &newname, float percent)
	{
		return update(percent);
	}
};


i4_status_class *i4_create_status(const i4_const_str &description, int flags)
{
	if (i4_stat_fun)
	{
		return i4_stat_fun(description, flags);
	}
	else
	{
		return new i4_null_status_class;
	}
}



#endif

// GUI_STAT.CPP
/********************************************************************** <BR>
   This file is part of Crack dot Com's free source code release of
   Golgotha. <a href="http://www.crack.com/golgotha_release"> <BR> for
   information about compiling & licensing issues visit this URL</a>
   <PRE> If that doesn't help, contact Jonathan Clark at
   golgotha_source@usa.net (Subject should have "GOLG" in it)
 ***********************************************************************/


class i4_gui_stat_handle :
	public i4_status_class
{
	int id;
public:
	i4_gui_stat_handle(int id, int flags) :
		id(id)
	{
	}
	virtual i4_bool update(float percent);
	virtual i4_bool update(const i4_const_str &newtext, float percent);
	virtual ~i4_gui_stat_handle();
};

class i4_gui_stat_window :
	public i4_parent_window_class
{
	i4_graphical_style_class * style;
	i4_str * show_str;
	int lx2;
	i4_bool canceled;
	int dir, flags;

public:
	i4_float percent;

	i4_gui_stat_window(float percent,
					   w16 w, w16 h,
					   i4_graphical_style_class * style,
					   const i4_const_str &str,
					   int flags) :
		i4_parent_window_class(w,h),
		style(style),
		percent(percent),
		flags(flags)
	{
		show_str=new i4_str(str);
		lx2=0;
		canceled=0;
		dir=1;


		if (flags & I4_STATUS_ALLOW_CANCEL)
		{
			i4_button_class * b;
			b=new i4_button_class(0, new i4_text_window_class(i4gets("cancel_operation"), style),
								  style, new i4_event_reaction_class(this, 1));

			resize(width() < b->width() ? b->width() : width(), height() + b->height() + 6);

			add_child(width()/2-b->width()/2, height()-b->height()-3, b);
		}
	}

	void receive_event(i4_event * ev)
	{
		if (ev->type()==i4_event::USER_MESSAGE)
		{
			canceled=i4_T;
		}
		else
		{
			i4_parent_window_class::receive_event(ev);
		}
	}

	void parent_draw(i4_draw_context_class &context)
	{

		int x1=2, x2=width()-3;
		int y=10;
		int bx2=(sw32)(x1+(x2-x1+1)*percent);

		if (undrawn_area.empty() && bx2==lx2)
		{
			return;
		}
		lx2=bx2;

		i4_font_class * f=style->font_hint->normal_font;
		int fh=f->height(*show_str);

		if (!undrawn_area.empty())
		{
			local_image->clear(style->color_hint->neutral(), context);


			f->set_color(style->color_hint->text_foreground);

			int fw=f->width(*show_str);

			f->put_string(local_image, width()/2-fw/2, y, *show_str, context);
		}


		y+=fh+10;

		if (!undrawn_area.empty())
		{
			local_image->bar(x1-1, y-1, x2+1, y+6+1, 0, context);
			local_image->bar(x1, y, x2, y+6, style->color_hint->window.active.dark, context);
		}

		x1++;
		x2--;
		local_image->bar(x1, y, bx2, y+6,
						 style->color_hint->window.active.bright,
						 context);
	}

	i4_bool update_text(const i4_const_str &newtext)
	{
		delete show_str;
		show_str=new i4_str(newtext);
		request_redraw(i4_T);
		return i4_T;
	}

	i4_bool update_percent(float p)
	{
		if (flags & I4_STATUS_UNKNOWN_TOTAL)
		{
			if (dir>0)
			{
				percent+=0.05f;
				if (percent>1)
				{
					percent=1;
					dir=-1;
				}
			}
			else
			{
				percent-=0.05f;
				if (percent<0)
				{
					percent=0;
					dir=1;
				}
			}
		}
		else
		{
			percent=p;
		}

		request_redraw();
		return !canceled;
	}

	~i4_gui_stat_window()
	{
		delete show_str;
	}

	void name(char * buffer)
	{
		static_name(buffer,"gui_stat_window");
	}
};

int show_blts=0;

class i4_gui_status_creator_class :
	public i4_event_handler_class
{
	i4_window_manager_class * wm;
	i4_display_class * display;

	struct stat
	{
		i4_gui_stat_window * win;
		i4_bool allocated;
		i4_time_class start_time;
		i4_str * desc;
		int flags;
	};

	i4_array<stat> wins;
	i4_time_class last_update_time;
	sw32 grab_id;

	int get_window_y(i4_window_class * w)
	{
		int y= wm->height() - w->height();

		for (int i=0; i<wins.size(); i++)
		{
			if (wins[i].allocated && wins[i].win)
			{
				y-=wins[i].win->height();
			}
		}

		return y;
	}


public:
	void name(char * buffer)
	{
		static_name(buffer,"gui_status");
	}
	i4_gui_status_creator_class(i4_window_manager_class * wm,
								i4_display_class * display)
		: wins(2,10),
		  wm(wm),
		  display(display)
	{
		grab_id=-1;
	}



	i4_status_class *create(const i4_const_str &description, int flags)
	{
		int id=0;

		for (id=0; id<wins.size() && wins[id].allocated; id++)
		{
			;
		}
		if (id==wins.size())
		{
			wins.add();
		}


		stat st;
		st.allocated=i4_T;
		st.desc=new i4_str(description);
		st.win=0;
		st.flags=flags;

		wins[id]=st;


		return new i4_gui_stat_handle(id, flags);
	}


	void ungrab_id(int id)
	{
		if (grab_id==id)
		{
			i4_parent_window_class * p=wins[id].win->get_parent();

			i4_window_request_mouse_ungrab_class ungrab(wins[id].win);
			i4_kernel.send_event(p, &ungrab);

			grab_id=-1;
		}
	}

	void grab_new_id()
	{
		for (int i=0; i<wins.size(); i++)
		{
			if (wins[i].allocated && wins[i].win)
			{
				grab_id=i;
				i4_window_request_mouse_grab_class grab(wins[i].win);
				i4_kernel.send_event(wins[i].win->get_parent(), &grab);
				if (!grab.return_result)
				{
					grab_id=-1;
				}
			}
		}


	}

	void receive_event(i4_event * ev)
	{
		if (ev->type()==i4_event::USER_MESSAGE)
		{
			CAST_PTR(uev, i4_user_message_event_class, ev);

			int id=uev->sub_type;
			if (id!=-1)
			{
				ungrab_id(id);
			}

			wins[id].win=0;

			grab_new_id();

		}
	}


	void delete_stat(int id)
	{
		wins[id].allocated=i4_F;
		delete wins[id].desc;
		//is a wm->root_draw() allowed here?
		if (wins[id].win)
		{
			if (id!=-1)
			{
				ungrab_id(id);
			}

			i4_kernel.delete_handler(wins[id].win->get_parent());
			wins[id].win=0;

			grab_new_id();
		}

	}

	i4_bool update(int id, const i4_const_str &newtext, float percent)
	{
		wins[id].win->update_text(newtext);
		last_update_time.add_milli(-1000); //we have been updated ages ago
		return update(id,percent);
	}

	i4_bool update(int id, float percent)
	{
		I4_ASSERT(wins[id].allocated, "id not allocated");

		i4_time_class now;
		i4_bool ret=i4_T;

		if (wins[id].win)
		{
			if (grab_id==-1)
			{
				grab_new_id();
			}

			if ((wins[id].flags & I4_STATUS_UNKNOWN_TOTAL) ||
				percent-wins[id].win->percent>0.01)
			{
				ret=wins[id].win->update_percent(percent);
			}

		}
		else if (now.milli_diff(wins[id].start_time)>500 || percent==1.0)
		{
			i4_graphical_style_class * s=wm->get_style();
			i4_font_class * f=s->font_hint->normal_font;

			int fw=f->width(*wins[id].desc)+4;
			if (fw<200)
			{
				fw=200;
			}
			int fh=f->height(*wins[id].desc) + 30;

			i4_deco_window_class * w;
//       i4_user_message_event_class *uev=new i4_user_message_event_class(id);
//       i4_event_reaction_class *re=new i4_event_reaction_class(this, uev);

			i4_gui_stat_window * gs=new i4_gui_stat_window(percent, fw, fh, s, *wins[id].desc,
														   wins[id].flags);


			w=new i4_deco_window_class(gs->width(), gs->height(), i4_F, s);
			w->add_child((short)w->get_x1(), (short)w->get_y1(), gs);

			int yp=get_window_y(w);
			wm->add_child(wm->width()/2-w->width()/2, yp, w);

			wins[id].win=gs;

			if (grab_id==-1)
			{
				grab_id=id;
				i4_window_request_mouse_grab_class grab(gs);
				i4_kernel.send_event(w, &grab);
				if (!grab.return_result)
				{
					grab_id=-1;
				}
			}

		}


		if (now.milli_diff(last_update_time)>100||percent>=0.99) // only refresh 10 fps so we don't slow down process
		{
			show_blts=1;

			if (!display->display_busy())
			{
				wm->root_draw();
			}
			show_blts=0;

			i4_kernel.process_events();

			last_update_time.get();
		}

		return ret;
	}

};


i4_gui_status_creator_class * i4_gui_status_creator=0;


i4_bool i4_gui_stat_handle::update(float percent)
{
	for (i4_idle_class * p=i4_idle_class::first; p; p=p->next)
	{
		p->idle();
	}

	return i4_gui_status_creator->update(id, percent);
}

i4_bool i4_gui_stat_handle::update(const i4_const_str &newtext, float percent)
{
	return update(percent);
}

i4_gui_stat_handle::~i4_gui_stat_handle()
{
	i4_gui_status_creator->delete_stat(id);

}



i4_status_class *i4_create_gui_stat(const i4_const_str &st, int flags)
{
	return i4_gui_status_creator->create(st, flags);
}

void i4_init_gui_status(i4_window_manager_class * wm,
						i4_display_class * display)
{
	if (i4_gui_status_creator)
	{
		i4_error("4_gui_status creator already created");
	}

	i4_gui_status_creator=new i4_gui_status_creator_class(wm, display);
	i4_set_status_create_function(i4_create_gui_stat);
}


void i4_set_status_create_function(i4_status_create_function_type fun);


void i4_uninit_gui_status()
{
	if (i4_gui_status_creator)
	{
		delete i4_gui_status_creator;
		i4_gui_status_creator=0;
	}
}
