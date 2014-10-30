all:
	cd src; make all


images:
	deps/ParaView-4.0.1-Linux-64bit/bin/pvbatch readerimg.py
#	~/ParaView-4.2.0-Linux-64bit/bin/pvbatch readerimg.py


pcdiffvtk: src/pcdiff.cpp
	./bin/pcdiff \
		./data/Precondition/Brian_Curtis_042213_1_vs_Brian_Curtis_042213_5 \
		./data/Brian_Curtis_042213_1 ./data/Brian_Curtis_042213_5

pcdiffimg: pcdiffimg.py
	~/ParaView-4.2.0-Linux-64bit/bin/pvbatch pcdiffimg.py \
		data/Precondition/Brian_Curtis_042213_1_vs_Brian_Curtis_042213_5

pcdiffanim:
	cd data/Precondition/Brian_Curtis_042213_1_vs_Brian_Curtis_042213_5; 
		 convert -delay 20 -loop 0 `ls pcdiff_Bz_*.png | sort -V` pcdiff_Bz.gif

tgz/ParaView-4.0.1-Linux-64bit.tar.gz:
	cd tgz; wget http://mag.gmu.edu/tmp/ParaView-4.0.1-Linux-64bit.tar.gz

ParaView-4.0.1-Linux-64bit: tgz/ParaView-4.0.1-Linux-64bit.tar.gz
	cd deps; tar zxvf tgz/ParaView-4.0.1-Linux-64bit.tar.gz

unzip:
	find ./data -name "*.vtk.gz" | xargs -i -P 12 gunzip {}

deps/ParaView-v4.0.1-source:
	cd deps; tar zxvf tgz/ParaView-v4.0.1-source.tgz

paraviewfromsrc: deps/ParaView-v4.0.1-source
	cd deps; mkdir ParaView-v4.0.1
	cd deps/ParaView-v4.0.1/; cmake ../ParaView-v4.0.1-source

clean:
	cd src; make clean

distclean:
	cd src; make distclean
	- rm -rf deps/ParaView-v4.0.1-source
	- rm -f tgz/ParaView-4.0.1-Linux-64bit.tgz
	- rm -rf deps/ParaView-4.0.1-Linux-64bit