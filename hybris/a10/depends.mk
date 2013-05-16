#whether to compile for armhf
USEARMHF=1
TARBALL=libhybris.tar.gz

ifeq ($(USEARMHF), 1)
HF=hf
else
HF=
endif

#native compile
ifeq ($(shell uname -m),armv7l)
#where is your arm rootfs
ROOTFS=/
#where is your libhybris install root 
LIBHYBRISPREFIX=/allwinner/libhybris
#where is your toolchain
TOOLCHAIN=/usr
JOBS=1
BUILD=arm-linux-gnueabi${HF}

#cross compile
else
#where is your arm rootfs
ROOTFS=/opt/a10hacking/rootfs/debrootfs
#where is your libhybris install root 
LIBHYBRISPREFIX=/allwinner/libhybris
#where is your toolchain
TOOLCHAIN=/usr/arm-linux-gnueabi${HF}
JOBS=4
BUILD=i686-linux
endif

LCLPREFIX=$(shell echo $(LIBHYBRISPREFIX) | sed 's/^\///g')
SDKSTAGE=${ROOTFS}
export HOST=arm-linux-gnueabi${HF}
export CROSS_COMPILE=${HOST}-
export ${BUILD}
export PREFIX=${LIBHYBRISPREFIX}

export RLINK_PATH=-Wl,-rpath,${LIBHYBRISPREFIX}/lib -Wl,-rpath-link,${LIBHYBRISPREFIX}/lib:${SDKSTAGE}/usr/local/lib:${SDKSTAGE}/lib:${SDKSTAGE}/lib/arm-linux-gnueabi${HF}:${SDKSTAGE}/usr/lib:${SDKSTAGE}/usr/lib/arm-linux-gnueabi${HF}

export LDFLAGS=\
${RLINK_PATH} \
-L${LIBHYBRISPREFIX}/lib \
-L${SDKSTAGE}/usr/local/lib \
-L${SDKSTAGE}/lib \
-L${SDKSTAGE}/lib/arm-linux-gnueabi${HF} \
-L${SDKSTAGE}/usr/lib \
-L${SDKSTAGE}/usr/lib/arm-linux-gnueabi${HF}

ifeq (${USEARMHF}, 1)
export CFLAGS=-pipe -O3 -mfloat-abi=hard -mtune=cortex-a8 -mcpu=cortex-a8 -D__ARM_NEON__ -DALLWINNERA10
else
export CFLAGS=-pipe -O3 -mfloat-abi=softfp -mtune=cortex-a8 -mcpu=cortex-a8 -D__ARM_NEON__ -DALLWINNERA10
endif

export CFLAGS+=\
-isystem${LIBHYBRISPREFIX}/include \
-isystem${SDKSTAGE}/usr/local/include \
-isystem${SDKSTAGE}/usr/include \
-isystem${SDKSTAGE}/usr/include/arm-linux-gnueabi${HF}

export CFLAGS+=${LDFLAGS}
export CXXFLAGS=${CFLAGS}
export CPPFLAGS=${CFLAGS}
export LD=${CROSS_COMPILE}ld
export AR=${CROSS_COMPILE}ar
export CC=${CROSS_COMPILE}gcc
export CXX=${CROSS_COMPILE}g++
export CXXCPP=${CXX} -E
export RANLIB=${CROSS_COMPILE}ranlib
export STRIP=${CROSS_COMPILE}strip
export OBJDUMP=${CROSS_COMPILE}objdump
export PKG_CONFIG_SYSROOT_DIR=${SDKSTAGE}
export PKG_CONFIG_LIBDIR=${PREFIX}/lib/pkgconfig:${SDKSTAGE}/lib/pkgconfig:${SDKSTAGE}/usr/lib/pkgconfig:${SDKSTAGE}/usr/lib/arm-linux-gnueabi${HF}/pkgconfig:${SDKSTAGE}/usr/share/pkgconfig:${SDKSTAGE}/usr/local/lib/pkgconfig
export PKG_CONFIG_PATH=${PREFIX}/bin/pkg-config
export PYTHON_VERSION=2.7
export PATH:=${PREFIX}/bin:${TOOLCHAIN}/bin:${PATH}
export TEXTUREPACKER_NATIVE_ROOT=/usr
export PYTHON_LDFLAGS=-L${SDKSTAGE}/usr/lib -lpython${PYTHON_VERSION}
export PYTHON_CPPFLAGS=-I${SDKSTAGE}/usr/include/python${PYTHON_VERSION}
export PYTHON_SITE_PKG=${SDKSTAGE}/usr/lib/python${PYTHON_VERSION}/site-packages
export PYTHON_NOVERSIONCHECK=no-check
