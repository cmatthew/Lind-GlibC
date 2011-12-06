# config.make.  Generated from config.make.in by configure.
# Don't edit this file.  Put configuration parameters in configparms instead.

version = 2.9
release = stable

# Installation prefixes.
install_root =
prefix = /usr
exec_prefix = ${prefix}
datadir = ${datarootdir}
libdir = ${exec_prefix}/lib
slibdir = /lib
localedir = 
sysconfdir = /etc
libexecdir = ${exec_prefix}/libexec
rootsbindir = /sbin
infodir = ${datarootdir}/info
includedir = ${prefix}/include
datarootdir = ${prefix}/share

# Should we use and build ldconfig?
use-ldconfig = yes

# Maybe the `ldd' script must be rewritten.
ldd-rewrite-script = ../sysdeps/unix/sysv/linux/ldd-rewrite.sed

# System configuration.
config-machine = i486
base-machine = i386
config-vendor = pc
config-os = linux-gnu
config-sysdirs =  sysdeps/i386/elf sysdeps/nacl sysdeps/nacl/unsupported nptl/sysdeps/unix/sysv/linux/i386/i486 nptl/sysdeps/unix/sysv/linux/i386 sysdeps/unix/sysv/linux/i386 nptl/sysdeps/unix/sysv/linux nptl/sysdeps/pthread sysdeps/pthread sysdeps/unix/sysv/linux sysdeps/gnu sysdeps/unix/common sysdeps/unix/mman sysdeps/unix/inet sysdeps/unix/sysv/i386 nptl/sysdeps/unix/sysv sysdeps/unix/sysv sysdeps/unix/i386 nptl/sysdeps/unix sysdeps/unix sysdeps/posix nptl/sysdeps/i386/i486 sysdeps/i386/i486 sysdeps/i386/fpu nptl/sysdeps/i386 sysdeps/i386 sysdeps/wordsize-32 sysdeps/ieee754/ldbl-96 sysdeps/ieee754/dbl-64 sysdeps/ieee754/flt-32 sysdeps/ieee754 sysdeps/generic/elf sysdeps/generic
cflags-cpu = 

defines =  -D_LIBC_REENTRANT
sysincludes = -nostdinc -isystem /home/cmatthew/bin/native_client_sdk_0_5_1052/toolchain/linux_x86/bin/../lib/gcc/x86_64-nacl/4.4.3/include -isystem /home/cmatthew/bin/native_client_sdk_0_5_1052/toolchain/linux_x86/bin/../lib/gcc/x86_64-nacl/4.4.3/include-fixed -isystem /home/cmatthew/lind/nacl-glibc/kernel-headers
c++-sysincludes = -isystem ../../../../include/c++/4.5.2 -isystem ../../../../include/c++/4.5.2/x86_64-linux-gnu -isystem ../../../../include/c++/4.5.2/backward
all-warnings = 

elf = yes
have-z-combreloc = no
have-z-execstack = yes
have-initfini = 
have-Bgroup = yes
have-as-needed = no
libgcc_s_suffix = 
need-nopic-initfini = 
with-fp = yes
with-cvs = yes
old-glibc-headers = no
unwind-find-fde = yes
have-cpp-asm-debuginfo = yes
enable-check-abi = no
have-forced-unwind = yes
have-fpie = no
gnu89-inline-CFLAGS = -fgnu89-inline
have-ssp = no
have-selinux = no
have-libaudit = 
have-libcap = 
have-cc-with-libunwind = no
fno-unit-at-a-time = -fno-toplevel-reorder -fno-section-anchors
bind-now = no
have-hash-style = yes

static-libgcc = -static-libgcc

versioning = yes
oldest-abi = default
no-whole-archive = -Wl,--no-whole-archive
exceptions = -fexceptions

mach-interface-list = 

have-bash2 = yes
have-ksh = yes

sizeof-long-double = 

# Configuration options.
build-static = yes
build-shared = yes
build-pic-default= no
build-profile = no
build-omitfp = no
build-bounded = no
build-static-nss = no
add-ons = libidn nptl
add-on-subdirs =  libidn
sysdeps-add-ons =  nptl
cross-compiling = maybe
force-install = yes

# Build tools.
LDFLAGS = -L$(..)nacl/dyn-link
CC = /home/cmatthew/bin/native_client_sdk_0_5_1052/toolchain/linux_x86/bin/nacl-gcc -L/home/cmatthew/lind/nacl-glibc/nacl/dyn-link 
CXX = g++
BUILD_CC = gcc
CFLAGS = -march=i486 -pipe -fno-strict-aliasing -O2 -mno-tls-direct-seg-refs -g
ASFLAGS-config =  -Wa,--noexecstack
AR = /home/cmatthew/bin/native_client_sdk_0_5_1052/toolchain/linux_x86/bin/../lib/gcc/x86_64-nacl/4.4.3/../../../../x86_64-nacl/bin/ar
RANLIB = :
MAKEINFO = makeinfo
AS = $(CC) -c
MIG = 
PWD_P = /bin/pwd
BISON = no
AUTOCONF = no
OBJDUMP = /home/cmatthew/bin/native_client_sdk_0_5_1052/toolchain/linux_x86/bin/../lib/gcc/x86_64-nacl/4.4.3/../../../../x86_64-nacl/bin/objdump

# Installation tools.
INSTALL = /usr/bin/install -c
INSTALL_PROGRAM = ${INSTALL}
INSTALL_SCRIPT = ${INSTALL}
INSTALL_DATA = ${INSTALL} -m 644
INSTALL_INFO = /usr/sbin/install-info
LN_S = ln -s
MSGFMT = msgfmt

# Script execution tools.
BASH = /bin/bash
KSH = /bin/bash
AWK = gawk
PERL = /usr/bin/perl

# Additional libraries.
LIBGD = no

# More variables may be inserted below by configure.

override stddef.h = # The installed <stddef.h> seems to be libc-friendly.
