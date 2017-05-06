# Project details
PROJECT_NAME=Konstruisto
PROJECT_VERSION=$(shell git describe --match "v[0-9]*" --abbrev=0 HEAD)
PROJECT_LAST_COMMIT=$(shell git rev-parse --short HEAD)

BUILD_DESC = $(PROJECT_VERSION)-$(PROJECT_LAST_COMMIT) $(CONFIG)

# Paths and dependencies
SRCDIR := src
TESTDIR := test
OBJDIR := obj
EXTDIR := ../ext

CC=clang
CXX=clang++
RM_R=rm -rf
LIBGTEST=$(EXTDIR)/googletest-release-1.8.0/googletest/lib/libgtest.a

ifeq ($(OS), Windows_NT)
	SYSTEM := WINDOWS
	EXTENSION := .exe

	ifeq ($(HIDE_CONSOLE), TRUE)
		LDFLAGS += -Wl,--subsystem,windows
	endif

	INCLUDES += -I$(EXTDIR)/glfw-3.2.1/include
	LDFLAGS  += -L$(EXTDIR)/glfw-3.2.1/lib

	INCLUDES += -I$(EXTDIR)/glew-2.0.0/include
	LDFLAGS  += -L$(EXTDIR)/glew-2.0.0/lib

	INCLUDES += -I$(EXTDIR)/glm/

	INCLUDES += -I$(EXTDIR)/nanovg/src
	LDFLAGS  += -L$(EXTDIR)/nanovg/build-windows

	INCLUDES += -I$(EXTDIR)/stb/

	LIBS := -lglfw3 -lglew32 -lopengl32 -lglu32 -lgdi32 -lnanovg

	OBJ_FILES += $(OBJDIR)/windows.rc.o
else
	SYSTEM := LINUX

	INCLUDES += -I$(EXTDIR)/glm/

	INCLUDES += -I$(EXTDIR)/nanovg/src
	LDFLAGS  += -L$(EXTDIR)/nanovg/build-linux

	INCLUDES += -I$(EXTDIR)/stb/

	LIBS := -lglfw -lGLEW -lGL -lGLU -lnanovg
endif

INCLUDES += -isystem $(EXTDIR)/googletest-release-1.8.0/googletest/include

DEFINES +=-D_USE_MATH_DEFINES -DPROJECT_NAME=\""$(PROJECT_NAME)\"" -DPROJECT_VERSION=\""$(PROJECT_VERSION)\"" -DBUILD_DESC=\""$(BUILD_DESC)\""
CPPFLAGS =-std=c++14 -Wall -Wextra -Werror -Wformat-nonliteral -Winit-self -Wno-nonportable-include-path --system-header-prefix=glm/  --system-header-prefix=nanovg -DGLEW_STATIC

DEFINES +=-DDEBUG_CONFIG
CPPFLAGS +=-g

CPPFLAGS += $(INCLUDES)

# Targets
rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))
HPP_FILES := $(call rwildcard,$(SRCDIR),*.hpp)
CPP_FILES := $(call rwildcard,$(SRCDIR),*.cpp)
OBJ_FILES := $(addprefix $(OBJDIR)/,$(subst src/, , $(subst .cpp,.o,$(CPP_FILES))))

all: clean build run

rebuild: clean build

clean:
	@$(RM_R) ./$(BINDIR)/*
	@$(RM_R) ./$(OBJDIR)/*

build: $(OBJ_FILES)
	@mkdir -p $(BINDIR)
	@echo "[LINK] $(CXX) $(CPPFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)"
	@$(CXX) $(CPPFLAGS) $(LDFLAGS) -o tests $^ $(LIBGTEST) $(LIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(@D)
	@echo "[COMPILE] $< $@"
	@$(CXX) -c -o $@ $< $(CPPFLAGS) $(DEFINES)

run:
	./tests

format-all:
	clang-format -i -style=file -fallback-style=llvm -sort-includes $(CPP_FILES) $(HPP_FILES)
