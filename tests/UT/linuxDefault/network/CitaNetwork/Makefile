# Source and Objects

SOURCES = $(wildcard *.c)
OBJECTS_DIR = $(BOAT_BUILD_DIR)/BoAT-Engine/tests/UT/linuxDefault/network/CitaNetwork/lib
EXECUTABLE_DIR  = $(BOAT_BUILD_DIR)/BoAT-Engine/tests/UT/linuxDefault/network/CitaNetwork
OBJECTS = $(patsubst %.c,$(OBJECTS_DIR)/%.o,$(SOURCES))

DEPENDENCE_LIBS = $(BOAT_LIB_DIR)/libboatengine.a \
				  $(BOAT_LIB_DIR)/libboatvendor.a \
				  $(LINK_LIBS) \
				  -lcheck\
				  -lpthread\
				  -lm\
				  -lrt\
				  -lsubunit

all: $(OBJECTS_DIR) $(OBJECTS)	
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) $(OBJECTS) $(DEPENDENCE_LIBS) -o $(EXECUTABLE_DIR)/citaNetworktest

$(OBJECTS_DIR):
	$(BOAT_MKDIR) -p $(OBJECTS_DIR)

$(OBJECTS_DIR)/%.o:%.c
	$(CC) -c $(BOAT_CFLAGS) $< -o $@

clean:
	-$(BOAT_RM) $(OBJECTS)
	-$(BOAT_RM) $(OBJECTS_DIR)
