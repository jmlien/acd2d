
#-----------------------------------------------------------------------------
# On Linux,    this should be 'Linux'
# On Mac OS X, this should be 'MAC_OS_X'
#-----------------------------------------------------------------------------

PLATFORM := $(shell uname -s)
MACHINE := $(shell uname -m)

ifeq ($(PLATFORM),Linux)
  ifeq ($(MACHINE),x86_64)
    PLATFORM := LINUX64
	TARGETLIB = libACD2d.linux64.a
  else
    PLATFORM := LINUX
	TARGETLIB = libACD2d.linux.a
  endif
else
  ifeq ($(PLATFORM),Darwin)
    PLATFORM := MAC_OS_X
	TARGETLIB = libACD2d.osx.a
  else
    PLATFORM := OTHER
	TARGETLIB = libACD2d.a
  endif
endif


#-----------------------------------------------------------------------------
# Choose a compiler & its options
#--------------------------------------------------------------------------

CXX  = g++
LINK = g++
OPTS = -std=c++11 -MMD -W -O3 -Wno-deprecated -Wno-unused-parameter

#--------------------------------------------------------------------
# Xlib and OpenGL
#--------------------------------------------------------------------

ifeq ($(PLATFORM),MAC_OS_X)
   GL_LIB = -framework OpenGL -framework GLUT -framework Foundation
else
   X_LIB  = -Wl,-rpath,/usr/X11R6/lib -L/usr/X11R6/lib -L/usr/local/lib -lXext -lX11 -lXt 
   GL_LIB = -lglut -lGLU -lGL -lm
endif

#--------------------------------------------------------------------
# Triangulation
#--------------------------------------------------------------------
TRI_DIR = ./libs/FIST
TRI_INCL= -I${TRI_DIR}
TRI_LIB = -L${TRI_DIR}

ifeq ($(PLATFORM),MAC_OS_X)
  TRI_LIB += -lFIST.osx
else 
  ifeq ($(PLATFORM),LINUX64)
    TRI_LIB +=  -lFIST.linux64
  else
    TRI_LIB +=  -lFIST.linux
  endif
endif

	
#--------------------------------------------------------------------
# Put all together
#--------------------------------------------------------------------
TARGET  = acd2d_gui
SUBDIRS = . src src/mathtool src/hull src/edge_visibility 
GUIS = gui gui/GL
INCLUDE = $(addprefix -I,$(SUBDIRS)) $(addprefix -I,$(GUIS)) $(TRI_INCL)
SRCS=$(wildcard $(addsuffix /*.cpp,$(SUBDIRS)))
OBJS=${SRCS:.cpp=.o}
GUI_SRCS=$(wildcard $(addsuffix /*.cpp,$(GUIS)))
GUI_OBJS=${GUI_SRCS:.cpp=.o}
LIB = $(X_LIB) $(GL_LIB) $(TRI_LIB)

CFLAGS   = $(OPTS) $(INCLUDE) 
CXXFLAGS = $(CFLAGS) 


.PHONY: all
all :  $(TARGET)

#--------------------------------------------------------------------

#--------------------------------------------------------------------
$(TARGET): $(OBJS) $(GUI_OBJS)
	${CXX} ${CXXFLAGS} -o $@ $(OBJS) $(GUI_OBJS) $(LIB)

lib: $(OBJS) 
	ar rcs $(TARGETLIB) $(OBJS)


clean:
	-rm -f $(OBJS) $(GUI_OBJS) $(TARGET) Dependencies $(TARGETLIB)

#--------------------------------------------------------------------
.SUFFIXES: .cpp

.cpp.o:
	${CXX} ${CXXFLAGS} -c $< -o $@ 
	cat $*.d >> Dependencies
	rm -f $*.d 

Dependencies:
	touch Dependencies

include Dependencies


