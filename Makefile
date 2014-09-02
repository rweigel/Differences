
readers:
	cd src; make all

paraview:
	tar zxvf ParaView-4.0.1-Linux-64bit.tar.gz
	mkdir ParaView-4.0.1;
	cd ParaView-4.0.1/; cmake

images:
	ParaView-4.0.1-Linux-64bit/bin/pvbatch readerimg.py
#	diffplotimg.py
#	pcdiffimg.py