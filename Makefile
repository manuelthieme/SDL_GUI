BUILD = build
ifneq ($(notdir $(CURDIR)),$(BUILD))

.SUFFIXES:
OBJDIR := $(CURDIR)/build
export INCDIR := $(CURDIR)/inc
export SRCDIR := $(CURDIR)/src
export ROOTDIR := $(CURDIR)

RM     := rm -rf
MKDIR  := mkdir -p

SRCSALL      := $(shell find $(CURDIR) -name "*.cc" -o -name "*.h")
SRCSCCABS    := $(filter %.cc, $(SRCSALL))
SRCSCC       := $(patsubst $(SRCDIR)/%,%,$(SRCSCCABS))

# create directories
$(foreach dirname,$(dir $(SRCSCC)),$(shell $(MKDIR) $(OBJDIR)/$(dirname)))

.PHONY: $(all)
all:
	+@$(MAKE) --no-print-directory -C $(OBJDIR) -f $(CURDIR)/Makefile $(MAKECMDGOALS)

Makefile : ;

% :: all ;

.PHONY: clean
clean:
	$(RM) $(OBJDIR)

.PHONY: sure
sure: clean
	+@$(MAKE) --no-print-directory

else

TARGET       := SDL_GUI

SRCSALL      := $(shell find $(ROOTDIR) -name "*.cc" -o -name "*.h")
SRCSCCABS    := $(filter %.cc, $(SRCSALL))
SRCSCC       := $(patsubst $(SRCDIR)/%,%,$(SRCSCCABS))
SRCHABS      := $(filter %.h, $(SRCSALL))
SRCSH        := $(patsubst $(INCDIR)/%,%,$(SRCSHABS))
OBJS         := $(SRCSCC:.cc=.o)
DEPS         := $(SRCSCC:.cc=.d)

CXXFLAGS     := -std=c++2a -Wall -Wextra -Wpedantic -ggdb -O0 `sdl2-config --cflags`
CXXFLAGS     += -I$(INCDIR)

CXXFLAGSTAGS := -I/home/manuel/.vim/tags

LIBS         := -lSDL2 -lSDL2_gfx -lSDL2_ttf -lSDL2_image

vpath %.h $(dir $(SRCSHABS))
vpath %.cc $(dir $(SRCSCCABS))
vpath %.d $(dir $(DEPS))

.PHONY: all
all: $(TARGET)

.PHONY: tags
tags: $(ROOTDIR)/tags

.PHONY: sanitized
sanitized: CXXFLAGS += -fsanitize=address
sanitized: LIBS += -fsanitize=address
sanitized: all

.PHONY: effective
effective: CXXFLAGS += -Weffc++
effective: all

.PHONY: makefile-debug
makefile-debug:
	@echo $(filter-out main.o, $(OBJS))

.PHONY: lib
lib: SDL_GUI.a

SDL_GUI.a: $(filter-out main.o, $(OBJS))
	$(AR) rvs $(TARGET).a $(filter-out main.o, $^)

%.o: %.d

$(TARGET): lib main.o
	$(CXX) -o $@ main.o $@.a $(LIBS)

%.d: %.cc
	$(CXX) $(CXXFLAGS) -MM -o $@ $<

$(ROOTDIR)/tags: $(SRCSCC)
	$(CXX) $(CXXFLAGSTAGS) $(CXXFLAGS) -M $(SRCSCCABS) | sed -e 's/[\\ ]/\n/g' | \
	sed -e '/^$$/d' -e '/\.o:[ \t]*$$/d' | \
	ctags -L - --c++-kinds=+p --fields=+iaS --extras=+q -o "$(ROOTDIR)/tags" --language-force=C++

-include $(DEPS)

endif
