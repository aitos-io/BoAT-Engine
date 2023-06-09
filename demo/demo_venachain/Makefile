# Source and Objects

# Specify demo folder name
DEMO_FOLDER    = demo_venachain

# Source and Objects
CONTRACT_SRC_DIR     			= $(BOAT_BASE_DIR)/BoAT-Engine/demo/$(DEMO_FOLDER)/demo_contract
OBJECTS_DIR          			= $(BOAT_BUILD_DIR)/BoAT-Engine/demo/$(DEMO_FOLDER)
CONTRACT_WASM_OBJECTS_DIR 		= $(BOAT_BUILD_DIR)/BoAT-Engine/demo/$(DEMO_FOLDER)/demo_contract/WASM
CONTRACT_SOLIDITY_OBJECTS_DIR 	= $(BOAT_BUILD_DIR)/BoAT-Engine/demo/$(DEMO_FOLDER)/demo_contract/Solidity
DEMO_SOURCES         			= $(wildcard *.c) 
CONTRACT_SOURCES     			= $(wildcard $(CONTRACT_WASM_OBJECTS_DIR)/*.c) \
								  $(wildcard $(CONTRACT_SOLIDITY_OBJECTS_DIR)/*.c)
CONTRACT_WASM_INCLUDE     		= -I$(BOAT_BUILD_DIR)/BoAT-Engine/demo/$(DEMO_FOLDER)/demo_contract/WASM
CONTRACT_SOLIDITY_INCLUDE     	= -I$(BOAT_BUILD_DIR)/BoAT-Engine/demo/$(DEMO_FOLDER)/demo_contract/Solidity

#dependence libs
DEPENDENCE_LIBS = $(BOAT_LIB_DIR)/libboatengine.a \
				  $(BOAT_LIB_DIR)/libboatvendor.a \
				  -lpthread \
				  $(LINK_LIBS)

.PHONY: all contract_generate
ifneq (,$(VENACHAIN_DEMO_IP))
BOAT_CFLAGS +=  -DVENACHAIN_DEMO_IP=\"$(VENACHAIN_DEMO_IP)\"
endif
all: $(OBJECTS_DIR) contract_generate	
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) $(CONTRACT_WASM_INCLUDE) $(CONTRACT_SOLIDITY_INCLUDE) $(CONTRACT_SOURCES) demo_venachain_mycontract.c \
		-DUSE_ONETIME_WALLET -DUSE_PRIKEY_FORMAT_INTERNAL_GENERATION \
		$(DEPENDENCE_LIBS) -o $(OBJECTS_DIR)/demo_venachain_mycontract_onetime_internalGen  -Wl,-Map,$(OBJECTS_DIR)/demo_venachain_mycontract_onetime_internalGen.map
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) $(CONTRACT_WASM_INCLUDE) $(CONTRACT_SOLIDITY_INCLUDE) $(CONTRACT_SOURCES) demo_venachain_mycontract.c \
		-DUSE_ONETIME_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS \
		$(DEPENDENCE_LIBS) -o $(OBJECTS_DIR)/demo_venachain_mycontract_onetime_pkcs  -Wl,-Map,$(OBJECTS_DIR)/demo_venachain_mycontract_onetime_pkcs.map
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) $(CONTRACT_WASM_INCLUDE) $(CONTRACT_SOLIDITY_INCLUDE) $(CONTRACT_SOURCES) demo_venachain_mycontract.c \
		-DUSE_ONETIME_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE \
		$(DEPENDENCE_LIBS) -o $(OBJECTS_DIR)/demo_venachain_mycontract_onetime_native  -Wl,-Map,$(OBJECTS_DIR)/demo_venachain_mycontract_onetime_native.map
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) $(CONTRACT_WASM_INCLUDE) $(CONTRACT_SOLIDITY_INCLUDE) $(CONTRACT_SOURCES) demo_venachain_mycontract.c \
		-DUSE_CREATE_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_INTERNAL_GENERATION \
		$(DEPENDENCE_LIBS) -o $(OBJECTS_DIR)/demo_venachain_mycontract_create_internalGen  -Wl,-Map,$(OBJECTS_DIR)/demo_venachain_mycontract_create_internalGen.map
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) $(CONTRACT_WASM_INCLUDE) $(CONTRACT_SOLIDITY_INCLUDE) $(CONTRACT_SOURCES) demo_venachain_mycontract.c \
		-DUSE_CREATE_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS \
		$(DEPENDENCE_LIBS) -o $(OBJECTS_DIR)/demo_venachain_mycontract_create_pkcs  -Wl,-Map,$(OBJECTS_DIR)/demo_venachain_mycontract_create_pkcs.map		
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) $(CONTRACT_WASM_INCLUDE) $(CONTRACT_SOLIDITY_INCLUDE) $(CONTRACT_SOURCES) demo_venachain_mycontract.c \
		-DUSE_CREATE_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE \
		$(DEPENDENCE_LIBS) -o $(OBJECTS_DIR)/demo_venachain_mycontract_create_native  -Wl,-Map,$(OBJECTS_DIR)/demo_venachain_mycontract_create_native.map	 

contract_generate:
	make -C $(CONTRACT_SRC_DIR) all

$(OBJECTS_DIR):
	$(BOAT_MKDIR) -p $(OBJECTS_DIR)
	$(BOAT_MKDIR) -p $(OBJECTS_DIR)/demo_contract
	$(BOAT_MKDIR) -p $(OBJECTS_DIR)/demo_contract/WASM
	$(BOAT_MKDIR) -p $(OBJECTS_DIR)/demo_contract/Solidity
	
clean:
	-$(BOAT_RM) $(OBJECTS_DIR)/*
