dnl acinclude.m4 generated automatically by ac-archive's acinclude 0.5.60

dnl Copyright (C) 1994, 1995-8, 1999 Free Software Foundation, Inc.
dnl This file is free software; the Free Software Foundation
dnl gives unlimited permission to copy and/or distribute it,
dnl with or without modifications, as long as this notice is preserved.

dnl This program is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY, to the extent permitted by law; without
dnl even the implied warranty of MERCHANTABILITY or FITNESS FOR A
dnl PARTICULAR PURPOSE.

dnl ______ /usr/share/aclocal/guidod/ax_cflags_gcc_option.m4 ______
dnl @synopsis AX_CFLAGS_GCC_OPTION (optionflag [,[shellvar][,[A][,[NA]]])
dnl
dnl AX_CFLAGS_GCC_OPTION(-fvomit-frame) would show a message as like
dnl "checking CFLAGS for gcc -fvomit-frame ... yes" and adds
dnl the optionflag to CFLAGS if it is understood. You can override
dnl the shellvar-default of CFLAGS of course. The order of arguments
dnl stems from the explicit macros like AX_CFLAGS_WARN_ALL.
dnl
dnl The cousin AX_CXXFLAGS_GCC_OPTION would check for an option to add
dnl to CXXFLAGS - and it uses the autoconf setup for C++ instead of C
dnl (since it is possible to use different compilers for C and C++).
dnl
dnl The macro is a lot simpler than any special AX_CFLAGS_* macro (or
dnl ac_cxx_rtti.m4 macro) but allows to check for arbitrary options.
dnl However, if you use this macro in a few places, it would be great
dnl if you would make up a new function-macro and submit it to the
dnl ac-archive.
dnl
dnl   - $1 option-to-check-for : required ("-option" as non-value)
dnl   - $2 shell-variable-to-add-to : CFLAGS (or CXXFLAGS in the other case)
dnl   - $3 action-if-found : add value to shellvariable
dnl   - $4 action-if-not-found : nothing
dnl
dnl note: in earlier versions, $1-$2 were swapped. We try to detect the
dnl situation and accept a $2=~/-/ as being the old option-to-check-for.
dnl
dnl also: there are other variants that emerged from the original macro
dnl variant which did just test an option to be possibly added. However,
dnl some compilers accept an option silently, or possibly for just
dnl another option that was not intended. Therefore, we have to do a
dnl generic test for a compiler family. For gcc we check "-pedantic"
dnl being accepted which is also understood by compilers who just want
dnl to be compatible with gcc even when not being made from gcc sources.
dnl
dnl see also:
dnl       AX_CFLAGS_SUN_OPTION               AX_CFLAGS_HPUX_OPTION
dnl       AX_CFLAGS_AIX_OPTION               AX_CFLAGS_IRIX_OPTION
dnl
dnl @, tested, experimental
dnl @version %Id: ax_cflags_gcc_option.m4,v 1.2 2003/10/28 23:57:38 guidod Exp %
dnl @author Guido Draheim <guidod@gmx.de>
dnl
AC_DEFUN([AX_CFLAGS_GCC_OPTION_OLD], [dnl
AS_VAR_PUSHDEF([FLAGS],[CFLAGS])dnl
AS_VAR_PUSHDEF([VAR],[ac_cv_cflags_gcc_option_$2])dnl
AC_CACHE_CHECK([m4_ifval($1,$1,FLAGS) for gcc m4_ifval($2,$2,-option)],
VAR,[VAR="no, unknown"
 AC_LANG_SAVE
 AC_LANG_C
 ac_save_[]FLAGS="$[]FLAGS"
for ac_arg dnl
in "-pedantic  % m4_ifval($2,$2,-option)"  dnl   GCC
   #
do FLAGS="$ac_save_[]FLAGS "`echo $ac_arg | sed -e 's,%%.*,,' -e 's,%,,'`
   AC_TRY_COMPILE([],[return 0;],
   [VAR=`echo $ac_arg | sed -e 's,.*% *,,'` ; break])
done
 FLAGS="$ac_save_[]FLAGS"
 AC_LANG_RESTORE
])
case ".$VAR" in
     .ok|.ok,*) m4_ifvaln($3,$3) ;;
   .|.no|.no,*) m4_ifvaln($4,$4) ;;
   *) m4_ifvaln($3,$3,[
   if echo " $[]m4_ifval($1,$1,FLAGS) " | grep " $VAR " 2>&1 >/dev/null
   then AC_RUN_LOG([: m4_ifval($1,$1,FLAGS) does contain $VAR])
   else AC_RUN_LOG([: m4_ifval($1,$1,FLAGS)="$m4_ifval($1,$1,FLAGS) $VAR"])
                      m4_ifval($1,$1,FLAGS)="$m4_ifval($1,$1,FLAGS) $VAR"
   fi ]) ;;
esac
AS_VAR_POPDEF([VAR])dnl
AS_VAR_POPDEF([FLAGS])dnl
])


dnl the only difference - the LANG selection... and the default FLAGS

AC_DEFUN([AX_CXXFLAGS_GCC_OPTION_OLD], [dnl
AS_VAR_PUSHDEF([FLAGS],[CXXFLAGS])dnl
AS_VAR_PUSHDEF([VAR],[ac_cv_cxxflags_gcc_option_$2])dnl
AC_CACHE_CHECK([m4_ifval($1,$1,FLAGS) for gcc m4_ifval($2,$2,-option)],
VAR,[VAR="no, unknown"
 AC_LANG_SAVE
 AC_LANG_CXX
 ac_save_[]FLAGS="$[]FLAGS"
for ac_arg dnl
in "-pedantic  % m4_ifval($2,$2,-option)"  dnl   GCC
   #
do FLAGS="$ac_save_[]FLAGS "`echo $ac_arg | sed -e 's,%%.*,,' -e 's,%,,'`
   AC_TRY_COMPILE([],[return 0;],
   [VAR=`echo $ac_arg | sed -e 's,.*% *,,'` ; break])
done
 FLAGS="$ac_save_[]FLAGS"
 AC_LANG_RESTORE
])
case ".$VAR" in
     .ok|.ok,*) m4_ifvaln($3,$3) ;;
   .|.no|.no,*) m4_ifvaln($4,$4) ;;
   *) m4_ifvaln($3,$3,[
   if echo " $[]m4_ifval($1,$1,FLAGS) " | grep " $VAR " 2>&1 >/dev/null
   then AC_RUN_LOG([: m4_ifval($1,$1,FLAGS) does contain $VAR])
   else AC_RUN_LOG([: m4_ifval($1,$1,FLAGS)="$m4_ifval($1,$1,FLAGS) $VAR"])
                      m4_ifval($1,$1,FLAGS)="$m4_ifval($1,$1,FLAGS) $VAR"
   fi ]) ;;
esac
AS_VAR_POPDEF([VAR])dnl
AS_VAR_POPDEF([FLAGS])dnl
])

dnl -------------------------------------------------------------------------

AC_DEFUN([AX_CFLAGS_GCC_OPTION_NEW], [dnl
AS_VAR_PUSHDEF([FLAGS],[CFLAGS])dnl
AS_VAR_PUSHDEF([VAR],[ac_cv_cflags_gcc_option_$1])dnl
AC_CACHE_CHECK([m4_ifval($2,$2,FLAGS) for gcc m4_ifval($1,$1,-option)],
VAR,[VAR="no, unknown"
 AC_LANG_SAVE
 AC_LANG_C
 ac_save_[]FLAGS="$[]FLAGS"
for ac_arg dnl
in "-pedantic  % m4_ifval($1,$1,-option)"  dnl   GCC
   #
do FLAGS="$ac_save_[]FLAGS "`echo $ac_arg | sed -e 's,%%.*,,' -e 's,%,,'`
   AC_TRY_COMPILE([],[return 0;],
   [VAR=`echo $ac_arg | sed -e 's,.*% *,,'` ; break])
done
 FLAGS="$ac_save_[]FLAGS"
 AC_LANG_RESTORE
])
case ".$VAR" in
     .ok|.ok,*) m4_ifvaln($3,$3) ;;
   .|.no|.no,*) m4_ifvaln($4,$4) ;;
   *) m4_ifvaln($3,$3,[
   if echo " $[]m4_ifval($2,$2,FLAGS) " | grep " $VAR " 2>&1 >/dev/null
   then AC_RUN_LOG([: m4_ifval($2,$2,FLAGS) does contain $VAR])
   else AC_RUN_LOG([: m4_ifval($2,$2,FLAGS)="$m4_ifval($2,$2,FLAGS) $VAR"])
                      m4_ifval($2,$2,FLAGS)="$m4_ifval($2,$2,FLAGS) $VAR"
   fi ]) ;;
esac
AS_VAR_POPDEF([VAR])dnl
AS_VAR_POPDEF([FLAGS])dnl
])


dnl the only difference - the LANG selection... and the default FLAGS

AC_DEFUN([AX_CXXFLAGS_GCC_OPTION_NEW], [dnl
AS_VAR_PUSHDEF([FLAGS],[CXXFLAGS])dnl
AS_VAR_PUSHDEF([VAR],[ac_cv_cxxflags_gcc_option_$1])dnl
AC_CACHE_CHECK([m4_ifval($2,$2,FLAGS) for gcc m4_ifval($1,$1,-option)],
VAR,[VAR="no, unknown"
 AC_LANG_SAVE
 AC_LANG_CXX
 ac_save_[]FLAGS="$[]FLAGS"
for ac_arg dnl
in "-pedantic  % m4_ifval($1,$1,-option)"  dnl   GCC
   #
do FLAGS="$ac_save_[]FLAGS "`echo $ac_arg | sed -e 's,%%.*,,' -e 's,%,,'`
   AC_TRY_COMPILE([],[return 0;],
   [VAR=`echo $ac_arg | sed -e 's,.*% *,,'` ; break])
done
 FLAGS="$ac_save_[]FLAGS"
 AC_LANG_RESTORE
])
case ".$VAR" in
     .ok|.ok,*) m4_ifvaln($3,$3) ;;
   .|.no|.no,*) m4_ifvaln($4,$4) ;;
   *) m4_ifvaln($3,$3,[
   if echo " $[]m4_ifval($2,$2,FLAGS) " | grep " $VAR " 2>&1 >/dev/null
   then AC_RUN_LOG([: m4_ifval($2,$2,FLAGS) does contain $VAR])
   else AC_RUN_LOG([: m4_ifval($2,$2,FLAGS)="$m4_ifval($2,$2,FLAGS) $VAR"])
                      m4_ifval($2,$2,FLAGS)="$m4_ifval($2,$2,FLAGS) $VAR"
   fi ]) ;;
esac
AS_VAR_POPDEF([VAR])dnl
AS_VAR_POPDEF([FLAGS])dnl
])

AC_DEFUN([AX_CFLAGS_GCC_OPTION],[ifelse(m4_bregexp([$2],[-]),-1,
[AX_CFLAGS_GCC_OPTION_NEW($@)],[AX_CFLAGS_GCC_OPTION_OLD($@)])])

AC_DEFUN([AX_CXXFLAGS_GCC_OPTION],[ifelse(m4_bregexp([$2],[-]),-1,
[AX_CXXFLAGS_GCC_OPTION_NEW($@)],[AX_CXXFLAGS_GCC_OPTION_OLD($@)])])

dnl ______  ______

dnl ______ /usr/share/aclocal/C_Support/acx_restrict.m4 ______
dnl @synopsis ACX_C_RESTRICT
dnl
dnl This macro determines whether the C compiler supports the "restrict"
dnl keyword introduced in ANSI C99, or an equivalent.  Does nothing if
dnl the compiler accepts the keyword.  Otherwise, if the compiler supports
dnl an equivalent (like gcc's __restrict__) defines "restrict" to be that.
dnl Otherwise, defines "restrict" to be empty.
dnl
dnl @version %Id: acx_restrict.m4,v 1.1.1.1 2001/07/26 00:46:18 guidod Exp %
dnl @author Steven G. Johnson <stevenj@alum.mit.edu>

AC_DEFUN([ACX_C_RESTRICT],
[AC_CACHE_CHECK([for C restrict keyword], acx_cv_c_restrict,
[acx_cv_c_restrict=unsupported
 AC_LANG_SAVE
 AC_LANG_C
 # Try the official restrict keyword, then gcc's __restrict__, then
 # SGI's __restrict.  __restrict has slightly different semantics than
 # restrict (it's a bit stronger, in that __restrict pointers can't
 # overlap even with non __restrict pointers), but I think it should be
 # okay under the circumstances where restrict is normally used.
 for acx_kw in restrict __restrict__ __restrict; do
   AC_TRY_COMPILE([], [float * $acx_kw x;], [acx_cv_c_restrict=$acx_kw; break])
 done
 AC_LANG_RESTORE
])
 if test "$acx_cv_c_restrict" != "restrict"; then
   acx_kw="$acx_cv_c_restrict"
   if test "$acx_kw" = unsupported; then acx_kw=""; fi
   AC_DEFINE_UNQUOTED(restrict, $acx_kw, [Define to equivalent of C99 restrict keyword, or to nothing if this is not supported.  Do not define if restrict is supported directly.])
 fi
])

dnl ______ /usr/share/aclocal/guidod/patch_libtool_sys_lib_search_path_spec.m4 ______
dnl @* PATCH_LIBTOOL_SYS_LIB_SEARCH_PATH_SPEC
dnl
dnl Cross-compiling to win32 from a unix system reveals a bug - the
dnl path-separator has been set to ";" depending on the target system.
dnl However, the crossgcc search_path_spec works in a unix-environment
dnl with unix-style directories and unix-stylish path_separator. The
dnl result: the search_path_spec is a single word still containing the
dnl ":" separators.
dnl
dnl This macro fixes the situation: when we see the libtool PATH_SEP
dnl to be ":" and search_path_spec to contain ":" characters, then
dnl these are replaced with spaces to let the resulting string work
dnl as a for-loop argument in libtool scripts that resolve -no-undefined
dnl libraries.
dnl
dnl Later libtool generations have fixed the situation with using
dnl $PATH_SEPARATOR in the first place as the original path delimiter
dnl that will be scanned for and replaced into spaces.
dnl
dnl @= guidod@gmx.de
dnl @%Id: patch_libtool_sys_lib_search_path_spec.m4,v 1.3 2003/03/23 13:20:27 guidod Exp %

AC_DEFUN([PATCH_LIBTOOL_SYS_LIB_SEARCH_PATH_SPEC],
[# patch libtool to fix sys_lib_search_path (e.g. crosscompiling a win32 dll)
if test "_$PATH_SEPARATOR" = "_:" ; then
  if grep "^sys_lib_search_path_spec.*:" libtool >/dev/null ; then
AC_MSG_RESULT(patching libtool to fix sys_lib_search_path_spec)
    test -f libtool.old || (mv libtool libtool.old && cp libtool.old libtool)
    sed -e "/^sys_lib_search_path_spec/s/:/ /g" libtool >libtool.new
    (test -s libtool.new || rm libtool.new) 2>/dev/null
    test -f libtool.new && mv libtool.new libtool # not 2>/dev/null !!
    test -f libtool     || mv libtool.old libtool
  fi
fi
])
dnl ______ /usr/share/aclocal/guidod/ax_warning_default_aclocaldir.m4 ______
dnl @* AX_WARNING_DEFAULT_ACLOCALDIR [(dirvariable [,[defsetting][,[A][,[N/A]]]])]
dnl @* AX_ENABLE_DEFAULT_ACLOCALDIR [(dirvariable [,defsetting])]
dnl
dnl print a warning message if the $(datadir)/aclocal directory
dnl is not in the dirlist searched by the aclocal tool. This macro
dnl is useful if some `make install` would target $(datadir)/aclocal
dnl to install an autoconf m4 file of your project to be picked up
dnl by other projects.
dnl
dnl  default $1 dirvariable = aclocaldir
dnl  default $2 defsetting  = ${datadir}/aclocal
dnl  default $3 action = nothing to do
dnl  default $4 action = warn the user about mismatch
dnl
dnl In the _WARNING_ variant, the defsetting is not placed in 
dnl dirvariable nor is it ac_subst'ed in any way. The default 
dnl fail-action $4 is to send a warning message to the user,
dnl and the default accept-action $3 is nothing. It is expected that
dnl a Makefile is generated with aclocaldir=${datadir}/aclocal
dnl
dnl The _ENABLE_ variant however will set not only the $aclocaldir shell
dnl var of the script, but it is also AC-SUBST'ed on default - and 
dnl furthermore a configure option "--enable-default-aclocaldir" is 
dnl provided. Only if that option is set then $2 default is not set to 
dnl the canonic default in the a $prefix subpath but instead $2 default
dnl is set to the primary path where `aclocal` looks for macros. The
dnl user may also override the default on the command line.
dnl
dnl @: guidod@gmx.de
dnl @%Id: ax_warning_default_aclocaldir.m4,v 1.1 2003/10/18 02:57:12 guidod Exp %

AC_DEFUN([AX_WARNING_DEFAULT_ACLOCALDIR],[dnl
AC_REQUIRE([AX_EXPAND_PREFIX])dnl
AS_VAR_PUSHDEF([DIR],[ax_warning_default_aclocal_dir])dnl
AS_VAR_PUSHDEF([BIN],[ax_warning_default_aclocal_bin])dnl
AS_VAR_PUSHDEF([LOC],[ax_warning_default_aclocal_loc])dnl
LOC='m4_if([$2],,[${datadir}/aclocal],[$2])'
m4_ifval([$1],[test ".$[]$1" != "." && LOC="$[]$1"])
 if test ".$ACLOCAL" = "." ; then
    AC_PATH_PROG([ACLOCAL],[aclocal],[:])
 fi
 BIN="$ACLOCAL"
 test ".$BIN" = "." && BIN="aclocal" 
 DIR=`test ".$SHELL" = "." && SHELL="'sh'" ; eval "$BIN --print-ac-dir"`
 test ".$DIR" = "." && test -d "/usr/share/aclocal" && DIR="/usr/share/aclocal"
 test ".$DIR" = "." && DIR="/tmp"
DIR=`eval "echo $DIR"`  # we need to expand 
DIR=`eval "echo $DIR"`
LOC=`eval "echo $LOC"`
LOC=`eval "echo $LOC"`
LOC=`eval "echo $LOC"`
LOC=`eval "echo $LOC"`
AC_RUN_LOG([: test "$LOC" = "$DIR"])
if test "$LOC" != "$DIR" ; then
   if test -f "$DIR/dirlist" ; then
      for DIR in `cat $DIR/dirlist` $DIR ; do
          AC_RUN_LOG([: test "$LOC" = "$DIR"])
          test "$LOC" = "$DIR" && break
      done
   fi
   if test "$LOC" != "$DIR" ; then
      m4_ifval([$4],[$4],[dnl
      AC_MSG_NOTICE([warning: m4_if([$1],,[aclocaldir],[$1])=$LOC dnl
(see config.log)])
   AC_MSG_NOTICE([perhaps: make install m4_if([$1],,[aclocaldir],[$1])=$DIR])
   cat m4_ifset([AS_MESSAGE_LOG_FD],[>&AS_MESSAGE_LOG_FD],[>>config.log]) <<EOF
  aclocaldir:   the m4_if([$1],,[default aclocaldir],[$1 value]) of $LOC
  aclocaldir:   is not listed in the dirlist where aclocal will look
  aclocaldir:   for macros - you can override the install-path using
  aclocaldir:   make install aclocaldir=$DIR
  aclocaldir:   or append the directory to aclocal reconfigures later as
  aclocaldir:   aclocal -I $LOC
  aclocaldir:   when an autoconf macro is needed from that directory
EOF
   m4_ifvaln([$5],[$5])])dnl
   m4_ifvaln([$3],[else $3])dnl
   fi
fi
AS_VAR_POPDEF([LOC])dnl
AS_VAR_POPDEF([BIN])dnl
AS_VAR_POPDEF([DIR])dnl
])

AC_DEFUN([AX_ENABLE_DEFAULT_ACLOCALDIR],[dnl
AS_VAR_PUSHDEF([BIN],[ax_warning_default_aclocal_bin])dnl
AS_VAR_PUSHDEF([DIR],[ax_warning_default_aclocal_def])dnl
AS_VAR_PUSHDEF([DEF],[ax_warning_default_aclocal_def])dnl
AC_ARG_ENABLE([enable-default-aclocaldir],
[  --enable-default-aclocaldir(=PATH)   override the datadir/aclocal default])
test ".$enable_default_aclocaldir" = "." && enable_default_aclocaldir="no"
case ".$enable_default_aclocaldir" in
  .no) DIR='m4_if([$2],,[${datadir}/aclocal],[$2])' ;;
  .yes) # autodetect
 if test ".$ACLOCAL" = "." ; then
    AC_PATH_PROG([ACLOCAL],[aclocal],[:])
 fi
 BIN="$ACLOCAL"
 test ".$BIN" = "." && BIN="aclocal" 
 DIR=`test ".$SHELL" = "." && SHELL="'sh'" ; eval "$BIN --print-ac-dir"`
 test ".$DIR" = "." && test -d "/usr/share/aclocal" && DIR="/usr/share/aclocal"
 test ".$DIR" = "." && DIR="/tmp" ;;
  *) DIR="$enable_default_aclocaldir" ;;
esac
AX_WARNING_DEFAULT_ACLOCALDIR([$1],[$DEF],[$3],[$4],[$5])
m4_if([$1],,[aclocaldir],[$1])="$ax_warning_default_aclocal_dir"
AC_SUBST(m4_if([$1],,[aclocaldir],[$1]))
AS_VAR_POPDEF([DEF])dnl
AS_VAR_POPDEF([DIR])dnl
AS_VAR_POPDEF([BIN])dnl
])

dnl ______ /usr/share/aclocal/guidod/ax_cflags_warn_all.m4 ______
dnl @synopsis AX_CFLAGS_WARN_ALL [(shellvar [,default, [A/NA]])]
dnl
dnl Try to find a compiler option that enables most reasonable warnings.
dnl This macro is directly derived from VL_PROG_CC_WARNINGS which is
dnl split up into two AX_CFLAGS_WARN_ALL and AX_CFLAGS_WARN_ALL_ANSI
dnl
dnl For the GNU CC compiler it will be -Wall (and -ansi -pedantic)
dnl The result is added to the shellvar being CFLAGS by default.
dnl
dnl Currently this macro knows about GCC, Solaris C compiler,
dnl Digital Unix C compiler, C for AIX Compiler, HP-UX C compiler,
dnl IRIX C compiler, NEC SX-5 (Super-UX 10) C compiler, and Cray J90
dnl (Unicos 10.0.0.8) C compiler.
dnl
dnl - $1 shell-variable-to-add-to : CFLAGS
dnl - $2 add-value-if-not-found : nothing 
dnl - $3 action-if-found : add value to shellvariable
dnl - $4 action-if-not-found : nothing 
dnl
dnl @version %Id: ax_cflags_warn_all.m4,v 1.2 2003/10/12 14:56:07 guidod Exp %
dnl @author Guido Draheim <guidod@gmx.de>
dnl
AC_DEFUN([AX_CFLAGS_WARN_ALL],[dnl
AS_VAR_PUSHDEF([FLAGS],[CFLAGS])dnl
AS_VAR_PUSHDEF([VAR],[ac_cv_cflags_warn_all])dnl
AC_CACHE_CHECK([m4_ifval($1,$1,FLAGS) for maximum warnings],
VAR,[VAR="no, unknown"
 AC_LANG_SAVE
 AC_LANG_C
 ac_save_[]FLAGS="$[]FLAGS"
for ac_arg dnl
in "-pedantic  % -Wall"       dnl   GCC
   "-xstrconst % -v"          dnl Solaris C 
   "-std1      % -verbose -w0 -warnprotos" dnl Digital Unix 
   "-qlanglvl=ansi % -qsrcmsg -qinfo=all:noppt:noppc:noobs:nocnd" dnl AIX
   "-ansi -ansiE % -fullwarn" dnl IRIX
   "+ESlit     % +w1"         dnl HP-UX C 
   "-Xc        % -pvctl[,]fullmsg" dnl NEC SX-5 (Super-UX 10)
   "-h conform % -h msglevel 2" dnl Cray C (Unicos)
   # 
do FLAGS="$ac_save_[]FLAGS "`echo $ac_arg | sed -e 's,%%.*,,' -e 's,%,,'`
   AC_TRY_COMPILE([],[return 0;],
   [VAR=`echo $ac_arg | sed -e 's,.*% *,,'` ; break])
done
 FLAGS="$ac_save_[]FLAGS"
 AC_LANG_RESTORE
])
case ".$VAR" in
     .ok|.ok,*) m4_ifvaln($3,$3) ;;
   .|.no|.no,*) m4_ifvaln($4,$4,[m4_ifval($2,[
        AC_RUN_LOG([: m4_ifval($1,$1,FLAGS)="$m4_ifval($1,$1,FLAGS) $2"])
                      m4_ifval($1,$1,FLAGS)="$m4_ifval($1,$1,FLAGS) $2"])]) ;;
   *) m4_ifvaln($3,$3,[
   if echo " $[]m4_ifval($1,$1,FLAGS) " | grep " $VAR " 2>&1 >/dev/null
   then AC_RUN_LOG([: m4_ifval($1,$1,FLAGS) does contain $VAR])
   else AC_RUN_LOG([: m4_ifval($1,$1,FLAGS)="$m4_ifval($1,$1,FLAGS) $VAR"])
                      m4_ifval($1,$1,FLAGS)="$m4_ifval($1,$1,FLAGS) $VAR"
   fi ]) ;;
esac
AS_VAR_POPDEF([VAR])dnl
AS_VAR_POPDEF([FLAGS])dnl
])

dnl the only difference - the LANG selection... and the default FLAGS

AC_DEFUN([AX_CXXFLAGS_WARN_ALL],[dnl
AS_VAR_PUSHDEF([FLAGS],[CXXFLAGS])dnl
AS_VAR_PUSHDEF([VAR],[ac_cv_cxxflags_warn_all])dnl
AC_CACHE_CHECK([m4_ifval($1,$1,FLAGS) for maximum warnings],
VAR,[VAR="no, unknown"
 AC_LANG_SAVE
 AC_LANG_CXX
 ac_save_[]FLAGS="$[]FLAGS"
for ac_arg dnl
in "-pedantic  % -Wall"       dnl   GCC
   "-xstrconst % -v"          dnl Solaris C 
   "-std1      % -verbose -w0 -warnprotos" dnl Digital Unix 
   "-qlanglvl=ansi % -qsrcmsg -qinfo=all:noppt:noppc:noobs:nocnd" dnl AIX
   "-ansi -ansiE % -fullwarn" dnl IRIX
   "+ESlit     % +w1"         dnl HP-UX C 
   "-Xc        % -pvctl[,]fullmsg" dnl NEC SX-5 (Super-UX 10)
   "-h conform % -h msglevel 2" dnl Cray C (Unicos)
   # 
do FLAGS="$ac_save_[]FLAGS "`echo $ac_arg | sed -e 's,%%.*,,' -e 's,%,,'`
   AC_TRY_COMPILE([],[return 0;],
   [VAR=`echo $ac_arg | sed -e 's,.*% *,,'` ; break])
done
 FLAGS="$ac_save_[]FLAGS"
 AC_LANG_RESTORE
])
case ".$VAR" in
     .ok|.ok,*) m4_ifvaln($3,$3) ;;
   .|.no|.no,*) m4_ifvaln($4,$4,[m4_ifval($2,[
        AC_RUN_LOG([: m4_ifval($1,$1,FLAGS)="$m4_ifval($1,$1,FLAGS) $2"])
                      m4_ifval($1,$1,FLAGS)="$m4_ifval($1,$1,FLAGS) $2"])]) ;;
   *) m4_ifvaln($3,$3,[
   if echo " $[]m4_ifval($1,$1,FLAGS) " | grep " $VAR " 2>&1 >/dev/null
   then AC_RUN_LOG([: m4_ifval($1,$1,FLAGS) does contain $VAR])
   else AC_RUN_LOG([: m4_ifval($1,$1,FLAGS)="$m4_ifval($1,$1,FLAGS) $VAR"])
                      m4_ifval($1,$1,FLAGS)="$m4_ifval($1,$1,FLAGS) $VAR"
   fi ]) ;;
esac
AS_VAR_POPDEF([VAR])dnl
AS_VAR_POPDEF([FLAGS])dnl
])

dnl  implementation tactics:
dnl   the for-argument contains a list of options. The first part of
dnl   these does only exist to detect the compiler - usually it is
dnl   a global option to enable -ansi or -extrawarnings. All other
dnl   compilers will fail about it. That was needed since a lot of
dnl   compilers will give false positives for some option-syntax
dnl   like -Woption or -Xoption as they think of it is a pass-through
dnl   to later compile stages or something. The "%" is used as a
dnl   delimimiter. A non-option comment can be given after "%%" marks.


dnl ______ /usr/share/aclocal/guidod/ax_cflags_strict_prototypes.m4 ______
dnl @synopsis AX_CFLAGS_STRICT_PROTOTYPES [(shellvar [,default, [A/NA]]
dnl
dnl Try to find a compiler option that requires strict prototypes.
dnl
dnl The sanity check is done by looking at sys/signal.h which has a set
dnl of macro-definitions SIG_DFL and SIG_IGN that are cast to the local
dnl signal-handler type. If that signal-handler type is not fully
dnl qualified then the system headers are not seen as strictly prototype clean.
dnl
dnl For the GNU CC compiler it will be -fstrict-prototypes -Wstrict-prototypes
dnl The result is added to the shellvar being CFLAGS by default.
dnl
dnl DEFAULTS:
dnl
dnl  - $1 shell-variable-to-add-to : CFLAGS
dnl  - $2 add-value-if-not-found : nothing
dnl  - $3 action-if-found : add value to shellvariable
dnl  - $4 action-if-not-found : nothing
dnl
dnl @version %Id: ax_cflags_strict_prototypes.m4,v 1.7 2003/06/05 00:52:22 guidod Exp %
dnl @author Guido Draheim <guidod@gmx.de>
dnl
AC_DEFUN([AX_CFLAGS_STRICT_PROTOTYPES],[dnl
AS_VAR_PUSHDEF([FLAGS],[CFLAGS])dnl
AS_VAR_PUSHDEF([VAR],[ac_cv_cflags_strict_prototypes])dnl
AC_CACHE_CHECK([m4_ifval($1,$1,FLAGS) for strict prototypes],
VAR,[VAR="no, unknown"
 AC_LANG_SAVE
 AC_LANG_C
 ac_save_[]FLAGS="$[]FLAGS"
for ac_arg dnl
in "-Wall     % -fstrict-prototypes -Wstrict-prototypes" dnl   GCC
   "-Wall     % -Wstrict-prototypes" dnl try to warn atleast
   #
do FLAGS="$ac_save_[]FLAGS "`echo $ac_arg | sed -e 's,%%.*,,' -e 's,%,,'`
   AC_TRY_COMPILE([],[return 0;],
   [VAR=`echo $ac_arg | sed -e 's,.*% *,,'` ; break])
done
case ".$VAR" in
   .|.no|.no,*) ;;
   *) # sanity check with signal() from sys/signal.h
    cp config.log config.tmp
    AC_TRY_COMPILE([#include <signal.h>],[
    if (signal (SIGINT, SIG_IGN) == SIG_DFL) return 1;
    if (signal (SIGINT, SIG_IGN) != SIG_DFL) return 2;],
    dnl the original did use test -n `$CC testprogram.c`
    [if test `diff config.log config.tmp | grep -i warning | wc -l` != 0
then if test `diff config.log config.tmp | grep -i warning | wc -l` != 1
then VAR="no, suppressed, signal.h," ; fi ; fi],
    [VAR="no, suppressed, signal.h"])
    rm config.tmp
  ;;
esac
 FLAGS="$ac_save_[]FLAGS"
 AC_LANG_RESTORE
])
case ".$VAR" in
     .ok|.ok,*) m4_ifvaln($3,$3) ;;
   .|.no|.no,*) m4_ifvaln($4,$4,[m4_ifval($2,[
        AC_RUN_LOG([: m4_ifval($1,$1,FLAGS)="$m4_ifval($1,$1,FLAGS) $2"])
                      m4_ifval($1,$1,FLAGS)="$m4_ifval($1,$1,FLAGS) $2"])]) ;;
   *) m4_ifvaln($3,$3,[
   if echo " $[]m4_ifval($1,$1,FLAGS) " | grep " $VAR " 2>&1 >/dev/null
   then AC_RUN_LOG([: m4_ifval($1,$1,FLAGS) does contain $VAR])
   else AC_RUN_LOG([: m4_ifval($1,$1,FLAGS)="$m4_ifval($1,$1,FLAGS) $VAR"])
                      m4_ifval($1,$1,FLAGS)="$m4_ifval($1,$1,FLAGS) $VAR"
   fi ]) ;;
esac
AS_VAR_POPDEF([VAR])dnl
AS_VAR_POPDEF([FLAGS])dnl
])

dnl the only difference - the LANG selection... and the default FLAGS

AC_DEFUN([AX_CXXFLAGS_STRICT_PROTOTYPES],[dnl
AS_VAR_PUSHDEF([FLAGS],[CXXFLAGS])dnl
AS_VAR_PUSHDEF([VAR],[ac_cv_cxxflags_strict_prototypes])dnl
AC_CACHE_CHECK([m4_ifval($1,$1,FLAGS) for strict prototypes],
VAR,[VAR="no, unknown"
 AC_LANG_SAVE
 AC_LANG_CXX
 ac_save_[]FLAGS="$[]FLAGS"
for ac_arg dnl
in "-Wall     % -fstrict-prototypes -Wstrict-prototypes" dnl   GCC
   "-Wall     % -Wstrict-prototypes" dnl try to warn atleast
   #
do FLAGS="$ac_save_[]FLAGS "`echo $ac_arg | sed -e 's,%%.*,,' -e 's,%,,'`
   AC_TRY_COMPILE([],[return 0;],
   [VAR=`echo $ac_arg | sed -e 's,.*% *,,'` ; break])
done
case ".$VAR" in
   .|.no|.no,*) ;;
   *) # sanity check with signal() from sys/signal.h
    cp config.log config.tmp
    AC_TRY_COMPILE([#include <signal.h>],[
    if (signal (SIGINT, SIG_IGN) == SIG_DFL) return 1;
    if (signal (SIGINT, SIG_IGN) != SIG_DFL) return 2;],
    dnl the original did use test -n `$CC testprogram.c`
    [if test `diff config.log config.tmp | grep -i warning | wc -l` != 0
then if test `diff config.log config.tmp | grep -i warning | wc -l` != 1
then VAR="no, suppressed, signal.h," ; fi ; fi],
    [VAR="no, suppressed, signal.h"])
    rm config.tmp
  ;;
esac
 FLAGS="$ac_save_[]FLAGS"
 AC_LANG_RESTORE
])
case ".$VAR" in
     .ok|.ok,*) m4_ifvaln($3,$3) ;;
   .|.no|.no,*) m4_ifvaln($4,$4,[m4_ifval($2,[
        AC_RUN_LOG([: m4_ifval($1,$1,FLAGS)="$m4_ifval($1,$1,FLAGS) $2"])
                      m4_ifval($1,$1,FLAGS)="$m4_ifval($1,$1,FLAGS) $2"])]) ;;
   *) m4_ifvaln($3,$3,[
   if echo " $[]m4_ifval($1,$1,FLAGS) " | grep " $VAR " 2>&1 >/dev/null
   then AC_RUN_LOG([: m4_ifval($1,$1,FLAGS) does contain $VAR])
   else AC_RUN_LOG([: m4_ifval($1,$1,FLAGS)="$m4_ifval($1,$1,FLAGS) $VAR"])
                      m4_ifval($1,$1,FLAGS)="$m4_ifval($1,$1,FLAGS) $VAR"
   fi ]) ;;
esac
AS_VAR_POPDEF([VAR])dnl
AS_VAR_POPDEF([FLAGS])dnl
])

dnl ______ /usr/share/aclocal/guidod/ax_spec_package_version.m4 ______
dnl @* AX_SPEC_PACKAGE_AND_VERSION ([specfile])
dnl @* AX_SPEC_PACKAGE_NAME ([shellvar],[defaultvalue])
dnl @* AX_SPEC_PACKAGE_VERSION ([shellvar],[defaultvalue])
dnl @* AX_SPEC_PACKAGE_SUMMARY ([shellvar],[defaultvalue])
dnl @* AX_SPEC_PACKAGE_LICENSE ([shellvar],[defaultvalue])
dnl @* AX_SPEC_PACKAGE_CATEGORY ([shellvar],[defaultvalue])
dnl @* AX_SPEC_PACKAGE_ICON ([shellvar],[defaultvalue])
dnl @* AX_SPEC_DEFAULTS([specfile])
dnl
dnl set PACKAGE from the given specfile - default to basename of
dnl the rpmspecfile if no "name:" could be found in the spec file.
dnl
dnl set VERSION from the given specfile - default to a date-derived
dnl value if no "version:" could be found in the spec file.
dnl
dnl this macro builds on top of AX_SPEC_FILE / AX_SPEC_EXTRACT
dnl
dnl more specific: if not "name:" or "%define name" was found in
dnl the myproject.spec file then the PACKAGE var is set to the
dnl basename "myproject". When no spec file was present then it
dnl will usually default to "TODO".
dnl
dnl The version spec looks for "version:" or "%define version" in
dnl the spec file. When no such value was seen or no spec file
dnl had been present then the value is set to `date +0.%y.%W%w`.
dnl
dnl the version value itself is sanitized somewhat with making
dnl it to always carry atleast three digits (1.2.3) and clensing
dnl superflous "0" chars around from generating numbers elsewhere.
dnl
dnl additional macros are provided that extract a specific value
dnl from the spec file, among these:
dnl
dnl set PACKAGE_SUMMARY from the given specfile - default to package and
dnl try to detect a type suffix if "summary:" was not in the spec file
dnl
dnl set PACKAGE_LICENSE from the given specfile - if no "license:" tag was
dnl given in the spec file then various COPYING files are grepped to
dnl have a guess and the final fallback will be GNU GPL (or GNU LGPL).
dnl
dnl set PACKAGE_ICON from the given specfile - if no "icon:" tag was
dnl given in the spec file then we default to $PACKAGE-icon.png
dnl
dnl the final AX_SPEC_INIT(specfile) will initialize all variables to
dnl its defaults according to the spec file given.
dnl
dnl @version %Id: ax_spec_package_version.m4,v 1.1 2003/10/18 23:38:41 guidod Exp %
dnl @author Guido Draheim <guidod@gmx.de>

AC_DEFUN([AX_SPEC_PACKAGE_LICENSE],[AC_REQUIRE([AX_SPEC_FILE])dnl
  AS_VAR_PUSHDEF([VAR],[PACKAGE_LICENSE])dnl
  AC_MSG_CHECKING([for spec license type])
  if test ".$VAR" = "." ; then if test ! -f $ax_spec_file 
  then k="(w/o spec)"
  else k=""
    AX_SPEC_EXTRACT(VAR,[license],m4_ifval($1,$1))
    VAR=`echo $VAR | sed -e 's/ *License//g'`
  fi fi
  test ".$VAR" = "." && k="(fallback)"
  ifelse($2,,[dnl here the defaults for LICENSE / COPYRIGHT
  if test ".$VAR"   = "."  ; then
    for ac_file in "$srcdir/COPYING" "$srcdir/COPYING" "$srcdir/LICENSE" ; do
      test -f "$ac_file" || continue
dnl  http://www.ibiblio.org/osrt/omf/omf_elements "16. Rights"
      if grep "GNU LESSER GENERAL PUBLIC LICENSE" "$ac_file" >/dev/null
      then VAR="GNU LGPL" ; break
      elif grep "GNU GENERAL PUBLIC LICENSE" "$ac_file" >/dev/null
      then VAR="GNU GPL" ; break
      elif grep "MOZILLA PUBLIC LICENSE" "$ac_file" >/dev/null
      then VAR="MPL" ; break
      elif grep "Mozilla Public License" "$ac_file" >/dev/null
      then VAR="MPL" ; break
      elif grep -i "artistic license" "$ac_file" >/dev/null
      then VAR="Artistic" ; break
      elif grep -i "artistic control" "$ac_file" >/dev/null
      then VAR="Artistic" ; break
      elif grep -i "semblance of artistic" "$ac_file" >/dev/null
      then VAR="Artistic" ; break
      elif grep -i "above copyright notice" "$ac_file" >/dev/null
      then VAR="BSD" ; break
      fi
    done
    if test ".$VAR" = "." ; then
      if test "$srcdir/COPYING.LIB" ; then VAR="GNU LGPL"
      elif test ".$ltmain" != "."   ; then VAR="GNU LGPL" 
       else VAR="GNU GPL"
      fi
    fi
  fi
  ],[test ".$VAR" = "." && VAR="$2"])
  test "$VAR" = "GPL" && VAR="GNU GPL"
  test "$VAR" = "LGPL" && VAR="GNU LGPL"
  AC_MSG_RESULT([m4_ifval([$1],[$1 = ])$VAR $k])
  AS_VAR_POPDEF([VAR])dnl
])

AC_DEFUN([AX_SPEC_PACKAGE_SUMMARY],[AC_REQUIRE([AX_SPEC_FILE])dnl
  AS_VAR_PUSHDEF([VAR],[PACKAGE_SUMMARY])dnl
  AC_MSG_CHECKING([for spec summary])
  if test ".$VAR" = "." ; then if test ! -f $ax_spec_file 
  then k="(w/o spec)"
  else k=""
    AX_SPEC_EXTRACT(VAR,[summary],m4_ifval($1,$1))
  fi fi
  test ".$VAR" = "." && k="(fallback)"
  ifelse($2,,[dnl here the defaults for SUMMARY
  if test ".$VAR"   = "."  ; then VAR="$PACKAGE"
     test ".$VAR" = "." && VAR="foo"
     test ".$ltmain" != "." && VAR="$VAR library"
  fi
  ],[test ".$VAR" = "." && VAR="$2"])
  AC_MSG_RESULT([m4_ifval([$1],[$1 = ])$VAR $k])
  AS_VAR_POPDEF([VAR])dnl
])

AC_DEFUN([AX_SPEC_PACKAGE_ICON],[AC_REQUIRE([AX_SPEC_FILE])dnl
  AS_VAR_PUSHDEF([VAR],[PACKAGE_ICON])dnl
  AC_MSG_CHECKING([for spec icon])
  if test ".$VAR" = "." ; then if test ! -f $ax_spec_file 
  then k="(w/o spec)"
  else k=""
    AX_SPEC_EXTRACT(VAR,[icon],m4_ifval($1,$1))
  fi fi
  test ".$VAR" = "." && k="(fallback)"
  ifelse($2,,[dnl here the defaults for ICON
  if test ".$VAR"   = "."  ; then VAR="$PACKAGE-icon.png" ; fi
  ],[test ".$VAR" = "." && VAR="$2"])
  AC_MSG_RESULT([m4_ifval([$1],[$1 = ])$VAR $k])
  AS_VAR_POPDEF([VAR])dnl
])

AC_DEFUN([AX_SPEC_PACKAGE_CATEGORY],[AC_REQUIRE([AX_SPEC_FILE])dnl
  AS_VAR_PUSHDEF([VAR],[PACKAGE_CATEGORY])dnl
  AC_MSG_CHECKING([for spec category])
  if test ".$VAR" = "." ; then if test ! -f $ax_spec_file 
  then k="(w/o spec)"
  else k=""
    AX_SPEC_EXTRACT(VAR,[group],m4_ifval($1,$1))
    VAR=`echo $VAR | sed -e 's/ /-/g'`
  fi fi
  test ".$VAR" = "." && k="(fallback)"
  ifelse($2,,[dnl here the defaults for CATEGORY
  if test ".$VAR" = "."  ; then if test ".$ltmain" != "."
     then VAR="Development/Library"
     else VAR="Development/Other"
  fi fi
  ],[test ".$VAR" = "." && VAR="$2"])
  AC_MSG_RESULT([m4_ifval([$1],[$1 = ])$VAR $k])
  AS_VAR_POPDEF([VAR])dnl
])

AC_DEFUN([AX_SPEC_PACKAGE_NAME],[AC_REQUIRE([AX_SPEC_FILE])dnl
  AS_VAR_PUSHDEF([VAR],[PACKAGE_NAME])dnl
  AC_MSG_CHECKING([for spec package])
  if test ".$VAR" = "." ; then if test ! -f $ax_spec_file 
  then k="(w/o spec)"
  else k=""
    AX_SPEC_EXTRACT(VAR,[name],m4_ifval($1,$1))
    VAR=`echo $VAR | sed -e 's/ /-/g'`
  fi fi
  test ".$VAR" = "." && k="(fallback)"
  ifelse($2,,[dnl here the defaults for PACKAGE
  test ".$VAR"   = "."  && VAR=`basename $ax_spec_file .spec`
  test ".$VAR"   = ".README" && VAR="TODO"
  test ".$VAR"   = ".TODO" && VAR="foo"
  ],[test ".$VAR" = "." && VAR="$2"])
  test "VAR" = "PACKAGE_NAME" && test ".$PACKAGE" = "." && PACKAGE="$VAR" 
  AC_MSG_RESULT([m4_ifval([$1],[$1 = ])$VAR $k])
  AS_VAR_POPDEF([VAR])dnl
])

AC_DEFUN([AX_SPEC_PACKAGE_VERSION_],[AC_REQUIRE([AX_SPEC_FILE])dnl
  AS_VAR_PUSHDEF([VAR],[PACKAGE_VERSION])dnl
  AC_MSG_CHECKING([for spec version])
  if test ".$VAR" = "." ; then if test ! -f $ax_spec_file
  then k="(w/o spec)"
  else k=""
    AX_SPEC_EXTRACT(VAR,[version],m4_ifval($1,$1))
    VAR=`echo $VAR | sed -e 's/ /-/g'`
  fi fi
  test ".$VAR" = "." && k="(fallback)"
  ifelse($2,,[dnl here the defaults for VERSION
  test ".$VAR"   = "."  && VAR=`date +0.%y.%W%w`
  ],[test ".$VAR" = "." && VAR="$2"])
  test "VAR" = "PACKAGE_VERSION" && test ".$VERSION" = "." && VERSION="$VAR"
  case "$VAR" in  # note we set traditional VERSION before cleaning things up
  *.*.) VAR="$VAR"`date +%W%w` ;;
  *.*.*) ;;
  *.)  VAR="$VAR"`date +%y.%W%w` ;;
  *.*) VAR="$VAR.0" ;;
  *) VAR=AS_TR_SH([$VAR]) ; VAR="$VAR.`date +%y.%W%w`" ;;
  esac
  VAR=`echo $VAR | sed -e "s/[[.]][0]\\([0-9]\\)/.\\1/g"`
  AC_MSG_RESULT([m4_ifval([$1],[$1 = ])$VAR $k])
  AS_VAR_POPDEF([VAR])dnl
])

dnl for compatibility, we define ax_spec_package_version
dnl to do all of ax_spec_package_name as well.
AC_DEFUN([AX_SPEC_PACKAGE_VERSION],[AC_REQUIRE([AX_SPEC_FILE])dnl
  ifelse($1,,
    AC_MSG_WARN([please use ax_spec_package_AND_version now!]),
    AC_MSG_ERROR([please use ax_spec_package_AND_version now!]))
  AX_SPEC_PACKAGE_NAME
  AX_SPEC_PACKAGE_VERSION_
  
])

AC_DEFUN([AX_SPEC_PACKAGE_AND_VERSION],[
  m4_ifset([m4_ax_spec_file],,[AX_SPEC_FILE($1)])
  AX_SPEC_PACKAGE_NAME
  AX_SPEC_PACKAGE_VERSION_
])

AC_DEFUN([AX_SPEC_DEFAULTS],[
  m4_ifset([m4_ax_spec_file],,[AX_SPEC_FILE($1)])
  AX_SPEC_PACKAGE_NAME
  AX_SPEC_PACKAGE_VERSION_
  AX_SPEC_PACKAGE_LICENSE
  AX_SPEC_PACKAGE_SUMMARY
  AX_SPEC_PACKAGE_CATEGORY
  AX_SPEC_PACKAGE_ICON
])

dnl ______ /usr/share/aclocal/guidod/ax_set_version_info.m4 ______
dnl AX_SET_VERSION_INFO [(VERSION  [,PREFIX])]      -*- m4 -*-
dnl   default $1 = $PACKAGE_VERSION
dnl   default $2 = <none>
dnl 
dnl This macro is the successor of AC_SET_RELEASEINFO_VERSIONINFO but
dnl it can be used in parallel because it uses all different variables.
dnl
dnl check the $VERSION number and cut the two last digit-sequences off
dnl which will form a -version-info in a @VERSION_INFO@ ac_subst while
dnl the rest is going to the -release name in a @RELEASE_INFO@ ac_subst.
dnl
dnl you should keep these two seperate - the release-name may contain
dnl alpha-characters and can be modified later with extra release-hints
dnl e.g. RELEASE_INFO="$RELEASE_INFO-debug" for a debug version 
dnl of your lib. The $VERSION_INFO however should not be touched.
dnl
dnl example: a VERSION="2.4.18" will be transformed into
dnl    RELEASE_INFO = -release 2
dnl    VERSION_INFO = -versioninfo 4:18
dnl then use these two variables and push them to your libtool linker
dnl    libtest_la_LIBADD = @RELEASE_INFO@ @VERSION_INFO@
dnl and for a linux-target this will tell libtool to install the lib as
dnl           libmy.so libmy.la libmy.a libmy-2.so.4 libmy-2.so.4.0.18
dnl and executables will get link-resolve-infos for libmy-2.so.4 - therefore
dnl the patch-level is ignored during ldso linking, and ldso will use the
dnl one with the highest patchlevel. Using just "-release $(VERSION)"
dnl during libtool-linking would not do that - omitting the -version-info
dnl will libtool install libmy.so libmy.la libmy.a libmy-2.4.18.so and
dnl executables would get hardlinked with the 2.4.18 version of your lib.
dnl
dnl This background does also explain the default dll name for a win32
dnl target : libtool will choose to make up libmy-2-4.dll for this 
dnl version spec.
dnl
dnl this macro does also set the usual three parts of a version spec
dnl $MAJOR_VERSION.$MINOR_VERSION.$MICRO_VERSION but does not ac_subst 
dnl for the plain AX_SET_VERSION_INFO macro. Use instead one of the numbered
dnl macros AX_SET_VERSION_INFO1 (use first number for release part) or
dnl that AX_SET_VERSION_INFO2 (use the first two numbers for release part).
dnl 
dnl You may add sublevel parts like "1.4.2-ac5" where the sublevel is 
dnl just killed from these version/release substvars. That allows to grab
dnl the version off a .spec file like with AX_SPEC_PACKAGE_VERSION where 
dnl the $VERSION is used to name a tarball or distpack like mylib-2.2.9pre4
dnl
dnl Unlike earlier macros, you can use this one to break up different
dnl VERSIONs and put them into different variables, just hint with
dnl PREFIX-setting - i.e. _VERSION(2.4.5,TEST) will set variables named
dnl TEST_MAJOR_VERSION=2... and of course $TEST_RELEASE_INFO etc.
dnl (for the moment, it needs to be a literal prefix *sigh*)
dnl
dnl @version %Id: ax_set_version_info.m4,v 1.1 2003/07/02 23:41:42 guidod Exp %
dnl @author Guido Draheim <guidod@gmx.de>

AC_DEFUN([AX_SET_VERSION_INFO1],[dnl
AS_VAR_PUSHDEF([MAJOR],ifelse($2,,[MAJOR_VERSION],[$2_MAJOR_VERSION]))dnl
AS_VAR_PUSHDEF([MINOR],ifelse($2,,[MINOR_VERSION],[$2_MINOR_VERSION]))dnl
AS_VAR_PUSHDEF([MICRO],ifelse($2,,[MICRO_VERSION],[$2_MICRO_VERSION]))dnl
AS_VAR_PUSHDEF([PATCH],ifelse($2,,[PATCH_VERSION],[$2_PATCH_VERSION]))dnl
AS_VAR_PUSHDEF([LTREL],ifelse($2,,[RELEASE_INFO],[$2_RELEASE_INFO]))dnl
AS_VAR_PUSHDEF([LTVER],ifelse($2,,[VERSION_INFO],[$2_VERSION_INFO]))dnl
test ".$PACKAGE_VERSION" = "." && PACKAGE_VERSION="$VERSION"
AC_MSG_CHECKING(ifelse($2,,,[$2 ])out linker version info dnl
ifelse($1,,$PACKAGE_VERSION,$1) )
  MINOR=`echo ifelse( $1, , $PACKAGE_VERSION, $1 )`
  MAJOR=`echo "$MINOR" | sed -e 's/[[.]].*//'` 
  MINOR=`echo "$MINOR" | sed -e "s/^$MAJOR//" -e 's/^.//'` 
  MICRO="$MINOR"
  MINOR=`echo "$MICRO" | sed -e 's/[[.]].*//'`
  MICRO=`echo "$MICRO" | sed -e "s/^$MINOR//" -e 's/^.//'`
  PATCH="$MICRO"
  MICRO=`echo "$PATCH" | sed -e 's/[[^0-9]].*//'` 
  PATCH=`echo "$PATCH" | sed -e "s/^$MICRO//" -e 's/^[[-.]]//'`
  if test "_$MICRO" = "_" ; then MICRO="0" ; fi
  if test "_$MINOR" = "_" ; then MINOR="$MAJOR" ; MAJOR="0" ; fi
  MINOR=`echo "$MINOR" | sed -e 's/[[^0-9]].*//'`
  LTREL="-release $MAJOR"
  LTVER="-version-info $MINOR:$MICRO"
AC_MSG_RESULT([/$MAJOR/$MINOR:$MICRO (-$MAJOR.so.$MINOR.0.$MICRO)])
AC_SUBST(MAJOR)
AC_SUBST(MINOR)
AC_SUBST(MICRO)
AC_SUBST(PATCH)
AC_SUBST(LTREL)
AC_SUBST(LTVER)
AS_VAR_POPDEF([LTVER])dnl
AS_VAR_POPDEF([LTREL])dnl
AS_VAR_POPDEF([PATCH])dnl
AS_VAR_POPDEF([MICRO])dnl
AS_VAR_POPDEF([MINOR])dnl
AS_VAR_POPDEF([MAJOR])dnl
])

AC_DEFUN([AX_SET_VERSION_INFO2],[dnl
AS_VAR_PUSHDEF([MAJOR],ifelse($2,,[MAJOR_VERSION],[$2_MAJOR_VERSION]))dnl
AS_VAR_PUSHDEF([MINOR],ifelse($2,,[MINOR_VERSION],[$2_MINOR_VERSION]))dnl
AS_VAR_PUSHDEF([MICRO],ifelse($2,,[MICRO_VERSION],[$2_MICRO_VERSION]))dnl
AS_VAR_PUSHDEF([PATCH],ifelse($2,,[PATCH_VERSION],[$2_PATCH_VERSION]))dnl
AS_VAR_PUSHDEF([LTREL],ifelse($2,,[RELEASE_INFO],[$2_RELEASE_INFO]))dnl
AS_VAR_PUSHDEF([LTVER],ifelse($2,,[VERSION_INFO],[$2_VERSION_INFO]))dnl
test ".$PACKAGE_VERSION" = "." && PACKAGE_VERSION="$VERSION"
AC_MSG_CHECKING(ifelse($2,,,[$2 ])out linker version info dnl
ifelse($1,,$PACKAGE_VERSION,$1) )
  MINOR=`echo ifelse( $1, , $PACKAGE_VERSION, $1 )`
  MAJOR=`echo "$MINOR" | sed -e 's/[[.]].*//'` 
  MINOR=`echo "$MINOR" | sed -e "s/^$MAJOR//" -e 's/^.//'` 
  MICRO="$MINOR"
  MINOR=`echo "$MICRO" | sed -e 's/[[.]].*//'`
  MICRO=`echo "$MICRO" | sed -e "s/^$MINOR//" -e 's/^.//'`
  PATCH="$MICRO"
  MICRO=`echo "$PATCH" | sed -e 's/[[^0-9]].*//'` 
  PATCH=`echo "$PATCH" | sed -e "s/^$MICRO//" -e 's/^[[-.]]//'`
  test "_$MICRO" != "_" || MICRO="0"
  if test "_$MINOR" != "_" ; then MINOR="$MAJOR" ; MAJOR="0" ; fi
  MINOR=`echo "$MINOR" | sed -e 's/[[^0-9]].*//'`
  LTREL="-release $MAJOR.$MINOR"
  LTVER="-version-info 0:$MICRO"
AC_MSG_RESULT([/$MAJOR/$MINOR:$MICRO (-$MAJOR.so.$MINOR.0.$MICRO)])
AC_SUBST(MAJOR)
AC_SUBST(MINOR)
AC_SUBST(MICRO)
AC_SUBST(PATCH)
AC_SUBST(LTREL)
AC_SUBST(LTVER)
AS_VAR_POPDEF([LTVER])dnl
AS_VAR_POPDEF([LTREL])dnl
AS_VAR_POPDEF([PATCH])dnl
AS_VAR_POPDEF([MICRO])dnl
AS_VAR_POPDEF([MINOR])dnl
AS_VAR_POPDEF([MAJOR])dnl
])

AC_DEFUN([AX_SET_VERSION_INFO],[dnl
AS_VAR_PUSHDEF([MAJOR],ifelse($2,,[MAJOR_VERSION],[$2_MAJOR_VERSION]))dnl
AS_VAR_PUSHDEF([MINOR],ifelse($2,,[MINOR_VERSION],[$2_MINOR_VERSION]))dnl
AS_VAR_PUSHDEF([MICRO],ifelse($2,,[MICRO_VERSION],[$2_MICRO_VERSION]))dnl
AS_VAR_PUSHDEF([PATCH],ifelse($2,,[PATCH_VERSION],[$2_PATCH_VERSION]))dnl
AS_VAR_PUSHDEF([LTREL],ifelse($2,,[RELEASE_INFO],[$2_RELEASE_INFO]))dnl
AS_VAR_PUSHDEF([LTVER],ifelse($2,,[VERSION_INFO],[$2_VERSION_INFO]))dnl
test ".$PACKAGE_VERSION" = "." && PACKAGE_VERSION="$VERSION"
AC_MSG_CHECKING(ifelse($2,,,[$2 ])out linker version info dnl
ifelse($1,,$PACKAGE_VERSION,$1) )
  MINOR=`echo ifelse( $1, , $PACKAGE_VERSION, $1 )`
  MAJOR=`echo "$MINOR" | sed -e 's/[[.]].*//'` 
  MINOR=`echo "$MINOR" | sed -e "s/^$MAJOR//" -e 's/^.//'` 
  MICRO="$MINOR"
  MINOR=`echo "$MICRO" | sed -e 's/[[.]].*//'`
  MICRO=`echo "$MICRO" | sed -e "s/^$MINOR//" -e 's/^.//'`
  PATCH="$MICRO"
  MICRO=`echo "$PATCH" | sed -e 's/[[^0-9]].*//'` 
  PATCH=`echo "$PATCH" | sed -e "s/^$MICRO//" -e 's/[[-.]]//'`
  if test "_$MICRO" = "_" ; then MICRO="0" ; fi
  if test "_$MINOR" = "_" ; then MINOR="$MAJOR" ; MAJOR="0" ; fi
  MINOR=`echo "$MINOR" | sed -e 's/[[^0-9]].*//'`
  LTREL="-release $MAJOR"
  LTVER="-version-info $MINOR:$MICRO"
AC_MSG_RESULT([/$MAJOR/$MINOR:$MICRO (-$MAJOR.so.$MINOR.0.$MICRO)])
AC_SUBST(LTREL)
AC_SUBST(LTVER)
AS_VAR_POPDEF([LTVER])dnl
AS_VAR_POPDEF([LTREL])dnl
AS_VAR_POPDEF([PATCH])dnl
AS_VAR_POPDEF([MICRO])dnl
AS_VAR_POPDEF([MINOR])dnl
AS_VAR_POPDEF([MAJOR])dnl
])

dnl ______ /usr/share/aclocal/guidod/ax_enable_builddir.m4 ______
dnl @synopsis AX_ENABLE_BUILDDIR [(dirstring-or-command [,Makefile.mk [,-all]])]
dnl
dnl if the current configure was run within the srcdir then
dnl we move all configure-files into a subdir and let the configure
dnl steps continue there. We provide an option --disable-builddir
dnl to suppress the move into a separate builddir.
dnl
dnl Defaults:
dnl
dnl   $1 = $host (overridden with $HOST)
dnl   $2 = Makefile.mk
dnl   $3 = -all
dnl
dnl This macro must be called before AM_INIT_AUTOMAKE.
dnl
dnl it creates a default toplevel srcdir Makefile from the information found
dnl in the created toplevel builddir Makefile. It just copies the variables
dnl and rule-targets, each extended with a default rule-execution that
dnl recurses into the build directory of the current "HOST". You can override
dnl the auto-dection through `config.guess` and build-time of course, as in
dnl
dnl   make HOST=i386-mingw-cross
dnl
dnl which can of course set at configure time as well using
dnl
dnl   configure --host=i386-mingw-cross
dnl
dnl After the default has been created, additional rules can be
dnl appended that will not just recurse into the subdirectories and
dnl only ever exist in the srcdir toplevel makefile - these parts
dnl are read from the $2 = Makefile.mk file
dnl
dnl The automatic rules are usually scanning the toplevel Makefile for
dnl lines like '#### $host |$builddir' to recognize the place where to
dnl recurse into. Usually, the last one is the only one used. However,
dnl almost all targets have an additional "*-all" rule which makes the
dnl script to recurse into _all_ variants of the current HOST (!!)
dnl setting. The "-all" suffix can be overriden for the macro as well.
dnl
dnl a special rule is only given for things like "dist" that will
dnl copy the tarball from the builddir to the sourcedir (or $(PUB))
dnl for reason of convenience.
dnl
dnl @author Guido Draheim
dnl @version %Id: ax_enable_builddir.m4,v 1.14 2004/12/29 05:22:31 guidod Exp %
dnl
AC_DEFUN([AX_ENABLE_BUILDDIR],[AC_REQUIRE([AC_CANONICAL_HOST])[]dnl
AC_BEFORE([$0],[AM_INIT_AUTOMAKE])dnl
AS_VAR_PUSHDEF([SUB],[ax_enable_builddir])dnl
AS_VAR_PUSHDEF([AUX],[ax_enable_builddir_auxdir])dnl
AS_VAR_PUSHDEF([SED],[ax_enable_builddir_sed])dnl
SUB="."
AC_ARG_ENABLE([builddir], AC_HELP_STRING(
  [--disable-builddir],[disable automatic build in subdir of sources])
  ,[SUB="$enableval"], [SUB="yes"])
if test ".$ac_srcdir_defaulted" != ".no" ; then
if test ".$srcdir" = ".." ; then
  if test -f config.status ; then 
    AC_MSG_NOTICE(toplevel srcdir already configured... skipping subdir build)
  else
    test ".$SUB" = "."  && SUB="."
    test ".$SUB" = ".no"  && SUB="."
    test ".$TARGET" = "." && TARGET="$target"
    test ".$SUB" = ".yes" && SUB="m4_ifval([$1], [$1],[$TARGET])"
    if test ".$SUB" != ".." ; then    # we know where to go and
      AS_MKDIR_P([$SUB])
      echo __.$SUB.__ > $SUB/conftest.tmp
      cd $SUB
      if grep __.$SUB.__ conftest.tmp >/dev/null 2>/dev/null ; then
        rm conftest.tmp
        AC_MSG_RESULT([continue configure in default builddir "./$SUB"])
      else
        AC_MSG_ERROR([could not change to default builddir "./$SUB"])
      fi
      srcdir=`echo "$SUB" | 
              sed -e 's,^\./,,;s,[[^/]]$,&/,;s,[[^/]]*/,../,g;s,[[/]]$,,;'`
      # going to restart from subdirectory location
      test -f $srcdir/config.log   && mv $srcdir/config.log   .
      test -f $srcdir/confdefs.h   && mv $srcdir/confdefs.h   .
      test -f $srcdir/conftest.log && mv $srcdir/conftest.log .
      test -f $srcdir/$cache_file  && mv $srcdir/$cache_file  .
      AC_MSG_RESULT(....exec $SHELL $srcdir/[$]0 "--srcdir=$srcdir" "--enable-builddir=$SUB" ${1+"[$]@"})
      case "[$]0" in # restart
       [/\\]) exec $SHELL [$]0 "--srcdir=$srcdir" "--enable-builddir=$SUB" ${1+"[$]@"} ;; 
       *) exec $SHELL $srcdir/[$]0 "--srcdir=$srcdir" "--enable-builddir=$SUB" ${1+"[$]@"} ;;
      esac 
    fi
  fi
fi fi
dnl ac_path_prog uses "set dummy" to override $@ which would defeat the "exec"
AC_PATH_PROG(SED,gsed sed, sed)
AS_VAR_POPDEF([SED])dnl
AS_VAR_POPDEF([AUX])dnl
AS_VAR_POPDEF([SUB])dnl
AC_CONFIG_COMMANDS([buildir],[dnl .............. config.status ..............
AS_VAR_PUSHDEF([SUB],[ax_enable_builddir])dnl
AS_VAR_PUSHDEF([TOP],[top_srcdir])dnl
AS_VAR_PUSHDEF([SRC],[ac_top_srcdir])dnl
AS_VAR_PUSHDEF([AUX],[ax_enable_builddir_auxdir])dnl
AS_VAR_PUSHDEF([SED],[ax_enable_builddir_sed])dnl
pushdef([END],[Makefile.mk])dnl
pushdef([_ALL],[ifelse([$3],,[-all],[$3])])dnl
  SRC="$ax_enable_builddir_srcdir"
  if test ".$SUB" = "." ; then
    if test -f "$TOP/Makefile" ; then
      AC_MSG_NOTICE([skipping TOP/Makefile - left untouched])
    else
      AC_MSG_NOTICE([skipping TOP/Makefile - not created])
    fi
  else
    if test -f "$SRC/Makefile" ; then
      a=`grep "^VERSION " "$SRC/Makefile"` ; b=`grep "^VERSION " Makefile`
      test "$a" != "$b" && rm "$SRC/Makefile"
    fi
    if test -f "$SRC/Makefile" ; then
	echo "$SRC/Makefile : $SRC/Makefile.in" > $tmp/conftemp.mk
	echo "	[]@ echo 'REMOVED,,,' >\$[]@" >> $tmp/conftemp.mk
      eval "${MAKE-make} -f $tmp/conftemp.mk 2>/dev/null >/dev/null"
      if grep '^REMOVED,,,' "$SRC/Makefile" >/dev/null 
      then rm $SRC/Makefile ; fi
      cp $tmp/conftemp.mk $SRC/makefiles.mk~      ## DEBUGGING
    fi
    if test ! -f "$SRC/Makefile" ; then
      AC_MSG_NOTICE([create TOP/Makefile guessed from local Makefile])
      x='`' ; cat >$tmp/conftemp.sed <<_EOF
/^\$/n
x
/^\$/bS
x
/\\\\\$/{H;d;}
{H;s/.*//;x;}
bM
:S
x
/\\\\\$/{h;d;}
{h;s/.*//;x;}
:M
s/\\(\\n\\)	/\\1 /g
/^	/d
/^[[ 	]]*[[\\#]]/d
/^VPATH *=/d
s/^srcdir *=.*/srcdir = ./
s/^top_srcdir *=.*/top_srcdir = ./
/[[:=]]/!d
/^\\./d
s/:.*/:/
/:\$/s/ /  /g
/:\$/s/ \\([[a-z]][[a-z-]]*[[a-z]]\\) / \\1 \\1[]_ALL /g
/:\$/s/^\\([[a-z]][[a-z-]]*[[a-z]]\\)\\([[ :]]\\)/\\1 \\1[]_ALL\\2/
/:\$/s/  / /g
/^all all[]_ALL[[ :]]/i\\
all-configured : all[]_ALL
dnl dist-all exists... and would make for dist-all-all
/[]_ALL[]_ALL/d
/^.*[[=]]/!a\\
	@ HOST="\$(HOST)\" \\\\\\
	; test ".\$\$HOST" = "." && HOST=$x sh $AUX/config.guess $x \\\\\\
	; BUILD=$x grep "^#### \$\$HOST " Makefile | sed -e 's/.*|//' $x \\\\\\
	; use=$x basename "\$\@" _ALL $x; n=$x echo \$\$BUILD | wc -w $x \\\\\\
	; echo "MAKE \$\$HOST : \$\$n * \$\@"; if test "\$\$n" = "0" ; then : \\\\\\
	; BUILD=$x grep "^####.*|" Makefile |tail -1| sed -e 's/.*|//' $x ; fi \\\\\\
	; test ".\$\$BUILD" = "." && BUILD="." \\\\\\
	; test "\$\$use" = "\$\@" && BUILD=$x echo "\$\$BUILD" | tail -1 $x \\\\\\
	; for i in \$\$BUILD ; do test ".\$\$i" = "." && continue \\\\\\
	; (cd "\$\$i" && test ! -f configure && \$(MAKE) \$\$use) ; done
dnl special rule add-on: "dist" copies the tarball to $(PUB). (source tree)
/dist[]_ALL *:/a\\
	@ HOST="\$(HOST)\" \\\\\\
	; test ".\$\$HOST" = "." && HOST=$x sh $AUX/config.guess $x \\\\\\
	; BUILD=$x grep "^#### \$\$HOST " Makefile | sed -e 's/.*|//' $x \\\\\\
	; found=$x echo \$\$BUILD | wc -w $x \\\\\\
	; echo "MAKE \$\$HOST : \$\$found \$(PACKAGE)-\$(VERSION).tar.*" \\\\\\
	; if test "\$\$found" = "0" ; then : \\\\\\
	; BUILD=$x grep "^#### .*|" Makefile |tail -1| sed -e 's/.*|//' $x \\\\\\
	; fi ; for i in \$\$BUILD ; do test ".\$\$i" = "." && continue \\\\\\
	; test -f "\$\$i/\$(PACKAGE)-\$(VERSION).tar.gz" \\\\\\
	&& mv "\$\$i/\$(PACKAGE)-\$(VERSION).tar.gz" \$(PUB). ; break ; done
dnl special rule add-on: "distclean" removes all local builddirs completely
/distclean[]_ALL *:/a\\
	@ HOST="\$(HOST)\" \\\\\\
	; test ".\$\$HOST" = "." && HOST=$x sh $AUX/config.guess $x \\\\\\
	; BUILD=$x grep "^#### .*| *\\./" Makefile | sed -e 's/.*|//' $x \\\\\\
	; use=$x basename "\$\@" _ALL $x; n=$x echo \$\$BUILD | wc -w $x \\\\\\
	; echo "MAKE \$\$HOST : \$\$n * \$\@ (all local builds)" \\\\\\
	; test ".\$\$BUILD" = "." && BUILD="." \\\\\\
	; for i in \$\$BUILD ; do test ".\$\$i" = "." && continue \\\\\\
	; echo "# rm -r \$\$i"; done ; echo "# (sleep 3)" ; sleep 3 \\\\\\
	; for i in \$\$BUILD ; do test ".\$\$i" = "." && continue \\\\\\
	; echo "rm -r \$\$i"; (rm -r "\$\$i") ; done ; rm Makefile
_EOF
      cp "$tmp/conftemp.sed" "$SRC/makefile.sed~"            ## DEBUGGING
      $SED -f $tmp/conftemp.sed Makefile >$SRC/Makefile
      if test -f "$SRC/m4_ifval([$2],[$2],[END])" ; then
        AC_MSG_NOTICE([extend TOP/Makefile with TOP/m4_ifval([$2],[$2],[END])])
        cat $SRC/END >>$SRC/Makefile
      fi ; xxxx="####"
      echo "$xxxx CONFIGURATIONS FOR TOPLEVEL MAKEFILE: " >>$SRC/Makefile
      # sanity check
      if grep '^; echo "MAKE ' $SRC/Makefile >/dev/null ; then
        AC_MSG_NOTICE([buggy sed found - it deletes tab in "a" text parts])
        $SED -e '/^@ HOST=/s/^/	/' -e '/^; /s/^/	/' $SRC/Makefile \
          >$SRC/Makefile~
        (test -s $SRC/Makefile~ && mv $SRC/Makefile~ $SRC/Makefile) 2>/dev/null
      fi
    else
      xxxx="\\#\\#\\#\\#"
      # echo "/^$xxxx *$ax_enable_builddir_host /d" >$tmp/conftemp.sed
      echo "s!^$xxxx [[^|]]* | *$SUB *\$!$xxxx ...... $SUB!" >$tmp/conftemp.sed
      $SED -f "$tmp/conftemp.sed" "$SRC/Makefile" >$tmp/mkfile.tmp
        cp "$tmp/conftemp.sed" "$SRC/makefiles.sed~"         ## DEBUGGING
        cp "$tmp/mkfile.tmp"   "$SRC/makefiles.out~"         ## DEBUGGING
      if cmp -s "$SRC/Makefile" "$tmp/mkfile.tmp" 2>/dev/null ; then
        AC_MSG_NOTICE([keeping TOP/Makefile from earlier configure])
        rm "$tmp/mkfile.tmp"
      else
        AC_MSG_NOTICE([reusing TOP/Makefile from earlier configure])
        mv "$tmp/mkfile.tmp" "$SRC/Makefile"
      fi
    fi
    AC_MSG_NOTICE([build in $SUB (HOST=$ax_enable_builddir_host)])
    xxxx="####"
    echo "$xxxx" "$ax_enable_builddir_host" "|$SUB" >>$SRC/Makefile
  fi
popdef([END])dnl
AS_VAR_POPDEF([SED])dnl
AS_VAR_POPDEF([AUX])dnl
AS_VAR_POPDEF([SRC])dnl
AS_VAR_POPDEF([TOP])dnl
AS_VAR_POPDEF([SUB])dnl
],[dnl
ax_enable_builddir_srcdir="$srcdir"                    # $srcdir
ax_enable_builddir_host="$HOST"                        # $HOST / $host
ax_enable_builddir_version="$VERSION"                  # $VERSION
ax_enable_builddir_package="$PACKAGE"                  # $PACKAGE
ax_enable_builddir_auxdir="$ax_enable_builddir_auxdir" # $AUX
ax_enable_builddir_sed="$ax_enable_builddir_sed"       # $SED
ax_enable_builddir="$ax_enable_builddir"               # $SUB
])dnl
])

dnl ______ /usr/share/aclocal/guidod/ax_warning_default_pkgconfig.m4 ______
dnl @* AX_WARNING_DEFAULT_PKGCONFIGDIR [(dirvariable [,[defsetting][,[A][,[N/A]]]])]
dnl @* AX_ENABLE_DEFAULT_PKGCONFIGDIR [(dirvariable [,defsetting])]
dnl
dnl print a warning message if the $(datadir)/aclocal directory
dnl is not in the dirlist searched by the aclocal tool. This macro
dnl is useful if some `make install` would target $(datadir)/aclocal
dnl to install an autoconf m4 file of your project to be picked up
dnl by other projects.
dnl
dnl  default $1 dirvariable = pkgconfigdir
dnl  default $2 defsetting  = ${libdir}/pkgconfig
dnl  default $3 action = nothing to do
dnl  default $4 action = warn the user about mismatch
dnl
dnl In the _WARNING_ variant, the defsetting is not placed in 
dnl dirvariable nor is it ac_subst'ed in any way. The default 
dnl fail-action $4 is to send a warning message to the user,
dnl and the default accept-action $3 is nothing. It is expected that
dnl a Makefile is generated with pkgconfigdir=${libdir}/pkgconfig
dnl
dnl The _ENABLE_ variant however will set not only the $pkgconfigdir shell
dnl var of the script, but it is also AC-SUBST'ed on default - and 
dnl furthermore a configure option "--enable-default-pkgconfigdir" is 
dnl provided. Only if that option is set then $2 default is not set to 
dnl the canonic default in the a $prefix subpath but instead $2 default
dnl is set to the primary path where `pkg-config` looks for .pc files. The
dnl user may also override the default on the command line.
dnl 
dnl @: guidod@gmx.de
dnl @%Id: ax_warning_default_pkgconfig.m4,v 1.1 2003/10/18 02:57:13 guidod Exp %

AC_DEFUN([AX_WARNING_DEFAULT_PKGCONFIGDIR],[dnl
AC_REQUIRE([AX_EXPAND_PREFIX])dnl
AS_VAR_PUSHDEF([DIR],[ax_warning_default_pkgconfig_dir])dnl
AS_VAR_PUSHDEF([BIN],[ax_warning_default_pkgconfig_bin])dnl
AS_VAR_PUSHDEF([LOC],[ax_warning_default_pkgconfig_loc])dnl
LOC='m4_if([$2],,[${libdir}/pkgconfig],[$2])'
m4_ifval([$1],[test ".$[]$1" != "." && LOC="$[]$1"])
 if test ".$PKG_CONFIG" = "." ; then # we use the same default as in pkg.m4
    AC_PATH_PROG([PKG_CONFIG],[pkg-config],[no])
 fi
 if test "$PKG_CONFIG" = "no"
 then DIR="/" ; test -d "/usr/lib/pkgconfig" && DIR="/usr/lib/pkgconfig"
 else BIN=`AS_DIRNAME(["$DIR"])` ;
      if test -d "$BIN/lib/pkgconfig" ; then
          DIR="$BIN/lib/pkgconfig"
      else BIN=`AS_DIRNAME(["$DIR"])` 
      if test -d "$BIN/lib/pkgconfig" ; then
          DIR="$BIN/lib/pkgconfig"
      else
      if test -d "/usr/lib/pkgconfig" ; then
          DIR="/usr/lib/pkgconfig"
      else
          DIR="/"
      fi fi fi
 fi
AC_RUN_LOG([: last pkgconfig dir is assumed as "$DIR"])
DIR=`eval "echo $DIR"`
DIR=`eval "echo $DIR"`
LOC=`eval "echo $LOC"`
LOC=`eval "echo $LOC"`
LOC=`eval "echo $LOC"`
LOC=`eval "echo $LOC"`
for DIR in `echo "$PKG_CONFIG_PATH:$DIR" | sed -e 's,:, ,g'` ; do
    AC_RUN_LOG([: test ".$LOC" = ".$DIR"])
    test ".$LOC" = ".$DIR" && break
done
if  test "$LOC" != "$DIR" ; then
        m4_ifval([$4],[$4],[dnl
 AC_MSG_NOTICE([warning: m4_if([$1],,[pkgconfigdir],[$1])=$LOC dnl
(see config.log)])
   AC_MSG_NOTICE([perhaps: make install m4_if([$1],,[pkgconfigdir],[$1])=$DIR])
   cat m4_ifset([AS_MESSAGE_LOG_FD],[>&AS_MESSAGE_LOG_FD],[>>config.log]) <<EOF
 pkgconfigdir:  the m4_if([$1],,[default pkgconfigdir],[$1 value]) of $LOC
 pkgconfigdir:  is not listed in the dirlist where pkg-config will look for
 pkgconfigdir:  package-configs - you can override the install-path using
 pkgconfigdir:  make install m4_if([$1],,[pkgconfigdir],[$1])=$DIR
 pkgconfigdir:  or set/append the directory to the environment variable
 pkgconfigdir:  PKG_CONFIG_PATH="$LOC"
EOF
   m4_ifvaln([$5],[$5])])dnl
   m4_ifvaln([$3],[else $3])dnl
fi
AS_VAR_POPDEF([LOC])dnl
AS_VAR_POPDEF([BIN])dnl
AS_VAR_POPDEF([DIR])dnl
])

AC_DEFUN([AX_ENABLE_DEFAULT_PKGCONFIGDIR],[dnl
AS_VAR_PUSHDEF([BIN],[ax_warning_default_pkgconfig_bin])dnl
AS_VAR_PUSHDEF([DIR],[ax_warning_default_pkgconfig_def])dnl
AS_VAR_PUSHDEF([DEF],[ax_warning_default_pkgconfig_def])dnl
AC_ARG_ENABLE([enable-default-pkgconfigdir],
[  --enable-default-pkgconfigdir(=PATH) override the libdir/pkgconfig default])
test ".$enable_default_pkgconfigdir" = "." && enable_default_pkgconfigdir="no"
case ".$enable_default_pkgconfigdir" in
  .no) DIR='m4_if([$2],,[${libdir}/pkgconfig],[$2])' ;;
  .yes) # autodetect
 if test ".$PKG_CONFIG" = "." ; then # we use the same default as in pkg.m4
    AC_PATH_PROG([PKG_CONFIG],[pkg-config],[no])
 fi
 if test "$PKG_CONFIG" = "no"
 then DIR="/tmp" ; test -d "/usr/lib/pkgconfig" && DIR="/usr/lib/pkgconfig"
 else BIN=`AS_DIRNAME(["$DIR"])` ;
      if test -d "$BIN/lib/pkgconfig" ; then
          DIR="$BIN/lib/pkgconfig"
      else BIN=`AS_DIRNAME(["$DIR"])` 
      if test -d "$BIN/lib/pkgconfig" ; then
          DIR="$BIN/lib/pkgconfig"
      else
      if test -d "/usr/lib/pkgconfig" ; then
          DIR="/usr/lib/pkgconfig"
      else
          DIR="/tmp"
      fi fi fi
 fi ;;
  *) DIR="$enable_default_pkgconfigdir" ;;
esac
AX_WARNING_DEFAULT_PKGCONFIGDIR([$1],[$DEF],[$3],[$4],[$5])
m4_if([$1],,[pkgconfigdir],[$1])="$ax_warning_default_pkgconfig_dir"
AC_SUBST(m4_if([$1],,[pkgconfigdir],[$1]))
AS_VAR_POPDEF([DEF])dnl
AS_VAR_POPDEF([DIR])dnl
AS_VAR_POPDEF([BIN])dnl
])

dnl ______ /usr/share/aclocal/Miscellaneous/ac_compile_check_sizeof.m4 ______
dnl @synopsis AC_COMPILE_CHECK_SIZEOF(TYPE [, HEADERS [, EXTRA_SIZES...]])
dnl
dnl This macro checks for the size of TYPE using compile checks, not
dnl run checks. You can supply extra HEADERS to look into. the check
dnl will cycle through 1 2 4 8 16 and any EXTRA_SIZES the user
dnl supplies. If a match is found, it will #define SIZEOF_`TYPE' to
dnl that value. Otherwise it will emit a configure time error
dnl indicating the size of the type could not be determined.
dnl
dnl The trick is that C will not allow duplicate case labels. While
dnl this is valid C code:
dnl
dnl      switch (0) case 0: case 1:;
dnl
dnl The following is not:
dnl
dnl      switch (0) case 0: case 0:;
dnl
dnl Thus, the AC_TRY_COMPILE will fail if the currently tried size
dnl does not match.
dnl
dnl Here is an example skeleton configure.in script, demonstrating the
dnl macro's usage:
dnl
dnl      AC_PROG_CC
dnl      AC_CHECK_HEADERS(stddef.h unistd.h)
dnl      AC_TYPE_SIZE_T
dnl      AC_CHECK_TYPE(ssize_t, int)
dnl
dnl      headers='#ifdef HAVE_STDDEF_H
dnl      #include <stddef.h>
dnl      #endif
dnl      #ifdef HAVE_UNISTD_H
dnl      #include <unistd.h>
dnl      #endif
dnl      '
dnl
dnl      AC_COMPILE_CHECK_SIZEOF(char)
dnl      AC_COMPILE_CHECK_SIZEOF(short)
dnl      AC_COMPILE_CHECK_SIZEOF(int)
dnl      AC_COMPILE_CHECK_SIZEOF(long)
dnl      AC_COMPILE_CHECK_SIZEOF(unsigned char *)
dnl      AC_COMPILE_CHECK_SIZEOF(void *)
dnl      AC_COMPILE_CHECK_SIZEOF(size_t, $headers)
dnl      AC_COMPILE_CHECK_SIZEOF(ssize_t, $headers)
dnl      AC_COMPILE_CHECK_SIZEOF(ptrdiff_t, $headers)
dnl      AC_COMPILE_CHECK_SIZEOF(off_t, $headers)
dnl
dnl @author Kaveh Ghazi <ghazi@caip.rutgers.edu>
dnl @version %Id: ac_compile_check_sizeof.m4,v 1.1.1.1 2001/07/26 00:46:24 guidod Exp %
dnl
AC_DEFUN([AC_COMPILE_CHECK_SIZEOF],
[changequote(<<, >>)dnl
dnl The name to #define.
define(<<AC_TYPE_NAME>>, translit(sizeof_$1, [a-z *], [A-Z_P]))dnl
dnl The cache variable name.
define(<<AC_CV_NAME>>, translit(ac_cv_sizeof_$1, [ *], [_p]))dnl
changequote([, ])dnl
AC_MSG_CHECKING(size of $1)
AC_CACHE_VAL(AC_CV_NAME,
[for ac_size in 4 8 1 2 16 $2 ; do # List sizes in rough order of prevalence.
  AC_TRY_COMPILE([#include "confdefs.h"
#include <sys/types.h>
$2
], [switch (0) case 0: case (sizeof ($1) == $ac_size):;], AC_CV_NAME=$ac_size)
  if test x$AC_CV_NAME != x ; then break; fi
done
])
if test x$AC_CV_NAME = x ; then
  AC_MSG_ERROR([cannot determine a size for $1])
fi
AC_MSG_RESULT($AC_CV_NAME)
AC_DEFINE_UNQUOTED(AC_TYPE_NAME, $AC_CV_NAME, [The number of bytes in type $1])
undefine([AC_TYPE_NAME])dnl
undefine([AC_CV_NAME])dnl
])

dnl ______ /usr/share/aclocal/guidod/patch_libtool_to_add_host_cc.m4 ______
dnl @* PATCH_LIBTOOL_TO_ADD_HOST_CC
dnl
dnl The libtool 1.4.x processing (and patched 1.3.5) uses a little
dnl "impgen" tool to turn a "*.dll" into an import "*.lib" as it is
dnl needed for win32 targets. However, this little tool is not shipped
dnl by binutils, it is not even a command option of dlltool or dllwrap.
dnl It happens to be a C source snippet implanted into the libtool
dnl sources - it gets written to ".libs", compiled into a binary 
dnl on-the-fly, and executed right away on the "dll" file to create
dnl the import-lib (dll.a files in gcc-speak).
dnl
dnl This mode works fine for a native build within mingw or cygwin,
dnl but it does not work in cross-compile mode since CC is a
dnl crosscompiler - it will create an .exe file on a non-win32
dnl system, and as a result an impgen.exe is created on-the-fly
dnl that can not be executed on-the-fly. Luckily, the actual
dnl libtool snippet uses HOST_CC to compile the sources which
dnl has a fallback to CC when the HOST_CC variable was not set.
dnl
dnl this ac-macro is trying to detect a valid HOST_CC which is not
dnl a cross-compiler. This is done by looking into the $PATH for
dnl a "cc" and the result is patched into libtool a HOST_CC, iow
dnl it adds another configured variable at the top of the libtool
dnl script.
dnl
dnl In discussions on the libtool mailinglist it occurred that
dnl later gcc/binutils generations are able to link with dlls 
dnl directly, i.e. there is no import-lib needed anymore. The
dnl import-table is created within the linker itself (in-memory)
dnl and bound to the .exe/.dll currently in the making. The
dnl whole stuff of impgen exe and compiling it on-the-fly, well,
dnl it is superflouos then. 
dnl
dnl Since mingw crosscompilers tend to be quite a fresh development
dnl it was agreed to remove the impgen stuff completly from
dnl libtool sources. Still however, this macro does not hurt
dnl since it does not patch impgen cmds but it just adds HOST_CC
dnl which might be useful in other cross-compiling cases as well.
dnl Therefore, you can leave it in for maximum compatibility and
dnl portability.
dnl
dnl @= guidod@gmx.de
dnl @%Id: patch_libtool_to_add_host_cc.m4,v 1.3 2003/03/23 13:20:27 guidod Exp %

AC_DEFUN([PATCH_LIBTOOL_TO_ADD_HOST_CC],
[# patch libtool to add HOST_CC sometimes needed in crosscompiling a win32 dll
if grep "HOST_CC" libtool >/dev/null; then
  if test "$build" != "$host" ; then
    if test "_$HOST_CC" = "_" ; then
      HOST_CC="false"
      for i in `echo $PATH | sed 's,:, ,g'` ; do
      test -x $i/cc && HOST_CC=$i/cc
      done
    fi
AC_MSG_RESULT(patching libtool to add HOST_CC=$HOST_CC)
    test -f libtool.old || (mv libtool libtool.old && cp libtool.old libtool)
    sed -e "/BEGIN.*LIBTOOL.*CONFIG/a\\
HOST_CC=$HOST_CC" libtool >libtool.new
    (test -s libtool.new || rm libtool.new) 2>/dev/null
    test -f libtool.new && mv libtool.new libtool # not 2>/dev/null !!
    test -f libtool     || mv libtool.old libtool
  fi
fi
])
dnl ______ /usr/share/aclocal/guidod/patch_libtool_on_darwin_zsh_overquoting.m4 ______
dnl @* PATCH_LIBTOOL_ON_DARWIN_ZSH_OVERQUOTING
dnl
dnl libtool 1.4.x has a bug on darwin where the "zsh" is installed
dnl as the bourne shell replacement. Of course, the zsh is called in
dnl a compatibility mode but there is a common problem with it,
dnl probably a bug of zsh. Newer darwin systems have a "bash"
dnl installed now, but the configure-default will be "zsh" in most
dnl systems still.
dnl
dnl The bug revelas itself as an overquoted statement in the
dnl libtool cmds-spec for sharedlib creation on testing for
dnl "module" builds. Later libtool has gone rid of it by simply
dnl removing the quotes at that point . Here we maintain the 
dnl original style and simply remove the extra escape character,
dnl i.e. we look for "archive_cmds" and replace a sequence of
dnl triple-backslash-and-doublequote with single-backslash-and-doublequote.
dnl 
dnl @= guidod@gmx.de
dnl %Id: patch_libtool_on_darwin_zsh_overquoting.m4,v 1.4 2003/03/24 16:58:21 guidod Exp %

AC_DEFUN([PATCH_LIBTOOL_ON_DARWIN_ZSH_OVERQUOTING],
[# libtool-1.4 specific, on zsh target the final requoting does one too much
case "$host_os" in
  darwin*)
    if grep "1.92" libtool >/dev/null ; then
AC_MSG_RESULT(patching libtool on .so-sharedlib creation (zsh overquoting))
      test -f libtool.old || (mv libtool libtool.old && cp libtool.old libtool)
      sed -e '/archive_cmds=/s:[[\\]][[\\]][[\\]]*":\\":g' libtool >libtool.new
      (test -s libtool.new || rm libtool.new) 2>/dev/null
      test -f libtool.new && mv libtool.new libtool # not 2>/dev/null !!
      test -f libtool     || mv libtool.old libtool
    fi
  ;;
esac
])
dnl ______ /usr/share/aclocal/guidod/ac_sys_largefile_sensitive.m4 ______
dnl @synopsis AC_SYS_LARGEFILE_SENSITIVE
dnl
dnl checker whether the current system is sensitive to -Ddefines
dnl making off_t having different types/sizes. Automatically define
dnl a config.h symbol LARGEFILE_SENSITIVE if that is the case,
dnl otherwise leave everything as is. 
dnl
dnl This macro builds on top of AC_SYS_LARGEFILE to detect whether
dnl special options are neede to make the code use 64bit off_t - in
dnl many setups this will also make the code use 64bit off_t immediatly.
dnl
dnl The common use of a LARGEFILE_SENSITIVE config.h-define is to rename
dnl exported functions, usually adding a 64 to the original function name.
dnl Such renamings are only needed on systems being both (a) 32bit off_t
dnl by default and (b) implementing large.file extensions (as for unix98).
dnl
dnl a renaming section could look like this:
dnl  #if defined LARGEFILE_SENSITIVE && _FILE_OFFSET_BITS+0 == 64
dnl  #define zzip_open zzip_open64
dnl  #define zzip_seek zzip_seek64
dnl  #endif
dnl
dnl for libraries, it is best to take advantage of the prefix-config.h
dnl macro, otherwise you want to export a renamed LARGEFILE_SENSITIVE
dnl in an installed header file. -> see AX_PREFIX_CONFIG_H
dnl
dnl @, System Headers
dnl @Author Guido Draheim <guidod@gmx.de>
dnl @Version %Id: ac_sys_largefile_sensitive.m4,v 1.2 2003/05/13 18:46:22 guidod Exp %

AC_DEFUN([AC_SYS_LARGEFILE_SENSITIVE],[dnl
AC_REQUIRE([AC_SYS_LARGEFILE])dnl
# we know about some internals of ac_sys_largefile here...
AC_MSG_CHECKING(whether system differentiates 64bit off_t by defines)
ac_cv_sys_largefile_sensitive="no"
if test ".$ac_cv_sys_file_offset_bits$ac_cv_sys_large_files" != ".nono"
then ac_cv_sys_largefile_sensitive="yes" 
  AC_DEFINE(LARGEFILE_SENSITIVE, 1,
  [whether the system defaults to 32bit off_t but can do 64bit when requested])
fi
AC_MSG_RESULT([$ac_cv_sys_largefile_sensitive])
])

dnl ______ /usr/share/aclocal/guidod/ax_maintainer_mode_auto_silent.m4 ______
dnl @* AX_MAINTAINER_MODE_AUTO_SILENT
dnl
dnl Set autotools to error/sleep settings so that they are not run when
dnl being errornously triggered. Likewise make libtool-silent when 
dnl libtool has been used.
dnl
dnl I am using the macro quite a lot since some automake versions had the
dnl tendency to try to rerun some autotools on a mere make even when not
dnl quite in --maintainer-mode. That is very annoying. Likewise, a user
dnl who installs from source does not want to see doubled compiler messages.
dnl
dnl I did not put an AC-REQUIRE(MAINTAINER_MODE) in here - should I?
dnl
dnl @: guidod@gmx.de
dnl @%Id: ax_maintainer_mode_auto_silent.m4,v 1.1 2003/10/19 00:12:45 guidod Exp %

AC_DEFUN([AX_MAINTAINER_MODE_AUTO_SILENT],[dnl
dnl ac_REQUIRE([am_MAINTAINER_MODE])dn
AC_MSG_CHECKING(auto silent in maintainer mode)
if test "$USE_MAINTAINER_MODE" = "no" ; then
   test ".$TIMEOUT" = "." && TIMEOUT="9"
   AUTOHEADER="sleep $TIMEOUT ; true || autoheader || skipped"
   AUTOMAKE="sleep $TIMEOUT ; true || automake || skipped"
   AUTOCONF="sleep $TIMEOUT ; true || autoconf || skipped"
   if test ".$LIBTOOL" != "." ; then
      LIBTOOL="$LIBTOOL --silent"
      AC_MSG_RESULT([libtool-silent, auto-sleep-9])
   else
      AC_MSG_RESULT([auto-sleep-9])
   fi
else
      AC_MSG_RESULT([no])
fi
])

dnl ______ /usr/share/aclocal/guidod/ax_expand_prefix.m4 ______
dnl @* AX_EXPAND_PREFIX
dnl
dnl when $prefix and $exec_prefix are still set to NONE then set them
dnl to the usual default values - being based on $ac_default_prefix.
dnl - this macro can be AC_REQUIREd by other macros that need to
dnl compute values for installation directories. It has been observed
dnl that it was done wrong over and over again, so this is a bit
dnl more safe to do.
dnl
dnl remember - setting exec_prefix='${prefix}' needs you interpolate
dnl directories multiple times, it is not sufficient to just say
dnl MYVAR="${datadir}/putter" but you do have to run `eval` a few 
dnl times, sth. like MYVAR=`eval "echo \"$MYVAR\""` done atleast
dnl two times.
dnl 
dnl The implementation of this macro simply picks up the lines that
dnl would be run at the start of AC_OUTPUT anyway to set the
dnl prefix/exec_prefix defaults. Between AC_INIT and the first
dnl command to AC_REQUIRE this macro you can set the two variables
dnl to something explicit instead. Probably, any command to compute
dnl installation directories should be run _after_ AM_INIT_AUTOMAKE
dnl
dnl @: guidod@gmx.de
dnl @%Id: ax_expand_prefix.m4,v 1.1 2003/10/19 00:05:18 guidod Exp %

AC_DEFUN([AX_EXPAND_PREFIX],[dnl
  # The prefix default can be set in configure.ac (otherwise it is /usr/local)
  test "x$prefix" = xNONE && prefix=$ac_default_prefix
  # Let make expand exec_prefix. Allows to override the makevar 'prefix' later
  test "x$exec_prefix" = xNONE && exec_prefix='${prefix}'
])

dnl ______ /usr/share/aclocal/guidod/ax_create_pkgconfig_info.m4 ______
dnl @* AX_CREATE_PKGCONFIG_INFO [(outputfile, [requires [,libs [,summary]]])]
dnl defaults:
dnl   $1 = $PACKAGE_NAME.pc
dnl   $2 = (empty)
dnl   $3 = $PACKAGE_LIBS $LIBS (as set at that point in configure.ac)
dnl   $4 = $PACKAGE_SUMMARY (or $1 Library)
dnl   $5 = $CPPFLAGS $PACKAGE_CFLAGS (as set at the point in configure.ac)
dnl
dnl   PACKAGE_NAME defaults to $PACKAGE if not set.
dnl   PACKAGE_LIBS defaults to -l$PACKAGE_NAME if not set.
dnl
dnl the resulting file is called $PACKAGE.pc.in / $PACKAGE.pc
dnl
dnl You will find this macro most useful in conjunction with ax_spec_defaults
dnl that can read good initializers from the .spec file. In consequencd, most
dnl of the generatable installable stuff can be made from information being
dnl updated in a single place for the whole project.
dnl
dnl @%Id: ax_create_pkgconfig_info.m4,v 1.1 2003/10/19 00:08:51 guidod Exp %

AC_DEFUN([AX_CREATE_PKGCONFIG_INFO],[dnl
AS_VAR_PUSHDEF([PKGCONFIG_libdir],[ax_create_pkgconfig_libdir])dnl
AS_VAR_PUSHDEF([PKGCONFIG_libfile],[ax_create_pkgconfig_libfile])dnl
AS_VAR_PUSHDEF([PKGCONFIG_libname],[ax_create_pkgconfig_libname])dnl
AS_VAR_PUSHDEF([PKGCONFIG_version],[ax_create_pkgconfig_version])dnl
AS_VAR_PUSHDEF([PKGCONFIG_description],[ax_create_pkgconfig_description])dnl
AS_VAR_PUSHDEF([PKGCONFIG_requires],[ax_create_pkgconfig_requires])dnl
AS_VAR_PUSHDEF([PKGCONFIG_pkglibs],[ax_create_pkgconfig_pkglibs])dnl
AS_VAR_PUSHDEF([PKGCONFIG_libs],[ax_create_pkgconfig_libs])dnl
AS_VAR_PUSHDEF([PKGCONFIG_cppflags],[ax_create_pkgconfig_cppflags])dnl
AS_VAR_PUSHDEF([PKGCONFIG_generate],[ax_create_pkgconfig_generate])dnl
AS_VAR_PUSHDEF([PKGCONFIG_src_libdir],[ax_create_pkgconfig_src_libdir])dnl
AS_VAR_PUSHDEF([PKGCONFIG_src_headers],[ax_create_pkgconfig_src_headers])dnl

# we need the expanded forms...
test "x$prefix" = xNONE && prefix=$ac_default_prefix
test "x$exec_prefix" = xNONE && exec_prefix='${prefix}'

AC_MSG_CHECKING(our pkgconfig libname)
test ".$PKGCONFIG_libname" != "." || \
PKGCONFIG_libname="ifelse($1,,${PACKAGE_NAME},`basename $1 .pc`)"
test ".$PKGCONFIG_libname" != "." || \
PKGCONFIG_libname="$PACKAGE"
PKGCONFIG_libname=`eval echo "$PKGCONFIG_libname"`
PKGCONFIG_libname=`eval echo "$PKGCONFIG_libname"`
AC_MSG_RESULT($PKGCONFIG_libname)

AC_MSG_CHECKING(our pkgconfig version)
test ".$PKGCONFIG_version" != "." || \
PKGCONFIG_version="${PACKAGE_VERSION}"
test ".$PKGCONFIG_version" != "." || \
PKGCONFIG_version="$VERSION"
PKGCONFIG_version=`eval echo "$PKGCONFIG_version"`
PKGCONFIG_version=`eval echo "$PKGCONFIG_version"`
AC_MSG_RESULT($PKGCONFIG_version)

AC_MSG_CHECKING(our pkgconfig_libdir)
test ".$pkgconfig_libdir" = "." && \
pkgconfig_libdir='${libdir}/pkgconfig'
PKGCONFIG_libdir=`eval echo "$pkgconfig_libdir"`
PKGCONFIG_libdir=`eval echo "$PKGCONFIG_libdir"`
PKGCONFIG_libdir=`eval echo "$PKGCONFIG_libdir"`
AC_MSG_RESULT($pkgconfig_libdir)
test "$pkgconfig_libdir" != "$PKGCONFIG_libdir" && (
AC_MSG_RESULT(expanded our pkgconfig_libdir... $PKGCONFIG_libdir))
AC_SUBST([pkgconfig_libdir])

AC_MSG_CHECKING(our pkgconfig_libfile)
test ".$pkgconfig_libfile" != "." || \
pkgconfig_libfile="ifelse($1,,$PKGCONFIG_libname.pc,`basename $1`)"
PKGCONFIG_libfile=`eval echo "$pkgconfig_libfile"`
PKGCONFIG_libfile=`eval echo "$PKGCONFIG_libfile"`
AC_MSG_RESULT($pkgconfig_libfile)
test "$pkgconfig_libfile" != "$PKGCONFIG_libfile" && (
AC_MSG_RESULT(expanded our pkgconfig_libfile... $PKGCONFIG_libfile))
AC_SUBST([pkgconfig_libfile])

AC_MSG_CHECKING(our pkgconfig description)
PKGCONFIG_description="ifelse($4,,$PACKAGE_SUMMARY,$4)"
test ".$PKGCONFIG_description" != "." || \
PKGCONFIG_description="$PKGCONFIG_libname Library"
PKGCONFIG_description=`eval echo "$PKGCONFIG_description"`
PKGCONFIG_description=`eval echo "$PKGCONFIG_description"`
AC_MSG_RESULT($PKGCONFIG_description)

AC_MSG_CHECKING(our pkgconfig requires)
PKGCONFIG_requires="ifelse($2,,$PACKAGE_REQUIRES,$2)"
PKGCONFIG_requires=`eval echo "$PKGCONFIG_requires"`
PKGCONFIG_requires=`eval echo "$PKGCONFIG_requires"`
AC_MSG_RESULT($PKGCONFIG_requires)

AC_MSG_CHECKING(our pkgconfig ext libs)
PKGCONFIG_pkglibs="$PACKAGE_LIBS"
test ".$PKGCONFIG_pkglibs" != "." || PKGCONFIG_pkglibs="-l$PKGCONFIG_libname"
PKGCONFIG_libs="ifelse($3,,$PKGCONFIG_pkglibs $LIBS,$3)"
PKGCONFIG_libs=`eval echo "$PKGCONFIG_libs"`
PKGCONFIG_libs=`eval echo "$PKGCONFIG_libs"`
AC_MSG_RESULT($PKGCONFIG_libs)

AC_MSG_CHECKING(our pkgconfig cppflags)
PKGCONFIG_cppflags="ifelse($5,,$CPPFLAGS $PACKAGE_CFLAGS,$5)"
PKGCONFIG_cppflags=`eval echo "$PKGCONFIG_cppflags"`
PKGCONFIG_cppflags=`eval echo "$PKGCONFIG_cppflags"`
AC_MSG_RESULT($PKGCONFIG_cppflags)

test ".$PKGCONFIG_generate" != "." || \
PKGCONFIG_generate="ifelse($1,,$PKGCONFIG_libname.pc,$1)"
PKGCONFIG_generate=`eval echo "$PKGCONFIG_generate"`
PKGCONFIG_generate=`eval echo "$PKGCONFIG_generate"`
test "$pkgconfig_libfile" != "$PKGCONFIG_generate" && (
AC_MSG_RESULT(generate the pkgconfig later... $PKGCONFIG_generate))

if test ".$PKGCONFIG_src_libdir" = "." ; then
PKGCONFIG_src_libdir=`pwd`
PKGCONFIG_src_libdir=`AX_DIRNAME("$PKGCONFIG_src_libdir/$PKGCONFIG_generate")`
test ! -d $PKGCONFIG_src_libdir/src || \
PKGCONFIG_src_libdir="$PKGCONFIG_src_libdir/src"
case ".$objdir" in 
*libs) PKGCONFIG_src_libdir="$PKGCONFIG_src_libdir/$objdir" ;; esac
AC_MSG_RESULT(noninstalled pkgconfig -L $PKGCONFIG_src_libdir)
fi

if test ".$PKGCONFIG_src_headers" = "." ; then
PKGCONFIG_src_headers=`pwd`
v="$ac_top_srcdir" ; 
test ".$v" != "." || v="$ax_spec_dir"
test ".$v" != "." || v="$srcdir"
case "$v" in /*) PKG_CONFIG_src_headers="" ;; esac
PKGCONFIG_src_headers=`AX_DIRNAME("$PKGCONFIG_src_headers/$v/x")`
test ! -d $PKGCONFIG_src_headers/incl[]ude || \
PKGCONFIG_src_headers="$PKGCONFIG_src_headers/incl[]ude"
AC_MSG_RESULT(noninstalled pkgconfig -I $PKGCONFIG_src_headers)
fi

dnl AC_CONFIG_COMMANDS crap disallows to use $PKGCONFIG_libfile here...
AC_CONFIG_COMMANDS([$ax_create_pkgconfig_generate],[ 
pkgconfig_generate="$ax_create_pkgconfig_generate"
if test ! -f "$pkgconfig_generate.in" 
then generate="true"
elif grep ' generated by configure ' $pkgconfig_generate.in >/dev/null 
then generate="true"
else generate="false"; 
fi
if $generate ; then
AC_MSG_NOTICE(creating $pkgconfig_generate.in)
cat > $pkgconfig_generate.in <<_EOF
# generated by configure / remove this line to disable regeneration
prefix=@prefix@
exec_prefix=@exec_prefix@
libdir=@libdir@
includedir=@includedir@

Name: @PACKAGE_NAME@
Description: @PACKAGE_DESCRIPTION@
Version: @PACKAGE_VERSION@
Requires: @PACKAGE_REQUIRES@
Libs: -L\${libdir} @LIBS@
Cflags: -I\${includedir} @CPPFLAGS@
_EOF
fi # DONE generate $pkgconfig_generate.in
AC_MSG_NOTICE(creating $pkgconfig_generate)
sed -e "s|@prefix@|${pkgconfig_prefix}|" \
    -e "s|@exec_prefix@|${pkgconfig_execprefix}|" \
    -e "s|@libdir@|${pkgconfig_libdir}|" \
    -e "s|@includedir@|${pkgconfig_includedir}|" \
    -e "s|@PACKAGE_NAME@|${pkgconfig_libname}|" \
    -e "s|@PACKAGE_DESCRIPTION@|${pkgconfig_description}|" \
    -e "s|@PACKAGE_VERSION@|${pkgconfig_version}|" \
    -e "s|@PACKAGE_REQUIRES@|${pkgconfig_requires}|" \
    -e "s|@LIBS@|${pkgconfig_libs}|" \
    -e "s|@CPPFLAGS@|${pkgconfig_cppflags}|" \
    $pkgconfig_generate.in > $pkgconfig_generate
if test ! -s $pkgconfig_generate ; then 
    AC_MSG_ERROR([$pkgconfig_generate is empty])
fi # DONE generate $pkgconfig_generate
pkgconfig_uninstalled=`echo $pkgconfig_generate |sed 's/.pc$/-uninstalled.pc/'`
AC_MSG_NOTICE(creating $pkgconfig_uninstalled)
sed -e "s|@prefix@|${pkgconfig_prefix}|" \
    -e "s|@exec_prefix@|${pkgconfig_execprefix}|" \
    -e "s|@libdir@|${pkgconfig_src_libdir}|" \
    -e "s|@includedir@|${pkgconfig_src_headers}|" \
    -e "s|@PACKAGE_NAME@|${pkgconfig_libname}|" \
    -e "s|@PACKAGE_DESCRIPTION@|${pkgconfig_description}|" \
    -e "s|@PACKAGE_VERSION@|${pkgconfig_version}|" \
    -e "s|@PACKAGE_REQUIRES@|${pkgconfig_requires}|" \
    -e "s|@LIBS@|${pkgconfig_libs}|" \
    -e "s|@CPPFLAGS@|${pkgconfig_cppflags}|" \
    $pkgconfig_generate.in > $pkgconfig_uninstalled
if test ! -s $pkgconfig_uninstalled ; then 
    AC_MSG_ERROR([$pkgconfig_uninstalled is empty])
fi # DONE generate $pkgconfig_uninstalled
],[
dnl AC_CONFIG_COMMANDS crap, the AS_PUSHVAR defines are invalid here...
ax_create_pkgconfig_generate="$ax_create_pkgconfig_generate"
pkgconfig_prefix='$prefix'
pkgconfig_execprefix='$exec_prefix'
pkgconfig_libdir='$libdir'
pkgconfig_includedir='$includedir'
pkgconfig_libname='$ax_create_pkgconfig_libname'
pkgconfig_description='$ax_create_pkgconfig_description'
pkgconfig_version='$ax_create_pkgconfig_version'
pkgconfig_requires='$ax_create_pkgconfig_requires'
pkgconfig_libs='$ax_create_pkgconfig_libs'
pkgconfig_cppflags='$ax_create_pkgconfig_cppflags'
pkgconfig_src_libdir='$ax_create_pkgconfig_src_libdir'
pkgconfig_src_headers='$ax_create_pkgconfig_src_headers'
])dnl
AS_VAR_POPDEF([PKGCONFIG_libdir])dnl
AS_VAR_POPDEF([PKGCONFIG_libfile])dnl
AS_VAR_POPDEF([PKGCONFIG_libname])dnl
AS_VAR_POPDEF([PKGCONFIG_version])dnl
AS_VAR_POPDEF([PKGCONFIG_description])dnl
AS_VAR_POPDEF([PKGCONFIG_requires])dnl
AS_VAR_POPDEF([PKGCONFIG_pkglibs])dnl
AS_VAR_POPDEF([PKGCONFIG_libs])dnl
AS_VAR_POPDEF([PKGCONFIG_cppflags])dnl
AS_VAR_POPDEF([PKGCONFIG_generate])dnl
AS_VAR_POPDEF([PKGCONFIG_src_libdir])dnl
AS_VAR_POPDEF([PKGCONFIG_src_headers])dnl
])

dnl ______ /usr/share/aclocal/guidod/ax_prefix_config_h.m4 ______
dnl @synopsis AX_PREFIX_CONFIG_H [(OUTPUT-HEADER [,PREFIX [,ORIG-HEADER]])]
dnl
dnl This is a new variant from ac_prefix_config_ this one will use a
dnl lowercase-prefix if the config-define was starting with a
dnl lowercase-char, e.g. "#define const", "#define restrict", or
dnl "#define off_t", (and this one can live in another directory, e.g.
dnl testpkg/config.h therefore I decided to move the output-header to
dnl be the first arg)
dnl
dnl takes the usual config.h generated header file; looks for each of
dnl the generated "#define SOMEDEF" lines, and prefixes the defined name
dnl (ie. makes it "#define PREFIX_SOMEDEF". The result is written to
dnl the output config.header file. The PREFIX is converted to uppercase
dnl for the conversions.
dnl
dnl Defaults:
dnl
dnl   OUTPUT-HEADER = $PACKAGE-config.h
dnl   PREFIX = $PACKAGE
dnl   ORIG-HEADER, from AM_CONFIG_HEADER(config.h)
dnl
dnl Your configure.ac script should contain both macros in this order,
dnl and unlike the earlier variations of this prefix-macro it is okay to
dnl place the AX_PREFIX_CONFIG_H call before the AC_OUTPUT invokation.
dnl
dnl Example:
dnl
dnl   AC_INIT(config.h.in)        # config.h.in as created by "autoheader"
dnl   AM_INIT_AUTOMAKE(testpkg, 0.1.1)    # makes #undef VERSION and PACKAGE
dnl   AM_CONFIG_HEADER(config.h)          # prep config.h from config.h.in
dnl   AX_PREFIX_CONFIG_H(mylib/_config.h) # prep mylib/_config.h from it..
dnl   AC_MEMORY_H                         # makes "#undef NEED_MEMORY_H"
dnl   AC_C_CONST_H                        # makes "#undef const"
dnl   AC_OUTPUT(Makefile)                 # creates the "config.h" now
dnl                                       # and also mylib/_config.h
dnl
dnl if the argument to AX_PREFIX_CONFIG_H would have been omitted then the
dnl default outputfile would have been called simply "testpkg-config.h", but
dnl even under the name "mylib/_config.h" it contains prefix-defines like
dnl
dnl   #ifndef TESTPKG_VERSION
dnl   #define TESTPKG_VERSION "0.1.1"
dnl   #endif
dnl   #ifndef TESTPKG_NEED_MEMORY_H
dnl   #define TESTPKG_NEED_MEMORY_H 1
dnl   #endif
dnl   #ifndef _testpkg_const
dnl   #define _testpkg_const _const
dnl   #endif
dnl
dnl and this "mylib/_config.h" can be installed along with other
dnl header-files, which is most convenient when creating a shared
dnl library (that has some headers) where some functionality is
dnl dependent on the OS-features detected at compile-time. No
dnl need to invent some "mylib-confdefs.h.in" manually. :-)
dnl
dnl Note that some AC_DEFINEs that end up in the config.h file are
dnl actually self-referential - e.g. AC_C_INLINE, AC_C_CONST, and the
dnl AC_TYPE_OFF_T say that they "will define inline|const|off_t if the
dnl system does not do it by itself". You might want to clean up about
dnl these - consider an extra mylib/conf.h that reads something like:
dnl
dnl    #include <mylib/_config.h>
dnl    #ifndef _testpkg_const
dnl    #define _testpkg_const const
dnl    #endif
dnl
dnl and then start using _testpkg_const in the header files. That is
dnl also a good thing to differentiate whether some library-user has
dnl starting to take up with a different compiler, so perhaps it could
dnl read something like this:
dnl
dnl   #ifdef _MSC_VER
dnl   #include <mylib/_msvc.h>
dnl   #else
dnl   #include <mylib/_config.h>
dnl   #endif
dnl   #ifndef _testpkg_const
dnl   #define _testpkg_const const
dnl   #endif
dnl
dnl @version %Id: ax_prefix_config_h.m4,v 1.5 2003/07/02 23:29:23 guidod Exp %
dnl @author  Guiodo Draheim <guidod@gmx.de>
dnl
AC_DEFUN([AX_PREFIX_CONFIG_H],[AC_REQUIRE([AC_CONFIG_HEADER])
AC_CONFIG_COMMANDS([ifelse($1,,$PACKAGE-config.h,$1)],[dnl
AS_VAR_PUSHDEF([_OUT],[ac_prefix_conf_OUT])dnl
AS_VAR_PUSHDEF([_DEF],[ac_prefix_conf_DEF])dnl
AS_VAR_PUSHDEF([_PKG],[ac_prefix_conf_PKG])dnl
AS_VAR_PUSHDEF([_LOW],[ac_prefix_conf_LOW])dnl
AS_VAR_PUSHDEF([_UPP],[ac_prefix_conf_UPP])dnl
AS_VAR_PUSHDEF([_INP],[ac_prefix_conf_INP])dnl
m4_pushdef([_script],[conftest.prefix])dnl
m4_pushdef([_symbol],[m4_cr_Letters[]m4_cr_digits[]_])dnl
_OUT=`echo ifelse($1, , $PACKAGE-config.h, $1)`
_DEF=`echo _$_OUT | sed -e "y:m4_cr_letters:m4_cr_LETTERS[]:" -e "s/@<:@^m4_cr_Letters@:>@/_/g"`
_PKG=`echo ifelse($2, , $PACKAGE, $2)`
_LOW=`echo _$_PKG | sed -e "y:m4_cr_LETTERS-:m4_cr_letters[]_:"`
_UPP=`echo $_PKG | sed -e "y:m4_cr_letters-:m4_cr_LETTERS[]_:"  -e "/^@<:@m4_cr_digits@:>@/s/^/_/"`
_INP=`echo "ifelse($3,,,$3)" | sed -e 's/ *//'`
if test ".$_INP" = "."; then
   for ac_file in : $CONFIG_HEADERS; do test "_$ac_file" = _: && continue
     case "$ac_file" in
        *.h) _INP=$ac_file ;;
        *)
     esac
     test ".$_INP" != "." && break
   done
fi
if test ".$_INP" = "."; then
   case "$_OUT" in
      */*) _INP=`basename "$_OUT"`
      ;;
      *-*) _INP=`echo "$_OUT" | sed -e "s/@<:@_symbol@:>@*-//"`
      ;;
      *) _INP=config.h
      ;;
   esac
fi
if test -z "$_PKG" ; then
   AC_MSG_ERROR([no prefix for _PREFIX_PKG_CONFIG_H])
else
  if test ! -f "$_INP" ; then if test -f "$srcdir/$_INP" ; then
     _INP="$srcdir/$_INP"
  fi fi
  AC_MSG_NOTICE(creating $_OUT - prefix $_UPP for $_INP defines)
  if test -f $_INP ; then
    echo "s/@%:@undef  *\\(@<:@m4_cr_LETTERS[]_@:>@\\)/@%:@undef $_UPP""_\\1/" > _script
    echo "s/@%:@undef  *\\(@<:@m4_cr_letters@:>@\\)/@%:@undef $_LOW""_\\1/" >> _script
    echo "s/@%:@def[]ine  *\\(@<:@m4_cr_LETTERS[]_@:>@@<:@_symbol@:>@*\\)\\(.*\\)/@%:@ifndef $_UPP""_\\1 \\" >> _script
    echo "@%:@def[]ine $_UPP""_\\1 \\2 \\" >> _script
    echo "@%:@endif/" >>_script
    echo "s/@%:@def[]ine  *\\(@<:@m4_cr_letters@:>@@<:@_symbol@:>@*\\)\\(.*\\)/@%:@ifndef $_LOW""_\\1 \\" >> _script
    echo "@%:@define $_LOW""_\\1 \\2 \\" >> _script
    echo "@%:@endif/" >> _script
    # now executing _script on _DEF input to create _OUT output file
    echo "@%:@ifndef $_DEF"      >$tmp/pconfig.h
    echo "@%:@def[]ine $_DEF 1" >>$tmp/pconfig.h
    echo ' ' >>$tmp/pconfig.h
    echo /'*' $_OUT. Generated automatically at end of configure. '*'/ >>$tmp/pconfig.h

    sed -f _script $_INP >>$tmp/pconfig.h
    echo ' ' >>$tmp/pconfig.h
    echo '/* once:' $_DEF '*/' >>$tmp/pconfig.h
    echo "@%:@endif" >>$tmp/pconfig.h
    if cmp -s $_OUT $tmp/pconfig.h 2>/dev/null; then
      AC_MSG_NOTICE([$_OUT is unchanged])
    else
      ac_dir=`AS_DIRNAME(["$_OUT"])`
      AS_MKDIR_P(["$ac_dir"])
      rm -f "$_OUT"
      mv $tmp/pconfig.h "$_OUT"
    fi
    cp _script _configs.sed
  else
    AC_MSG_ERROR([input file $_INP does not exist - skip generating $_OUT])
  fi
  rm -f conftest.*
fi
m4_popdef([_symbol])dnl
m4_popdef([_script])dnl
AS_VAR_POPDEF([_INP])dnl
AS_VAR_POPDEF([_UPP])dnl
AS_VAR_POPDEF([_LOW])dnl
AS_VAR_POPDEF([_PKG])dnl
AS_VAR_POPDEF([_DEF])dnl
AS_VAR_POPDEF([_OUT])dnl
],[PACKAGE="$PACKAGE"])])

dnl ______ /usr/share/aclocal/guidod/ax_dirname.m4 ______
dnl @* AX_DIRNAME(PATHNAME)
dnl
dnl Parts of the implementation have been taken from AS_DIRNAME from the
dnl main autoconf package in generation 2.5x. However, we do only use 
dnl "sed" to cut out the dirname, and we do additionally clean up some 
dnl dir/.. parts in the resulting pattern.
dnl
dnl this macro may be used in autoconf 2.13 scripts as well.
dnl
dnl @%Id: ax_dirname.m4,v 1.1 2003/07/02 23:42:59 guidod Exp %

AC_DEFUN([AX_DIRNAME],
[echo X[]$1 |
    sed ['s/\/[^\/:][^\/:]*\/..\//\//g
          s/\/[^\/:][^\/:]*\/..\//\//g
          s/\/[^\/:][^\/:]*\/..\//\//g
          s/\/[^\/:][^\/:]*\/..\//\//g
          /^X\(.*[^/]\)\/\/*[^/][^/]*\/*$/{ s//\1/; q; }
          /^X\(\/\/\)[^/].*/{ s//\1/; q; }
          /^X\(\/\/\)$/{ s//\1/; q; }
          /^X\(\/\).*/{ s//\1/; q; }
          s/.*/./; q']])

dnl ______ /usr/share/aclocal/guidod/ax_not_enable_frame_pointer.m4 ______
dnl @* AX_NOT_ENABLE_FRAME_POINTER ([shellvar])
dnl
dnl add --enable-frame-pointer option, the default will add the gcc
dnl --fomit-frame-pointer option to the shellvar (per default CFLAGS)
dnl and remove the " -g " debuginfo option from it. In other words,
dnl the default is "--disable-frame-pointer"
dnl
dnl @: guidod@gmx.de
dnl @%Id: %

AC_DEFUN([AX_NOT_ENABLE_FRAME_POINTER],[dnl
AS_VAR_PUSHDEF([VAR],[enable_frame_pointer])dnl
AC_MSG_CHECKING([m4_ifval($1,$1,CFLAGS) frame-pointer])
AC_ARG_ENABLE([frame-pointer], AC_HELP_STRING(
  [--enable-frame-pointer],[enable callframe generation for debugging]))
case ".$VAR" in
  .|.no|.no,*) test ".$VAR" = "." && VAR="no"
     m4_ifval($1,$1,CFLAGS)=`echo dnl
  " $m4_ifval($1,$1,CFLAGS) " | sed -e 's/ -g / /'`
     if test ".$GCC" = ".yes" ; then
        m4_ifval($1,$1,CFLAGS)="$m4_ifval($1,$1,CFLAGS) -fomit-frame-pointer" 
        AC_MSG_RESULT([$VAR, -fomit-frame-pointer added]) 
     else
        AC_MSG_RESULT([$VAR, -g removed])
     fi  ;;
   *)  AC_MSG_RESULT([$VAR, kept]) ;;
esac
AS_VAR_POPDEF([VAR])dnl
])

dnl ______ /usr/share/aclocal/guidod/ac_set_default_paths_system.m4 ______
dnl @synopsis AC_SET_DEFAULT_PATHS_SYSTEM
dnl
dnl the most interesting changes go about windows-targets - where the
dnl default_prefix is set to /programs, and quite some directories
dnl are aliased: sbindir := libdir := bindir  and the docprefix-defaults
dnl are also a bit different, even on FHS2-compliant systems where the
dnl mandir is going to $prefix/man only if prefix=/usr, otherwise they
dnl shall go to $datadir/man. We use an extra docprefix to express it
dnl which is either defined as being prefix or datadir. not SUBSTed here.
dnl
dnl @, (very useful)
dnl @version %Id: ac_set_default_paths_system.m4,v 1.5 2003/02/02 19:32:30 guidod Exp %
dnl @author Guido Draheim <guidod@gmx.de>
AC_DEFUN([AC_SET_DEFAULT_PATHS_SYSTEM],
[AC_REQUIRE([AC_CANONICAL_HOST]) # --------------------------------------------
case "$prefix:$ac_default_prefix" in
  NONE:/usr/local)
    result=""
    AC_MSG_CHECKING(default prefix path)
    case "${target_os}" in
      *cygwin* | *mingw* | *uwin* | *djgpp | *emx* )
	if test "${host_os}" = "${target_os}" ; then
           ac_default_prefix="/programs"
           result="(win/dos target)"
        else
           case "$PATH" in
              *:/usr/local/cross-tools/$target_alias/bin:*)
	          ac_default_prefix="/usr/local/cross-tools/$target_alias" ;;
              *:/usr/local/$target_alias/bin:*)
	          ac_default_prefix="/usr/local/$target_alias" ;;
              *:/usr/local/$target_cpu-$target_os/bin:*)
	          ac_default_prefix="/usr/local/$target_cpu-$target_os" ;;
              *)  
                  ac_default_prefix="/programs" ;;
           esac
           result="(win/dos cross-compiler)"
        fi
    ;;
    esac
    AC_MSG_RESULT($ac_default_prefix $result)
  ;;
esac
AC_MSG_CHECKING(default prefix system)
result="$prefix" ; test "$result" = "NONE" && result="$ac_default_prefix"
case ${result} in
  /programs | /programs/*) result="is win-/programs"
     # on win/dos, .exe .dll and .cfg live in the same directory
     libdir=`echo $libdir |sed -e 's:^..exec_prefix./lib$:${bindir}:'`
     sbindir=`echo $sbindir |sed -e 's:^..exec_prefix./sbin$:${libdir}:'`
     sysconfdir=`echo $sysconfdir |sed -e 's:^..prefix./etc$:${sbindir}:'`
     libexecdir=`echo $libexecdir |sed -e 's:/libexec$:/system:'`
     # help-files shall be set with --infodir, docprefix is datadir
     docprefix="${datadir}"
     mandir=`echo $mandir \
	                     |sed -e 's:^..prefix./man$:${datadir}/info:'`
     includedir=`echo $includedir \
                |sed -e 's:^..prefix./include$:${datadir}/include:'`
     # other state files (but /etc) are moved to datadir
     sharedstatedir=`echo $sharedstatedir \
                     |sed -e 's:^..prefix./com$:${datadir}/default:'`
     localstatedir=`echo $localstatedir \
                     |sed -e 's:^..prefix./var$:${datadir}/current:'`
  ;;
  /usr) result="is /usr-shipped"
     # doc files are left at prefix
     docprefix="${prefix}"
     # state files go under /top
     sysconfdir=`echo $sysconfdir |sed -e 's:^..prefix./etc$:/etc:'`
     sharedstatedir=`echo $sharedstatedir \
                     |sed -e 's:^..prefix./com$:/etc/default:'`
     # $prefix/var is going to end up in /var/lib
     localstatedir=`echo $localstatedir \
                     |sed -e 's:^..prefix./var$:/var/lib:'`
  ;;
  /opt | /opt/*) result="is /opt-package"
     # state files go under /top/prefix
     sysconfdir=`echo $sysconfdir \
                     |sed -e 's:^..prefix./etc$:/etc${prefix}:'`
     sharedstatedir=`echo $sharedstatedir \
                     |sed -e 's:^..prefix./com$:/etc/default${prefix}:'`
     # $prefix/var is going to to be /var$prefix... once again
     localstatedir=`echo $localstatedir \
                     |sed -e 's:^..prefix./var$:/var${prefix}:'`
     # doc files are left at prefix
     docprefix="${prefix}"
  ;;
  *) result="is /local-package"
     # doc files are moved from prefix down to datadir
     docprefix="${datadir}"
     mandir=`echo $mandir \
                     |sed -e 's:^..prefix./man$:${datadir}/man:'`
     infodir=`echo $infodir \
                     |sed -e 's:^..prefix./infodir$:${datadir}/info:'`
     # never use $prefix/com - that is no good idea
     sharedstatedir=`echo $sharedstatedir \
                     |sed -e 's:^..prefix./com$:${sysconfdir}/default:'`
  ;;
esac
AC_MSG_RESULT($result)
# -------------------------------------------------------- 
])     

dnl ______ /usr/share/aclocal/guidod/ax_cflags_no_writable_strings.m4 ______
dnl @synopsis AX_CFLAGS_NO_WRITABLE_STRINGS [(shellvar [,default, [A/NA]])]
dnl
dnl Try to find a compiler option that makes all stringliteral readonly.
dnl
dnl The sanity check is done by looking at string.h which has a set
dnl of strcpy definitions that should be defined with const-modifiers
dnl to not emit a warning in all so many places.
dnl
dnl For the GNU CC compiler it will be -fno-writable-strings -Wwrite-strings
dnl The result is added to the shellvar being CFLAGS by default.
dnl
dnl DEFAULTS:
dnl
dnl  - $1 shell-variable-to-add-to : CFLAGS
dnl  - $2 add-value-if-not-found : nothing
dnl  - $3 action-if-found : add value to shellvariable
dnl  - $4 action-if-not-found : nothing
dnl
dnl @version %Id: ax_cflags_no_writable_strings.m4,v 1.6 2003/06/05 00:52:15 guidod Exp %
dnl @author Guido Draheim <guidod@gmx.de>
dnl
AC_DEFUN([AX_CFLAGS_NO_WRITABLE_STRINGS],[dnl
AS_VAR_PUSHDEF([FLAGS],[CFLAGS])dnl
AS_VAR_PUSHDEF([VAR],[ac_cv_cflags_no_writable_strings])dnl
AC_CACHE_CHECK([m4_ifval([$1],[$1],FLAGS) making strings readonly],
VAR,[VAR="no, unknown"
 AC_LANG_SAVE
 AC_LANG_C
 ac_save_[]FLAGS="$[]FLAGS"
# IRIX C compiler:
#      -use_readonly_const is the default for IRIX C,
#       puts them into .rodata, but they are copied later.
#       need to be "-G0 -rdatashared" for strictmode but
#       I am not sure what effect that has really.         - guidod
for ac_arg dnl
in "-Wall     % -fno-writable-strings -Wwrite-strings" dnl   GCC
   "-v -Xc    % -xstrconst" dnl Solaris C - strings go into readonly segment
   "+w1 -Aa   % +ESlit"      dnl HP-UX C - strings go into readonly segment
   "-w0 -std1 % -readonly_strings" dnl Digital Unix - again readonly segment
   "-fullwarn -use_readonly_const %% ok, its the default" dnl IRIX C
   #
do FLAGS="$ac_save_[]FLAGS "`echo $ac_arg | sed -e 's,%%.*,,' -e 's,%,,'`
   AC_TRY_COMPILE([],[return 0;],
   [VAR=`echo $ac_arg | sed -e 's,.*% *,,'` ; break])
done
case ".$VAR" in
   .|.no|.no,*) ;;
   *) # sanity check - testing strcpy() from string.h
      cp config.log config.tmp
      AC_TRY_COMPILE([#include <string.h>],[
      char test[16];
      if (strcpy (test, "test")) return 1;],
      dnl the original did use test -n `$CC testprogram.c`
      [if test `diff config.log config.tmp | grep -i warning | wc -l` != 0
  then VAR="no, suppressed, string.h," ; fi],
      [VAR="no, suppressed, string.h"])
      rm config.tmp
   ;;
esac
   FLAGS="$ac_save_[]FLAGS"
   AC_LANG_RESTORE
])
case ".$VAR" in
     .ok|.ok,*) m4_ifvaln($3,$3) ;;
   .|.no|.no,*) m4_ifvaln($4,$4,[m4_ifval($2,[
        AC_RUN_LOG([: m4_ifval($1,$1,FLAGS)="$m4_ifval($1,$1,FLAGS) $2"])
                      m4_ifval($1,$1,FLAGS)="$m4_ifval($1,$1,FLAGS) $2"])]) ;;
   *) m4_ifvaln($3,$3,[
   if echo " $[]m4_ifval($1,$1,FLAGS) " | grep " $VAR " 2>&1 >/dev/null
   then AC_RUN_LOG([: m4_ifval($1,$1,FLAGS) does contain $VAR])
   else AC_RUN_LOG([: m4_ifval($1,$1,FLAGS)="$m4_ifval($1,$1,FLAGS) $VAR"])
                      m4_ifval($1,$1,FLAGS)="$m4_ifval($1,$1,FLAGS) $VAR"
   fi ]) ;;
esac
AS_VAR_POPDEF([VAR])dnl
AS_VAR_POPDEF([FLAGS])dnl
])

dnl the only difference - the LANG selection... and the default FLAGS

AC_DEFUN([AX_CXXFLAGS_NO_WRITABLE_STRINGS],[dnl
AS_VAR_PUSHDEF([FLAGS],[CXXFLAGS])dnl
AS_VAR_PUSHDEF([VAR],[ac_cv_cxxflags_no_writable_strings])dnl
AC_CACHE_CHECK([m4_ifval($1,$1,FLAGS) making strings readonly],
VAR,[VAR="no, unknown"
 AC_LANG_SAVE
 AC_LANG_CXX
 ac_save_[]FLAGS="$[]FLAGS"
# IRIX C compiler:
#      -use_readonly_const is the default for IRIX C,
#       puts them into .rodata, but they are copied later.
#       need to be "-G0 -rdatashared" for strictmode but
#       I am not sure what effect that has really.         - guidod
for ac_arg dnl
in "-Wall     % -fno-writable-strings -Wwrite-strings" dnl   GCC
   "-v -Xc    % -xstrconst" dnl Solaris C - strings go into readonly segment
   "+w1 -Aa   % +ESlit"      dnl HP-UX C - strings go into readonly segment
   "-w0 -std1 % -readonly_strings" dnl Digital Unix - again readonly segment
   "-fullwarn -use_readonly_const %% ok, its the default" dnl IRIX C
   #
do FLAGS="$ac_save_[]FLAGS "`echo $ac_arg | sed -e 's,%%.*,,' -e 's,%,,'`
   AC_TRY_COMPILE([],[return 0;],
   [VAR=`echo $ac_arg | sed -e 's,.*% *,,'` ; break])
done
case ".$VAR" in
   .|.no|.no,*) ;;
   *) # sanity check - testing strcpy() from string.h
      cp config.log config.tmp
      AC_TRY_COMPILE([#include <string.h>],[[
      char test[16];
      if (strcpy (test, "test")) return 1;]],
      dnl the original did use test -n `$CC testprogram.c`
      [if test `diff config.log config.tmp | grep -i warning | wc -l` != 0
  then VAR="no, suppressed, string.h," ; fi],
      [VAR="no, suppressed, string.h"])
      rm config.tmp
   ;;
esac
 FLAGS="$ac_save_[]FLAGS"
 AC_LANG_RESTORE
])
case ".$VAR" in
     .ok|.ok,*) m4_ifvaln($3,$3) ;;
   .|.no|.no,*) m4_ifvaln($4,$4,[m4_ifval($2,[
        AC_RUN_LOG([: m4_ifval($1,$1,FLAGS)="$m4_ifval($1,$1,FLAGS) $VAR"])
                      m4_ifval($1,$1,FLAGS)="$m4_ifval($1,$1,FLAGS) $2"])]) ;;
   *) m4_ifvaln($3,$3,[
   if echo " $[]m4_ifval($1,$1,FLAGS) " | grep " $VAR " 2>&1 >/dev/null
   then AC_RUN_LOG([: m4_ifval($1,$1,FLAGS) does contain $VAR])
   else AC_RUN_LOG([: m4_ifval($1,$1,FLAGS)="$m4_ifval($1,$1,FLAGS) $VAR"])
                      m4_ifval($1,$1,FLAGS)="$m4_ifval($1,$1,FLAGS) $VAR"
   fi ]) ;;
esac
AS_VAR_POPDEF([VAR])dnl
AS_VAR_POPDEF([FLAGS])dnl
])

dnl ______ /usr/share/aclocal/guidod/ax_spec_file.m4 ______
dnl @* AX_SPEC_FILE [(rpmspecfile [,subdirpath])]
dnl
dnl set the specfile - if no argument has been given then check whether
dnl the ac_unique_file looks like a .spec file and use it. This macro
dnl is ac_REQUIRED by many AX_SPEC_ routines.
dnl
dnl the AX_SPEC_EXTRACT macro is a helper used by many AX_SPEC_
dnl routines and therefore placed here to be automatically included
dnl in the macro set of (older) aclocal/acinclude.
dnl
dnl AX_SPEC_EXTRACT(shellvar [,specvar [,defaultvalue]])
dnl
dnl @version %Id: ax_spec_file.m4,v 1.1 2003/10/18 23:38:41 guidod Exp %
dnl @author Guido Draheim <guidod@gmx.de>

AC_DEFUN([AX_SPEC_FILE],
[ AC_MSG_CHECKING([rpm spec file])
  if test ".$1" != "." ; then
    ax_spec_file_="$1"
    ax_spec_file=`basename $1`
  else
    case ".$ac_unique_file" in
      *.spec) ax_spec_file="$ac_unique_file" 
              ax_spec_file_="$ac_unique_file" ;;
      *) ax_spec_file="TODO"
         if test ".$PACKAGE" != "." 
         then ax_spec_file_="$PACKAGE.spec"
         else ax_spec_file_="README"
         fi
   cat m4_ifset([AS_MESSAGE_LOG_FD],[>&AS_MESSAGE_LOG_FD],[>>config.log]) <<EOF
         no ax_spec_file argument has been given, using defaults of
         (1) $ax_spec_file_ 
         (2) $ax_spec_file
EOF
         ;;
    esac
  fi
  # find specfile
  for i in ifelse($2,,,$2) . $srcdir $srcdir/.. .. ; do
    if test -f "$i/$ax_spec_file_" ; then
      ax_spec_dir="$i"
      ax_spec_file="$ax_spec_dir/$ax_spec_file_"
      break
    fi
    if test -f "$i/$ax_spec_file" ; then
      ax_spec_dir="$i"
      ax_spec_file="$ax_spec_dir/$ax_spec_file"
      break
    fi
  done
  case "$ax_spec_file" in
   ./*) if test "$ax_spec_file" = "./$ax_spec_file_" ; then
          ax_spec_file="$ax_spec_file_"
        fi
        AC_MSG_RESULT([$ax_spec_file]) ;;
   */*) AC_MSG_RESULT([$ax_spec_file]) ;;
   *)   AC_MSG_ERROR([no rpm spec file found]) ;;
  esac
  m4_define([m4_ax_spec_file],[ax_spec_file])
])

dnl AX_SPEC_EXTRACT(shellvar [,specvar [,defaultvalue]])

AC_DEFUN([AX_SPEC_EXTRACT],
[ 
    if test ".$[]$1" = "." ; then
    $1=`grep -i '^[[ 	]]*m4_ifval([$2],[$2],[$1])[[ 	]]*:' dnl
  "$ax_spec_file" | sed -e 's/.*:[[ 	]]*//' dnl
    -e 's/[[ 	]][[ 	]]*/ /g' -e 's/^ //' -e 's/ $[]//' -e 'q'`
    if test ".$[]$1" = "." ; then
  for $1 in $2 $1 m4_tolower([$1]) ; do
    $1=`echo "$[]$1" | sed -e 's/^%//g'` 
    $1=`sed dnl
    -e  "/^%[[ 	]]*def[]ine[[ 	]][[ 	]]*$[]$1[[ 	]]/!d"  dnl
    -e "s/^%[[ 	]]*def[]ine[[ 	]][[ 	]]*$[]$1[[ 	]]*//" dnl
    -e 's/[[ 	]][[ 	]]*/ /' -e 's/ $[]//' -e 'q' $ax_spec_file`
    test ".$1" != "." && break 
  done
    fi 
  case ".$[]$1" in
    .%{*) 
    $1=`echo "$[]$1" | sed -e 's/%{//' -e 's/}$[]//'`
    $1=`sed dnl
   -e  "/^%[[ 	]]*def[]ine[[ 	]][[ 	]]*$[]$1[[ 	]]/!d"  dnl
   -e "s/^%[[ 	]]*def[]ine[[ 	]][[ 	]]*$[]$1[[ 	]]*//" dnl
   -e 's/[[ 	]][[ 	]]*/ /' -e 's/ $[]//' -e 'q' $ax_spec_file` ;;
    .%*) 
    $1=`echo "$[]$1" | sed -e 's/%//'` 
    $1=`sed dnl
    -e  "/^%[[ 	]]*def[]ine[[ 	]][[ 	]]*$[]$1[[ 	]]/!d"  dnl
    -e "s/^%[[ 	]]*def[]ine[[ 	]][[ 	]]*$[]$1[[ 	]]*//" dnl
    -e 's/[[ 	]][[ 	]]*/ /' -e 's/ $[]//' -e 'q' $ax_spec_file` ;;
  esac 
  m4_ifval([$3],[test ".$[]$1" = "." && $1="$3"])
    fi # test ".$[]$1" = "."
])

