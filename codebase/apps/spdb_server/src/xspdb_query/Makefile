# *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=* 
# ** Copyright UCAR (c) 1990 - 2016                                         
# ** University Corporation for Atmospheric Research (UCAR)                 
# ** National Center for Atmospheric Research (NCAR)                        
# ** Boulder, Colorado, USA                                                 
# ** BSD licence applies - redistribution and use in source and binary      
# ** forms, with or without modification, are permitted provided that       
# ** the following conditions are met:                                      
# ** 1) If the software is modified to produce derivative works,            
# ** such modified software should be clearly marked, so as not             
# ** to confuse it with the version available from UCAR.                    
# ** 2) Redistributions of source code must retain the above copyright      
# ** notice, this list of conditions and the following disclaimer.          
# ** 3) Redistributions in binary form must reproduce the above copyright   
# ** notice, this list of conditions and the following disclaimer in the    
# ** documentation and/or other materials provided with the distribution.   
# ** 4) Neither the name of UCAR nor the names of its contributors,         
# ** if any, may be used to endorse or promote products derived from        
# ** this software without specific prior written permission.               
# ** DISCLAIMER: THIS SOFTWARE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS  
# ** OR IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED      
# ** WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.    
# *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=* 
###########################################################################
#
# Makefile for xspdb_client
#
# Frank Hage RAP, NCAR, Boulder, CO, 80307, USA
#
###########################################################################

include $(LROSE_CORE_DIR)/build/make_include/lrose_make_macros

TARGET_FILE = xspdb_query

LOC_INCLUDES = $(SYS_X_INCLUDES) $(SYS_XVIEW_INCLUDES) $(NETCDF4_INCS)

LOC_LIBS = -ltitan -lSpdb -lsymprod -lmdv \
	-ldsserver -ldidss -lrapformats -lrapmath \
	-ltoolsa -ldataport -ldevguide -lxview \
	-lolgx -lX11 $(NETCDF4_LIBS) -lbz2 \
	-lz -lpthread -lm

LOC_LDFLAGS = -g $(SYS_X_LDFLAGS) $(SYS_XVIEW_LDFLAGS) $(NETCDF4_LDFLAGS)

LOC_CFLAGS = $(DEBUG_CFLAGS)
LOC_CPPFLAGS = -g

HDRS =  xspdb_client.h \
    print_chunks.h \
	xspdb_client_gui_ui.h 

SRCS =  xspdb_client.c \
	xspdb_client_gui_ui.c \
	print_chunks.c \
	xspdb_client_gui_proc.c 
#
# objects and lint files
#


#
# standard targets
#

include $(LROSE_CORE_DIR)/build/make_include/lrose_make_c++_targets

#
# local targets
#

depend: depend_generic

# DO NOT DELETE THIS LINE -- make depend depends on it.

