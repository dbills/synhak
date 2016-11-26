all: bldc_control1

clean: 
	rm -f *.o
	rm -f test/*.o

setup:
	for i in `ls *.ino`;do ln $$i `basename $$i .ino`.cpp;done

CPPFLAGS=-Itest -D_PC -Wall


bldc_control1 : bldc_control1.o ds1809.o test/*.cpp
	g++ -o $@ $^

bldc_control1.o: ds1809.h defines.h
ds1809.cpp: ds1809.h defines.h
