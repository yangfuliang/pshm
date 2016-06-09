dnl $Id$
dnl config.m4 for extension pshm

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(pshm, for pshm support,
dnl Make sure that the comment is aligned:
dnl [  --with-pshm             Include pshm support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(pshm, whether to enable pshm support,
dnl Make sure that the comment is aligned:
[  --enable-pshm           Enable pshm support])

if test "$PHP_PSHM" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-pshm -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/pshm.h"  # you most likely want to change this
  dnl if test -r $PHP_PSHM/$SEARCH_FOR; then # path given as parameter
  dnl   PSHM_DIR=$PHP_PSHM
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for pshm files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       PSHM_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$PSHM_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the pshm distribution])
  dnl fi

  dnl # --with-pshm -> add include path
  dnl PHP_ADD_INCLUDE($PSHM_DIR/include)

  dnl # --with-pshm -> check for lib and symbol presence
  dnl LIBNAME=pshm # you may want to change this
  dnl LIBSYMBOL=pshm # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $PSHM_DIR/$PHP_LIBDIR, PSHM_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_PSHMLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong pshm lib version or lib not found])
  dnl ],[
  dnl   -L$PSHM_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(PSHM_SHARED_LIBADD)

  PHP_NEW_EXTENSION(pshm, pshm.c, $ext_shared)
fi
