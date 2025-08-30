.PHONY: all clean prep install export_commands

###############################################################################
## Figure out the directory and dependency structure of the project build.
###############################################################################

BUILD_DIRECTORY:=$(abspath build)
SOURCE:=$(abspath src)
INCLUDE:=$(abspath include)
ASSETS_NAME:=rss
ASSETS:=$(abspath $(ASSETS_NAME)) 

PUBLIC_DIR=$(PREFIX)/include/waterlily
LIB_DIR=$(PREFIX)/lib
CONFIG_DIR=$(PREFIX)/share/pkgconfig
DEPENDENCIES=vulkan wayland-client xkbcommon

WATERLILY_DIRECTORY:=$(BUILD_DIRECTORY)/waterlily

###############################################################################
## Define the project's output files.
###############################################################################

OBJECTS:=entry.o 
OUTPUTS:=$(foreach obj, $(OBJECTS), $(addprefix $(BUILD_DIRECTORY)/, $(obj)))

EXECUTABLE_NAME:=rogue
EXECUTABLE:=$(BUILD_DIRECTORY)/$(EXECUTABLE_NAME)

COMPILEDB:=$(BUILD_DIRECTORY)/compile_commands.json
PKG_CONFIG_PATH:=$(WATERLILY_DIRECTORY)/build
export PKG_CONFIG_PATH

###############################################################################
## Define the project's setup tasks.
###############################################################################

define find_mode
	$(if $(wildcard $(BUILD_DIRECTORY)/$(1).mode),clean,) 
endef
 
all: waterlily-update $(EXECUTABLE)

$(WATERLILY_DIRECTORY):
	git clone https://github.com/israfiel-a/waterlily.git $@

waterlily-update: $(WATERLILY_DIRECTORY)
	cd $(WATERLILY_DIRECTORY) && git pull
	cd $(WATERLILY_DIRECTORY) && $(MAKE) $(MAKECMDGOALS) 

clean:
	$(RM) -rf $(BUILD_DIRECTORY)

debug: $(call find_mode,release) all $(COMPILEDB) $(BUILD_DIRECTORY)/debug.mode
release: $(call find_mode,debug) all $(BUILD_DIRECTORY)/release.mode

$(BUILD_DIRECTORY)/debug.mode:
	touch $(BUILD_DIRECTORY)/debug.mode

$(BUILD_DIRECTORY)/release.mode:
	touch $(BUILD_DIRECTORY)/release.mode

$(COMPILEDB):
	$(if $(strip $(GENERATING)),,$\
		$(if $(shell command -v compiledb),$\
			compiledb -n -o $(COMPILEDB) $(MAKE) debug GENERATING=on,$\
		)$\
	)

###############################################################################
## Define the project's build tasks.
###############################################################################

$(EXECUTABLE): $(OUTPUTS) | $(BUILD_DIRECTORY)/$(ASSETS_NAME)
	$(CC) $(OUTPUTS) -o $(EXECUTABLE) $(shell pkg-config --cflags --libs waterlily)

$(BUILD_DIRECTORY)/%.o: $(SOURCE)/%.c | $(BUILD_DIRECTORY) 
	$(CC) -c $< -DFILENAME=\"$(notdir $<)\" -o $@ $(shell pkg-config --cflags waterlily) 

$(BUILD_DIRECTORY):
	mkdir -p $(BUILD_DIRECTORY)

$(BUILD_DIRECTORY)/$(ASSETS_NAME): | $(BUILD_DIRECTORY)
	cp -r $(ASSETS) $(BUILD_DIRECTORY)

