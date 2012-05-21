/* Header describing internals of libintl library.
   Copyright (C) 1995-1999, 2000, 2001, 2004-2005, 2007
   Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Written by Ulrich Drepper <drepper@cygnus.com>, 1995.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

#ifndef _GETTEXTP_H
#define _GETTEXTP_H

#include <stddef.h>		/* Get size_t.  */

#ifdef _LIBC
# include "../iconv/gconv_int.h"
#else
# if HAVE_ICONV
#  include <iconv.h>
# endif
#endif

#include "loadinfo.h"

#include "gmo.h"		/* Get nls_uint32.  */

/* @@ end of prolog @@ */

#ifndef PARAMS
# if __STDC__ || defined __GNUC__ || defined __SUNPRO_C || defined __cplusplus || __PROTOTYPES
#  define PARAMS(args) args
# else
#  define PARAMS(args) ()
# endif
#endif

#ifndef internal_function
# define internal_function
#endif

#ifndef attribute_hidden
# define attribute_hidden
#endif

/* Tell the compiler when a conditional or integer expression is
   almost always true or almost always false.  */
#ifndef HAVE_BUILTIN_EXPECT
# define __builtin_expect(expr, val) (expr)
#endif

#ifndef W
# define W(flag, data) ((flag) ? SWAP (data) : (data))
#endif


#ifdef _LIBC
# include <byteswap.h>
# define SWAP(i) bswap_32 (i)
#else
static nls_uint32 SWAP PARAMS ((nls_uint32 i));

static inline nls_uint32
SWAP (i)
     nls_uint32 i;
{
  return (i << 24) | ((i & 0xff00) << 8) | ((i >> 8) & 0xff00) | (i >> 24);
}
#endif


/* In-memory representation of system dependent string.  */
struct sysdep_string_desc
{
  /* Length of addressed string, including the trailing NUL.  */
  size_t length;
  /* Pointer to addressed string.  */
  const char *pointer;
};

/* Cache of translated strings after charset conversion.
   Note: The strings are converted to the target encoding only on an as-needed
   basis.  */
struct converted_domain
{
  /* The target encoding name.  */
  const char *encoding;
  /* The descriptor for conversion from the message catalog's encoding to
     this target encoding.  */
#ifdef _LIBC
  __gconv_t conv;
#else
# if HAVE_ICONV
  iconv_t conv;
# endif
#endif
  /* The table of translated strings after charset conversion.  */
  char **conv_tab;
};

/* The representation of an opened message catalog.  */
struct loaded_domain
{
  /* Pointer to memory containing the .mo file.  */
  const char *data;
  /* 1 if the memory is mmap()ed, 0 if the memory is malloc()ed.  */
  int use_mmap;
  /* Size of mmap()ed memory.  */
  size_t mmap_size;
  /* 1 if the .mo file uses a different endianness than this machine.  */
  int must_swap;
  /* Pointer to additional malloc()ed memory.  */
  void *malloced;

  /* Number of static strings pairs.  */
  nls_uint32 nstrings;
  /* Pointer to descriptors of original strings in the file.  */
  const struct string_desc *orig_tab;
  /* Pointer to descriptors of translated strings in the file.  */
  const struct string_desc *trans_tab;

  /* Number of system dependent strings pairs.  */
  nls_uint32 n_sysdep_strings;
  /* Pointer to descriptors of original sysdep strings.  */
  const struct sysdep_string_desc *orig_sysdep_tab;
  /* Pointer to descriptors of translated sysdep strings.  */
  const struct sysdep_string_desc *trans_sysdep_tab;

  /* Size of hash table.  */
  nls_uint32 hash_size;
  /* Pointer to hash table.  */
  const nls_uint32 *hash_tab;
  /* 1 if the hash table uses a different endianness than this machine.  */
  int must_swap_hash_tab;

  /* Cache of charset conversions of the translated strings.  */
  struct converted_domain *conversions;
  size_t nconversions;
  __libc_rwlock_define (, conversions_lock);

  const struct expression *plural;
  unsigned long int nplurals;
};

/* We want to allocate a string at the end of the struct.  But ISO C
   doesn't allow zero sized arrays.  */
#ifdef __GNUC__
# define ZERO 0
#else
# define ZERO 1
#endif

/* A set of settings bound to a message domain.  Used to store settings
   from bindtextdomain() and bind_textdomain_codeset().  */
struct binding
{
  struct binding *next;
  char *dirname;
  char *codeset;
  char domainname[ZERO];
};

/* A counter which is incremented each time some previous translations
   become invalid.
   This variable is part of the external ABI of the GNU libintl.  */
extern int _nl_msg_cat_cntr;

#ifndef _LIBC
const char *_nl_locale_name PARAMS ((int category, const char *categoryname));
#endif

struct loaded_l10nfile *_nl_find_domain PARAMS ((const char *__dirname,
						 char *__locale,
						 const char *__domainname,
					      struct binding *__domainbinding))
     internal_function;
void _nl_load_domain PARAMS ((struct loaded_l10nfile *__domain,
			      struct binding *__domainbinding))
     internal_function;

char *_nl_find_msg PARAMS ((struct loaded_l10nfile *domain_file,
			    struct binding *domainbinding, const char *msgid,
			    int convert, size_t *lengthp))
     internal_function;

#ifdef _LIBC
extern char *__gettext PARAMS ((const char *__msgid));
extern char *__dgettext PARAMS ((const char *__domainname,
				 const char *__msgid));
extern char *__dcgettext PARAMS ((const char *__domainname,
				  const char *__msgid, int __category));
extern char *__ngettext PARAMS ((const char *__msgid1, const char *__msgid2,
				 unsigned long int __n));
extern char *__dngettext PARAMS ((const char *__domainname,
				  const char *__msgid1, const char *__msgid2,
				  unsigned long int n));
extern char *__dcngettext PARAMS ((const char *__domainname,
				   const char *__msgid1, const char *__msgid2,
				   unsigned long int __n, int __category));
extern char *__dcigettext PARAMS ((const char *__domainname,
				   const char *__msgid1, const char *__msgid2,
				   int __plural, unsigned long int __n,
				   int __category));
extern char *__textdomain PARAMS ((const char *__domainname));
extern char *__bindtextdomain PARAMS ((const char *__domainname,
				       const char *__dirname));
extern char *__bind_textdomain_codeset PARAMS ((const char *__domainname,
						const char *__codeset));
extern void _nl_finddomain_subfreeres PARAMS ((void)) attribute_hidden;
extern void _nl_unload_domain PARAMS ((struct loaded_domain *__domain))
     internal_function attribute_hidden;
#else
extern char *libintl_gettext PARAMS ((const char *__msgid));
extern char *libintl_dgettext PARAMS ((const char *__domainname,
				       const char *__msgid));
extern char *libintl_dcgettext PARAMS ((const char *__domainname,
					const char *__msgid, int __category));
extern char *libintl_ngettext PARAMS ((const char *__msgid1,
				       const char *__msgid2,
				       unsigned long int __n));
extern char *libintl_dngettext PARAMS ((const char *__domainname,
					const char *__msgid1,
					const char *__msgid2,
					unsigned long int __n));
extern char *libintl_dcngettext PARAMS ((const char *__domainname,
					 const char *__msgid1,
					 const char *__msgid2,
					 unsigned long int __n,
					 int __category));
extern char *libintl_dcigettext PARAMS ((const char *__domainname,
					 const char *__msgid1,
					 const char *__msgid2,
					 int __plural, unsigned long int __n,
					 int __category));
extern char *libintl_textdomain PARAMS ((const char *__domainname));
extern char *libintl_bindtextdomain PARAMS ((const char *__domainname,
					     const char *__dirname));
extern char *libintl_bind_textdomain_codeset PARAMS ((const char *__domainname,
						      const char *__codeset));
#endif

/* @@ begin of epilog @@ */

#endif /* gettextP.h  */
