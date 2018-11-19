#ID=Sean_Blake_081318_1
ID=Brian_Curtis_042213_2

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
HDF5=$(CURDIR)/../deps/hdf5-1.8.10-patch1/src/.libs/libhdf5.a

SHELL := /bin/bash

# Not used (counts non-empty lines in file)
N=$(shell grep -e "^[0-9]" ../points/points.csv | wc | sed -e 's/\s\+/ /g' | sed -e 's/^ //' | cut -d ' ' -f 1)

all:
	make data
	make pointsin
	make pointsreader
	make pointsout

../data/$(ID):
	@echo "Data directory for ID=$(ID) does not exist in ../data/$(ID). Downloading."
	cd ../data; \
	wget -np -m -nH --cut-dirs=2 https://ccmc.gsfc.nasa.gov/RoR_WWW/output_files/$(ID) | \
	wget -np -m -nH -R "index.html*" --cut-dirs=4 \
		http://mag.gmu.edu/git-data/bcurtiswx/Differences/data/$(ID)/

data: | ../data/$(ID)
	@echo "Data directory ../data/$(ID) is ready for processing."

pointsreader: 
	make ../bin/points

../bin/points: points.cpp grid.h export.h $(BOOST_DIR) $(KAMELEON) $(CDF)
	make gunzip
	- mkdir ../bin
	g++ points.cpp -o ../bin/points -I${BOOST_DIR} -I${KAMELEON_DIR}/src/ ${KAMELEON} ${CDF}

pointsin: points_writer.py
	python points_writer.py > ../points/points.csv

pointsout: ../points/points.csv
	mkdir -p ../output/$(ID)/points
	cp ../points/points.csv ../output/$(ID)/points
	../bin/points ../data/$(ID) ../output/$(ID)/points/points.csv


all2:
	make reader
	make data ID=$(ID)
	make ascii ID=$(ID)
	make vtk ID=$(ID)

reader: reader.cpp grid.h export.h $(BOOST_DIR) $(KAMELEON) $(CDF)
	- mkdir ../bin
	g++ reader.cpp -o ../bin/reader -I${BOOST_DIR} -I${KAMELEON_DIR}/src/ ${KAMELEON} ${CDF}

ascii: ../bin/reader
	make gunzip
	cd ..;bin/reader data/$(ID) 0

vtk:
	make gunzip
	cd ..;bin/reader data/$(ID) 1

gunzip:
	- find ../data/$(ID)/GM_CDF -name "*.cdf.gz" | xargs -i -P 12 gunzip {}

gzip:
	find ../data/$(ID)/GM_CDF -name "*.cdf" | xargs -i -P 12 gzip --fast {}

hdf:
	make ${HDF5}

${HDF5_DIR}:
	mkdir -p ../deps
	cd ../deps; tar zxvf ${HDF5_TGZ}

# Lots of warnings
${HDF5}: ${HDF5_DIR}
	cd ${HDF5_DIR}; make all

${BOOST_DIR}:
	- mkdir -p ../deps
	cd ../deps; tar zxvf ${BOOST_TGZ}

cdf:
	make ${CDF}

${CDF_DIR}:
	mkdir -p ../deps
	cd ../deps; tar zxvf ${CDF_TGZ}

${CDF}: ${CDF_DIR}
	cd ${CDF_DIR}; make OS=linux ENV=gnu CURSES=no all


${KAMELEON_DIR}:
	mkdir -p ../deps
	cd ../deps; tar zxvf ${KAMELEON_TGZ}

#readerx: ${KAMELEON}
#	@echo "Done"

#${KAMELEON}: ${CDF} ${HDF5} ${BOOST_DIR} |${KAMELEON_DIR}
#	@echo "Making Kameleon"
#	ls -l ${KAMELEON_DIR}

${KAMELEON}: ${KAMELEON_DIR} ${CDF} ${HDF5} ${BOOST_DIR} 
	cd ${KAMELEON_DIR}; ./configure --with-cdf=${CDF_LIB} --with-hdf5=${HDF5_LIB} \
		--with-boost-includes=${BOOST_DIR} --with-boost=${BOOST_DIR}
	cd ${KAMELEON_DIR}; make all

#reader: ${KAMELEON} reader.cpp grid.h export.h

#pcdiff: pcdiff.cpp misc.h ${KAMELEON}
# Should be above, but Kameleon is remade every time make pcdiff is executed.
pcdiff: pcdiff.cpp misc.h
	g++ pcdiff.cpp -o ../bin/pcdiff -I${BOOST_DIR} -I${KAMELEON_DIR}/src/ ${KAMELEON} ${CDF}


#diff: ${KAMELEON} diff.cpp misc.h
#	g++ diff.cpp -o ../bin/diff -I${BOOST_DIR} -I${KAMELEON_DIR}/src/ ${KAMELEON} ${CDF}


clean:
	- rm ../bin/reader ../bin/diff ../bin/pcdiff

distclean:
	make clean
	- rm -rf ${HDF5_DIR}
	- rm -rf ${CDF_DIR}
	- rm -rf ${BOOST_DIR}
	- rm -rf ${KAMELEON_DIR}
