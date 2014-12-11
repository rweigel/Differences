all: T89
	T89 > data/T89c.txt

# Example 1 from header of original Geopack-2008.for
Geopack-2008-Example1: Geopack-2008-Example1.for Geopack-2008.for T96_01.f
	g77 -c Geopack-2008.for
	g77 -c T96_01.f
	g77 Geopack-2008.o T96_01.o Geopack-2008-Example1.for -o Geopack-2008-Example1
	./Geopack-2008-Example1
	diff test/Geopack-2008-Example1.dat LINTEST1.DAT

# Example 2 from header of original Geopack-2008.for
Geopack-2008-Example2: Geopack-2008-Example2.for Geopack-2008.for T96_01.f
	g77 -c Geopack-2008.for
	g77 -c T96_01.f
	g77 Geopack-2008.o T96_01.o Geopack-2008-Example2.for -o Geopack-2008-Example2
	./Geopack-2008-Example2
	diff test/Geopack-2008-Example2.dat LINTEST2.DAT

# IGRF Example
Geopack-2008-Example3: Geopack-2008.for Geopack-2008-Example3.c 
	gfortran -c Geopack-2008.for
	gcc -lm -lgfortran Geopack-2008.o Geopack-2008-Example3.c -o Geopack-2008-Example3 
	./Geopack-2008-Example3 > Geopack-2008-Example3.dat
	diff test/Geopack-2008-Example3.dat Geopack-2008-Example3.dat

IGRF: Geopack-2008.for IGRF.c grid.h
	gfortran -c Geopack-2008.for
	gcc -lm -lgfortran Geopack-2008.o IGRF.c -o IGRF
	./IGRF

T89: T89c.for T89c.c grid.h
	g77 -c T89c.for
	gcc -lm T89c.o T89c.c -o T89

T89test: T89
	./T89 1 > T89c_test.txt
	diff test/T89c_test.txt T89c_test.txt || exit 0;

clean:
	- rm -f *.o *~
	- rm -f T89 T89c_test.txt  LINTEST*
	- rm -f Geopack-2008-Example1 Geopack-2008-Example2 Geopack-2008-Example3