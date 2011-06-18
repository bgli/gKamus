## Process this file with automake to produce Makefile.in

INCLUDES = $(PACKAGE_CFLAGS)

AM_CFLAGS = -Wall -fstack-protector

bin_PROGRAMS = gkamus

gkamus_SOURCES = \
	callback.c \
	function.c \
	gui.c \
	main.c

gkamus_LDFLAGS =

gkamus_LDADD = $(PACKAGE_LIBS)
