#! /bin/sh
# this is the sh/sed variant of the mksite script. It is largely
# derived from snippets that I was using to finish doc pages for 
# website publishing. For the mksite project the functionaliy has
# been expanded of course. Still this one does only use simple unix
# commands like sed, date, and test. And it still works. :-)=)
#                                               http://zziplib.sf.net/mksite/
#   THE MKSITE.SH (ZLIB/LIBPNG) LICENSE
#       Copyright (c) 2004 Guido Draheim <guidod@gmx.de>
#   This software is provided 'as-is', without any express or implied warranty
#       In no event will the authors be held liable for any damages arising
#       from the use of this software.
#   Permission is granted to anyone to use this software for any purpose, 
#       including commercial applications, and to alter it and redistribute it 
#       freely, subject to the following restrictions:
#    1. The origin of this software must not be misrepresented; you must not
#       claim that you wrote the original software. If you use this software 
#       in a product, an acknowledgment in the product documentation would be 
#       appreciated but is not required.
#    2. Altered source versions must be plainly marked as such, and must not
#       be misrepresented as being the original software.
#    3. This notice may not be removed or altered from any source distribution.
# $Id: mksite.sh,v 1.3 2004/05/11 17:58:03 guidod Exp $

# initialize some defaults
test ".$SITEFILE" = "." && test -f site.htm  && SITEFILE=site.htm
test ".$SITEFILE" = "." && test -f site.html && SITEFILE=site.html
MK="-mksite"     # note the "-" at the start
SED="sed"
CAT="cat"        # "sed -e n" would be okay too
GREP="grep"
DATE_NOW="date"  # should be available on all posix systems
DATE_R="date -r" # gnu date has it / solaris date not
STAT_R="stat"    # gnu linux
LS_L="ls -l"     # linux uses one less char than solaris

INFO="~~"     # extension for meta data files
HEAD="~head~" # extension for head sed script
BODY="~body~" # extension for body sed script
FOOT="~foot~" # append to body text (non sed)

NULL="/dev/null"                             # to divert stdout/stderr
CATNULL="$CAT $NULL"                         # to create 0-byte files
SED_LONGSCRIPT="$SED -f"

LOWER="abcdefghijklmnopqrstuvwxyz"
UPPER="ABCDEFGHIJKLMNOPQRSTUVWXYZ"
az="$LOWER"                                  # some old sed tools can not
AZ="$UPPER"                                  # use char-ranges in the 
NN="0123456789"                              # match expressions so that
AA="_$NN$AZ$az"                              # we use their unrolled
AX="$AA.+-"                                  # definition here
if $SED -V 2>$NULL | $GREP "GNU sed" >$NULL ; then
az="a-z"                                     # but if we have GNU sed
AZ="A-Z"                                     # then we assume there are
NN="0-9"                                     # char-ranges available
AA="_$NN$AZ$az"                              # that makes the resulting
AX="$AA.+-"                                  # script more readable
elif uname -s | $GREP HP-UX >$NULL ; then
SED_LONGSCRIPT="sed_longscript"              # due to 100 sed lines limit
fi

LANG="C" ; LANGUAGE="C" ; LC_COLLATE="C"     # these are needed for proper
export LANG LANGUAGE LC_COLLATE              # lowercasing as some collate
                                             # treat A-Z to include a-z

# ==========================================================================
# reading options from the command line                            GETOPT
opt_fileseparator="?"
opt_files=""
opt_main_file=""
opt=""
for arg in "$@"        # this variant should allow to embed spaces in $arg
do if test ".$opt" != "." ; then
      eval "export opt_$opt='$arg'"
      opt=""
   else
      case "$arg" in
      -*=*) 
         opt=`echo "$arg" | $SED -e "s/-*\\([$AA][$AA-]*\\).*/\\1/" -e y/-/_/`
         if test ".$opt" = "." ; then
            echo "ERROR: invalid option $arg" >&2
         else
            arg=`echo "$arg" | $SED -e "s/^[^=]*=//"`
            eval "export opt_$opt='$arg'"
         fi
         opt="" ;;
      -*-*)   
         opt=`echo "$arg" | $SED -e "s/-*\\([$AA][$AA-]*\\).*/\\1/" -e y/-/_/`
         if test ".$opt" = "." ; then
            echo "ERROR: invalid option $arg" >&2
            opt=""
         else :
            # keep the option for next round
         fi ;;
      -*)  
         opt=`echo "$arg" | $SED -e "s/^-*\\([$AA][$AA-]*\\).*/\\1/" -e y/-/_/`
         if test ".$opt" = "." ; then
            echo "ERROR: invalid option $arg" >&2
         else
            arg=`echo "$arg" | $SED -e "s/^[^=]*=//"`
            eval "export opt_$opt=' '"
         fi
         opt="" ;;
      *) test ".$opt_main_file" = "." && opt_main_file="$arg"
         test ".$opt_files" != "." && opt_files="$opt_files$opt_fileseparator"
         opt_files="$opt_files$arg"
         opt="" ;;
      esac
   fi
done
### env | grep ^opt

test ".$opt_main_file" != "." && test -f "$opt_main_file" && \
SITEFILE="$opt_main_file"
test ".$opt_site_file" != "." && test -f "$opt_site_file" && \
SITEFILE="$opt_site_file"

if test ".$SITEFILE" = "." ; then
   echo "error: no SITEFILE found (default would be 'site.htm')"
   exit 1
else
   echo "NOTE: sitefile: `ls -s $SITEFILE`"
fi

if ($STAT_R "$SITEFILE" >$NULL) 2>$NULL ; then : ; else STAT_R=":" ; fi
# ==========================================================================
# init a few global variables
#                                                                  0. INIT

# $MK.tags.tmp - originally, we would use a lambda execution on each 
# uppercased html tag to replace <P> with <p class="P">. Here we just
# walk over all the known html tags and make an sed script that does
# the very same conversion. There would be a chance to convert a single
# tag via "h;y;x" or something we do want to convert all the tags on
# a single line of course.
$CATNULL > $MK.tags.tmp
for P in P H1 H2 H3 H4 H5 H6 DL DD DT UL OL LI PRE CODE TABLE TR TD TH \
         B U I S Q EM STRONG STRIKE CITE BIG SMALL SUP SUB TT THEAD TBODY \
         CENTER HR BR NOBR WBR SPAN DIV IMG ADRESS BLOCKQUOTE
do M=`echo $P | $SED -e "y/$UPPER/$LOWER/"`
  echo "s|<$P>|<$M class=\"$P\">|g"         >>$MK.tags.tmp
  echo "s|<$P |<$M class=\"$P\" |g"         >>$MK.tags.tmp
  echo "s|</$P>|</$M>|g"                    >>$MK.tags.tmp
done
  echo "s|<>|\\&nbsp\\;|g"                  >>$MK.tags.tmp
  echo "s|<->|<WBR />\\;|g"                 >>$MK.tags.tmp
# also make sure that some non-html entries are cleaned away that
# we are generally using to inject meta information. We want to see
# that meta ino in the *.htm browser view during editing but they
# shall not get present in the final html page for publishing.
DC_VARS="contributor date source language coverage identifier rights"
DC_VARS="$DC_VARS relation creator subject description publisher DCMIType"
for P in $DC_VARS ; do # dublin core embedded
   echo "s|<$P>[^<>]*</$P>||g"              >>$MK.tags.tmp
done
   echo "s|<!--sect[$AZ$NN]-->||g"          >>$MK.tags.tmp
   echo "s|<!--[$AX]*[?]-->||g"             >>$MK.tags.tmp
   echo "s|<!--\\\$[$AX]*[?]:-->||g"        >>$MK.tags.tmp
   echo "s|<!--\\\$[$AX]*:[?=]-->||g"        >>$MK.tags.tmp
   echo "s|\\(<[^<>]*\\)\\\${[$AX]*:[?=]\\([^<{}>]*\\)}\\([^<>]*>\\)|\\1\\2\\3|g"        >>$MK.tags.tmp


TRIMM=" -e 's:^ *::' -e 's: *\$::'"  # trimm away leading/trailing spaces
# ======================================================================
#                                                                FUNCS

sed_longscript ()
{
    # hpux sed has a limit of 100 entries per sed script !
    $SED             -e "100q" "$1" > "$1~1~"
    $SED -e "1,100d" -e "200q" "$1" > "$1~2~"
    $SED -e "1,200d" -e "300q" "$1" > "$1~3~"
    $SED -e "1,300d" -e "400q" "$1" > "$1~4~"
    $SED -e "1,400d" -e "500q" "$1" > "$1~5~"
    $SED -e "1,500d" -e "600q" "$1" > "$1~6~"
    $SED -e "1,600d" -e "700q" "$1" > "$1~7~"
    $SED -e "1,700d" -e "800q" "$1" > "$1~8~"
    $SED -e "1,800d" -e "900q" "$1" > "$1~9~"
    $SED -f "$1~1~"  -f "$1~2~" -f "$1~3~" -f "$1~4~" -f "$1~5~" \
         -f "$1~6~"  -f "$1~7~" -f "$1~8~" -f "$1~9~" "$2"
}

sed_slash_key ()      # helper to escape chars special in /anchor/ regex
{                     # currently escaping "/" "[" "]" "."
    echo "$1" | $SED -e "s|[./[-]|\\\\&|g" -e "s|\\]|\\\\&|g"
}
sed_piped_key ()      # helper to escape chars special in s|anchor|| regex
{                     # currently escaping "|" "[" "]" "."
    echo "$1" | $SED -e "s/[.|[-]/\\\\&/g" -e "s/\\]/\\\\&/g"
}
SLASH_VALUE="s:[&/]:\\\\&:g"
PIPED_VALUE="s:[&/]:\\\\&:g"

back_path ()          # helper to get the series of "../" for a given path
{
    echo "$1" | $SED -e "/\\//!d" -e "s|/[^/]*\$|/|" -e "s|[^/]*/|../|g"
}

dir_name ()
{
    echo "$1" | $SED -e "s:/[^/][^/]*\$::"
}

info2test ()          # cut out all old-style <!--vars--> usages
{
  OUT="$1" ; test ".$OUT" = "." && OUT="./$MK.test.tmp"
  INP="$2" ; test ".$INP" = "." && INP="./$F.$INFO"
  V8=" *\\([^ ][^ ]*\\) \\(.*\\)"
  V9=" *DC[.]\\([^ ][^ ]*\\) \\(.*\\)"
   q="\\\$"
   _x_="WARNING: assumed simplevar <!--\\\\1--> changed to <!--$q\\\\1:=-->"
   _y_="WARNING: assumed simplevar <!--\\\\1--> changed to <!--$q\\\\1:?-->"
   _X_="WARNING: assumed tailvar <!--$q\\\\1:--> changed to <!--$q\\\\1:=-->"
   _Y_="WARNING: assumed tailvar <!--$q\\\\1:--> changed to <!--$q\\\\1:?-->"
   echo "s/^/ /" > $OUT
  $SED -e "/=....=formatter /d" \
  -e "/=text=/s%=text=$V9%s|.*<!--\\\\(\\1\\\\)-->.*|$_x_|%" \
  -e "/=Text=/s%=Text=$V9%s|.*<!--\\\\(\\1\\\\)-->.*|$_x_|%" \
  -e "/=name=/s%=name=$V9%s|.*<!--\\\\(\\1\\\\)[?]-->.*|$_y_|%" \
  -e "/=Name=/s%=Name=$V9%s|.*<!--\\\\(\\1\\\\)[?]-->.*|$_y_|%" \
  -e "/=text=/s%=text=$V8%s|.*<!--\\\\(\\1\\\\)-->.*|$_x_|%" \
  -e "/=Text=/s%=Text=$V8%s|.*<!--\\\\(\\1\\\\)-->.*|$_x_|%" \
  -e "/=name=/s%=name=$V8%s|.*<!--\\\\(\\1\\\\)[?]-->.*|$_y_|%" \
  -e "/=Name=/s%=Name=$V8%s|.*<!--\\\\(\\1\\\\)[?]-->.*|$_y_|%" \
  -e "/^=/d" -e "s|&|\\\\&|g"  $INP >> $OUT
  $SED -e "/=....=formatter /d" \
  -e "/=text=/s%=text=$V9%s|.*<!--$q\\\\(\\1\\\\):-->.*|$_X_|%" \
  -e "/=Text=/s%=Text=$V9%s|.*<!--$q\\\\(\\1\\\\):-->.*|$_X_|%" \
  -e "/=name=/s%=name=$V9%s|.*<!--$q\\\\(\\1\\\\)[?]:-->.*|$_Y_|%" \
  -e "/=Name=/s%=Name=$V9%s|.*<!--$q\\\\(\\1\\\\)[?]:-->.*|$_Y_|%" \
  -e "/=text=/s%=text=$V8%s|.*<!--$q\\\\(\\1\\\\):-->.*|$_X_|%" \
  -e "/=Text=/s%=Text=$V8%s|.*<!--$q\\\\(\\1\\\\):-->.*|$_X_|%" \
  -e "/=name=/s%=name=$V8%s|.*<!--$q\\\\(\\1\\\\)[?]:-->.*|$_Y_|%" \
  -e "/=Name=/s%=Name=$V8%s|.*<!--$q\\\\(\\1\\\\)[?]:-->.*|$_Y_|%" \
  -e "/^=/d" -e "s|&|\\\\&|g"  $INP >> $OUT
  echo "/^WARNING:/!d" >> $OUT
}

info2vars ()          # generate <!--$vars--> substition sed addon script
{
  OUT="$1" ; test ".$OUT" = "." && OUT="./$MK.vars.tmp"
  INP="$2" ; test ".$INP" = "." && INP="./$F.$INFO"
  V8=" *\\([^ ][^ ]*\\) \\(.*\\)"
  V9=" *DC[.]\\([^ ][^ ]*\\) \\(.*\\)"
  N8=" *\\([^ ][^ ]*\\) \\([$NN].*\\)"
  N9=" *DC[.]\\([^ ][^ ]*\\) \\([$NN].*\\)"
  V0="\\\\([<]*\\\\)\\\\\\\$"
  V1="\\\\([^<>]*\\\\)\\\\\\\$"
  V2="\\\\([^{<>}]*\\\\)"
  V3="\\\\([^<>]*\\\\)"
  SS="<""<>"">" # spacer so value="2004" does not make for s|\(...\)|\12004|
  test ".$commentvars"  = ".no" && updatevars="no"   # duplicated from
  test ".$commentvars"  = ".no" && expandvars="no"   # option handling
  test ".$commentvars"  = ".no" && simplevars="no"   # tests below
  test ".$expandvars" != ".no" && \
  $SED -e "/=....=formatter /d" \
  -e "/=name=/s,=name=$V9,s|<!--$V0\\1[?]-->|- \\2|," \
  -e "/=Name=/s,=Name=$V9,s|<!--$V0\\1[?]-->|(\\2)|," \
  -e "/=name=/s,=name=$V8,s|<!--$V0\\1[?]-->|- \\2|," \
  -e "/=Name=/s,=Name=$V8,s|<!--$V0\\1[?]-->|(\\2)|," \
  -e "/^=/d" -e "s|&|\\\\&|g"  $INP > $OUT
  test ".$expandvars" != ".no" && \
  $SED -e "/=....=formatter /d" \
  -e "/=text=/s,=text=$V9,s|<!--$V1\\1-->|\\\\1$SS\\2|," \
  -e "/=Text=/s,=Text=$V9,s|<!--$V1\\1-->|\\\\1$SS\\2|," \
  -e "/=name=/s,=name=$V9,s|<!--$V1\\1[?]-->|\\\\1$SS\\2|," \
  -e "/=Name=/s,=Name=$V9,s|<!--$V1\\1[?]-->|\\\\1$SS\\2|," \
  -e "/=text=/s,=text=$V8,s|<!--$V1\\1-->|\\\\1$SS\\2|," \
  -e "/=Text=/s,=Text=$V8,s|<!--$V1\\1-->|\\\\1$SS\\2|," \
  -e "/=name=/s,=name=$V8,s|<!--$V1\\1[?]-->|\\\\1$SS\\2|," \
  -e "/=Name=/s,=Name=$V8,s|<!--$V1\\1[?]-->|\\\\1$SS\\2|," \
  -e "/^=/d" -e "s|&|\\\\&|g"  $INP >> $OUT
  test ".$simplevars" != ".no" && test ".$updatevars" != ".no" && \
  $SED -e "/=....=formatter /d" \
  -e "/=text=/s,=text=$V9,s|<!--$V0\\1:-->[$AX]*|\\2|," \
  -e "/=Text=/s,=Text=$V9,s|<!--$V0\\1:-->[$AX]*|\\2|," \
  -e "/=name=/s,=name=$V9,s|<!--$V0\\1[?]:-->[$AX]*|- \\2|," \
  -e "/=Name=/s,=Name=$V9,s|<!--$V0\\1[?]:-->[$AX]*| (\\2) |," \
  -e "/=text=/s,=text=$V8,s|<!--$V0\\1:-->[$AX]*|\\2|," \
  -e "/=Text=/s,=Text=$V8,s|<!--$V0\\1:-->[$AX]*|\\2|," \
  -e "/=name=/s,=name=$V8,s|<!--$V0\\1[?]:-->[$AX]*|- \\2|," \
  -e "/=Name=/s,=Name=$V8,s|<!--$V0\\1[?]:-->[$AX]*| (\\2) |," \
  -e "/^=/d" -e "s|&|\\\\&|g"  $INP >> $OUT
  test ".$updatevars" != ".no" && \
  $SED -e "/=....=formatter /d" \
  -e "/=name=/s,=name=$V9,s|<!--$V0\\1:[?]-->[^<>]*|- \\2|," \
  -e "/=Name=/s,=Name=$V9,s|<!--$V0\\1:[?]-->[^<>]*| (\\2) |," \
  -e "/=name=/s,=name=$V8,s|<!--$V0\\1:[?]-->[^<>]*|- \\2|," \
  -e "/=Name=/s,=Name=$V8,s|<!--$V0\\1:[?]-->[^<>]*| (\\2) |," \
  -e "/^=/d" -e "s|&|\\\\&|g"  $INP >> $OUT
  test ".$updatevars" != ".no" && \
  $SED -e "/=....=formatter /d" \
  -e "/=text=/s,=text=$V9,s|<!--$V1\\1:[=]-->[^<>]*|\\\\1$SS\\2|," \
  -e "/=Text=/s,=Text=$V9,s|<!--$V1\\1:[=]-->[^<>]*|\\\\1$SS\\2|," \
  -e "/=name=/s,=name=$V9,s|<!--$V1\\1:[?]-->[^<>]*|\\\\1$SS\\2|," \
  -e "/=Name=/s,=Name=$V9,s|<!--$V1\\1:[?]-->[^<>]*|\\\\1$SS\\2|," \
  -e "/=text=/s,=text=$V8,s|<!--$V1\\1:[=]-->[^<>]*|\\\\1$SS\\2|," \
  -e "/=Text=/s,=Text=$V8,s|<!--$V1\\1:[=]-->[^<>]*|\\\\1$SS\\2|," \
  -e "/=name=/s,=name=$V8,s|<!--$V1\\1:[?]-->[^<>]*|\\\\1$SS\\2|," \
  -e "/=Name=/s,=Name=$V8,s|<!--$V1\\1:[?]-->[^<>]*|\\\\1$SS\\2|," \
  -e "/^=/d" -e "s|&|\\\\&|g"  $INP >> $OUT
  test ".$attribvars" != ".no" && \
  $SED -e "/=....=formatter /d" \
  -e "/=text=/s,=text=$V9,s|<$V1{\\1:[=]$V2}$V3>|<\\\\1$SS\\2\\\\3>|," \
  -e "/=Text=/s,=Text=$V9,s|<$V1{\\1:[=]$V2}$V3>|<\\\\1$SS\\2\\\\3>|," \
  -e "/=name=/s,=name=$V9,s|<$V1{\\1:[?]$V2}$V3>|<\\\\1$SS\\2\\\\3>|," \
  -e "/=Name=/s,=Name=$V9,s|<$V1{\\1:[?]$V2}$V3>|<\\\\1$SS\\2\\\\3>|," \
  -e "/=text=/s,=text=$V8,s|<$V1{\\1:[=]$V2}$V3>|<\\\\1$SS\\2\\\\3>|," \
  -e "/=Text=/s,=Text=$V8,s|<$V1{\\1:[=]$V2}$V3>|<\\\\1$SS\\2\\\\3>|," \
  -e "/=name=/s,=name=$V8,s|<$V1{\\1:[?]$V2}$V3>|<\\\\1$SS\\2\\\\3>|," \
  -e "/=Name=/s,=Name=$V8,s|<$V1{\\1:[?]$V2}$V3>|<\\\\1$SS\\2\\\\3>|," \
  -e "/^=/d" -e "s|&|\\\\&|g"  $INP >> $OUT
  test ".$simplevars" != ".no" && \
  $SED -e "/=....=formatter /d" \
  -e "/=text=/s,=text=$V9,s|<!--\\1-->[$AX]*|\\2|," \
  -e "/=Text=/s,=Text=$V9,s|<!--\\1-->[$AX]*|\\2|," \
  -e "/=name=/s,=name=$V9,s|<!--\\1[?]-->[$AX]*| - \\2|," \
  -e "/=Name=/s,=Name=$V9,s|<!--\\1[?]-->[$AX]*| (\\2) |," \
  -e "/=text=/s,=text=$V8,s|<!--\\1-->[$AX]*|\\2|," \
  -e "/=Text=/s,=Text=$V8,s|<!--\\1-->[$AX]*|\\2|," \
  -e "/=name=/s,=name=$V8,s|<!--\\1[?]-->[$AX]*| - \\2|," \
  -e "/=Name=/s,=Name=$V8,s|<!--\\1[?]-->[$AX]*| (\\2) |," \
  -e "/^=/d" -e "s|&|\\\\&|g"  $INP >> $OUT
  # if value="2004" then generated sed might be "\\12004" which is bad
  # instead we generate an edited value of "\\1$SS$value" and cut out
  # the spacer now after expanding the variable values:
  echo "s|$SS||g" >> $OUT
}

info2meta ()         # generate <meta name..> text portion
{
  # http://www.metatab.de/meta_tags/DC_type.htm
  OUT="$1" ; test ".$OUT" = "." && OUT="./$MK.meta.tmp"
  INP="$2" ; test ".$INP" = "." && INP="./$F.$INFO"
  V8=" *\\([^ ][^ ]*\\) \\(.*\\)" ; SCHEME="scheme=\"\\1\""
  V9=" *DC[.]\\([^ ][^ ]*\\) \\(.*\\)"
  INFO_META_TYPE_SCHEME="name=\"DC.type\" content=\"\\2\" scheme=\"\\1\""
  INFO_META_TYPEDCMI="name=\"\\1\" content=\"\\2\" scheme=\"DCMIType\""
  INFO_META_NAME="name=\"\\1\" content=\"\\2\""
  INFO_META_NAME_TZ="name=\"\\1\" content=\"\\2 `$DATE_NOW +%z`\"" 
  $SED -e "/=....=today /d" \
  -e "/=meta=DC[.]DCMIType /s,=meta=$V9,<meta $INFO_META_TYPE_SCHEME />," \
  -e "/=meta=DC[.]type Collection$/s,=meta=$V8,<meta $INFO_META_TYPEDCMI />," \
  -e "/=meta=DC[.]type Dataset$/s,=meta=$V8,<meta $INFO_META_TYPEDCMI />," \
  -e "/=meta=DC[.]type Event$/s,=meta=$V8,<meta $INFO_META_TYPEDCMI />," \
  -e "/=meta=DC[.]type Image$/s,=meta=$V8,<meta $INFO_META_TYPEDCMI />," \
  -e "/=meta=DC[.]type Service$/s,=meta=$V8,<meta $INFO_META_TYPEDCMI />," \
  -e "/=meta=DC[.]type Software$/s,=meta=$V8,<meta $INFO_META_TYPEDCMI />," \
  -e "/=meta=DC[.]type Sound$/s,=meta=$V8,<meta $INFO_META_TYPEDCMI />," \
  -e "/=meta=DC[.]type Text$/s,=meta=$V8,<meta $INFO_META_TYPEDCMI />," \
  -e "/=meta=DC[.]date[.].*[+]/s,=meta=$V8,<meta $INFO_META_NAME />," \
  -e "/=meta=DC[.]date[.].*[:]/s,=meta=$V8,<meta $INFO_META_NAME_TZ />," \
  -e "/=meta=/s,=meta=$V8,<meta $INFO_META_NAME />," \
  -e "/<meta name=\"[^\"]*\" content=\"\" /d" \
  -e "/^=/d" $INP > $OUT
}

info_get_entry () # get the first <!--vars--> value known so far
{
  TXT="$1" ; test ".$TXT" = "." && TXT="sect"
  INP="$2" ; test ".$INP" = "." && INP="./$F.$INFO"
  $SED -e "/=text=$TXT /!d" -e "s/=text=$TXT //" -e "q" $INP # to stdout
}

info1grep () # test for a <!--vars--> substition to be already present
{
  TXT="$1" ; test ".$TXT" = "." && TXT="sect"
  INP="$2" ; test ".$INP" = "." && INP="./$F.$INFO"
  $GREP "^=text=$TXT " $INP >$NULL
  return $?
}

DX_text ()   # add a <!--vars--> substition includings format variants
{
  N="$1" ; T="$2"
  if test ".$N" != "." ; then
    if test ".$T" != "." ; then
      text=`echo "$T" | $SED -e "y/$UPPER/$LOWER/" -e "s/<[^<>]*>//g"`
      echo       "=text=$N $T"                       >> $F.$INFO
      echo       "=name=$N $text"                    >> $F.$INFO
      varname=`echo "$N" | $SED -e 's/.*[.]//'`    # cut out front part
      if test ".$N" != ".$varname" ; then 
      text=`echo "$varname $T" | $SED -e "y/$UPPER/$LOWER/" -e "s/<[^<>]*>//g"`
      echo       "=Text=$varname $T"                 >> $F.$INFO
      echo       "=Name=$varname $text"              >> $F.$INFO
      fi
    fi
  fi
}

DX_meta ()  # add simple meta entry and its <!--vars--> subsitution
{
   echo "=meta=$1 $2"  >> $F.$INFO
   DX_text "$1" "$2"
}

DC_meta ()   # add new DC.meta entry plus two <!--vars--> substitutions
{
   echo "=meta=DC.$1 $2"  >> $F.$INFO
   DX_text "DC.$1" "$2"
   DX_text "$1" "$2"
}

DC_VARS_Of () # check DC vars as listed in $DC_VARS global and generate DC_meta
{             # the results will be added to .meta.tmp and .vars.tmp later
   FILENAME="$1" ; test ".$FILENAME" = "." && FILENAME="$SOURCEFILE"   
   for M in $DC_VARS title ; do
      # scan for a <markup> of this name
      term="-e '/<$M>/!d' -e 's|.*<$M>||' -e 's|</$M>.*||'"
      part=`eval $SED $term $TRIMM -e q $FILENAME`
      text=`echo "$part" | eval $SED -e "'s|^[$AA]*:||'" $TRIMM`
      # <mark:part> will be <meta name="mark.part">
      if test ".$text" != ".$part" ; then
         N=`echo "$part" | $SED -e "s/:.*//"`
         DC_meta "$M.$N" "$text"
      elif test ".$M" = ".date" ; then
         DC_meta "$M.issued" "$text" # "<date>" -> "<date>issued:"
      else
         DC_meta "$M" "$text"
      fi
   done
}

DC_isFormatOf ()       # make sure there is this DC.relation.isFormatOf tag
{                      # choose argument for a fallback (usually $SOURCEFILE)
   NAME="$1" ; test ".$NAME" = "." && NAME="$SOURCEFILE"   
   info1grep DC.relation.isFormatOf || DC_meta relation.isFormatOf "$NAME"
}

DC_publisher ()        # make sure there is this DC.publisher meta tag
{                      # choose argument for a fallback (often $USER)
   NAME="$1" ; test ".$NAME" = "." && NAME="$USER"
   info1grep DC.publisher || DC_meta publisher "$NAME"
}

DC_modified ()         # make sure there is a DC.date.modified meta tag
{                      # maybe choose from filesystem dates if possible
   Q="$1" # target file
   if info1grep DC.date.modified 
   then :
   else meta='<meta name="DC.date.modified"'
      _42_chars="........................................."
      cut_42_55="s/^$_42_chars\\(.............\\).*/\\1/" # i.e.`cut -b 42-55`
      modified=`$STAT_R $Q 2>$NULL | $SED -e '/odify:/!d' -e 's|.*fy:||' -e q`
      modified=`echo "$modified" | eval $SED -e "'s/:..[.][$NN]*//'" $TRIMM`
      test ".$modified" = "." && \
      modified=`$DATE_R "$Q" +%Y-%m-%d 2>$NULL`   # GNU sed
      test ".$modified" = "." && 
      modified=`$LS_L "$Q" | $SED -e "$cut_42_55" -e "s/^ *//g" -e "q"`
      DC_meta date.modified "$modified"
   fi
}

DC_date ()             # make sure there is this DC.date meta tag
{                      # choose from one of the available DC.date.* specials
   Q="$1" # source file
   if info1grep DC.date 
   then DX_text issue "dated `info_get_entry DC.date`"
        DX_text updated     "`info_get_entry DC.date`"
   else text=""
      for kind in available issued modified created ; do
        text=`info_get_entry DC.date.$kind` 
      # test ".$text" != "." && echo "$kind = date = $text ($Q)"
        test ".$text" != "." && break
      done
      if test ".$text" = "." ; then
        M="date"
        term="-e '/<$M>/!d' -e 's|.*<$M>||' -e 's|</$M>.*||'"
        part=`eval $SED $term $TRIMM -e q $Q`
        text=`echo "$part" | eval $SED -e "'s|^[$AA]*:||'" $TRIMM`
      fi
      if test ".$text" = "." ; then 
         # this should be rewritten.... ugly way to parse out a date:
         CLEAN=" -e '/^%%%%%/!d' -e 's:^%*::' -e 's:</.*::' -e 's:.*>::'"
         EDATE=" -e 's:.*<date>:%%%%%%:' -e 's:.*<!--date-->:%%%%%%:' $CLEAN"
         text=`eval $SED $EDATE $TRIMM -e "'s|^ *[$AA]*:||'" -e q $Q` 
         text=`echo "$text" | $SED -e 's|\\&.*||'`
      fi
      text=`echo "$text" | $SED -e "s/[$NN]*:.*//"` # cut way seconds
      DX_text updated "$text"
      text1=`echo "$text" | $SED -e "s|^.* *updated ||"`
      if test ".$text" != ".$text1" ; then
        kind="modified" ; text=`echo "$text1" | $SED -e "s|,.*||"`
      fi
      text1=`echo "$text" | $SED -e "s|^.* *modified ||"`
      if test ".$text" != ".$text1" ; then
        kind="modified" ; text=`echo "$text1" | $SED -e "s|,.*||"`
      fi
      text1=`echo "$text" | $SED -e "s|^.* *created ||"`
      if test ".$text" != ".$text1" ; then
        kind="created" ; text=`echo "$text1" | $SED -e "s|,.*||"`
      fi
      DC_meta date "$text"
      DX_text issue "$kind $text"
   fi
}

DC_title ()
{
   # choose a title for the document, either an explicit title-tag
   # or one of the section headers in the document or fallback to filename
   Q="$1" # target file
   if info1grep DC.title 
   then :
   else
      for M in TITLE title H1 h1 H2 h2 H3 H3 H4 H4 H5 h5 H6 h6 ; do
        term="-e '/<$M>/!d' -e 's|.*<$M>||' -e 's|</$M>.*||'"
        text=`eval $SED $term $TRIMM -e q $Q`
        test ".$text" != "." && break
        term="-e '/<$M [^<>]*>/!d' -e 's|.*<$M [^<>]*>||' -e 's|</$M>.*||'"
        text=`eval $SED $term $TRIMM -e q $Q`
        test ".$text" != "." && break
      done
      if test ".text" = "." ; then
        text=`basename $Q .html | $SED -e 'y/_/ /' -e "s/\\$/ info/"`
      fi
      term=`echo "$text" | $SED -e 's/.*[(]//' -e 's/[)].*//'`
      text=`echo "$text" | $SED -e 's/[(][^()]*[)]//'`
      if test ".$term" = "." || test ".$term" = ".$text" ; then
         DC_meta title "$text"
      else
         DC_meta title "$term - $text"
      fi
   fi
}    

site_get_section () # return parent section page of given page
{
   _F_=`sed_slash_key "$1"`
   $SED -e "/^=sect=$_F_ /!d" -e "s/^=sect=$_F_ //" -e q ./$MK.$INFO
}

DC_section () # not really a DC relation (shall we use isPartOf ?) 
{             # each document should know its section father
   sectn=`site_get_section "$F"`
   if test ".$sectn" != "." ; then
      DC_meta relation.section "$sectn"
   fi
}

site_get_selected ()  # return section of given page
{
   _F_=`sed_slash_key "$1"`
   $SED -e "/=use.=$_F_ /!d" -e "s/=use.=[^ ]* //" -e q ./$MK.$INFO
}

DC_selected () # not really a DC title (shall we use alternative ?)
{
   # each document might want to highlight the currently selected item
   short=`site_get_selected "$F"`
   if test ".$short" != "." ; then
      DC_meta title.selected "$short"
   fi
}

site_get_rootsections () # return all sections from root of nav tree
{
   $SED -e "/=use1=/!d" -e "s/=use.=\\([^ ]*\\) .*/\\1/" ./$MK.$INFO
}

site_get_sectionpages () # return all children pages in the given section
{
   _F_=`sed_slash_key "$1"`
   $SED -e "/^=sect=[^ ]* $_F_\$/!d" -e "s/^=sect=//" -e "s/ .*//" ./$MK.$INFO
}

site_get_subpages () # return all page children of given page
{
   _F_=`sed_slash_key "$1"`
   $SED -e "/^=node=[^ ]* $_F_\$/!d" -e "s/^=node=//" -e "s/ .*//" ./$MK.$INFO
}

site_get_parentpage () # return parent page for given page (".." for sections)
{
   _F_=`sed_slash_key "$1"`
   $SED -e "/^=node=$_F_ /!d" -e "s/^=node=[^ ]* //" -e "q" ./$MK.$INFO
}

mksite_magic_option ()
{
    # $1 is word/option to check for
    INP="$2" ; test ".$INP" = "." && INP="$SITEFILE"
    $SED \
      -e "s/\\(<!--mksite:\\)\\($1\\)-->/\\1\\2: -->/g" \
      -e "s/\\(<!--mksite:\\)\\([$AA][$AA]*\\)\\($1\\)-->/\\1\\3:\\2-->/g" \
      -e "/<!--mksite:$1:/!d" \
      -e "s/.*<!--mksite:$1:\\([^<>]*\\)-->.*/\\1/" \
      -e "s/.*<!--mksite:$1:\\([^-]*\\)-->.*/\\1/" \
      -e "/<!--mksite:$1:/d" -e q $INP
}

DX_alternative ()        # detect wether page asks for alternative style
{                        # which is generally a shortpage variant
    x=`mksite_magic_option alternative $1 | sed -e "s/^ *//" -e "s/ .*//"`
    if test ".$x" != "." ; then
      DX_text alternative "$x"
    fi
}

info2head_append ()      # append alternative handling script to $HEAD
{
    OUT="$1" ; test ".$OUT" = "." && OUT="$F.$HEAD"
    have=`info_get_entry alternative`
    if test ".$have" != "." ; then
       echo "/<!--mksite:alternative:$have .*-->/{" >> $OUT
       echo "s/<!--mksite:alternative:$have\\( .*\\)-->/\\1/" >> $OUT
       echo "q" >> $OUT ; echo "}" >> $OUT
    fi
}
info2body_append ()      # append alternative handling script to $BODY
{
    OUT="$1" ; test ".$OUT" = "." && OUT="$F.$HEAD"
    have=`info_get_entry alternative`
    if test ".$have" != "." ; then
       _replace_="s/<!--mksite:alternative:$have\\( .*\\)-->/\\1/"
       echo "/<!--mksite:alternative:$have .*-->/$_replace_" >> $OUT
    fi
}

bodymaker_for_sectioninfo ()
{
    test ".$sectioninfo" = ".no" && return
    _x_="<!--mksite:sectioninfo::-->"
    _q_="\\([^<>]*[$AX][^<>]*\\)"
    test ".$sectioninfo" != ". " && _q_="[ ][ ]*$sectioninfo\\([ ]\\)"
    echo "s|\\(^<[hH][$NN][ >].*</[hH][$NN]>\\)$_q_|\\1$_x_\\2|"
    echo "/$_x_/s|^|<table width=\"100%\"><tr valign=\"bottom\"><td>|"
    echo "/$_x_/s|</[hH][$NN]>|&</td><td align=\"right\"><i>|"
    echo "/$_x_/s|\$|</i></td></tr></table>|"
    echo "s|$_x_||"
}

moved_href ()  # args "$FILETOREFERENCE" "$FROMCURRENTFILE:$F"
{   # prints path to $FILETOREFERENCE href-clickable in $FROMCURRENTFILE
    # if no subdirectoy then output is the same as input $FILETOREFERENCE
    R="$2" ; test ".$R" = "." && R="$F"
    S=`back_path "$R"`
    if test ".$S" = "." 
    then echo "$1"
    else _1_=`echo "$1" | \
         $SED -e "/^ *\$/d" -e "/^\\//d" -e "/^[.][.]/d" -e "/^[$AA]*:/d" `
         if test ".$_1_" = "." # don't move any in the pattern above
         then echo "$1" 
         else echo "$S$1"
    fi fi
}

make_move () # experimental - make a ~move~ file that can be applied
{            # to htm sourcefiles in a subdirectory of the sitefile.
#   R="$1" ; test ".$R" = "." && R="$F"
    S=`back_path "$F"` 
    if test ".$S" = "" ; then
       # echo "backpath '$F' = none needed"
       $CATNULL > $F.~move~
    else
       # echo "backpath '$F' -> '$S'"
       $SED -e "/href=\"[^\"]*\"/!d" -e "s/.*href=\"//" -e "s/\".*//" \
            -e "/^ *\$/d" -e "/^\\//d" -e "/^[.][.]/d" -e "/^[$AA]*:/d" \
       $SITEFILE $SOURCEFILE | sort | uniq \
       | $SED -e "s,.*,s|href=\"&\"|href=\"$S&\"|," > $F.~move~
    fi
}


siteinfo2sitemap ()  # generate <name><page><date> addon sed scriptlet
{                    # the resulting script will act on each item/line
                     # containing <!--"filename"--> and expand any following
                     # reference of <!--name--> or <!--date--> or <!--long-->
  OUT="$1" ; test ".$OUT" = "." && OUT="./$MK.site.tmp"
  INP="$2" ; test ".$INP" = "." && INP="./$MK.$INFO"
  _list_="s|<!--\"\\1\"-->.*<!--name-->|\\&<name href=\"\\1\">\\2</name>|"
  _date_="s|<!--\"\\1\"-->.*<!--date-->|\\&<date>\\2</date>|"
  _long_="s|<!--\"\\1\"-->.*<!--long-->|\\&<long>\\2</long>|"
  $SED -e "s:&:\\\\&:g" \
       -e "s:=list=\\([^ ]*\\) \\(.*\\):$_list_:" \
       -e "s:=date=\\([^ ]*\\) \\(.*\\):$_date_:" \
       -e "s:=long=\\([^ ]*\\) \\(.*\\):$_long_:" \
       -e "/^s|/!d" $INP > $OUT
}

make_multisitemap ()
{  # each category gets its own column along with the usual entries
   OUTPUT="$1" ; test ".$OUTPUT" = "." && OUTPUT="./$F.$BODY"
   INPUTS="$2" ; test ".$INPUTS" = "." && INPUTS="./$MK.$INFO"
   siteinfo2sitemap ./$MK.site.tmp # have <name><long><date> addon-sed
  _form_="<!--\"\\2\"--><!--use\\1--><!--long--><!--end\\1-->"
  _form_="$_form_<br><!--name--><!--date-->"
  _tiny_="small><small><small" ; _tinyX_="small></small></small "
  _tabb_="<br><$_tiny_> </$_tinyX_>" ; _bigg_="<big> </big>"
  echo "<table width=\"100%\"><tr><td> " > $OUTPUT
  $SED -e "/=use.=/!d" -e "s|=use\\(.\\)=\\([^ ]*\\) .*|$_form_|" \
       -f ./$MK.site.tmp -e "/<name/!d" \
       -e "s|<!--use1-->|</td><td valign=\"top\"><b>|" \
       -e "s|<!--end1-->|</b>|"  \
       -e "s|<!--use2-->|<br>|"  \
       -e "s|<!--use.-->|<br>|" -e "s/<!--[^<>]*-->/ /g" \
       -e "s|<long>||" -e "s|</long>||" \
       -e "s|<name |<$_tiny_><a |" -e "s|</name>||" \
       -e "s|<date>| |" -e "s|</date>|</a><br></$_tinyX_>|" \
       $INPUTS              >> $OUTPUT
   echo "</td><tr></table>" >> $OUTPUT
}

make_listsitemap ()
{   # traditional - the body contains a list with date and title extras
   OUTPUT="$1" ; test ".$OUTPUT" = "." && OUTPUT="$F.$BODY"
   INPUTS="$2" ; test ".$INPUTS" = "." && INPUTS="./$MK.$INFO"
   siteinfo2sitemap ./$MK.site.tmp # have <name><long><date> addon-sed
   _form_="<!--\"\\2\"--><!--use\\1--><!--name--><!--date--><!--long-->"
   _tabb_="<td>\\&nbsp\\;</td>" 
   echo "<table cellspacing=\"0\" cellpadding=\"0\">" > $OUTPUT
   $SED -e "/=use.=/!d" -e "s|=use\\(.\\)=\\([^ ]*\\) .*|$_form_|" \
        -f ./$MK.site.tmp -e "/<name/!d" \
        -e "s|<!--use1-->|<tr><td>*</td>|" \
        -e "s|<!--use2-->|<tr><td>-</td>|" \
        -e  "/<!--use3-->/s|<name [^<>]*>|&- |" \
        -e "s|<!--use.-->|<tr><td> </td>|" -e "s/<!--[^<>]*-->/ /g" \
        -e "s|<name |<td><a |" -e "s|</name>|</a></td>$_tabb_|" \
        -e "s|<date>|<td><small>|" -e "s|</date>|</small></td>$_tabb_|" \
        -e "s|<long>|<td><em>|" -e "s|</long>|</em></td></tr>|" \
        $INPUTS             >> $OUTPUT
   echo "</table>"          >> $OUTPUT
}

print_extension ()
{
    ARG="$1" ; test ".$ARG" = "." && ARG="$opt_print"
    case "$ARG" in
      -*|.*) echo "$ARG" ;;
      *)     echo ".print" ;;
    esac
}
    
html_sourcefile ()  # generally just cut away the trailing "l" (ell)
{                   # making "page.html" argument into "page.htm" return
    _SRCFILE_=`echo "$1" | $SED -e "s/l\\$//"`
    if test -f "$_SRCFILE_" ; then echo "$_SRCFILE_"
    elif test -f "$opt_srcdir/$_SRCFILE_" ; then echo "$opt_srcdir/$_SRCFILE_"
    else echo ".//$_SRCFILE_"
    fi
}
html_printerfile_sourcefile () 
{                   
    if test ".$printerfriendly" = "."
    then 
    echo "$1" | sed -e "s/l\$//"
    else 
    _ext_=`print_extension "$printerfriendly"`
    _ext_=`sed_slash_key "$_ext_"`
    echo "$1" | sed -e "s/l\$//" -e "s/$_ext_\\([.][$AA]*\\)\$/\\1/"
    fi
}

moved_html_printerfile () {
    x=`html_printerfile "$1"` ; moved_href "$x" $2
}

html_printerfile () # generate the printerfile for a given normal output
{
    _ext_=`print_extension "$printerfriendly" | sed -e "s/&/\\\\&/"`
    echo "$1" | sed -e "s/\\([.][$AA]*\\)\$/$_ext_\\1/"
}

make_printerfile_move () # generate s/file.html/file.print.html/ for hrefs
{                        # we do that only for the $FILELIST
   OUTPUT="$1"   ; test ".$OUTPUT"   = "." && OUTPUT="$MK.move.tmp"
   ALLPAGES="$2" ; test ".$ALLPAGES" = "." && ALLPAGES="$FILELIST"
   $CATNULL > $OUTPUT
   for p in $ALLPAGES ; do
       a=`sed_slash_key "$p"`
       b=`html_printerfile "$p"`
       if test "$b" != "$p" ; then
         b=`html_printerfile "$p" | sed -e "s:&:\\\\&:g" -e "s:/:\\\\\\/:g"`
         echo "s/<a href=\"$a\">/<a href=\"$b\">/" >> $OUTPUT
       fi
   done
}

# these alt-texts will be only visible in with a text-mode browser:
printsitefile_square="width=\"8\" height=\"8\" border=\"0\""
printsitefile_img_1="<img alt=\"|go text:\" $printsitefile_square />"
printsitefile_img_2="<img alt=\"||topics:\" $printsitefile_square />"
printsitefile_img_3="<img alt=\"|||pages:\" $printsitefile_square />"
make_printsitefile ()
{
   OUTPUT="$1" ; test ".$OUTPUT" = "." && OUTPUT="$SITEFILE.print.html"
   INPUTS="$2" ; test ".$INPUTS" = "." && INPUTS="./$MK.$INFO"
   _bold_="text-decoration : none ; font-weight : bold ; "
   echo "   <style>"                                     > $MK.style.tmp
   echo "     a:link    { $_bold_ color : #000060 ; }"  >> $MK.style.tmp
   echo "     a:visited { $_bold_ color : #000040 ; }"  >> $MK.style.tmp
   echo "     body      { background-color : white ; }" >> $MK.style.tmp
   echo "   </style>"                                   >> $MK.style.tmp
   siteinfo2sitemap ./$MK.site.tmp # have <name><long><date> addon-sed
   _form_="<!--\"\\2\"--><!--use\\1--><!--name--><!--date--><!--long-->"
   _tabb_="<td>\\&nbsp\\;</td>" 
   $SED -e "/<title>/p" -e "/<title>/d" \
        -e "/<head>/p"   -e "/<head>/d" \
        -e "/<\/head>/p"   -e "/<\/head>/d" \
        -e "/<body>/p"   -e "/<body>/d" \
        -e "/^.*<link [^<>]*rel=\"shortcut icon\"[^<>]*>.*\$/p" \
        -e "d" $SITEFILE | $SED -e "/<head>/r ./$MK.style.tmp" > $OUTPUT

   sep=" - " ; _left_=" [ " ; _right_=" ] "
   _sect1="<a href=\"#.\" title=\"section\">$printsitefile_img_1</a> ||$sep"
   _sect2="<a href=\"#.\" title=\"topics\">$printsitefile_img_2</a> ||$sep"
   _sect3="<a href=\"#.\" title=\"pages\">$printsitefile_img_3</a> ||$sep"
   site_get_rootsections > ./$MK.sect1.tmp
   test -d DEBUG && echo "# rootsections"       > DEBUG/printsitemap.txt
   test -d DEBUG && cat ./$MK.sect1.tmp        >> DEBUG/printsitemap.txt
   for r in `cat ./$MK.sect1.tmp` ; do
   echo "<!--mksite:sect:\"$r\"--><!--mksite:sect1:A--><br>$_sect1" >> $OUTPUT
   for s in `cat ./$MK.sect1.tmp` ; do 
   rr=`sed_slash_key "$r"`  
   echo "<!--mksite:sect:\"$r\"--><a href=\"$s\"><!--\"$s\"--><!--name--></a>$sep" \
        | $SED -f ./$MK.site.tmp -e "s/<name[^<>]*>//" -e "s/<\\/name>//" \
         -e "/<a href=\"$rr\"/s/<a href/$_left_&/" \
         -e "/<a href=\"$rr\"/s/<\\/a>/&$_right_/" \
         -e "s/<!--\"[^\"]*\"--><!--name-->//" >> $OUTPUT
   done
   echo "<!--mksite:sect:\"$s\"--><!--mksite:sect1:Z-->" >> $OUTPUT

#  site_get_sectionpages "$r" > ./$MK.sect2.tmp
   site_get_subpages "$r"     > ./$MK.sect2.tmp
   test -d DEBUG && echo "# subsections $r"    >> DEBUG/printsitemap.txt
   test -d DEBUG && cat ./$MK.sect2.tmp        >> DEBUG/printsitemap.txt
   for s in `cat ./$MK.sect2.tmp` ; do test "$r" = "$s" && continue
   echo "<!--mksite:sect:\"$s\"--><!--mksite:sect2:A--><br>$_sect2" >> $OUTPUT
   for t in `cat ./$MK.sect2.tmp` ; do test "$r" = "$t" && continue
   ss=`sed_slash_key "$s"`  
   echo "<!--mksite:sect:\"$s\"--><a href=\"$t\"><!--\"$t\"--><!--name--></a>$sep" \
        | $SED -f ./$MK.site.tmp -e "s/<name[^<>]*>//" -e "s/<\\/name>//" \
         -e "/<a href=\"$ss\"/s/<a href/$_left_&/" \
         -e "/<a href=\"$ss\"/s/<\\/a>/&$_right_/" \
         -e "s/<!--\"[^\"]*\"--><!--name-->//" >> $OUTPUT
   done # "$t"
   echo "<!--mksite:sect:\"$s\"--><!--mksite:sect2:Z-->" >> $OUTPUT


#  site_get_sectionpages "$s" > ./$MK.sect3.tmp
   site_get_subpages "$s"     > ./$MK.sect3.tmp
   test -d DEBUG && echo "# subsubsections $s" >> DEBUG/printsitemap.txt
   test -d DEBUG && cat ./$MK.sect3.tmp        >> DEBUG/printsitemap.txt
   for t in `cat ./$MK.sect3.tmp` ; do test "$s" = "$t" && continue
   echo "<!--mksite:sect:\"$t\"--><!--mksite:sect3:A--><br>$_sect3" >> $OUTPUT
   for u in `cat ./$MK.sect3.tmp` ; do test "$s" = "$u" && continue
   tt=`sed_slash_key "$t"` 
   echo "<!--mksite:sect:\"$t\"--><a href=\"$u\"><!--\"$u\"--><!--name--></a>$sep" \
        | $SED -f ./$MK.site.tmp -e "s/<name[^<>]*>//" -e "s/<\\/name>//" \
         -e "/<a href=\"$tt\"/s/<a href/$_left_&/" \
         -e "/<a href=\"$tt\"/s/<\\/a>/&$_right_/" \
         -e "s/<!--\"[^\"]*\"--><!--name-->//" >> $OUTPUT
   done # "$u"
   echo "<!--mksite:sect:\"$t\"--><!--mksite:sect3:Z-->" >> $OUTPUT
   done # "$t"

   _have_children_="0"
   for u in `cat ./$MK.sect3.tmp` ; do test "$r" = "$t" && continue
   test "$_have_children_" = "0" && _have_children_="1" && \
   echo "<!--mksite:sect:*:\"$s\"--><!--mksite:sect3:A--><br>$_sect3" >> $OUTPUT
   echo "<!--mksite:sect:*:\"$s\"--><a href=\"$u\"><!--\"$u\"--><!--name--></a>$sep" \
        | $SED -f ./$MK.site.tmp -e "s/<name[^<>]*>//" -e "s/<\\/name>//" \
         -e "s/<!--\"[^\"]*\"--><!--name-->//" >> $OUTPUT
   done # "$u"
   test "$_have_children_" = "1" && \
   echo "<!--mksite:sect:*:\"$s\"--><!--mksite:sect3:Z-->" >> $OUTPUT
   done # "$s"

   _have_children_="0"
   for t in `cat ./$MK.sect2.tmp` ; do test "$r" = "$t" && continue
   test "$_have_children_" = "0" && _have_children_="1" && \
   echo "<!--mksite:sect:*:\"$r\"--><!--mksite:sect2:A--><br>$_sect2" >> $OUTPUT
   echo "<!--mksite:sect:*:\"$r\"--><a href=\"$t\"><!--\"$t\"--><!--name--></a>$sep" \
        | $SED -f ./$MK.site.tmp -e "s/<name[^<>]*>//" -e "s/<\\/name>//" \
         -e "s/<!--\"[^\"]*\"--><!--name-->//" >> $OUTPUT
   done # "$t"
   test "$_have_children_" = "1" && \
   echo "<!--mksite:sect:*:\"$r\"--><!--mksite:sect2:Z-->" >> $OUTPUT
   done # "$r"
   echo "<a name=\".\"></a>" >> $OUTPUT
   echo "</body></html>"     >> $OUTPUT
}

select_in_printsitefile () # arg = "page" : return to stdout >> $P.$HEAD
{
   _selected_="$1" ; test ".$_selected_" = "." && _selected_="$F"
   _section_=`sed_slash_key "$_selected_"`
   echo "s/^<!--mksite:sect:\"$_section_\"-->//"        # sect3
   echo "s/^<!--mksite:sect:[*]:\"$_section_\"-->//"    # children
   _selected_=`site_get_parentpage "$_selected_"` 
   _section_=`sed_slash_key "$_selected_"`
   echo "s/^<!--mksite:sect:\"$_section_\"-->//"        # sect2
   _selected_=`site_get_parentpage "$_selected_"` 
   _section_=`sed_slash_key "$_selected_"`
   echo "s/^<!--mksite:sect:\"$_section_\"-->//"        # sect1
   echo "/^<!--mksite:sect:\"[^\"]*\"-->/d"     
   echo "/^<!--mksite:sect:[*]:\"[^\"]*\"-->/d" 
   echo "s/^<!--mksite:sect[$NN]:[$AZ]-->//" 
}

body_for_emailfooter ()
{
    test ".$emailfooter" = ".no" && return
    _email_=`echo "$emailfooter" | sed -e "s|[?].*||"`
    _dated_=`info_get_entry updated`
    echo "<hr><table border=\"0\" width=\"100%\"><tr><td>"
    echo "<a href=\"mailto:$emailfooter\">$_email_</a>"
    echo "</td><td align=\"right\">"
    echo "$_dated_</td></tr></table>"
}

# ==========================================================================
#  
#  During processing we will create a series of intermediate files that
#  store relations. They all have the same format being
#   =relationtype=key value
#  where key is usually s filename or an anchor. For mere convenience
#  we assume that the source html text does not have lines that start
#  off with =xxxx= (btw, ye remember perl section notation...). Of course
#  any other format would be usuable as well.
#

# we scan the SITEFILE for href references to be converted
# - in the new variant we use a ".gets.tmp" sed script that            SECTS
# marks all interesting lines so they can be checked later
# with an sed anchor of <!--sect[$NN]--> (or <!--sect[$AZ]-->)
S="\\&nbsp\\;"
# S="[&]nbsp[;]"

# HR and EM style markups must exist in input - BR sometimes left out 
# these routines in(ter)ject hardspace before, between, after markups
# note that "<br>" is sometimes used with HR - it must exist in input
echo_HR_EM_PP ()
{
    echo "/^$1$2$3*<a href=/s/^/$4/"
    echo "/^<>$1$2$3*<a href=/s/^/$4/"
    echo "/^$S$1$2$3*<a href=/s/^/$4/"
    echo "/^$1<>$2$3*<a href=/s/^/$4/"
    echo "/^$1$S$2$3*<a href=/s/^/$4/"
    echo "/^$1$2<>$3*<a href=/s/^/$4/"
    echo "/^$1$2$S$3*<a href=/s/^/$4/"
}

echo_br_EM_PP ()
{
    echo_HR_EM_PP  "$1" "$2" "$3" "$4"
    echo "/^$2$3*<a href=/s/^/$4/"
    echo "/^<>$2$3*<a href=/s/^/$4/"
    echo "/^$S$2$3*<a href=/s/^/$4/"
    echo "/^$2<>$3*<a href=/s/^/$4/"
    echo "/^$2$S$3*<a href=/s/^/$4/"
}    

echo_HR_PP ()
{
    echo "/^$1$2*<a href=/s/^/$3/"
    echo "/^<>$1$2*<a href=/s/^/$3/"
    echo "/^$S$1$2*<a href=/s/^/$3/"
    echo "/^$1<>$2*<a href=/s/^/$3/"
    echo "/^$1$S$2*<a href=/s/^/$3/"
}
echo_br_PP ()
{
    echo_HR_PP "$1" "$2" "$3"
    echo "/^$2*<a href=/s/^/$3/"
    echo "/^<>$2*<a href=/s/^/$3/"
    echo "/^$S$2*<a href=/s/^/$3/"
}
echo_sp_PP ()
{
    echo "/^<>$1*<a href=/s/^/$2/"
    echo "/^$S$1*<a href=/s/^/$2/"
    echo "/^<><>$1*<a href=/s/^/$2/"
    echo "/^$S$S$1*<a href=/s/^/$2/"
    echo "/^<>$1<>*<a href=/s/^/$2/"
    echo "/^$S$1$S*<a href=/s/^/$2/"
    echo "/^$1<><>*<a href=/s/^/$2/"
    echo "/^$1$S$S*<a href=/s/^/$2/"
    echo "/^$1<>*<a href=/s/^/$2/"
    echo "/^$1$S*<a href=/s/^/$2/"
}

h1="[-|[]"
b1="[*=]"
b2="[-|[]"
b3="[\\/:]"
q3="[\\/:,[]"
echo_HR_PP    "<hr>"            "$h1"    "<!--sect1-->"      > $MK.gets.tmp
echo_HR_EM_PP "<hr>" "<em>"     "$h1"    "<!--sect1-->"     >> $MK.gets.tmp
echo_HR_EM_PP "<hr>" "<strong>" "$h1"    "<!--sect1-->"     >> $MK.gets.tmp
echo_HR_PP    "<br>"            "$b1$b1" "<!--sect1-->"     >> $MK.gets.tmp
echo_HR_PP    "<br>"            "$b2$b2" "<!--sect2-->"     >> $MK.gets.tmp
echo_HR_PP    "<br>"            "$b3$b3" "<!--sect3-->"     >> $MK.gets.tmp
echo_br_PP    "<br>"            "$b2$b2" "<!--sect2-->"     >> $MK.gets.tmp
echo_br_PP    "<br>"            "$b3$b3" "<!--sect3-->"     >> $MK.gets.tmp
echo_br_EM_PP "<br>" "<small>"  "$q3"    "<!--sect3-->"     >> $MK.gets.tmp
echo_br_EM_PP "<br>" "<em>"     "$q3"    "<!--sect3-->"     >> $MK.gets.tmp
echo_br_EM_PP "<br>" "<u>"      "$q3"    "<!--sect3-->"     >> $MK.gets.tmp
echo_HR_PP    "<br>"            "$q3"    "<!--sect3-->"     >> $MK.gets.tmp
echo_sp_PP                      "$q3"    "<!--sect3-->"     >> $MK.gets.tmp
$SED -e "s/>\\[/> *[/" ./$MK.gets.tmp > $MK.puts.tmp
# the .puts.tmp variant is used to <b><a href=..></b> some hrefs which
# shall not be used otherwise for being generated - this is nice for
# some quicklinks somewhere. The difference: a whitspace "<hr> <a...>"

# scan sitefile for references pages - store as =use+= relation
_uses_="=use\\1=\\2 \\3" ; 
_getX_="<!--sect\\([$NN]\\)--><[^<>]*>[^<>]*"
_getY_="<!--sect\\([$NN]\\)--><[^<>]*>[^<>]*<[^<>]*>[^<>]*"
$SED -f $MK.gets.tmp           -e "/^<!--sect[$NN]-->/!d" \
     -e "s:^$_getX_<a href=\"\\([^\"]*\\)\"[^<>]*>\\(.*\\)</a>.*:$_uses_:" \
     -e "s:^$_getY_<a href=\"\\([^\"]*\\)\"[^<>]*>\\(.*\\)</a>.*:$_uses_:" \
     -e "/^=....=/!d" $SITEFILE > $MK.$INFO
# scan used pages and store relation =sect= pointing to section group
$SED -e "/=use.=/!d" \
     -e "/=use1=/{" -e "h" -e "s:=use1=\\([^ ]*\\) .*:\\1:" -e "x" -e "}" \
     -e "s/=use.=\\([^ ]*\\) .*/=sect=\\1/" \
     -e G -e "s:\\n: :" ./$MK.$INFO >> $MK.$INFO
$SED -e "/=use.=/!d" \
     -e "/=use1=/{" -e "h" -e "s:=use1=\\([^ ]*\\) .*:\\1:" -e "x" -e "}" \
     -e "/=use1=/d" \
     -e "/=use3=/d" \
     -e "s/=use.=\\([^ ]*\\) .*/=node=\\1/" \
     -e G -e "s:\\n: :" ./$MK.$INFO >> $MK.$INFO
# scan used pages and store relation =page= pointing to topic group
$SED -e "/=use.=/!d" \
     -e "/=use1=/{" -e "h" -e "s:=use1=\\([^ ]*\\) .*:\\1:" -e "x" -e "}" \
     -e "/=use2=/{" -e "h" -e "s:=use2=\\([^ ]*\\) .*:\\1:" -e "x" -e "}" \
     -e "/=use1=/d" \
     -e "s/=use.=\\([^ ]*\\) .*/=page=\\1/" \
     -e G -e "s:\\n: :" ./$MK.$INFO >> $MK.$INFO
$SED -e "/=use.=/!d" \
     -e "/=use1=/{" -e "h" -e "s:=use1=\\([^ ]*\\) .*:\\1:" -e "x" -e "}" \
     -e "/=use2=/{" -e "h" -e "s:=use2=\\([^ ]*\\) .*:\\1:" -e "x" -e "}" \
     -e "/=use1=/d" \
     -e "/=use2=/d" \
     -e "s/=use.=\\([^ ]*\\) .*/=node=\\1/" \
     -e G -e "s:\\n: :" ./$MK.$INFO >> $MK.$INFO
# and for the root sections we register ".." as the parenting group
$SED -e "/=use1=/!d" \
     -e "s/=use.=\\([^ ]*\\) .*/=node=\\1 ../"  ./$MK.$INFO >> $MK.$INFO

FILELIST=`$SED -e "/=use.=/!d" -e "s/=use.=//" -e "s/ .*//" ./$MK.$INFO`


# ==========================================================================
#                                                             MAGIC VARS
#                                                            IN $SITEFILE
printerfriendly=""
sectionlayout="list"
sitemaplayout="list"
simplevars="warn"      # <!--varname-->default
attribvars=" "         # <x ref="${varname:=default}">
updatevars=" "         # <!--$varname:=-->default
expandvars=" "         # <!--$varname-->
commentvars=" "        # $updatevars && $expandsvars && $simplevars
sectiontab=" "         # highlight ^<td class=...>...href="$section"
currenttab=" "         # highlight ^<br>..<a href="$topic">
headsection="no"
tailsection="no"
sectioninfo="no"       # using <h2> title <h2> = info text
emailfooter="no"

if $GREP "<!--multi-->"               $SITEFILE >$NULL ; then
echo \
"WARNING: do not use <!--multi-->, change to <!--mksite:multi--> " "$SITEFILE"
echo \
"warning: or <!--mksite:multisectionlayout--> <!--mksite:multisitemaplayout-->"
sectionlayout="multi"
sitemaplayout="multi"
fi
if $GREP "<!--mksite:multi-->"               $SITEFILE >$NULL ; then
sectionlayout="multi"
sitemaplayout="multi"
fi
if $GREP "<!--mksite:multilayout-->"         $SITEFILE >$NULL ; then
sectionlayout="multi"
sitemaplayout="multi"
fi
x=`mksite_magic_option sectionlayout` ; case "$x" in
       "list"|"multi") sectionlayout="$x" ;; esac
x=`mksite_magic_option sitemaplayout` ; case "$x" in
       "list"|"multi") sitemaplayout="$x" ;; esac
x=`mksite_magic_option simplevars` ; case "$x" in
      " "|"no"|"warn") simplevars="$x" ;; esac
x=`mksite_magic_option attribvars` ; case "$x" in
      " "|"no"|"warn") attribvars="$x" ;; esac
x=`mksite_magic_option updatevars` ; case "$x" in
      " "|"no"|"warn") updatevars="$x" ;; esac
x=`mksite_magic_option expandvars` ; case "$x" in
      " "|"no"|"warn") expandvars="$x" ;; esac
x=`mksite_magic_option commentvars` ; case "$x" in
      " "|"no"|"warn") commentvars="$x" ;; esac
x=`mksite_magic_option printerfriendly` ; case "$x" in
        " "|".*"|"-*") printerfriendly="$x" ;; esac
x=`mksite_magic_option sectiontab` ; case "$x" in
      " "|"no"|"warn") sectiontab="$x" ;; esac
x=`mksite_magic_option currenttab` ; case "$x" in
      " "|"no"|"warn") currenttab="$x" ;; esac
x=`mksite_magic_option sectioninfo` ; case "$x" in
      " "|"no"|"[=-:]") sectioninfo="$x" ;; esac
x=`mksite_magic_option emailfooter` 
   test ".$x" != "." && emailfooter="$x"

test ".$opt_print" != "." && printerfriendly="$opt_print"
test ".$commentvars"  = ".no" && updatevars="no"   # duplicated into
test ".$commentvars"  = ".no" && expandvars="no"   # info2vars ()
test ".$commentvars"  = ".no" && simplevars="no"   # function above

test -d DEBUG && \
echo "NOTE: '$sectionlayout'sectionlayout '$sitemaplayout'sitemaplayout"
test -d DEBUG && \
echo "NOTE: '$simplevars'simplevars '$printerfriendly'printerfriendly"
test -d DEBUG && \
echo "NOTE: '$attribvars'attribvars '$updatevars'updatevars"
test -d DEBUG && \
echo "NOTE: '$expandvars'expandvars '$commentvars'commentvars "
test -d DEBUG && \
echo "NOTE: '$currenttab'currenttab '$sectiontab'sectiontab"
test -d DEBUG && \
echo "NOTE: '$headsection'headsection '$tailsection'tailsection"

# ==========================================================================
# originally this was a one-pass compiler but the more information
# we were scanning out the more slower the system ran - since we
# were rescanning files for things like section information. Now
# we scan the files first for global information.
#                                                                    1.PASS

for F in $FILELIST ; do case "$F" in
http:*|*://*) ;; # skip
${SITEFILE}|${SITEFILE}l) SOURCEFILE=`html_sourcefile "$F"`
if test "$SOURCEFILE" != "$F" ; then
   echo "=text=today `$DATE_NOW +%Y-%m-%d`"          > $F.$INFO
   echo "=meta=formatter `basename $0`"             >> $F.$INFO
   short=`echo "$F" | $SED -e "s:.*/::" -e "s:[.].*::"`
   short="$short *"
   DC_meta title "$short"
   DC_meta date.available `$DATE_NOW +%Y-%m-%d`
   DC_meta subject sitemap
   DC_meta DCMIType Collection
   DC_VARS_Of $SOURCEFILE 
   DC_modified $SOURCEFILE ; DC_date $SOURCEFILE
   DC_section "$F"
   DX_text date.formatted `$DATE_NOW +%Y-%m-%d`
   test ".$printerfriendly" != "." && \
   DX_text printerfriendly `moved_html_printerfile "$F"`
   test ".$USER" != "." && DC_publisher "$USER"
   echo "'$SOURCEFILE': $short (sitemap)"
   echo "=list=$F $short"          >> $MK.$INFO
   echo "=date=$F `$DATE_NOW +%Y-%m-%d`" >> $MK.$INFO
   echo "=long=$F generated sitemap index" >> $MK.$INFO
fi ;;
../*) 
   echo "!! -> '$F' (skipping topdir build)"
   ;;
# */*.html) 
#    make_move  # try for later subdir build
#    echo "!! -> '$F' (skipping subdir build)"
#    ;;
# */*/*/|*/*/|*/|*/index.htm|*/index.html) 
#    echo "!! -> '$F' (skipping subdir index.html)"
#    ;;
*.html)  SOURCEFILE=`html_sourcefile "$F"`                           # SCAN :
if test "$SOURCEFILE" != "$F" ; then :                               # HTML :
if test -f "$SOURCEFILE" ; then make_move "$F"
   echo "=text=today `$DATE_NOW +%Y-%m-%d`"  > $F.$INFO
   echo "=text=todays `$DATE_NOW +%Y-%m%d`" >> $F.$INFO
   echo "=meta=formatter `basename $0`"     >> $F.$INFO
   DC_VARS_Of "$SOURCEFILE" 
   DC_title "$SOURCEFILE"
   DC_isFormatOf "$SOURCEFILE" 
   DC_modified "$SOURCEFILE" ; DC_date "$SOURCEFILE" ; DC_date "$SITEFILE"
   DC_section "$F" ;  DC_selected "$F" ;  DX_alternative "$SOURCEFILE"
   test ".$USER" != "." && DC_publisher "$USER"
   DX_text date.formatted `$DATE_NOW +%Y-%m-%d`
   test ".$printerfriendly" != "." && \
   DX_text printerfriendly `moved_html_printerfile "$F"`
   sectn=`info_get_entry DC.relation.section`
   short=`info_get_entry DC.title.selected`
   echo "=list=$short" >> $F.$INFO    ; echo "=list=$F $short" >> $MK.$INFO
   title=`info_get_entry DC.title`
   echo "=long=$title" >> $F.$INFO    ; echo "=long=$F $title" >> $MK.$INFO
   edate=`info_get_entry DC.date`
   issue=`info_get_entry issue`
   echo "=date=$edate" >> $F.$INFO    ; echo "=date=$F $edate" >> $MK.$INFO
   echo "'$SOURCEFILE':  '$title' ('$short') @ '$issue' ('$sectn')"
else
   echo "'$SOURCEFILE': does not exist"
   echo "=list=$F $F"                 >> $MK.$INFO
   echo "=long=$F (no source)"        >> $MK.$INFO
fi ; else
   echo "<$F> - skipped"
fi ;;
*/) echo "'$F' : directory - skipped"
   echo "=list=$F `sed_slash_key $F`" >> $MK.$INFO
   echo "=long=$F (directory)"        >> $MK.$INFO
   ;;
*) echo "?? -> '$F'"
   ;;
esac done

if test ".$simplevars" = ". " ; then
$CATNULL > $MK.olds.tmp
fi

# ==========================================================================
if test ".$printerfriendly" != "." ; then                          #  PRINT
_ext_=`print_extension "$printerfriendly" | sed -e "s/&/\\\\&/"`   # VERSION
PRINTSITEFILE=`echo "$SITEFILE" | sed -e "s/\\.[$AA]*\$/$_ext_&/"`
echo "NOTE: going to create printer-friendly sitefile $PRINTSITEFILE"
make_printsitefile "$PRINTSITEFILE"
fi

# ==========================================================================
# and now generate the output pages
#                                                                   2.PASS

for F in $FILELIST ; do case "$F" in
http:*|*://*) : ;; # skip
# .............................................................. SITE FILE
${SITEFILE}|${SITEFILE}l)  SOURCEFILE=`html_sourcefile "$F"`
if test "$SOURCEFILE" != "$F" ; then
if test -f "$SOURCEFILE" ; then
   # remember that in this case "${SITEFILE}l" = "$F" = "${SOURCEFILE}l"
   test ".$simplevars" = ".warn" && \
   info2test $MK.test.tmp          # check <!--title--> vars old-style
   info2vars $MK.vars.tmp          # have <!--title--> vars substituted
   info2meta $MK.meta.tmp          # add <meta name="DC.title"> values
   test ".$simplevars" = ".warn" && \
   $SED_LONGSCRIPT ./$MK.test.tmp $SOURCEFILE | tee -a ./$MK.olds.tmp
   SECTS="<!--sect[$NN$AZ]-->" ; SECTN="<!--sect[$NN]-->" # lines with hrefs
   CURRENT_SECTION=`info_get_entry DC.relation.section`
   SECTION=`sed_slash_key "$CURRENT_SECTION"`
   $CAT ./$MK.puts.tmp                                       > $F.$HEAD 
   echo "/^$SECTS.*<a href=\"$F\">/s|</a>|</a></b>|"        >> $F.$HEAD
   echo "/^$SECTS.*<a href=\"$F\">/s|<a href=|<b><a href=|" >> $F.$HEAD
   test ".$sectiontab" != ".no" && \
   echo "/ href=\"$SECTION\"/s|^<td class=\"[^\"]*\"|<td |" >> $F.$HEAD
   echo "/<head>/r $MK.meta.tmp"                            >> $F.$HEAD
   $CAT ./$MK.vars.tmp ./$MK.tags.tmp >> $F.$HEAD
   echo "/<\\/body>/d"                >> $F.$HEAD
   case "$sitemaplayout" in
   multi) make_multisitemap $F.$BODY ;;       # here we use ~body~ as
   *)     make_listsitemap  $F.$BODY ;;       # a plain text file
   esac
   $SED_LONGSCRIPT $F.$HEAD                  $SITEFILE  > $F   # ~head~
   $CAT            $F.$BODY                            >> $F   # ~body~
   $SED -e "/<\\/body>/!d" -f ./$MK.vars.tmp $SITEFILE >> $F   #</body>
   echo "'$SOURCEFILE': " `ls -s $SOURCEFILE` "->" `ls -s $F` "(sitemap)"
else
   echo "'$SOURCEFILE': does not exist"
fi fi ;;
../*) 
   echo "!! -> '$F' (skipping topdir build)"
   ;;
# */*.html) 
#   echo "!! -> '$F' (skipping subdir build)"
#   ;;
# */*/*/|*/*/|*/|*/index.htm|*/index.html) 
#   echo "!! -> '$F' (skipping subdir index.html)"
#   ;;
# ............................................................... HTML FILES
*.html)  SOURCEFILE=`html_sourcefile "$F"`                      #     2.PASS
if test "$SOURCEFILE" != "$F" ; then
if test -f "$SOURCEFILE" ; then
   test ".$simplevars" = ".warn" && \
   info2test $MK.test.tmp          # check <!--title--> vars old-style
   info2vars $MK.vars.tmp          # have <!--$title--> vars substituted
   info2meta $MK.meta.tmp          # add <meta name="DC.title"> values
   test ".$simplevars" = ".warn" && \
   $SED_LONGSCRIPT ./$MK.test.tmp $SOURCEFILE | tee -a ./$MK.olds.tmp
   SECTS="<!--sect[$NN$AZ]-->" ; SECTN="<!--sect[$NN]-->" # lines with hrefs
   CURRENT_SECTION=`info_get_entry DC.relation.section`
   SECTION=`sed_slash_key "$CURRENT_SECTION"`
   FF=`sed_slash_key "$F"`
   case "$sectionlayout" in
   multi) # sitefile navigation bar is split into sections
      $CAT ./$MK.puts.tmp                                          > $F.$HEAD 
      # grep all pages with a =sect= relation to current $SECTION and
      # build foreach an sed line "s|$SECTS\(<a href=$F>\)|<!--sectX-->\1|"
      # after that all the (still) numeric SECTNs are deactivated / killed.
      for i in $SECTION $headsection $tailsection ; do
      $SED -e "/^=sect=[^ ]* $SECTION/!d" \
           -e "s, .*,\"\\\\)|<!--sectX-->\\\\1|,"  \
           -e "s,^=sect=,s|^$SECTS\\\\(.*<a href=\"," ./$MK.$INFO >> $F.$HEAD
      done
      echo "s|^$SECTN[^ ]*\\(<a href=[^<>]*>\\).*|<!-- \\1 -->|"  >> $F.$HEAD
      echo "/^$SECTS.*<a href=\"$FF\">/s|</a>|</a></b>|"          >> $F.$HEAD
      echo "/^$SECTS.*<a href=\"$FF\">/s|<a href=|<b><a href=|"   >> $F.$HEAD
      test ".$sectiontab" != ".no" && \
      echo "/ href=\"$SECTION\"/s|^<td class=\"[^\"]*\"|<td |"    >> $F.$HEAD
      $CAT ./$MK.vars.tmp ./$MK.tags.tmp >> $F.$HEAD #tag and vars
      echo "/<\\/body>/d"                >> $F.$HEAD #cut lastline
      echo "/<head>/r $MK.meta.tmp"      >> $F.$HEAD #add metatags
   ;;
   *) # traditional.... the sitefile is the full navigation bar
      $CAT ./$MK.puts.tmp                                          > $F.$HEAD  
      echo "/^$SECTS.*<a href=\"$FF\">/s|</a>|</a></b>|"          >> $F.$HEAD
      echo "/^$SECTS.*<a href=\"$FF\">/s|<a href=|<b><a href=|"   >> $F.$HEAD
      test ".$sectiontab" != ".no" && \
      echo "/ href=\"$SECTION\"/s|^<td class=\"[^\"]*\"|<td |"    >> $F.$HEAD
      $CAT ./$MK.vars.tmp ./$MK.tags.tmp >> $F.$HEAD #tag and vars
      echo "/<\\/body>/d"                >> $F.$HEAD #cut lastline
      echo "/<head>/r $MK.meta.tmp"      >> $F.$HEAD #add metatags
   ;; esac
      echo "/<title>/d"                   > $F.$BODY #not that line
      $CAT ./$MK.vars.tmp ./$MK.tags.tmp >> $F.$BODY #tag and vars
      bodymaker_for_sectioninfo          >> $F.$BODY #if sectioninfo
      info2body_append                      $F.$BODY #cut early
      info2head_append                      $F.$HEAD
      $CAT ./$F.~move~                   >> $F.$HEAD
      test ".$emailfooter" != ".no" && \
      body_for_emailfooter                > $F.$FOOT
      $SED_LONGSCRIPT ./$F.$HEAD $SITEFILE               > $F # ~head~
      $SED_LONGSCRIPT ./$F.$BODY $SOURCEFILE            >> $F # ~body~
      test -f ./$F.$FOOT && $CAT ./$F.$FOOT             >> $F # ~foot~
      $SED -e "/<\\/body>/!d" -f $MK.vars.tmp $SITEFILE >> $F #</body>
   echo "'$SOURCEFILE': " `ls -s $SOURCEFILE` "->" `ls -s $F`
else
   echo "'$SOURCEFILE': does not exist"
fi ; else
   echo "<$F> - skipped"
fi ;;
*/) echo "'$F' : directory - skipped"
   ;;
*) echo "?? -> '$F'"
   ;;
esac
   # .......................................................................
if test ".$printerfriendly" != "." ; then                         # PRINTER
  printsitefile="0"                                               # FRIENDLY
  case "$F" in
  ${SITEFILE}|${SITEFILE}l) make_move "$F"
          printsitefile="*>" ; BODY_TXT="./$F.$BODY" ; BODY_SED="./$P.$HEAD";;
  *.html) printsitefile="=>" ; BODY_TXT="$SOURCEFILE"; BODY_SED="./$F.$BODY";;
  esac
  if test ".$printsitefile" != ".0" && test -f "$SOURCEFILE" ; then
      make_printerfile_move ./$MK.move.tmp
      P=`html_printerfile "$F"`
      $CAT ./$MK.vars.tmp ./$MK.tags.tmp ./$MK.move.tmp > ./$P.$HEAD
      $SED -e "/DC.relation.isFormatOf/s|content=\"[^\"]*\"|content=\"$F\"|" \
           ./$MK.meta.tmp > ./$MK.mett.tmp
      echo "/<head>/r $MK.mett.tmp"                    >> ./$P.$HEAD # meta
      echo "/<\\/body>/d"                              >> ./$P.$HEAD
      select_in_printsitefile "$F"                     >> ./$P.$HEAD
      _ext_=`print_extension "$printerfriendly"`                     # head-
      $SED -e "s/[.]html\"|/$_ext_&/g" ./$F.~move~     >> ./$P.$HEAD # hrefs
      line_=`sed_slash_key "$printsitefile_img_2"`                   # back-
      echo "/$line_/s| href=\"[#][.]\"| href=\"$F\"|"  >> ./$P.$HEAD # link.
      $CAT                             ./$F.~move~     >> ./$P.$HEAD # subdir
      $CAT ./$MK.vars.tmp ./$MK.tags.tmp ./$MK.move.tmp > ./$P.$BODY
      $SED -e "s/[.]html\"|/$_ext_&/g" ./$F.~move~     >> ./$P.$BODY # body-
      $CAT                             ./$F.~move~     >> ./$P.$BODY # hrefs
#     $CAT                                $BODY_SED    >> ./$P.$BODY # ORIG
      $SED_LONGSCRIPT ./$P.$HEAD              $PRINTSITEFILE  > $P # ~head~
      $SED_LONGSCRIPT ./$P.$BODY                   $BODY_TXT >> $P # ~body~
      $SED -e "/<\\/body>/!d" -f $MK.vars.tmp $PRINTSITEFILE >> $P #</body>
   echo "'$SOURCEFILE': " `ls -s $SOURCEFILE` "$printsitefile" `ls -s $P`
   fi 
fi
# .............. debug ....................
   if test -d DEBUG && test -f "./$F" ; then
      cp ./$F.$INFO DEBUG/$F.info.TMP
      for P in tags vars meta page date list html sect info ; do
      test -f ./$MK.$P.tmp && cp ./$MK.$P.tmp DEBUG/$F.$P.tmp
      test -f ./$MK.$P.TMP && cp ./$MK.$P.TMP DEBUG/$F.$P.TMP
      done
   fi
done
if test ".$simplevars" = ".warn" ; then 
oldvars=`cat ./$MK.olds.tmp | wc -l | $SED -e "s/ *//g"`
if test "$oldvars" = "0" ; then
echo "HINT: you have no simplevars in your htm sources, so you may want to"
echo "hint: set the magic <!--mksite:nosimplevars--> in your $SITEFILE"
echo "hint: which makes execution _faster_ actually in the 2. pass"
echo "note: simplevars expansion was the oldstyle way of variable expansion"
else
echo "HINT: there were $oldvars simplevars found in your htm sources."
echo "hint: This style of variable expansion will be disabled in the near"
echo "hint: future. If you do not want change then add the $SITEFILE magic"
echo "hint: <!--mksite:simplevars--> somewhere to suppress this warning"
echo "note: simplevars expansion will be an explicit option in the future."
echo "note: errornous simplevar detection can be suppressed with a magic"
echo "note: hint of <!--mksite:nosimplevars--> in the $SITEFILE for now."
fi fi

rm ./$MK.*.tmp
exit 0
