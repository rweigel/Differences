

PARAVIEW=deps/ParaView-4.2.0-Linux-64bit

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
