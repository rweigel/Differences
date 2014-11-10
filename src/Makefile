CDF_TGZ=$(CURDIR)/../tgz/cdf34_1-dist-all.tar.gz
CDF_DIR=$(CURDIR)/../deps/cdf34_1-dist
CDF_LIB=${CDF_DIR}/src/lib
CDF=${CDF_LIB}/libcdf.a

KAMELEON_TGZ=$(CURDIR)/../tgz/KameleonPlus.tgz
KAMELEON_DIR=$(CURDIR)/../deps/KameleonPlus
KAMELEON=${KAMELEON_DIR}/src/ccmc/.libs/libkameleon_plus.a

BOOST_TGZ=$(CURDIR)/../tgz/boost_1_52_0.tar.gz
BOOST_DIR=$(CURDIR)/../deps/boost_1_52_0

HDF5_TGZ=$(CURDIR)/../tgz/hdf5-1.8.10-patch1.tar.gz
HDF5_DIR=$(CURDIR)/../deps/hdf5-1.8.10-patch1
HDF5_LIB=${HDF5_DIR}/lib
HDF5=${HDF5_LIB}/lib/libhdf5.a

all: reader diff pcdiff

unzip:
	find ../data -name "*.cdf.gz" | xargs -i -P 12 gunzip {}

# Lots of warnings
${HDF5_DIR}:
	cd ../deps; tar zxvf ${HDF5_TGZ}

${HDF5}: ${HDF5_DIR}
	cd ${HDF5_DIR}; make all

${BOOST_DIR}:
	cd ../deps; tar zxvf ${BOOST_TGZ}

${CDF_DIR}:
	cd ../deps; tar zxvf ${CDF_TGZ}

${CDF}: ${CDF_DIR}
	cd ${CDF_DIR}; make OS=linux ENV=gnu CURSES=no all

${KAMELEON_DIR}:
	cd ../deps; tar zxvf ${KAMELEON_TGZ}

${KAMELEON}: ${KAMELEON_DIR} ${CDF} ${HDF5_DIR} ${BOOST_DIR} 
	cd ${KAMELEON_DIR}; ./configure --with-cdf=${CDF_LIB} --with-hdf5=${HDF5_LIB} \
		--with-boost-includes=${BOOST_DIR} --with-boost=${BOOST_DIR}
	cd ${KAMELEON_DIR}; make all

reader: ${KAMELEON} reader.cpp misc.h
	g++ reader.cpp -o ../bin/reader -I${BOOST_DIR} -I${KAMELEON_DIR}/src/ ${KAMELEON} ${CDF}

diff: ${KAMELEON} diff.cpp misc.h
	g++ diff.cpp -o ../bin/diff -I${BOOST_DIR} -I${KAMELEON_DIR}/src/ ${KAMELEON} ${CDF}

pcdiff: ${KAMELEON} pcdiff.cpp misc.h
	g++ pcdiff.cpp -o ../bin/pcdiff -I${BOOST_DIR} -I${KAMELEON_DIR}/src/ ${KAMELEON} ${CDF}

clean:
	- rm ../bin/reader ../bin/diff ../bin/pcdiff

distclean:
	make clean
	- rm -rf ${HDF5_DIR}
	- rm -rf ${CDF_DIR}
	- rm -rf ${BOOST_DIR}
	- rm -rf ${KAMELEON_DIR}