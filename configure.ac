#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.64])
AC_INIT(MutatrisHD, 1.5, jared@lostsidedead.com)

AC_CONFIG_HEADERS(config.h)
AC_CANONICAL_SYSTEM

AM_INIT_AUTOMAKE
# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_LANG_CPLUSPLUS
# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

dnl KDE_FIND_PATH(programm-name, variable-name, list of directories,
dnl     if-not-found, test-parameter)
AC_DEFUN(KDE_FIND_PATH,
[
AC_MSG_CHECKING([for $1])
if test -n "$$2"; then
kde_cv_path="$$2";
else
kde_cache=`echo $1 | sed 'y%./+-%__p_%'`

AC_CACHE_VAL(kde_cv_path_$kde_cache,
[
kde_cv_path="NONE"
dirs="$3"
kde_save_IFS=$IFS
IFS=':'
for dir in $PATH; do
dirs="$dirs $dir"
done
IFS=$kde_save_IFS

for dir in $dirs; do
if test -x "$dir/$1"; then
if test -n "$5"
then
evalstr="$dir/$1 $5 2>&1 "
if eval $evalstr; then
kde_cv_path="$dir/$1"
break
fi
else
kde_cv_path="$dir/$1"
break
fi
fi
done

eval "kde_cv_path_$kde_cache=$kde_cv_path"

])

eval "kde_cv_path=\"`echo '$kde_cv_path_'$kde_cache`\""

fi

if test -z "$kde_cv_path" || test "$kde_cv_path" = NONE; then
AC_MSG_RESULT(not found)
$4
else
AC_MSG_RESULT($kde_cv_path)
$2=$kde_cv_path

fi
])

KDE_FIND_PATH(sdl2-config, LIBSDL_CONFIG, [${prefix}/bin ${exec_prefix}/bin ], [
AC_MSG_ERROR([Could not find libsdl anywhere, check http://www.sdl.org])
])

if test -n "$LIBSDL_CONFIG"; then
LIBSDL_LIBS="`$LIBSDL_CONFIG --libs`"
LIBSDL_RPATH=
for args in $LIBSDL_LIBS; do
case $args in
-L*)
LIBSDL_RPATH="$LIBSDL_RPATH $args"
;;
esac
done
LIBSDL_RPATH=`echo $LIBSDL_RPATH | sed -e "s/-L/-R/g"`
LIBSDL_CFLAGS="`$LIBSDL_CONFIG --cflags` `pkg-config opencv4 acidcam --cflags`"

AC_DEFINE_UNQUOTED(HAVE_LIBSDL, 1, [Defines if your system has the LIBSDL library])
fi


LDPREFIX=${prefix}

if test  ${prefix} = "NONE"; then
LDPREFIX=/usr/local
fi

export PKG_CONFIG_PATH="${LDPREFIX}/lib/pkgconfig:$PKG_CONFIG_PATH"

LIBGLEW_GLEWLIB="`pkg-config opencv4 acidcam  glew --libs`"

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h inttypes.h limits.h stddef.h stdint.h string.h strings.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT64_T
AC_TYPE_SIZE_T
AC_TYPE_UINT64_T
AC_CHECK_TYPES([ptrdiff_t])

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([memset])


LIBGL_LIB=""


case "${host}" in
*darwin*|*apple*)
AC_DEFINE_UNQUOTED(DARWIN, 1, [darwin])
LIBGL_LIB=" -framework OpenGL"
;;

x86_64*linux*)
LIBGL_LIB=" -lGL -lGLU"
;;
esac

AC_SUBST(LIBSDL_CFLAGS)
AC_SUBST(LIBSDL_LIBS)
AC_SUBST(LIBGLEW_GLEWLIB)
AC_SUBST(LIBGL_LIB)


echo "SDL2 CFLAGS found: $LIBSDL_CFLAGS"
echo "SDL2 LDFLAGS $LIBSDL_LIBS"
echo "GLEW LDFLAGS $LIBGLEW_GLEWLIB"
echo "GL LDFLAGS: $LIBGL_LIB"



AC_OUTPUT(Makefile mutatris/Makefile)

