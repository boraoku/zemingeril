# Makefile for Writing Make Files Example
 
# *****************************************************
# Variables to control Makefile operation
 
CC = g++
CPPFLAGS="-I/opt/X11/include"
PRGNAME=ZeminGeril
BUILDPATH="../build/"

# ****************************************************
# Targets needed to bring the executable up to date
 
main:
	$(CC) $(CPPFLAGS) -std=c++11 main.cpp -L /opt/X11/lib -lX11 -o $(PRGNAME) 
	mv $(PRGNAME) $(BUILDPATH)
