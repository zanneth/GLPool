TARGET  := homework
SRCS    := src/application.cpp \
			src/descriptor_file.cpp \
			src/homework5.cpp \
			src/jpeg_image.cpp \
			src/main.cpp \
			src/picture.cpp \
			src/scene_graph.cpp \
			src/shader_program.cpp
OBJS    := ${SRCS:.cpp=.o} 
DEPS    := ${SRCS:.cpp=.dep} 
XDEPS   := $(wildcard ${DEPS})
CC      := g++
BUILDDIR:= build/
INCLUDESDIR := includes/

CCFLAGS = -O2 -Wall -Werror -ggdb
LDFLAGS = 

# Detect if Mac or Linux, and set the proper library flags.
UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
	LIBS = -framework GLUT -framework OpenGL -ljpeg -lglew
else
	LIBS = -lGL -lGLU -lglut -lm -ljpeg -lGLEW
endif


.PHONY: all clean distclean 
all:: ${TARGET}

ifneq (${XDEPS},)
include ${XDEPS} 
endif 

${TARGET}: ${OBJS}
	${CC} ${LDFLAGS} -o $@ $(addprefix $(BUILDDIR), $(notdir $^)) ${LIBS} 

${OBJS}: %.o: %.cpp %.dep
	@-mkdir -p $(BUILDDIR)
	${CC} ${CCFLAGS} -c $< -o $(addprefix $(BUILDDIR), $(notdir $@))

${DEPS}: %.dep: %.cpp Makefile 
	@-mkdir -p $(BUILDDIR)
	${CC} ${CCFLAGS} -MM $< > $(addprefix $(BUILDDIR), $(notdir $@)) 

clean::
	-rm -rf $(BUILDDIR)
	-rm -f *~ *.o *.dep *.a *.so *.so.* *.d ${TARGET} 

distclean:: clean
