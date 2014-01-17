.PHONY: all clean binclean, resclean, bundle
.DEFAULT: all
.SUFFIXES: .nib .xib

# This project was created since Xcode2 times and may produce stupid warnings with some modern compilers,
# made programmers life better. Use 'xcode-select' cmdline tool for switch gcc from Xcode4 to avoid this.

DEVELOPER_DIR=/Developer

TARGET=THE
TARGET_DIR=$(BINDIR)/$(TARGET).app
TARGET_CONTENTS_DIR=$(TARGET_DIR)/Contents
TARGET_RESOURCES_DIR=$(TARGET_CONTENTS_DIR)/Resources

RESDIR=Resources
SRCDIR=Sources
BINDIR=build

CONFIG:=Release

ifeq ($(DEBUG),1)
	CONFIG:=Debug
endif

FRAMEWORKS:=\
	Foundation Cocoa OpenGL GLUT

SRCDIRS=$(shell find $(SRCDIR) -type d -mindepth 0 -maxdepth 9 -print)
RESDIRS=$(shell find $(RESDIR) -type d -mindepth 0 -maxdepth 9 -print)

QUOTED=$(addprefix -iquote , $(SRCDIRS))

SRCEXT:=c

SRCLIST=$(shell find $(SRCDIRS) -type f -name *.$(SRCEXT) -mindepth 1 -maxdepth 1 -print)
IMGLIST=$(shell find $(RESDIRS) -type f -name *.$(RESEXT) -mindepth 1 -maxdepth 1 -print)
NIBLIST=$(shell find $(RESDIRS) -type d -name *.nib -mindepth 1 -maxdepth 1 -print)
XIBLIST=$(shell find $(RESDIRS) -type f -name *.xib -mindepth 1 -maxdepth 1 -print)

SOURCES:=$(SRCLIST)

override SRCEXT=m
SOURCES+=$(SRCLIST)

override SRCEXT=cpp
SOURCES+=$(SRCLIST)

override SRCEXT=mm
SOURCES+=$(SRCLIST)

OBJDIR:=$(BINDIR)
OBJ:=$(addprefix $(OBJDIR)/,$(addsuffix .o,$(basename $(SOURCES))))

RESEXT=png
RESOURCES+=$(IMGLIST)

RES:=$(addsuffix .picture,$(RESOURCES))

NIBS:=$(addprefix $(TARGET_CONTENTS_DIR)/,$(NIBLIST))
XIBS:=$(addprefix $(TARGET_CONTENTS_DIR)/,$(XIBLIST:.xib=.nib))

LIBS:=$(addprefix -framework ,$(FRAMEWORKS))

MAKEPATH=@mkdir -p $(dir $@)

CC=gcc
CXX=g++
IBTOOL=ibtool

CFLAGS+=-std=gnu99
LDFLAGS+=-L/usr/lib/

COMPILEFLAGS=$(QUOTED) -c $< -o $@
USEFLAGS=$^ -L$(BINDIR) $(LIBS) $(LDFLAGS) -o $(TARGET_CONTENTS_DIR)/MacOS/$@
IBFLAGS=--errors --output-format human-readable-text

ARCHIVEFLAGS=cr $(TARG) $(OBJ)
LIBRARYFLAGS=-dynamiclib -single_module -o $(TARG) -install_name @loader_path/$(LIB)$(TARGET).$(EXTENTION) -Wl,-executable_path,$(BINDIR) $(OBJ) $(LIBSPATH) $(LIBS) $(LDFLAGS) 
BINFLAGS=-o $(TARG) $(OBJ) $(LIBSPATH) $(LIBS) $(LDFLAGS)
PLUGINFLAGS=-dynamiclib -single_module -o $(TARG) $(OBJ) -install_name @executable_path/$(TARGET).$(EXTENTION) -Wl,-executable_path,$(BINDIR) -L$(LIBDIR) -L$(BINDIR) $(LIBS) $(LDFLAGS) -exported_symbols_list $(EXPORTS)
BUNDLEFLAGS=$^ -bundle -L$(BINDIR) $(LIBS) $(LDFLAGS) -o $(TARGET_CONTENTS_DIR)/MacOS/$@

all: bundle $(TARGET) $(NIBS) $(XIBS) $(RES)
	@echo [$(RES)] [$(RESOURCES)]

bundle:
	@echo === [BUNDLE] ===
	@mkdir -p $(TARGET_DIR)
	@mkdir -p $(TARGET_CONTENTS_DIR)
	@mkdir -p $(TARGET_CONTENTS_DIR)/MacOS
	@mkdir -p $(TARGET_RESOURCES_DIR)
	@mkdir -p $(TARGET_RESOURCES_DIR)/English.lproj
	@echo APPL???? > $(TARGET_CONTENTS_DIR)/PkgInfo
	@cp -fv Info.plist $(TARGET_CONTENTS_DIR)
	@cp -fv $(RESDIR)/English.lproj/InfoPlist.strings $(TARGET_RESOURCES_DIR)/English.lproj

clean: binclean resclean
	@rm -rf $(OBJ)

binclean:
	@rm -rf $(TARGET_CONTENTS_DIR)/MacOS/$(TARGET)

resclean:
	@rm -rf $(NIBS)
	@rm -rf $(XIBS)
	@rm -rf $(TARGET_CONTENTS_DIR)/PkgInfo
	@rm -rf $(TARGET_CONTENTS_DIR)/Info.plist
	@rm -rf $(TARGET_RESOURCES_DIR)/English.lproj/InfoPlist.strings
	@rm -rf `find $(TARGET_RESOURCES_DIR) -type f -name *.png -mindepth 1 -maxdepth 9 -print`
	@rm -rf `find $(TARGET_RESOURCES_DIR) -type f -name *.jpg -mindepth 1 -maxdepth 9 -print`
	@rm -rf `find $(TARGET_RESOURCES_DIR) -type f -name *.jpeg -mindepth 1 -maxdepth 9 -print`

stdout:
	@clear
	@echo "[BUILD] $(SRCDIR), $(RESDIR) ==> $(BINDIR)/$(TARGET).app"
	@echo
	@echo SRC: $(SOURCES)
	@echo OBJ: $(OBJ)
	@echo NIB: $(NIBS)
	@echo XIB: $(XIBS)
	@echo RES: $(RESOURCES)
	@echo
	@echo ARCHIVEFLAGS=$(ARCHIVEFLAGS)
	@echo COMPILEFLAGS=$(COMPILEFLAGS)
	@echo LIBRARYFLAGS=$(LIBRARYFLAGS)
	@echo BINFLAGS=$(BINFLAGS)
	@echo PLUGINFLAGS=$(PLUGINFLAGS)
	@echo BUNDLEFLAGS=$(BUNDLEFLAGS)
	@echo REALFLAGS=$(REALFLAGS)
	@echo

$(TARGET): $(OBJ)
	@echo === [LINK] ===
	$(CXX) $(USEFLAGS)

OBJECT=$(addprefix $(OBJDIR)/,$(notdir %.o))

$(OBJECT): %.c
	$(MAKEPATH)
	$(VERB)$(CC) $(CFLAGS) $(COMPILEFLAGS)

$(OBJECT): %.m
	$(MAKEPATH)
	$(VERB)$(CC) $(CFLAGS) $(COMPILEFLAGS)

$(OBJECT): %.mm
	$(MAKEPATH)
	$(VERB)$(CXX) $(CXXFLAGS) $(COMPILEFLAGS)

$(OBJECT): %.cpp
	$(MAKEPATH)
	$(VERB)$(CXX) $(CXXFLAGS) $(COMPILEFLAGS)


IBFILE=$(addprefix $(TARGET_CONTENTS_DIR)/,%.nib)

$(addprefix $(TARGET_CONTENTS_DIR)/,%.nib): %.xib
	@echo === [COMPILE XIBS] ===
	$(MAKEPATH)
	$(IBTOOL) --compile $@ $(IBFLAGS) $<

$(addprefix $(TARGET_CONTENTS_DIR)/,%.nib): %.nib
	@echo === [STRIP NIBS] ===
	$(MAKEPATH)
	$(IBTOOL) --strip $@ $(IBFLAGS) $<

%.picture:
	@echo === [COPY RESOURCES] ===
	$(MAKEPATH)
	@cp -fv "$(basename $@)" "$(addprefix $(TARGET_RESOURCES_DIR)/,$(notdir $(basename $@)))"
