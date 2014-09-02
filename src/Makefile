CDF_TGZ=cdf34_1-dist-all.tar.gz
CDF_DIR=$(CURDIR)/../cdf34_1-dist
CDF_LIB=${CDF_DIR}/src/lib
CDF=${CDF_LIB}/libcdf.a

KAMELEON_TGZ=KameleonPlus.tgz
KAMELEON_DIR=$(CURDIR)/../KameleonPlus
KAMELEON_LIB=${KAMELEON_DIR}/src/ccmc/.libs
KAMELEON=${KAMELEON_LIB}/libkameleon_plus.a

BOOST_TGZ=boost_1_52_0.tar.gz
BOOST_DIR=$(CURDIR)/../boost_1_52_0
BOOST_INC=${BOOST_DIR}

HDF5_TGZ=hdf5-1.8.10-patch1.tar.gz
HDF5_DIR=$(CURDIR)/../hdf5-1.8.10-patch1
HDF5_LIB=${HDF5_DIR}/lib
HDF5=${HDF5_LIB}/lib/libhdf5.a

all: base diff precondition

hdf5:
	make ${HDF5}

# Lots of warnings
${HDF5}:
	cd ../; tar zxvf ${HDF5_TGZ}
	cd ${HDF5_DIR}; make all

cdf:
	make ${CDF}

${CDF}:
	cd ../; tar zxvf ${CDF_TGZ}
	cd ${CDF_DIR}; make OS=linux ENV=gnu CURSES=no all

boost:
	cd ../; tar zxvf ${BOOST_TGZ}

kameleon:
	make ${KAMELEON}

${KAMELEON}: ${CDF}
	cd ../; tar zxvf ${KAMELEON_TGZ}
	cd ${KAMELEON_DIR}; ./configure --with-cdf=${CDF_LIB} --with-hdf5=${HDF5_LIB} \
		--with-boost-includes=${BOOST_DIR} --with-boost=${BOOST_DIR}
	cd ${KAMELEON_DIR}; make all

base: ${KAMELEON}
	g++ reader.cpp -o reader -I${BOOST_DIR} -I${KAMELEON_DIR}/src/ ${KAMELEON} ${CDF}

diff: ${KAMELEON}
	g++ diff.cpp -o diffplot -I${BOOST_DIR} -I${KAMELEON_DIR}/src/ ${KAMELEON} ${CDF}

precondition: ${KAMELEON}
	g++ pcdiff.cpp -o pcdiff -I${BOOST_DIR} -I${KAMELEON_DIR}/src/ ${KAMELEON} ${CDF}

clean:
	rm reader diffplot pcdiff
