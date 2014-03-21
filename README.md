lkm_fixscript
=============
Some useful utilities for purely cosmetic changes in Linux Kernel Modules (LKM`s) so that they can be used on nearby kernel versions.

Abstract
=============
lkm_fixscript changes kernel version number tag in binary LKM (vermagic) to match the version of the currently running kernel. It also renames any symbol that the current kernel can't resolve into their equivalent resolvable symbols.

The script is written in bash and to perform its magic requires:
* objcopy (version 2.9.5 or later, binutils package)
* awk (gawk package or other)
* cut (usually, coreutils package)
* sed (sed package)
* tr (usually, coreutils package)

Original scripts [1] has two versions - v1.9 (for earlier kernels) and v1.10 (supports the newer vermagic kernel versioning system used by most of the latest kernels).

WARNING!!! The use of version-matched binaries whenever possible is strongly advised!
Changes made by the script may crash your kernel due to inconsistencies in data structures between the kernel as it stands and the headers used to originally compile the LKM being fixed (symbol name changes do not modify structure definitions). So extreme care must be taken to ensure the common structures between the kernel and non-matching LKM`s are the same. No guarantees are given or implied under any circumstances.

Tips and Tricks for Precompiled Modules with different Kernel Ver­sions [2]
=============
A lot of Linux drivers are only available as precompiled, binary kernel modules. Generally, modules/binaries transparently function only with the kernel against which they were co-compiled.  Therefore, getting such precompiled driver to work with your particular kernel could be a challenge.

Since the Linux kernel is a dynamically changing beast, it is very unfortunate that many vendors have not yet chosen to release source-code versions of their drivers, which would ensure ability to modify these drivers appropriately as kernel source code evolves. Some of the binary modules have been coaxed to function under some later kernel versions using various tricks; however, even though a module may be rendered functional, it is advisable to use them minimally. Quoting an email from Mark Spieth, "A driver can never work properly if there are unresolved symbols, as it means something is not going to work. Furthermore, it means that that something that would have been called will call something else in the kernel and this could be anything. This is very bad."

Therefore, you should be careful in using binary modules with a kernel of a different version; proceed at your own risk.  If you require above all that your driver function, consider downgrading your kernel to match the module - this is by no means a ridiculous prospect. Despite these warnings, however, many others have used mismatched binary modules and kernels with only minor annoyances (such as the occasional kernel panic) using tricks and tools such as the following.

Mark Spieth has contributed a progressively improved series of "fixscripts" for editing a binary module so that version mismatch warnings are eliminated. Insertion of the "fixed" module then proceeds without the forcing flag, i.e. simply insmod module_name. Later versions also rename module symbols to match those exported by the kernel, so that "Unresolved symbols" errors are not returned by the test depmod -e.  It must be emphasized that this change is almost entirely cosmetic - it is still recommended that the module be used minimally.

Usage:
=============
	1. generate a "fixed" LKM from original LKM	-	./lkm_fixscript old_lkm.ko new_lkm.ko
	2. insert "fixed" LKM simply, non-forced	-	insmod new_lkm.ko

References:
=============
* [1] Code based on fixscript and fixscript2, authored by Mark Spieth <mark@digivation.com.au>, http://www.digivation.com.au/linmodem/
* [2] http://www.tldp.org/HOWTO/text/Linmodem-HOWTO
