#
# NBKE Makefile
#
# GNU Make required
#

COMPILE_PLATFORM=$(shell uname|sed -e s/_.*//|tr '[:upper:]' '[:lower:]'|sed -e 's/\//_/g')

COMPILE_ARCH=$(shell uname -m | sed -e s/i.86/i386/)

ifeq ($(COMPILE_PLATFORM),sunos)
  # Solaris uname and GNU uname differ
  COMPILE_ARCH=$(shell uname -p | sed -e s/i.86/i386/)
endif
ifeq ($(COMPILE_PLATFORM),darwin)
  # Apple does some things a little differently...
  COMPILE_ARCH=$(shell uname -p | sed -e s/i.86/i386/)
endif

ifeq ($(COMPILE_PLATFORM),mingw32)
  ifeq ($(COMPILE_ARCH),i386)
    COMPILE_ARCH=x86
  endif
  ifeq ($(COMPILE_ARCH),x86_64)
    COMPILE_ARCH=x64
  endif
endif

ifndef BUILD_NBKE
  BUILD_NBKE     =
endif

#############################################################################
#
# If you require a different configuration from the defaults below, create a
# new file named "Makefile.local" in the same directory as this file and define
# your parameters there. This allows you to change configuration without
# causing problems with keeping up to date with the repository.
#
#############################################################################
-include Makefile.local

ifndef PLATFORM
PLATFORM=$(COMPILE_PLATFORM)
endif
export PLATFORM

ifeq ($(COMPILE_ARCH),powerpc)
  COMPILE_ARCH=ppc
endif
ifeq ($(COMPILE_ARCH),powerpc64)
  COMPILE_ARCH=ppc64
endif

ifndef ARCH
ARCH=$(COMPILE_ARCH)
endif
export ARCH

ifneq ($(PLATFORM),$(COMPILE_PLATFORM))
  CROSS_COMPILING=1
else
  CROSS_COMPILING=0

  ifneq ($(ARCH),$(COMPILE_ARCH))
    CROSS_COMPILING=1
  endif
endif
export CROSS_COMPILING

ifndef VERSION
VERSION=0.2
endif

ifndef NBKEBIN
NBKEBIN=nbke
endif

ifndef COPYDIR
COPYDIR=.
endif

ifndef COPYBINDIR
COPYBINDIR=$(COPYDIR)
endif

ifndef MOUNT_DIR
MOUNT_DIR=.
endif

ifndef BUILD_DIR
BUILD_DIR=build
endif

ifndef TEMPDIR
TEMPDIR=/tmp
endif

ifndef USE_LOCAL_HEADERS
USE_LOCAL_HEADERS=0
endif

ifndef DEBUG_CXXFLAGS
DEBUG_CXXFLAGS=-g -O0
endif

#############################################################################

BD=$(BUILD_DIR)/debug-$(PLATFORM)-$(ARCH)
BR=$(BUILD_DIR)/release-$(PLATFORM)-$(ARCH)
CDIR=$(MOUNT_DIR)
CMLDIR=$(MOUNT_DIR)/Externals
SDLHDIR=$(MOUNT_DIR)/Externals/SDL12
LIBSDIR=$(MOUNT_DIR)/libs

bin_path=$(shell which $(1) 2> /dev/null)

# set PKG_CONFIG_PATH to influence this, e.g.
# PKG_CONFIG_PATH=/opt/cross/i386-mingw32msvc/lib/pkgconfig
ifneq ($(call bin_path, pkg-config),)
  SDL_CFLAGS=$(shell pkg-config --silence-errors --cflags sdl|sed 's/-Dmain=SDL_main//')
  SDL_LIBS=$(shell pkg-config --silence-errors --libs sdl)
endif
# Use sdl-config if all else fails
ifeq ($(SDL_CFLAGS),)
  ifneq ($(call bin_path, sdl-config),)
    SDL_CFLAGS=$(shell sdl-config --cflags)
    SDL_LIBS=$(shell sdl-config --libs)
  endif
endif
SDL_CFLAGS+=$(shell pkg-config --silence-errors --cflags SDL_image)
SDL_LIBS+=$(shell pkg-config --silence-errors --libs SDL_image)
LUA_CFLAGS=$(shell pkg-config --silence-errors --cflags lua)
LUA_LIBS=$(shell pkg-config --silence-errors --libs lua)

#############################################################################
# SETUP AND BUILD -- LINUX
#############################################################################

## Defaults
LIB=lib

INSTALL=install
MKDIR=mkdir

ifneq (,$(findstring "$(PLATFORM)", "linux" "gnu_kfreebsd" "kfreebsd-gnu"))

  ifeq ($(ARCH),axp)
    ARCH=alpha
  else
  ifeq ($(ARCH),x86_64)
    LIB=lib64
  else
  ifeq ($(ARCH),ppc64)
    LIB=lib64
  else
  ifeq ($(ARCH),s390x)
    LIB=lib64
  endif
  endif
  endif
  endif

  BASE_CXXFLAGS = -Wall -fno-strict-aliasing -pipe -Wno-unknown-pragmas \
  		-fmessage-length=0 -fpermissive -fvisibility=hidden
  BASE_CXXFLAGS += -fabi-version=5 -Wabi
  
  # TODO Associer uniquement avec les fichiers en ayant éventuellement besoin
  BASE_CXXFLAGS += $(LUA_CFLAGS)

  OPTIMIZE = -O3 -Winline -fomit-frame-pointer -ffast-math \
  		-fno-unsafe-math-optimizations

  ifeq ($(ARCH),x86_64)
    OPTIMIZE = -O3 -Winline -fomit-frame-pointer -ffast-math \
      -falign-loops=2 -falign-jumps=2 -falign-functions=2 -fstrength-reduce
  else
  ifeq ($(ARCH),i386)
    OPTIMIZE = -O3 -march=i586 -fomit-frame-pointer \
      -funroll-loops -falign-loops=2 -falign-jumps=2 \
      -falign-functions=2 -fstrength-reduce -ffast-math
  else
  ifeq ($(ARCH),ppc)
    BASE_CXXFLAGS += -maltivec
  endif
  ifeq ($(ARCH),ppc64)
    BASE_CXXFLAGS += -maltivec
  endif
  ifeq ($(ARCH),sparc)
    OPTIMIZE += -mtune=ultrasparc3 -mv8plus
  endif
  endif
  endif

  LIBS=-ldl -lm

  NBKE_LIBS  = -lGL -lGLU -lGLEW
  NBKE_LIBS += $(LUA_LIBS)

  ifeq ($(USE_LOCAL_HEADERS),1)
    NBKE_CXXFLAGS += -I$(SDLHDIR)/include -DUSE_LOCAL_HEADERS
    NBKE_CXXFLAGS += -I$(MOUNT_DIR)/Externals
  else
    NBKE_CXXFLAGS += $(SDL_CFLAGS)
    NBKE_LIBS += $(SDL_LIBS)
  endif

  ifeq ($(ARCH),i386)
    # linux32 make ...
    BASE_CXXFLAGS += -m32
    LDFLAGS += -L/usr/lib
  else
  ifeq ($(ARCH),ppc64)
    BASE_CXXFLAGS += -m64
  endif
  endif
else # ifeq Linux

#############################################################################
# SETUP AND BUILD -- MAC OS X
#############################################################################

ifeq ($(PLATFORM),darwin)
  HAVE_VM_COMPILED=true
  LIBS = -framework Cocoa
  NBKE_LIBS=
  OPTIMIZEVM=
  
  BASE_CFLAGS = -Wall -Wimplicit -Wstrict-prototypes

  ifeq ($(ARCH),ppc)
    BASE_CFLAGS += -arch ppc -faltivec -mmacosx-version-min=10.2
    OPTIMIZEVM += -O3
  endif
  ifeq ($(ARCH),ppc64)
    BASE_CFLAGS += -arch ppc64 -faltivec -mmacosx-version-min=10.2
  endif
  ifeq ($(ARCH),i386)
    OPTIMIZEVM += -march=prescott -mfpmath=sse
    # x86 vm will crash without -mstackrealign since MMX instructions will be
    # used no matter what and they corrupt the frame pointer in VM calls
    BASE_CFLAGS += -arch i386 -m32 -mstackrealign
  endif
  ifeq ($(ARCH),x86_64)
    OPTIMIZEVM += -arch x86_64 -mfpmath=sse
  endif

  BASE_CFLAGS += -fno-strict-aliasing -DMACOS_X -fno-common -pipe
  BASE_CFLAGS += -D_THREAD_SAFE=1

  ifeq ($(USE_LOCAL_HEADERS),1)
    BASE_CFLAGS += -I$(SDLHDIR)/include
  endif

  # We copy sdlmain before ranlib'ing it so that subversion doesn't think
  #  the file has been modified by each build.
  LIBSDLMAIN=$(B)/libSDLmain.a
  LIBSDLMAINSRC=$(LIBSDIR)/macosx/libSDLmain.a
  NBKE_LIBS += -framework IOKit $(LIBSDIR)/macosx/libSDL-1.2.0.dylib

  OPTIMIZEVM += -falign-loops=16
  OPTIMIZE = $(OPTIMIZEVM) -ffast-math

else # ifeq darwin


#############################################################################
# SETUP AND BUILD -- MINGW32
#############################################################################

ifeq ($(PLATFORM),mingw32)

  # Some MinGW installations define CC to cc, but don't actually provide cc,
  # so explicitly use gcc instead (which is the only option anyway)
  ifeq ($(call bin_path, $(CC)),)
    CC=gcc
  endif

  ifndef WINDRES
    WINDRES=windres
  endif

  BASE_CFLAGS = -Wall -fno-strict-aliasing -Wimplicit -Wstrict-prototypes \
    -DUSE_ICON

  # In the absence of wspiapi.h, require Windows XP or later
  ifeq ($(shell test -e $(CMDIR)/wspiapi.h; echo $$?),1)
    BASE_CFLAGS += -DWINVER=0x501
  endif

  ifeq ($(ARCH),x64)
    OPTIMIZEVM = -O3 -fno-omit-frame-pointer \
      -falign-loops=2 -funroll-loops -falign-jumps=2 -falign-functions=2 \
      -fstrength-reduce
    OPTIMIZE = $(OPTIMIZEVM) --fast-math
    HAVE_VM_COMPILED = true
  endif
  ifeq ($(ARCH),x86)
    OPTIMIZEVM = -O3 -march=i586 -fno-omit-frame-pointer \
      -falign-loops=2 -funroll-loops -falign-jumps=2 -falign-functions=2 \
      -fstrength-reduce
    OPTIMIZE = $(OPTIMIZEVM) -ffast-math
    HAVE_VM_COMPILED = true
  endif

  BINEXT=.exe

  LIBS= -lws2_32 -lwinmm -lpsapi
  LDFLAGS += -mwindows
  NBKE_LIBS = -lgdi32 -lole32

  ifeq ($(ARCH),x86)
    # build 32bit
    BASE_CFLAGS += -m32
  else
    BASE_CFLAGS += -m64
  endif

  # libmingw32 must be linked before libSDLmain
  NBKE_LIBS += -lmingw32
  
  ifeq ($(USE_LOCAL_HEADERS),1)
    NBKE_CXXFLAGS += -I$(SDLHDIR)/include
    ifeq ($(ARCH), x86)
    NBKE_LIBS += $(LIBSDIR)/win32/libSDLmain.a \
                      $(LIBSDIR)/win32/libSDL.dll.a
    else
    NBKE_LIBS += $(LIBSDIR)/win64/libSDLmain.a \
                      $(LIBSDIR)/win64/libSDL64.dll.a
    endif
  else
    NBKE_CXXFLAGS += $(SDL_CFLAGS)
    NBKE_LIBS += $(SDL_LIBS)
  endif

endif #MINGW32
endif #MAC OS X
endif #LINUX

TARGETS =

ifndef FULLBINEXT
  FULLBINEXT=.$(ARCH)$(BINEXT)
endif

ifneq ($(BUILD_NBKE),0)
  TARGETS += $(B)/$(NBKEBIN)$(FULLBINEXT)
endif

ifeq ($(NO_STRIP),1)
  STRIP_FLAG =
else
  STRIP_FLAG = -s
endif

#BASE_CXXFLAGS += -DPRODUCT_VERSION=\\\"$(VERSION)\\\"
BASE_CXXFLAGS += -Wformat=2 -Wformat-security -Wno-format-nonliteral
BASE_CXXFLAGS += -Wstrict-aliasing=2 -Wmissing-format-attribute
BASE_CXXFLAGS += -Wdisabled-optimization
BASE_CXXFLAGS += -Werror-implicit-function-declaration
BASE_CXXFLAGS += -I$(CMLDIR)

# TODO REMOVE
#BASE_CXXFLAGS += --no-warnings

ifeq ($(V),1)
echo_cmd=@:
Q=
else
echo_cmd=@echo
Q=@
endif

define DO_CXX
$(echo_cmd) "CXX $<"
$(Q)$(CXX) $(NOTSHLIBCFLAGS) $(CXXFLAGS) $(NBKE_CXXFLAGS) $(OPTIMIZE) -o $@ -c $<
endef

define DO_WINDRES
$(echo_cmd) "WINDRES $<"
$(Q)$(WINDRES) -i $< -o $@
endef


#############################################################################
# MAIN TARGETS
#############################################################################

default: release
all: debug release

debug:
	@$(MAKE) targets B=$(BD) CXXFLAGS="$(CXXFLAGS) $(BASE_CXXFLAGS) $(DEPEND_CXXFLAGS)" \
	  OPTIMIZE="$(DEBUG_CXXFLAGS)" NBKE_CXXFLAGS="$(NBKE_CXXFLAGS)" V=$(V)

release:
	@$(MAKE) targets B=$(BR) CXXFLAGS="$(CXXFLAGS) $(BASE_CXXFLAGS) $(DEPEND_CXXFLAGS)" \
	  OPTIMIZE="-DNDEBUG $(OPTIMIZE)" NBKE_CXXFLAGS="$(NBKE_CXXFLAGS)" V=$(V)

# Create the build directories, check libraries and print out
# an informational message, then start building
targets: makedirs
	@echo ""
	@echo "Building $(CLIENTBIN) in $(B):"
	@echo "  PLATFORM: $(PLATFORM)"
	@echo "  ARCH: $(ARCH)"
	@echo "  VERSION: $(VERSION)"
	@echo "  COMPILE_PLATFORM: $(COMPILE_PLATFORM)"
	@echo "  COMPILE_ARCH: $(COMPILE_ARCH)"
	@echo "  CXX: $(CXX)"
	@echo ""
	@echo "  CXXFLAGS:"
	-@for i in $(CXXFLAGS); \
	do \
		echo "    $$i"; \
	done
	-@for i in $(OPTIMIZE); \
	do \
		echo "    $$i"; \
	done
	@echo ""
	@echo "  NBKE_CXXFLAGS:"
	-@for i in $(NBKE_CXXFLAGS); \
	do \
		echo "    $$i"; \
	done
	@echo ""
	@echo "  LDFLAGS:"
	-@for i in $(LDFLAGS); \
	do \
		echo "    $$i"; \
	done
	@echo ""
	@echo "  LIBS:"
	-@for i in $(LIBS); \
	do \
		echo "    $$i"; \
	done
	@echo ""
	@echo "  NBKE_LIBS:"
	-@for i in $(NBKE_LIBS); \
	do \
		echo "    $$i"; \
	done
	@echo ""
	@echo "  Output:"
	-@for i in $(TARGETS); \
	do \
		echo "    $$i"; \
	done
	@echo ""
ifneq ($(TARGETS),)
	@$(MAKE) $(TARGETS) V=$(V)
endif

makedirs:
	@if [ ! -d $(BUILD_DIR) ];then $(MKDIR) $(BUILD_DIR);fi
	@if [ ! -d $(B) ];then $(MKDIR) $(B);fi
	@if [ ! -d $(B)/Core ];then $(MKDIR) $(B)/Core;fi
	@if [ ! -d $(B)/Gui ];then $(MKDIR) $(B)/Gui;fi
	@if [ ! -d $(B)/Lua ];then $(MKDIR) $(B)/Lua;fi

#############################################################################
# NBKE
#############################################################################

NBKEOBJ = \
	$(B)/Core/AnimatedTerrainManager.o \
	$(B)/Core/BasicLight.o \
	$(B)/Core/BinaryHeap.o \
	$(B)/Core/Block.o \
	$(B)/Core/BlockManager.o \
	$(B)/Core/BlockObject.o \
	$(B)/Core/BR5Model.o \
	$(B)/Core/Camera.o \
	$(B)/Core/CollisionManager.o \
	$(B)/Core/Console.o \
	$(B)/Core/ConsoleListener.o \
	$(B)/Core/ControlManager.o \
	$(B)/Core/Conversions.o \
	$(B)/Core/Creature.o \
	$(B)/Core/CreatureManager.o \
	$(B)/Core/CreatureTxtReader.o \
	$(B)/Core/DeltaTime.o \
	$(B)/Core/EconomyManager.o \
	$(B)/Core/Entity.o \
	$(B)/Core/EntityManager.o \
	$(B)/Core/Frustum.o \
	$(B)/Core/GameManager.o \
	$(B)/Core/GUIManager.o \
	$(B)/Core/HeroGate.o \
	$(B)/Core/Imp.o \
	$(B)/Core/Input.o \
	$(B)/Core/InputListener.o \
	$(B)/Core/LevelManager.o \
	$(B)/Core/LightingManager.o \
	$(B)/Core/LightingObject.o \
	$(B)/Core/LightSource.o \
	$(B)/Core/Logger.o \
	$(B)/Core/Manager.o \
	$(B)/Core/NewRenderer.o \
	$(B)/Core/ObjectPool.o \
	$(B)/Core/OGLUtils.o \
	$(B)/Core/PathManager.o \
	$(B)/Core/PathManager1.o \
	$(B)/Core/PickingManager.o \
	$(B)/Core/Primitives.o \
	$(B)/Core/RenderManager.o \
	$(B)/Core/RenderManager1.o \
	$(B)/Core/ResourceManager.o \
	$(B)/Core/Room.o \
	$(B)/Core/RoomConstructor.o \
	$(B)/Core/RoomManager.o \
	$(B)/Core/SettingsManager.o \
	$(B)/Core/ShaderManager.o \
	$(B)/Core/ShaderObject.o \
	$(B)/Core/SpellManager.o \
	$(B)/Core/SubtileData.o \
	$(B)/Core/Text.o \
	$(B)/Core/TextureList.o \
	$(B)/Core/TextureLoader.o \
	$(B)/Core/Trap.o \
	$(B)/Core/VBO.o

NBKEGUIOBJ  = $(B)/Gui/AbstractGUIItem.o \
	$(B)/Gui/AnimatedSpeedButton.o \
	$(B)/Gui/DKGUILabel.o \
	$(B)/Gui/DKInput.o \
	$(B)/Gui/DKProgressBar.o \
	$(B)/Gui/DKRoomInfoTexture.o \
	$(B)/Gui/DKSelectProgressBar.o \
	$(B)/Gui/DKText.o \
	$(B)/Gui/DKTextureList.o \
	$(B)/Gui/DKTextureLoader.o \
	$(B)/Gui/GUI.o \
	$(B)/Gui/GUIBackground.o \
	$(B)/Gui/GUIButton.o \
	$(B)/Gui/GUILabel.o \
	$(B)/Gui/ItemButton.o \
	$(B)/Gui/JumpLabel.o \
	$(B)/Gui/MainMenu.o \
	$(B)/Gui/MenuButton.o \
	$(B)/Gui/MoneyLabel.o \
	$(B)/Gui/PlayGUI.o \
	$(B)/Gui/Primitives.o \
	$(B)/Gui/ProgressBar.o \
	$(B)/Gui/RoomDependancy.o \
	$(B)/Gui/SpeedButton.o \
	$(B)/Gui/TabButton.o \
	$(B)/Gui/TexturedGUIButton.o

NBKELUAOBJ = \
	$(B)/Lua/LUAEffect.o \
	$(B)/Lua/LUAEffectLoader.o \
	$(B)/Lua/LUAEmitter.o \
	$(B)/Lua/LUAParticle.o

ifeq ($(PLATFORM),mingw32)
	NBKEOBJ += \
		$(B)/Core/project.o \
		$(B)/Core/project_rc.o
else
	NBKEOBJ += $(B)/Core/project_unix.o
endif

ifeq ($(PLATFORM),darwin)
	NBKEOBJ += $(B)/Core/project_osx.o
endif

$(B)/$(NBKEBIN)$(FULLBINEXT): $(NBKEOBJ) $(NBKEGUIOBJ) $(NBKELUAOBJ) $(LIBSDLMAIN)
	$(echo_cmd) "LD $@"
	$(Q)$(CXX) $(NBKE_CXXFLAGS) $(CXXFLAGS) $(LDFLAGS) \
		-o $@ $(NBKEOBJ) $(NBKEGUIOBJ) $(NBKELUAOBJ) \
		$(LIBSDLMAIN) $(NBKE_LIBS) $(LIBS)

ifneq ($(strip $(LIBSDLMAIN)),)
ifneq ($(strip $(LIBSDLMAINSRC)),)
$(LIBSDLMAIN) : $(LIBSDLMAINSRC)
	cp $< $@
	ranlib $@
endif
endif

#############################################################################
## OBJECTS RULES
#############################################################################

$(B)/Core/%.o: $(CDIR)/%.cpp
	$(DO_CXX)

$(B)/Gui/%.o: $(CDIR)/GUI/%.cpp
	$(DO_CXX)

$(B)/Lua/%.o: $(CDIR)/LUA/%.cpp
	$(DO_CXX)

$(B)/Core/%_rc.o: $(CDIR)/%.rc
	$(DO_WINDRES)

#############################################################################
# MISC
#############################################################################

OBJ = $(NBKEOBJ) $(NBKEGUIOBJ) $(NBKELUAOBJ)

copyfiles: release
	@if [ ! -d "$(COPYDIR)" ]; then echo "You need to set COPYDIR to where your NBKE data folder is!"; fi

ifneq ($(BUILD_NBKE),0)
	$(INSTALL) $(STRIP_FLAG) -m 0755 $(BR)/$(NBKEBIN)$(FULLBINEXT) $(COPYBINDIR)/$(NBKEBIN)$(FULLBINEXT)
endif

clean: clean-debug clean-release

clean-debug:
	@$(MAKE) clean2 B=$(BD)

clean-release:
	@$(MAKE) clean2 B=$(BR)

clean2:
	@echo "CLEAN $(B)"
	@rm -f $(OBJ)
	@rm -f $(TARGETS)

distclean: clean
	@rm -rf $(BUILD_DIR)

#############################################################################
# DEPENDENCIES
#############################################################################

.PHONY: all clean clean2 clean-debug clean-release copyfiles \
	debug default distclean makedirs release targets
