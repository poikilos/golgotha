/********************************************************************** <BR>
   This file is part of Crack dot Com's free source code release of
   Golgotha. <a href="http://www.crack.com/golgotha_release"> <BR> for
   information about compiling & licensing issues visit this URL</a>
   <PRE> If that doesn't help, contact Jonathan Clark at
   golgotha_source@usa.net (Subject should have "GOLG" in it)
 ***********************************************************************/

#ifndef I4_TGA_WRITE_HH
#define I4_TGA_WRITE_HH

#include "arch.h"
class i4_image_class;
class i4_file_class;
i4_bool i4_write_tga(i4_image_class * im, i4_file_class * fp, int include_alpha=0);
i4_bool i4_write_tga(i4_image_class * im, const i4_const_str &name, i4_bool include_alpha=false);

#endif
