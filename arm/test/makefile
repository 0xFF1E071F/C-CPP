
NDK_ROOT = F:\android-ndk-r12b
NDK_TOOLCHAINS = $(NDK_ROOT)/toolchains
NDK_TOOLCHAINS_BIN = $(NDK_TOOLCHAINS)/arm-linux-androideabi-4.9/prebuilt/windows-x86_64/bin
NDK_TOOLCHAINS_GCC = $(NDK_TOOLCHAINS_BIN)/arm-linux-androideabi-gcc
NDK_PLATFORMS = $(NDK_ROOT)/platforms
NDK_PLATFORMS_VERSION = $(NDK_PLATFORMS)/android-24
NDK_INCLUDE = $(NDK_PLATFORMS_VERSION)/arch-arm/usr/include
NDK_LIBRARY = $(NDK_PLATFORMS_VERSION)/arch-arm/usr/lib

NDK_BUILD_FLAGS = -c -I $(NDK_INCLUDE) -fPIE
NDK_LINK_FLAGS = -L $(NDK_LIBRARY) -Bdynamic -lc -nostdlib -pie -o $(NDK_MODULE_NAME).o $(NDK_LIBRARY)/crtbegin_dynamic.o $(NDK_LIBRARY)/crtend_android.o
NDK_MODULE_NAME = main

all:
#gcc -E hello.c -o hello.i
	$(NDK_TOOLCHAINS_GCC) -E $(NDK_BUILD_FLAGS)  $(NDK_MODULE_NAME).c -o $(NDK_MODULE_NAME).i
#gcc -S hello.i -o hello.s
	$(NDK_TOOLCHAINS_GCC) -S $(NDK_BUILD_FLAGS)  $(NDK_MODULE_NAME).i -o $(NDK_MODULE_NAME).s
#gcc -c hello.s -o hello.o
#	$(NDK_TOOLCHAINS_GCC) -c $(NDK_BUILD_FLAGS) $(NDK_MODULE_NAME).s -o $(NDK_MODULE_NAME).o
#gcc hello.o -o hello
#	$(NDK_TOOLCHAINS_GCC)  $(NDK_LINK_FLAGS) $(NDK_MODULE_NAME).o -o $(NDK_MODULE_NAME)
    