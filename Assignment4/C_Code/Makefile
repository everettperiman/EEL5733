# EEL4732/5733 Advanced System Programming
# Assignment 2 Makefile
# to run this, type $make 
#			./location_updater ${# of buffer entries} < input.txt

all:	location_updater

location_updater: location_updater.c
	gcc -o location_updater location_updater.c -pthread
clean:
	rm -rf location_updater
