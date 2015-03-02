#	bin/reader data/Brian_Curtis_042213_1b 0 & \

# Create ASCII file of Y=0 plane.  Execute at same time.
cuts:
	bin/reader data/Brian_Curtis_042213_1 0 & \
	bin/reader data/Brian_Curtis_042213_2 0 & \
	bin/reader data/Brian_Curtis_042213_3 0 & \
	bin/reader data/Brian_Curtis_042213_5 0 & \
	bin/reader data/Brian_Curtis_042213_6 0 & \
	bin/reader data/Brian_Curtis_042213_7 0 & \
	bin/reader data/Brian_Curtis_102114_1 0 & \
	bin/reader data/Brian_Curtis_102114_2 0 & \
	bin/reader data/Brian_Curtis_102114_3 0 
	make backup

backup:
	- rsync -avm --delete-before /mnt/Disk3/Differences/output/ /swd4/Users/weigel/Differences/output
	- rsync -avm --delete-before /mnt/Disk3/Differences/data/ /swd4/Users/weigel/Differences/data

# Create VTK file of full volume
volumes:
	bin/reader data/Brian_Curtis_042213_1 1
	bin/reader data/Brian_Curtis_042213_2 1
	bin/reader data/Brian_Curtis_042213_3 1
	bin/reader data/Brian_Curtis_042213_5 1
	bin/reader data/Brian_Curtis_042213_6 1
	bin/reader data/Brian_Curtis_042213_7 1
	bin/reader data/Brian_Curtis_102114_1 1
	bin/reader data/Brian_Curtis_102114_2 1
	bin/reader data/Brian_Curtis_102114_3 1
	make backup

diffs:
	make pcdiffvtk A=Brian_Curtis_042213_1 B=Brian_Curtis_042213_5 O=18
	make pcdiffvtk A=Brian_Curtis_042213_1 B=Brian_Curtis_102114_1 O=35
	make pcdiffvtk A=Brian_Curtis_042213_2 B=Brian_Curtis_042213_6 O=18
	make pcdiffvtk A=Brian_Curtis_042213_2 B=Brian_Curtis_102114_2 O=35
	make pcdiffvtk A=Brian_Curtis_042213_3 B=Brian_Curtis_042213_7 O=18
	make pcdiffvtk A=Brian_Curtis_042213_3 B=Brian_Curtis_102114_3 O=35

cutanimation:
	cd data/Precondition/$(B)_minus_$(A); 					\
	convert -delay 20 -loop 0 `ls pcdiff_Bz_*.png | sort -V` pcdiff_Bz.gif	\
	convert -delay 20 -loop 0 `ls pcdiff_Jx_*.png | sort -V` pcdiff_Jx.gif	\
	convert -delay 20 -loop 0 `ls pcdiff_Ux_*.png | sort -V` pcdiff_Ux.gif	\
	convert -delay 20 -loop 0 `ls pcdiff_Vx_*.png | sort -V` pcdiff_Vx.gif	\

clean:
	cd src; make clean

distclean:
	cd src; make distclean
	- rm -rf deps/ParaView-v4.2.0-source
	- rm -f tgz/ParaView-4.2.0-Linux-64bit.tgz
	- rm -rf deps/ParaView-4.2.0-Linux-64bit