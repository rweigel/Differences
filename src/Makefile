CDF_TGZ=tgz/cdf34_1-dist-all.tar.gz
CDF_DIR=$(CURDIR)/../cdf34_1-dist
CDF_LIB=${CDF_DIR}/src/lib
CDF=${CDF_LIB}/libcdf.a

KAMELEON_TGZ=tgz/KameleonPlus.tgz
KAMELEON_DIR=$(CURDIR)/../KameleonPlus
KAMELEON=${KAMELEON_DIR}/src/ccmc/.libs/libkameleon_plus.a

BOOST_TGZ=tgz/boost_1_52_0.tar.gz
BOOST_DIR=$(CURDIR)/../boost_1_52_0

HDF5_TGZ=tgz/hdf5-1.8.10-patch1.tar.gz
HDF5_DIR=$(CURDIR)/../hdf5-1.8.10-patch1
HDF5_LIB=${HDF5_DIR}/lib
HDF5=${HDF5_LIB}/lib/libhdf5.a

all: reader diffplot pcdiff

# Lots of warnings
${HDF5_DIR}:
	cd ../; tar zxvf ${HDF5_TGZ}

${HDF5}: ${HDF5_DIR}
	cd ${HDF5_DIR}; make all

${BOOST_DIR}:
	cd ../; tar zxvf ${BOOST_TGZ}

${CDF_DIR}:
	cd ../; tar zxvf ${CDF_TGZ}

${CDF}: ${CDF_DIR}
	cd ${CDF_DIR}; make OS=linux ENV=gnu CURSES=no all

${KAMELEON_DIR}:
	cd ../; tar zxvf ${KAMELEON_TGZ}

${KAMELEON}: ${KAMELEON_DIR} ${CDF} ${HDF5_DIR} ${BOOST_DIR} 
	cd ${KAMELEON_DIR}; ./configure --with-cdf=${CDF_LIB} --with-hdf5=${HDF5_LIB} \
		--with-boost-includes=${BOOST_DIR} --with-boost=${BOOST_DIR}
	cd ${KAMELEON_DIR}; make all

reader: ${KAMELEON} reader.cpp
	g++ reader.cpp -o reader -I${BOOST_DIR} -I${KAMELEON_DIR}/src/ ${KAMELEON} ${CDF}

diffplot: ${KAMELEON} diff.cpp
	g++ diff.cpp -o diffplot -I${BOOST_DIR} -I${KAMELEON_DIR}/src/ ${KAMELEON} ${CDF}

pcdiff: ${KAMELEON} pcdiff.cpp
	g++ pcdiff.cpp -o pcdiff -I${BOOST_DIR} -I${KAMELEON_DIR}/src/ ${KAMELEON} ${CDF}

clean:
	- rm reader diffplot pcdiff

distclean:
	make clean
	- rm -rf ${HDF5_DIR}
	- rm -rf ${CDF_DIR}
	- rm -rf ${BOOST_DIR}
	- rm -rf ${KAMELEON_DIR}