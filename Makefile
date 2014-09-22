all:
	cd src; make all
	make images

ParaView-v4.0.1-source:
	tar zxvf tgz/ParaView-v4.0.1-source.tgz

paraviewfromsrc: ParaView-v4.0.1-source
	mkdir ParaView-v4.0.1
	cd ParaView-v4.0.1/; cmake ../ParaView-v4.0.1-source

tgz/ParaView-4.0.1-Linux-64bit.tar.gz:
	cd tgz; wget http://mag.gmu.edu/tmp/ParaView-4.0.1-Linux-64bit.tar.gz

ParaView-4.0.1-Linux-64bit: tgz/ParaView-4.0.1-Linux-64bit.tar.gz
	tar zxvf tgz/ParaView-4.0.1-Linux-64bit.tar.gz

images: ParaView-4.0.1-Linux-64bit
	ParaView-4.0.1-Linux-64bit/bin/pvbatch readerimg.py
#	diffplotimg.py
#	pcdiffimg.py

clean:
	cd src; make clean

distclean:
	cd src; make distclean
	- rm -rf ParaView-v4.0.1-source
	- rm -f tgz/ParaView-4.0.1-Linux-64bit.tgz
	- rm -rf ParaView-4.0.1-Linux-64bit