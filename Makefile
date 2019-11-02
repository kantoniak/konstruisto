# Project details
PROJECT_NAME=Konstruisto
PROJECT_VERSION=$(shell git describe --match "v[0-9]*" --abbrev=0 HEAD)
PROJECT_LAST_COMMIT=$(shell git rev-parse --short HEAD)

ifneq ($(CONFIG), RELEASE)
   CONFIG=DEBUG
endif

ifneq ($(HIDE_CONSOLE), TRUE)
   HIDE_CONSOLE=FALSE
endif

BUILD_DESC = $(PROJECT_VERSION)-$(PROJECT_LAST_COMMIT) $(CONFIG)

# Paths and dependencies
SRCDIR := src
OBJDIR := obj
BINDIR := bin
EXTDIR := ext

CC=clang
CXX=clang++
RM_R=rm -rf

ifeq ($(OS), Windows_NT)
	SYSTEM := WINDOWS
	EXTENSION := .exe

	ifeq ($(HIDE_CONSOLE), TRUE)
		LDFLAGS += -Wl,--subsystem,windows
	endif

	INCLUDES += -I$(EXTDIR)/glfw-3.3.0/include
	LDFLAGS  += -L$(EXTDIR)/glfw-3.3.0/lib

	INCLUDES += -I$(EXTDIR)/glew-2.1.0/include
	LDFLAGS  += -L$(EXTDIR)/glew-2.1.0/lib

	INCLUDES += -I$(EXTDIR)/glm-0.9.9.6/

	INCLUDES += -I$(EXTDIR)/cereal-1.3.0/include

	INCLUDES += -I$(EXTDIR)/nanovg/src
	LDFLAGS  += -L$(EXTDIR)/nanovg/build-windows

	INCLUDES += -I$(EXTDIR)/stb/

	LIBS := -lglfw3 -lglew32 -lopengl32 -lglu32 -lgdi32 -lnanovg
else
	SYSTEM := LINUX

	INCLUDES += -I$(EXTDIR)/glm-0.9.9.6/

	INCLUDES += -I$(EXTDIR)/cereal-1.3.0/include

	INCLUDES += -I$(EXTDIR)/nanovg/src
	LDFLAGS  += -L$(EXTDIR)/nanovg/build-linux

	INCLUDES += -I$(EXTDIR)/stb/

	LIBS := -lglfw -lGLEW -lGL -lGLU -lnanovg
endif

DEFINES +=-D_USE_MATH_DEFINES -DPROJECT_NAME=\""$(PROJECT_NAME)\"" -DPROJECT_VERSION=\""$(PROJECT_VERSION)\"" -DBUILD_DESC=\""$(BUILD_DESC)\""
CPPFLAGS =-std=c++17 -Wall -Wextra -Werror -Wformat-nonliteral -Winit-self -Wno-nonportable-include-path --system-header-prefix=glm  --system-header-prefix=nanovg -DGLEW_STATIC

ifeq ($(CONFIG), DEBUG)
	DEFINES +=-D_DEBUG
	CPPFLAGS +=-g
else
	CPPFLAGS +=-O3
endif

CPPFLAGS += $(INCLUDES)

# Targets
rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))
HPP_FILES := $(call rwildcard,$(SRCDIR),*.hpp)
CPP_FILES := $(call rwildcard,$(SRCDIR),*.cpp)
OBJ_FILES := $(addprefix $(OBJDIR)/,$(subst src/, , $(subst .cpp,.o,$(CPP_FILES))))
RELEASE_ZIP_NAME := $(PROJECT_NAME) $(BUILD_DESC) $(SYSTEM)

ifeq ($(OS), Windows_NT)
	ifeq ($(CONFIG), RELEASE)
		RELEASE_DLLS += libstdc++-6.dll
		RELEASE_DLLS += libgcc_s_seh-1.dll
		RELEASE_DLLS += libwinpthread-1.dll
	endif

	CPPFLAGS := -target x86_64-w64-pc-windows-gnu $(CPPFLAGS)
	OBJ_FILES += $(OBJDIR)/windows.rc.o
endif

all: clean build run

rebuild: clean build

clean:
	@echo "Removing $(BINDIR)/$(PROJECT_NAME)$(EXTENSION)..."
	@$(RM) $(BINDIR)/$(PROJECT_NAME)$(EXTENSION)
	@echo "Removing $(OBJDIR)/*..."
	@$(RM_R) ./$(OBJDIR)/*

build: $(BINDIR)/$(PROJECT_NAME)$(EXTENSION)

$(BINDIR)/$(PROJECT_NAME)$(EXTENSION): $(OBJ_FILES)
	@mkdir -p $(BINDIR)
	@echo "[LINK] $(CXX) $(CPPFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)"
	@$(CXX) $(CPPFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp	
	@mkdir -p $(@D)
	@echo "[COMPILE] $< $@"
	@$(CXX) -c -o $@ $< $(CPPFLAGS) $(DEFINES)

$(OBJDIR)/windows.rc.o:
	windres $(SRCDIR)/windows.rc $(OBJDIR)/windows.rc.o

run:
	@echo
	@cd $(BINDIR); ./$(PROJECT_NAME)$(EXTENSION)

format-all:
	clang-format-9 -i -style=file -fallback-style=llvm -sort-includes $(CPP_FILES) $(HPP_FILES)

modernize-all:
	clang-tidy-9 -checks="modernize-*,-modernize-use-trailing-return-type" -fix $(CPP_FILES) $(HPP_FILES) -- $(CPPFLAGS) $(DEFINES)

todos:
	@grep -norwP src/ -e '(TODO|FIXME).*$''

release-zip: rebuild $(RELEASE_DLLS)
	@echo "Copying to releases/$(RELEASE_ZIP_NAME)..."
	@mkdir -p "releases/$(RELEASE_ZIP_NAME)"
	@cd $(BINDIR); cp -r assets "../releases/$(RELEASE_ZIP_NAME)"
	@cd $(BINDIR); cp "$(PROJECT_NAME)$(EXTENSION)" "../releases/$(RELEASE_ZIP_NAME)"
	@cd $(EXTDIR)/dlls/; cp -t "../../releases/$(RELEASE_ZIP_NAME)" $(RELEASE_DLLS)
	@echo "Zipping to releases/$(RELEASE_ZIP_NAME).zip..."
	@cd releases; zip -r "$(RELEASE_ZIP_NAME).zip" "$(RELEASE_ZIP_NAME)"

%.dll:
	@mkdir -p $(EXTDIR)/dlls
	@wget -q -nv -N http://konstruisto.com/download/dlls/$@ -O $(EXTDIR)/dlls/$@

help:
	@echo $(PROJECT_NAME) $(PROJECT_VERSION)-$(PROJECT_LAST_COMMIT)
	@echo ""
	@echo "Targets:"
	@echo "  clean"
	@echo "  format-all"
	@echo "  modernize-all"
	@echo "  build"
	@echo "  rebuild (clean + build)"
	@echo "  run"
	@echo "  all"
	@echo "  todos"
	@echo "  release-zip"
	@echo "  help"
	@echo ""
	@echo "Flags:"
	@echo "  CONFIG=[DEBUG|RELEASE]"
	@echo "  HIDE_CONSOLE=[TRUE|FALSE] (Windows)"
