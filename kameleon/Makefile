# Had to make edits to CMakeLists.txt.  See CMakeLists.txt.diff for
# changes.  (replace all instances of libhdf5*.a with libhdf5*.so to
# prevent errors saying "recompile with -fPIC")

# Sometimes compile fails on first try with errors about gfortran
# symbols not being found.  A second try seems to always work.

prep:
	sudo apt-get install libhdf5-serial-dev
	mkdir deps;
	cd deps; tar zxvf ../tgz/boost_1_57_0.tar.gz
	cd deps; tar zxvf ../tgz/cdf35_0-dist-all.tar.gz
	cd deps/cdf35_0-dist; make OS=linux ENV=gnu CURSES=no all
	git clone https://code.google.com/p/ccmc-software/
	cd ccmc-software; git checkout c5a017fce737

#	cd deps/boost_1_57_0; ./bootstrap.sh && ./b2

cuts:
	./reader ../data/Robert_Weigel_020215_1 0
#	./reader ../data/Brian_Curtis_042213_8 0
#	./reader ../data/Brian_Curtis_042213_4 0
#	./reader ../data/Brian_Curtis_042213_1 0

kameleon:
	- mkdir kameleon-plus;
	cd kameleon-plus; cp ../CMakeLists.txt ../ccmc-software/kameleon-plus/trunk/kameleon-plus-working/CMakeLists.txt  
	cd kameleon-plus; cmake -DBOOST_ROOT=../deps/boost_1_57_0 \
				-DCDF_LIB=../deps/cdf35_0-dist/src/lib/libcdf.so \
				-DCDF_INCLUDES=../deps/cdf35_0-dist/src/include/ \
				../ccmc-software/kameleon-plus/trunk/kameleon-plus-working
	cd kameleon-plus; cp ../CMakeCache.txt .
	cd kameleon-plus; sed -i "s|/home/weigel/tmp2/|$(PWD)/|g" CMakeCache.txt # Modify paths
	cd kameleon-plus; make -j24

test:
	cp ./ccmc-software/kameleon-plus/trunk/kameleon-plus-working/src/example/c++/kameleon_prog.cpp .
	g++ -fPIE -I/home/weigel/git/Differences/kameleon/deps/cdf35_0-dist/src/include -I/home/weigel/git/Differences/kameleon/ccmc-software/kameleon-plus/trunk/kameleon-plus-working/src \
	-o kameleon_prog.cpp.o -c kameleon_prog.cpp 
	g++ kameleon_prog.cpp.o -o kameleon_prog -rdynamic \
	-L/home/weigel/git/Differences/kameleon/deps/cdf35_0-dist/src/lib \
	/home/weigel/git/Differences/kameleon/ccmc-software/kameleon-plus/trunk/kameleon-plus-working/lib/ccmc/libccmc.a \
	-lcdf -lhdf5_cpp -lhdf5 -Wl,-rpath,/home/weigel/git/Differences/kameleon/deps/cdf35_0-dist/src/lib
	./kameleon_prog ../data/Brian_Curtis_042213_1/GM_CDF/Brian_Curtis_042213_1.3df.007200.cdf bx -10 -10 -10
	./kameleon_prog ../data/Brian_Curtis_042213_4/GM_CDF/null.LFM.Brian_Curtis_042213_4_mhd_2000-01-01T06-00-00Z.cdf bx -10 -10 -10

reader: reader.cpp
	g++ -fPIE -I/home/weigel/git/Differences/kameleon/deps/cdf35_0-dist/src/include -I/home/weigel/git/Differences/kameleon/ccmc-software/kameleon-plus/trunk/kameleon-plus-working/src \
	-o reader.cpp.o -c reader.cpp 
	g++ reader.cpp.o -o reader -rdynamic \
	-L/home/weigel/git/Differences/kameleon/deps/cdf35_0-dist/src/lib \
	/home/weigel/git/Differences/kameleon/ccmc-software/kameleon-plus/trunk/kameleon-plus-working/lib/ccmc/libccmc.a \
	-lcdf -lhdf5_cpp -lhdf5 -Wl,-rpath,/home/weigel/git/Differences/kameleon/deps/cdf35_0-dist/src/lib


clean: 
	rm -f *.o
	rm -f kameleon_prog
	rm -f *~
	rm -rf deps
	rm -rf ccmc-software
	rm -rf kameleon-plus

xall:
	sudo g++ lfm_test.cpp \
	-I/home/bcurtis/Builds/boost_1_57_0/ \
	-I/home/bcurtis/Builds/kameleon-plus/trunk/kameleon-plus-working/src/ \
	-L/home/bcurtis/Builds/kameleon-plus/trunk/kameleon-plus-working/lib/ccmc/libccmc.a \
	-L/home/bcurtis/Builds/cdf35_0-dist/lib/libcdf.a

xaall:
	sudo g++ lfm_test.cpp \
	-I/home/bcurtis/Builds/boost_1_57_0/ \
	-I/home/bcurtis/Builds/cdf35_0-dist/src/include \
	-I/home/bcurtis/Builds/hdf5-1.8.14/hdf5/include \
	-I/home/bcurtis/Builds/kameleon-plus/trunk/kameleon-plus-working/src/ \
	-L/home/bcurtis/Builds/cdf35_0-dist/lib/libcdf.a \
	-L/home/bcurtis/Builds/hdf5-1.8.14/hdf5/lib/libhdf5.a \
	-L/home/bcurtis/Builds/kameleon-plus/trunk/kameleon-plus-working/lib/ccmc/libccmc.a

xx:
	export LD_LIBRARY_PATH=/home/bcurtis/Builds/cdf35_0-dist/lib; ./kameleon_prog ../data/Brian_Curtis_042213_1/GM_CDF/Brian_Curtis_042213_1.3df.007200.cdf bx -10 -10 -10
	export LD_LIBRARY_PATH=/home/bcurtis/Builds/cdf35_0-dist/lib; ./kameleon_prog ../data/Brian_Curtis_042213_4/GM_CDF/null.LFM.Brian_Curtis_042213_4_mhd_2000-01-01T06-00-00Z.cdf bx -10 -10 -10


yall:
	sudo g++ pcdiff.cpp \
	-I/home/bcurtis/Builds/boost_1_57_0/ \
	-I/home/bcurtis/Builds/cdf35_0-dist/src/include \
	-I/home/bcurtis/Builds/hdf5-1.8.14/hdf5/include \
	-I/home/bcurtis/Builds/kameleon-plus/trunk/kameleon-plus-working/src \
	-L/home/bcurtis/Builds/hdf5-1.8.14/hdf5/lib \
	-L/home/bcurtis/Builds/kameleon-plus/trunk/kameleon-plus-working/lib/ccmc \
	-L/home/bcurtis/Builds/cdf35_0-dist/lib \
	-lccmc -lcdf -lhdf5
	export LD_LIBRARY_PATH=/home/bcurtis/Builds/cdf35_0-dist/lib; ./a.out


yall2:
	sudo g++ pcdiff.cpp \
	-I/home/bcurtis/Builds/boost_1_57_0/ \
	-I/home/bcurtis/Builds/cdf35_0-dist/src/include \
	-I/home/bcurtis/Builds/hdf5-1.8.14/hdf5/include \
	-I/home/bcurtis/Builds/kameleon-plus/trunk/kameleon-plus-working/src \
	/home/bcurtis/Builds/cdf35_0-dist/lib/libcdf.a \
	/home/bcurtis/Builds/hdf5-1.8.14/hdf5/lib/libhdf5.a \
	/home/bcurtis/Builds/kameleon-plus/trunk/kameleon-plus-working/lib/ccmc/libccmc.a 

