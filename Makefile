A=Brian_Curtis_042213_1
B=Brian_Curtis_042213_5
O=18

B=Brian_Curtis_102114_1
O=36
PARAVIEW=deps/ParaView-4.2.0-Linux-64bit

# rsync -avz --include='*.txt' --include='*/' --exclude='*' data/ weigel@mag.gmu.edu:git/Differences/data/
all:
	cd src; make all

images:
	~/ParaView-4.2.0-Linux-64bit/bin/pvbatch readerimg.py

pcdiffvtk1: src/pcdiff.cpp
	cd data/$(A); find . -name "*cdf.gz" | xargs -i -P12 gunzip {}
	cd data/$(B); find . -name "*cdf.gz" | xargs -i -P12 gunzip {}
	mkdir -p data/$(A)/Results
	mkdir -p data/$(B)/Results
	mkdir -p data/Precondition/$(B)_minus_$(A)
	./bin/pcdiff data/Precondition/$(B)_minus_$(A) data/$(A) data/$(B) $(O)

pcdiffimg1: pcdiffimg.py
	$(PARAVIEW)/bin/pvbatch pcdiffimg.py data/Precondition/$(B)_minus_$(A) $(O)

pcdiffanim1:
	cd data/Precondition/$(B)_minus_$(A); 					\
	convert -delay 20 -loop 0 `ls pcdiff_Bz_*.png | sort -V` pcdiff_Bz.gif	\
	convert -delay 20 -loop 0 `ls pcdiff_Jx_*.png | sort -V` pcdiff_Jx.gif	\
	convert -delay 20 -loop 0 `ls pcdiff_Ux_*.png | sort -V` pcdiff_Ux.gif	\
	convert -delay 20 -loop 0 `ls pcdiff_Vx_*.png | sort -V` pcdiff_Vx.gif	\

tgz/ParaView-4.2.0-Linux-64bit.tar.gz:
	cd tgz; wget http://mag.gmu.edu/tmp/ParaView-4.2.0-Linux-64bit.tar.gz

ParaView-4.2.0-Linux-64bit: tgz/ParaView-4.2.0-Linux-64bit.tar.gz
	cd deps; tar zxvf tgz/ParaView-4.2.0-Linux-64bit.tar.gz

unzip:
	find ./data -name "*.vtk.gz" | xargs -i -P 12 gunzip {}

deps/ParaView-v4.2.0-source:
	cd deps; tar zxvf tgz/ParaView-v4.2.0-source.tgz

paraviewfromsrc: deps/ParaView-v4.2.0-source
	cd deps; mkdir ParaView-v4.2.0
	cd deps/ParaView-v4.2.0/; cmake ../ParaView-v4.2.0-source

clean:
	cd src; make clean

distclean:
	cd src; make distclean
	- rm -rf deps/ParaView-v4.2.0-source
	- rm -f tgz/ParaView-4.2.0-Linux-64bit.tgz
	- rm -rf deps/ParaView-4.2.0-Linux-64bit