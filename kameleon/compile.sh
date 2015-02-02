###########################################
# System information
# uname -a
# 3.5.0-18-generic #29~precise1-Ubuntu SMP Mon Oct 22 16:31:46 UTC 2012 x86_64 x86_64 x86_64 GNU/Linux

# gcc -v
# Using built-in specs.
# COLLECT_GCC=gcc
# COLLECT_LTO_WRAPPER=/usr/lib/gcc/x86_64-linux-gnu/4.6/lto-wrapper
# Target: x86_64-linux-gnu
# Configured with: ../src/configure -v --with-pkgversion='Ubuntu/Linaro 4.6.3-1ubuntu5' --with-bugurl=file:///usr/share/doc/gcc-4.6/README.Bugs --enable-languages=c,c++,fortran,objc,obj-c++ --prefix=/usr --program-suffix=-4.6 --enable-shared --enable-linker-build-id --with-system-zlib --libexecdir=/usr/lib --without-included-gettext --enable-threads=posix --with-gxx-include-dir=/usr/include/c++/4.6 --libdir=/usr/lib --enable-nls --with-sysroot=/ --enable-clocale=gnu --enable-libstdcxx-debug --enable-libstdcxx-time=yes --enable-gnu-unique-object --enable-plugin --enable-objc-gc --disable-werror --with-arch-32=i686 --with-tune=generic --enable-checking=release --build=x86_64-linux-gnu --host=x86_64-linux-gnu --target=x86_64-linux-gnu
#Thread model: posix
#gcc version 4.6.3 (Ubuntu/Linaro 4.6.3-1ubuntu5) 

# gfortran -v
#Using built-in specs.
#COLLECT_GCC=gfortran
#COLLECT_LTO_WRAPPER=/usr/local/libexec/gcc/x86_64-apple-darwin11.3.0/4.8.0/lto-wrapper
#Target: x86_64-apple-darwin11.3.0
#Configured with: ../gcc-4.8-20120408/configure --enable-languages=fortran
#Thread model: posix
#gcc version 4.8.0 20120408 (experimental) (GCC) 
###########################################

###########################################
# Compile procedure and notes

# (Tried to use local compile, but was not able to tell cmake correct location of HDF files
# Flags seemed to be ignored).
sudo apt-get install libhdf5-serial-dev

mkdir deps;
cd deps;
tar zxvf ../tgz/boost_1_57_0.tar.gz
cd ../
cd deps/boost_1_57_0
./bootstrap.sh && ./b2
cd ../..

mkdir kameleon-plus;
cd kameleon-plus

cd ../deps
tar zxvf ../tgz/cdf35_0-dist-all.tar.gz
cd cdf35_0-dist; make OS=linux ENV=gnu CURSES=no all
cd ../../

git clone https://code.google.com/p/ccmc-software/

# Had to make edits to CMakeLists.txt.  See CMakeLists.txt.diff for changes.
cd kameleon-plus;
cp ../CMakeLists.txt ../ccmc-software/kameleon-plus/trunk/kameleon-plus-working/CMakeLists.txt  
cmake -DBOOST_ROOT=../deps/boost_1_57_0 -DCDF_LIB=../deps/cdf35_0-dist/src/lib/libcdf.so -DCDF_INCLUDES=../deps/cdf35_0-dist/src/include/ ../ccmc-software/kameleon-plus/trunk/kameleon-plus-working

# Had to replace all instances of libhdf5*.a with libhdf5*.so to prevent errors saying "recompile with -fPIC".
cp ../CMakeCache.txt .
# Modify paths
sed -i "s|/tmp/|\$PWD/|g" CMakeCache.txt
make -j24

# Sometimes compile fails on first try with errors about gfortran symbols not being found.
# A second try seems to always work.
cd ../
rm -rf kameleon-plus/*
cd kameleon-plus
cp ../CMakeLists.txt ../ccmc-software/kameleon-plus/trunk/kameleon-plus-working/CMakeLists.txt  
cmake -DBOOST_ROOT=../deps/boost_1_57_0 -DCDF_LIB=../deps/cdf35_0-dist/src/lib/libcdf.so -DCDF_INCLUDES=../deps/cdf35_0-dist/src/include/ ../ccmc-software/kameleon-plus/trunk/kameleon-plus-working
cp ../CMakeCache.txt .
make -j24
