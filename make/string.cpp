/********************************************************************** <BR>
   This file is part of Crack dot Com's free source code release of
   Golgotha. <a href="http://www.crack.com/golgotha_release"> <BR> for
   information about compiling & licensing issues visit this URL</a>
   <PRE> If that doesn't help, contact Jonathan Clark at
   golgotha_source@usa.net (Subject should have "GOLG" in it)
 ***********************************************************************/

#include "string.h"

char *form(char * form, ... )
{
	va_list args;
	char buf[1024];

	va_start(args,form);
	vsprintf(buf,form,args);
	va_end(args);

	return strdup(buf);
}
