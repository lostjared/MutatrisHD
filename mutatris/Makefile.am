AM_CXXFLAGS = -std=c++11 -O2 $(LIBSDL_CFLAGS) -I$(top_srcdir)/include -I$(prefix)/include 
AM_LDFLAGS = #$(LIBSDL_LIBS) $(LIBGLEW_GLEWLIB) #$(prefix)/lib/libpng.a $(prefix)/lib/libz.a
bin_PROGRAMS=MutatrisHD
MutatrisHD_SOURCES=fixedsys.cc font_source.cpp loadbmp.cpp mpmut.cpp mutatris3d.cpp types.cpp video.cpp MX_interface.cpp
MutatrisHD_LDADD=$(LIBSDL_LIBS) $(LIBGLEW_GLEWLIB) $(LIBGL_LIB)  #$(prefix)/lib/libpng.a $(prefix)/lib/libz.a


