#!/bin/sh

set -e

. $(dirname $0)/config.sh

cd $CACHE

if [ -d bear ]
then
    cd bear
else
    git clone git@github.com:j-jorge/bear.git
    cd bear
fi

BEAR_ROOT_DIR=$PWD
BUILD_TYPE=$(build_mode $@)
BUILD_DIR="build/$BUILD_TYPE"

mkdir --parents "$BUILD_DIR"
cd "$BUILD_DIR"

CC="$ANDROID_CC $CFLAGS" CXX="$ANDROID_CXX $CXXFLAGS" AR=$ANDROID_AR \
   cmake $BEAR_ROOT_DIR \
   -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
   -DCMAKE_PREFIX_PATH=$INSTALL_PREFIX \
   -DBoost_INCLUDE_DIR:PATH="$ANDROID_BOOST_INCLUDE_DIR" \
   -DBoost_USE_STATIC_RUNTIME=ON \
   -DFREETYPE_LIBRARY:FILEPATH="$ANDROID_FREETYPE_LIBRARY" \
   -DFREETYPE_INCLUDE_DIRS:PATH="$ANDROID_FREETYPE_INCLUDE_DIR" \
   -DPNG_PNG_INCLUDE_DIR:PATH="$ANDROID_PNG_INCLUDE_DIR" \
   -DPNG_LIBRARY:FILEPATH="$ANDROID_PNG_LIBRARY" \
   -DBEAR_EDITORS_ENABLED=0 \
   -DTESTING_ENABLED=0 \
   -DBEAR_ENGINE_NO_MANPAGES=1 \
   -DBEAR_ENGINE_CORE_LINK_TYPE=STATIC \
   -DBUILD_PLATFORM="android" \
   -DCMAKE_INSTALL_PREFIX="$INSTALL_PREFIX"

make -j$(nproc) install

set_shell_variable BEAR_ROOT_DIR $BEAR_ROOT_DIR
