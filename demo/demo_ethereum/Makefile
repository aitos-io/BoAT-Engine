# Source and Objects

# Specify demo folder name
DEMO_FOLDER    = demo_ethereum

# Source and Objects
CONTRACT_SRC_DIR     = $(BOAT_BASE_DIR)/BoAT-Engine/demo/$(DEMO_FOLDER)/demo_contract
OBJECTS_DIR          = $(BOAT_BUILD_DIR)/BoAT-Engine/demo/$(DEMO_FOLDER)
CONTRACT_OBJECTS_DIR = $(BOAT_BUILD_DIR)/BoAT-Engine/demo/$(DEMO_FOLDER)/demo_contract
DEMO_SOURCES         = $(wildcard *.c) 
CONTRACT_SOURCES     = $(wildcard $(CONTRACT_OBJECTS_DIR)/*.c)
CONTRACT_INCLUDE     = -I$(BOAT_BUILD_DIR)/BoAT-Engine/demo/$(DEMO_FOLDER)/demo_contract

#dependence libs
DEPENDENCE_LIBS = $(BOAT_LIB_DIR)/libboatengine.a \
				  $(BOAT_LIB_DIR)/libboatvendor.a \
				  $(LINK_LIBS)

ifneq (,$(ETHEREUM_DEMO_URL))
BOAT_CFLAGS += -DETHEREUM_DEMO_URL=\"$(ETHEREUM_DEMO_URL)\"
endif

.PHONY: all contract_generate
all: $(OBJECTS_DIR) contract_generate
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) $(CONTRACT_INCLUDE) $(CONTRACT_SOURCES) demo_ethereum_transfer.c \
		-DUSE_ONETIME_WALLET -DUSE_PRIKEY_FORMAT_INTERNAL_GENERATION \
		$(DEPENDENCE_LIBS) -o $(OBJECTS_DIR)/demo_ethereum_transfer_onetime_internalGen
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) $(CONTRACT_INCLUDE) $(CONTRACT_SOURCES) demo_ethereum_transfer.c \
		-DUSE_ONETIME_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS \
		$(DEPENDENCE_LIBS) -o $(OBJECTS_DIR)/demo_ethereum_transfer_onetime_pkcs
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) $(CONTRACT_INCLUDE) $(CONTRACT_SOURCES) demo_ethereum_transfer.c \
		-DUSE_ONETIME_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE \
		$(DEPENDENCE_LIBS) -o $(OBJECTS_DIR)/demo_ethereum_transfer_onetime_native
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) $(CONTRACT_INCLUDE) $(CONTRACT_SOURCES) demo_ethereum_transfer.c \
		-DUSE_CREATE_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_INTERNAL_GENERATION \
		$(DEPENDENCE_LIBS) -o $(OBJECTS_DIR)/demo_ethereum_transfer_create_internalGen
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) $(CONTRACT_INCLUDE) $(CONTRACT_SOURCES) demo_ethereum_transfer.c \
		-DUSE_CREATE_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS \
		$(DEPENDENCE_LIBS) -o $(OBJECTS_DIR)/demo_ethereum_transfer_create_pkcs		
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) $(CONTRACT_INCLUDE) $(CONTRACT_SOURCES) demo_ethereum_transfer.c \
		-DUSE_CREATE_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE \
		$(DEPENDENCE_LIBS) -o $(OBJECTS_DIR)/demo_ethereum_transfer_create_native

	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) $(CONTRACT_INCLUDE) $(CONTRACT_SOURCES) demo_ethereum_storeread.c \
		-DUSE_ONETIME_WALLET -DUSE_PRIKEY_FORMAT_INTERNAL_GENERATION \
		$(DEPENDENCE_LIBS) -o $(OBJECTS_DIR)/demo_ethereum_storeread_onetime_internalGen
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) $(CONTRACT_INCLUDE) $(CONTRACT_SOURCES) demo_ethereum_storeread.c \
		-DUSE_ONETIME_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS \
		$(DEPENDENCE_LIBS) -o $(OBJECTS_DIR)/demo_ethereum_storeread_onetime_pkcs
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) $(CONTRACT_INCLUDE) $(CONTRACT_SOURCES) demo_ethereum_storeread.c \
		-DUSE_ONETIME_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE \
		$(DEPENDENCE_LIBS) -o $(OBJECTS_DIR)/demo_ethereum_storeread_onetime_native
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) $(CONTRACT_INCLUDE) $(CONTRACT_SOURCES) demo_ethereum_storeread.c \
		-DUSE_CREATE_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_INTERNAL_GENERATION \
		$(DEPENDENCE_LIBS) -o $(OBJECTS_DIR)/demo_ethereum_storeread_create_internalGen
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) $(CONTRACT_INCLUDE) $(CONTRACT_SOURCES) demo_ethereum_storeread.c \
		-DUSE_CREATE_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS \
		$(DEPENDENCE_LIBS) -o $(OBJECTS_DIR)/demo_ethereum_storeread_create_pkcs		
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) $(CONTRACT_INCLUDE) $(CONTRACT_SOURCES) demo_ethereum_storeread.c \
		-DUSE_CREATE_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE \
		$(DEPENDENCE_LIBS) -o $(OBJECTS_DIR)/demo_ethereum_storeread_create_native	 

contract_generate:
	make -C $(CONTRACT_SRC_DIR) all

$(OBJECTS_DIR):
	$(BOAT_MKDIR) -p $(OBJECTS_DIR)
	$(BOAT_MKDIR) -p $(OBJECTS_DIR)/demo_contract
	
clean:
	-$(BOAT_RM) $(OBJECTS_DIR)/*
