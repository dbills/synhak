all: bldc_control1

setup:
	for i in `ls *.ino`;do ln $$i `basename $$i .ino`.cpp;done

CPPFLAGS=-Itest -D_PC


bldc_control1 : bldc_control1.o ds1809.o test/*.cpp
	g++ -o $@ $^
