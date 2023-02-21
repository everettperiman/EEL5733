# EEL4732/5733 Advanced System Programming
# Assignment 2 Makefile
# to run this, type $make 
#			./location_updater ${# of buffer entries} < input.txt

all:	transProg

location_updater: transProg.c
	gcc -o transProg transProg.c -pthread
clean:
	rm -rf transProg
