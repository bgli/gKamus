## Process this file with automake to produce Makefile.in

SUBDIRS = src data pixmaps

gkamusdocdir = ${prefix}/doc/gkamus
gkamusdoc_DATA = \
        README \
        COPYING \
        AUTHORS \
        ChangeLog \
        INSTALL

gkamusmenudir = ${prefix}/share/applications
gkamusmenu_DATA = gkamus.desktop

EXTRA_DIST = $(gkamusdoc_DATA) $(gkamusmenu_DATA)
