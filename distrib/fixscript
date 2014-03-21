#! /bin/bash
# To make this file executable:  chmod +x fixscript
# This is a very slight edit of the ltfixscript contributed to
# the Linmodems Newslist 
#  From - Sun Jul 23 04:27:38 2000
#  From: "Mark Spieth" <mark at  digivation.com.au>
#  To: <discuss@linmodems.org>
#  Subject: ltmodem symbols and version fixed
#  Date: Sun, 23 Jul 2000 12:39:44 +1000
#  Organization: Digivation Pty Ltd

echo "Fixscript V1.9"

OTHERMODULES=
while : ; do
case "$1" in
"-m" )	shift
	if [ -f "$1" ] ; then
		OTHERMODULES="$OTHERMODULES $1"
	else
		echo "ERROR: module $1 doesnt exist"
		exit 1
	fi
	shift 
	;;
*)	break;;
esac
done

echo $OTHERMODULES

if [ -z "$1" -o -z "$2" ]; then
cat <<END
This script changes version number tags of binary
kernel modules to match the version of the currently
running kernel. It also renames any symbol that the
current kernel can't resolve into their equivalent
resolvable symbols.

For inserting binary modules into kernels, the 'fixed' 
module can be inserted with:
	insmod module
which is used in automated kernel module management,
rather than forcing module loading with:
	insmod -f module
which is necessary when kernel and module versions
are not matched.

WARNING! This change is purely cosmetic, and the use
of version matched binaries whenever possible is
strongly advised. It may crash your kernel due to
inconsistencies in data structures between the kernel
as it stands and the headers used to originally compile
the module being fixed. No guarantees are given or implied
under any circumstances.

GNU objcopy version 2.9.5 or later is required;
this is provided as part of the 'binary utilities'
packages such as the Debian binutils.deb

USAGE: fixscript [-m othermodule] input-file output-file
  option -m can be repeated. space is mandatory
END
exit 1
fi

MI=/tmp/modinfo

[ -z "$DEPMOD" ] && DEPMOD=depmod

#extract existing .modinfo section as binary
#and new kernel version modinfo section
echo -ne "kernel_version="`uname -r`"\0" > $MI
echo -ne `objdump -s -j .modinfo $1 | 
	awk 'BEGIN{v=0;} /Contents/ {v=1; next; } { if (v==1) print $0; }' | 
	cut -c7-41 | 
	awk '{ printf($0); }' | 
	sed 's/ //g;s/../\\\\x&/g;'` | 
	tr '\000' '\n' | 
	awk '/^kernel_version=/ { next;} {print $0;}' | 
	tr '\n' '\000' >> $MI

#build the objcopy command
#$DEPMOD -e $1 $OTHERMODULES 2>&1 | sed 's/depmod://g' | grep -vE "^$1:|Unresolved symbols|^#|:$"
#$DEPMOD -e $1 2>&1 | sed 's/depmod://g' | grep -vE "^$1:|Unresolved symbols|^#"
CMD="objcopy"
for i in `$DEPMOD -e $1 $OTHERMODULES 2>&1 | sed 's/depmod://g' | grep -vE "^$1|Unresolved symbols|^#|:$|^$"` ; do
 echo -n doing $i
 i1=`echo $i | awk '{ 
  gsub(/_R[0-9a-fA-F]+/,""); 
  printf("%s", $1); 
 }'`
 echo -n " trunc=$i1"
 new=`awk '/ '$i1'_R/ {
  printf("%s", $2);
 }' < /proc/ksyms`
 if [ -z "$new" ]; then
	 new=`awk '/ '$i1'/ {
	  printf("%s", $2);
	 }' < /proc/ksyms`
 fi
 #echo "new is x${new}x"
 if [ -z "$new" -a -n "$OTHERMODULES" ]; then
    new=`nm $OTHERMODULES | grep " *T " | awk '/ '$i1'_R/ {
       printf("%s", $3);
    }'`
   #echo "new2 is x${new}x"
 fi
 echo " new=$new"
 if [ -n "$i" -a -n "$new" ] ; then
  CMD="$CMD --redefine-sym=$i=$new"
 else
  echo "ERROR: Symbol $i not found in kernel symbols."
  echo "Insert the appropriate module and try again."
 fi
done

#fix symbols which conflict with the serial driver
CMD="$CMD --redefine-sym=register_serial=register_lucent"
CMD="$CMD --redefine-sym=unregister_serial=unregister_lucent"
CMD="$CMD --redefine-sym=serial_console_init=ltmodem_console_init"

#replace the modinfo section with the new one
CMD="$CMD --remove-section=.modinfo --add-section=.modinfo=$MI"
CMD="$CMD $*"

#run the command
$CMD

#remove the section file
rm -f $MI
