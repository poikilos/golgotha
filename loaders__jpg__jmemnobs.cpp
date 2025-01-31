/********************************************************************** <BR>
   This file is part of Crack dot Com's free source code release of
   Golgotha. <a href="http://www.crack.com/golgotha_release"> <BR> for
   information about compiling & licensing issues visit this URL</a>
   <PRE> If that doesn't help, contact Jonathan Clark at
   golgotha_source@usa.net (Subject should have "GOLG" in it)
 ***********************************************************************/

/*
 * jmemnobs.c
 *
 * Copyright (C) 1992-1996, Thomas G. Lane.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file provides a really simple implementation of the system-
 * dependent portion of the JPEG memory manager.  This implementation
 * assumes that no backing-store files are needed: all required space
 * can be obtained from malloc().
 * This is very portable in the sense that it'll compile on almost anything,
 * but you'd better have lots of main memory (or virtual memory) if you want
 * to process big images.
 * Note that the max_memory_to_use option is ignored by this implementation.
 */
#include "pch.h"
#define JPEG_INTERNALS
#include "loaders/jpg/jinclude.h"
#include "loaders/jpg/jpeglib.h"
#include "loaders/jpg/jmemsys.h"     /* import the system-dependent declarations */
#include "memory/malloc.h"

//#ifndef HAVE_STDLIB_H		/* <stdlib.h> should declare malloc(),free() */
//extern void * malloc JPP((size_t size));
//extern void free JPP((void *ptr));
//#endif


/*
 * Memory allocation and freeing are controlled by the regular library
 * routines malloc() and free().
 */

GLOBAL(void *)
jpeg_get_small(j_common_ptr cinfo, size_t sizeofobject)
{
	return (void *) I4_MALLOC(sizeofobject, "jpeg_get_small");
}

GLOBAL(void)
jpeg_free_small(j_common_ptr cinfo, void * object, size_t sizeofobject)
{
	i4_free(object);
}


/*
 * "Large" objects are treated the same as "small" ones.
 * NB: although we include FAR keywords in the routine declarations,
 * this file won't actually work in 80x86 small/medium model; at least,
 * you probably won't be able to process useful-size images in only 64KB.
 */

GLOBAL(void FAR *)
jpeg_get_large(j_common_ptr cinfo, size_t sizeofobject)
{
	return (void FAR *) I4_MALLOC(sizeofobject, "jpeg_get_large");
}

GLOBAL(void)
jpeg_free_large(j_common_ptr cinfo, void FAR * object, size_t sizeofobject)
{
	i4_free(object);
}


/*
 * This routine computes the total memory space available for allocation.
 * Here we always say, "we got all you want bud!"
 */

GLOBAL(long)
jpeg_mem_available(j_common_ptr cinfo, long min_bytes_needed,
				   long max_bytes_needed, long already_allocated)
{
	return max_bytes_needed;
}


/*
 * Backing store (temporary file) management.
 * Since jpeg_mem_available always promised the moon,
 * this should never be called and we can just error out.
 */

GLOBAL(void)
jpeg_open_backing_store(j_common_ptr cinfo, backing_store_ptr info,
						long total_bytes_needed)
{
	ERREXIT(cinfo, JERR_NO_BACKING_STORE);
}


/*
 * These routines take care of any system-dependent initialization and
 * cleanup required.  Here, there isn't any.
 */

GLOBAL(long)
jpeg_mem_init(j_common_ptr cinfo)
{
	return 0;       /* just set max_memory_to_use to 0 */
}

GLOBAL(void)
jpeg_mem_term(j_common_ptr cinfo)
{
	/* no work */
}
