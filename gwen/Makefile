TARGET_LIB = libGWEN.a
INCLUDES   = include

PREFIX  ?= ${VITASDK}/arm-vita-eabi
CC      = arm-vita-eabi-gcc
CXX     = arm-vita-eabi-g++
AR      = arm-vita-eabi-ar
CFLAGS  = -Wl,-q -Wall -O3 -I$(INCLUDES) -ffat-lto-objects -flto
CXXFLAGS = $(CFLAGS) -std=c++11 -g
ASFLAGS = $(CFLAGS)

rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))

LIBS := -lfreetype -lvita2d -lpng -ljpeg -lm -lc \
		-lSceAppMgr_stub -lSceAppUtil_stub -lSceCommonDialog_stub \
		-lSceCtrl_stub -lSceDisplay_stub -lSceGxm_stub -lSceIme_stub \
		-lSceKernel_stub -lSceSysmodule_stub -lSceTouch_stub

SRC_C :=$(call rwildcard, src/, *.c)
SRC_CPP :=$(call rwildcard, src/, *.cpp)

OBJ_DIRS := $(addprefix out/, $(dir $(SRC_C:src/%.c=%.o))) $(addprefix out/, $(dir $(SRC_CPP:src/%.cpp=%.o)))
OBJS := $(addprefix out/, $(SRC_C:src/%.c=%.o)) $(addprefix out/, $(SRC_CPP:src/%.cpp=%.o))

all: $(TARGET_LIB)

debug: CFLAGS += -DDEBUG_BUILD
debug: all

$(TARGET_LIB): $(OBJS)
	$(AR) -r $@ $^

$(OBJ_DIRS):
	mkdir -p $@

out/%.o : src/%.cpp | $(OBJ_DIRS)
	$(CXX) -Og -g -c $(CXXFLAGS) -o $@ $< $(LIBS)

out/%.o : src/%.c | $(OBJ_DIRS)
	$(CC) -g -Og -c $(CFLAGS) -o $@ $< $(LIBS)

clean:
	rm -rf $(TARGET_LIB) $(OBJS)

install: $(TARGET_LIB)
	@mkdir -p $(PREFIX)/lib/
	@mkdir -p $(PREFIX)/include/Gwen/
	cp $(TARGET_LIB) $(PREFIX)/lib/
	cp -r include/Gwen $(PREFIX)/include