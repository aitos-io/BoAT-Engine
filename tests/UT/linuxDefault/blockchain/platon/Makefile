# Source and Objects

SOURCES = $(wildcard *.c)
OBJECTS_DIR = $(BOAT_BUILD_DIR)/BoAT-Engine/tests/UT/linuxDefault/blockchain/platon/lib
EXECUTABLE_DIR  = $(BOAT_BUILD_DIR)/BoAT-Engine/tests/UT/linuxDefault/blockchain/platon
OBJECTS = $(patsubst %.c,$(OBJECTS_DIR)/%.o,$(SOURCES))

DEPENDENCE_LIBS = $(BOAT_LIB_DIR)/libboatengine.a \
				  $(BOAT_LIB_DIR)/libboatvendor.a \
				  $(LINK_LIBS) \
				  -lcheck\
				  -lpthread\
				  -lm\
				  -lrt\
				  -lsubunit
ifneq (,$(PLATON_NODE_URL))
BOAT_CFLAGS +=  -DTEST_PLATON_NODE_URL=\"$(PLATON_NODE_URL)\"
endif
ifneq (,$(KEY_TYPE))
BOAT_CFLAGS +=  -DTEST_KEY_TYPE=\"$(KEY_TYPE)\"
endif


all: $(OBJECTS_DIR) $(OBJECTS)	
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) $(OBJECTS) $(DEPENDENCE_LIBS) -o $(EXECUTABLE_DIR)/boattest
	$(BOAT_COPY) -rf pri_key $(EXECUTABLE_DIR)

$(OBJECTS_DIR):
	$(BOAT_MKDIR) -p $(OBJECTS_DIR)

$(OBJECTS_DIR)/%.o:%.c
	$(CC) -c $(BOAT_CFLAGS) $< -o $@

clean:
	-$(BOAT_RM) $(OBJECTS)
	-$(BOAT_RM) $(OBJECTS_DIR)
