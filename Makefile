PROGRAM=testmaze
SRCDIR:=../src
INCDIR:=../include
PREFIX?=/usr/local
BINDIR?=bin

SRC:=$(wildcard $(SRCDIR)/*.cc)
OBJECTS:=$(patsubst $(SRCDIR)/%.cc,./%.o,$(SRC))
DEPFILES:=$(patsubst $(SRCDIR)/%.cc,./%.d,$(SRC))

CXX?=/usr/bin/g++
STRIP?=/usr/bin/strip --strip-all  -R .comment -R .note $(PROGRAM)
INSTALL?=/usr/bin/install
VALGRIND?=/usr/bin/valgrind

DEPFLAGS=-MT $@ -MMD -MP -MF $*.d
CPPFLAGS+=$(DEPFLAGS) -I$(INCDIR) $(NCURSESFLAGS)
CXXFLAGS+=-std=c++17 -Wall -Wextra -Wpedantic -Weffc++ -flto
LDFLAGS+=-ffunction-sections -fdata-sections -Wl,-gc-sections
LIBS=
get_builddir = '$(findstring '$(notdir $(CURDIR))', 'debug' 'release')'

.cc.o:

$(PROGRAM): $(OBJECTS) | checkinbuilddir
	$(LINK.cc) $(OUTPUT_OPTION) $^ $(LIBS)
	$(STRIP)

$(DEPFILES):

checkinbuilddir:
ifeq ($(call get_builddir), '')
	$(error 'Change to the debug or release directories and run make from there.')
endif

checkintopdir:
ifneq ($(call get_builddir), '')
	$(error 'Make this target from the top-level directory.')
endif

memcheck: $(PROGRAM) | checkinbuilddir
	$(VALGRIND) --suppressions=../valgrind.suppressions --quiet --verbose --trace-children=yes --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=valgrind.log ./$(PROGRAM)

install:
	@cd release && $(MAKE) install-$(PROGRAM)

clean:
	-$(RM) *.o *.d valgrind.log $(PROGRAM)

distclean: | checkintopdir
	cd debug && $(MAKE) clean
	cd release && $(MAKE) clean

.PHONY: checkinbuilddir checkintopdir memcheck install clean distclean

.DELETE_ON_ERROR:

-include $(wildcard $(DEPFILES))
