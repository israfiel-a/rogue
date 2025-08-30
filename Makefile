.PHONY: all clean prep install export_commands

###############################################################################
## Figure out the directory and dependency structure of the project build.
###############################################################################

BUILD:=$(abspath build)
SOURCE:=$(abspath src)
INCLUDE:=$(abspath include)
ASSETS_NAME:=rss
ASSETS:=$(abspath $(ASSETS_NAME)) 

PUBLIC_DIR=$(PREFIX)/include/waterlily
LIB_DIR=$(PREFIX)/lib
CONFIG_DIR=$(PREFIX)/share/pkgconfig
DEPENDENCIES=vulkan wayland-client xkbcommon

###############################################################################
## Define the project's output files.
###############################################################################

OBJECTS:=entry.o 
OUTPUTS:=$(foreach obj, $(OBJECTS), $(addprefix $(BUILD)/, $(obj)))

EXECUTABLE_NAME:=rogue
EXECUTABLE:=$(BUILD)/$(EXECUTABLE_NAME)

COMPILEDB:=$(BUILD)/compile_commands.json

###############################################################################
## Define the project's setup tasks.
###############################################################################

all: waterlily-update $(EXECUTABLE) $(COMPILEDB) 

$(BUILD)/waterlily:
	git clone https://github.com/israfiel-a/waterlily.git $@

waterlily-update: $(BUILD)/waterlily
	cd $(BUILD)/waterlily && git pull
	cd $(BUILD)/waterlily && $(MAKE) debug
	export PKG_CONFIG_PATH=$(BUILD)/waterlily/build

clean:
	$(RM) -rf $(BUILD)

$(COMPILEDB):
	$(if $(strip $(GENERATING)),,$\
		$(if $(shell command -v compiledb),$\
			compiledb -n -o $(COMPILEDB) $(MAKE) debug GENERATING=on,$\
		)$\
	)

###############################################################################
## Define the project's build tasks.
###############################################################################

$(EXECUTABLE): $(OUTPUTS) | $(BUILD)/$(ASSETS_NAME)
	$(CC) $(OUTPUTS) -o $(EXECUTABLE) $(shell pkg-config --cflags --libs waterlily)

$(BUILD)/%.o: $(SOURCE)/%.c | $(BUILD) 
	$(CC) -c $< -DFILENAME=\"$(notdir $<)\" -o $@ $(shell pkg-config --cflags waterlily) 

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/$(ASSETS_NAME): | $(BUILD)
	cp -r $(ASSETS) $(BUILD)

