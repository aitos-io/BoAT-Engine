all: testIT testUT

testIT:
	make -C IT all

testUT:
	make -C UT all

clean:
	make -C IT clean
	make -C UT clean
	-$(BOAT_RM) `$(BOAT_FIND) -name \*.gcda`
	-$(BOAT_RM) `$(BOAT_FIND) -name \*.gcno`
