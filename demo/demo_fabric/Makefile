# Source and Objects

# Specify demo folder name
DEMO_FOLDER    = demo_fabric

# Source and Objects
OBJECTS_DIR          = $(BOAT_BUILD_DIR)/BoAT-Engine/demo/$(DEMO_FOLDER)
DEMO_SOURCES         = $(wildcard *.c) 

#dependence libs
DEPENDENCE_LIBS = $(BOAT_LIB_DIR)/libboatengine.a \
				  $(BOAT_LIB_DIR)/libboatvendor.a \
				  $(LINK_LIBS)
ifneq (,$(FABRIC_DEMO_IP))
BOAT_CFLAGS +=  -DFABRIC_DEMO_IP=\"$(FABRIC_DEMO_IP)\"
endif
all: $(OBJECTS_DIR)
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) demo_fabric_abac.c \
	    -DUSE_ONETIME_WALLET \
		$(DEPENDENCE_LIBS) -o $(OBJECTS_DIR)/demo_fabric_abac_onetime	 -Wl,-Map,$(OBJECTS_DIR)/demo_fabric_abac_onetime.map
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) demo_fabric_abac.c \
	    -DUSE_CREATE_PERSIST_WALLET \
		$(DEPENDENCE_LIBS) -o $(OBJECTS_DIR)/demo_fabric_abac_persist	 -Wl,-Map,$(OBJECTS_DIR)/demo_fabric_abac_persist.map		

$(OBJECTS_DIR):
	$(BOAT_MKDIR) -p $(OBJECTS_DIR)
	
clean:
	-$(BOAT_RM) $(OBJECTS_DIR)/*
