.PHONY: all clean prep install export_commands

###############################################################################
## Figure out the directory and dependency structure of the project build.
###############################################################################

BUILD:=$(abspath build)
SOURCE:=$(abspath src)
INCLUDE:=$(abspath include)

PUBLIC_DIR=$(PREFIX)/include/waterlily
LIB_DIR=$(PREFIX)/lib
CONFIG_DIR=$(PREFIX)/share/pkgconfig
DEPENDENCIES=vulkan wayland-client xkbcommon

###############################################################################
## Set up the flags we're going to use in all compilations.
###############################################################################

CFLAGS+=-std=gnu2x -Wall -Wextra -Wpedantic -Werror -I$(INCLUDE)
CFLAGS+=$(if $(strip $(DEBUG)),-Og -g3 -ggdb -fanalyzer -fsanitize=leak $\
			-fsanitize=address -fsanitize=pointer-compare $\
			-fsanitize=pointer-subtract -fsanitize=undefined,-march=native $\
			-mtune=native -Ofast -flto)

LDFLAGS+=$(if $(strip $(DEBUG)),-fsanitize=address -fsanitize=undefined $\
		 	-fsanitize=leak,-flto)

###############################################################################
## Define the project's output files.
###############################################################################

OBJECTS:=entry.o 
OUTPUTS:=$(foreach obj, $(OBJECTS), $(addprefix $(BUILD)/, $(obj)))

EXECUTABLE_NAME:=rogue
EXECUTABLE:=$(BUILD)/$(EXECUTABLE_NAME)

###############################################################################
## Define the project's setup tasks.
###############################################################################

EXPORT_COMMAND:=grep -wE '$(CC)'$\
	| grep -w '\-c'$\
	| jq -nR '[inputs|{directory:"$(BUILD)",$\
		command:.,$\
		file:match(" [^ ]+$$").string[1:],$\
		output:"$(BUILD)"+match(" [^ ]+$$").$\
			string[1+("$(SOURCE)"|length):-2]+".o"}]'$\
	> $(BUILD)/compile_commands.json

all: waterlily-update $(EXECUTABLE) 

$(BUILD)/waterlily:
	git clone https://github.com/israfiel-a/waterlily.git $@

waterlily-update: $(BUILD)/waterlily
	cd $(BUILD)/waterlily && git pull
	cd $(BUILD)/waterlily && $(MAKE) install AR=gcc-ar PREFIX=..
	export PKG_CONFIG_PATH=$(BUILD)/share/pkgconfig

clean:
	$(RM) -rf $(BUILD)

export_commands: | $(BUILD) 
	$(MAKE) EXPORT_COMMAND_RUN=on --always-make --dry-run | $(EXPORT_COMMAND) 

###############################################################################
## Define the project's build tasks.
###############################################################################

$(EXECUTABLE): $(OUTPUTS) 
	$(CC) $(OUTPUTS) -o $(EXECUTABLE) $(CFLAGS) $(LDFLAGS) $(shell pkg-config --libs --cflags waterlily) 

$(BUILD)/%.o: $(SOURCE)/%.c | $(BUILD) 
	$(CC) -c $< $(CFLAGS) $(FLAGS) -DFILENAME=\"$(notdir $<)\" -o $@ $(shell pkg-config --libs --cflags waterlily) 

$(BUILD):
	mkdir -p $(BUILD)

