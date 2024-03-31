#include <CUnit/Basic.h>
#include "../inc/enigma.h"

void charCasingTest() {
    Enigma *enigma = enigmaInit("AB cd eF Gh ij KL");
    char expPLugboardSubstitue[] = {1, 0, 3, 2, 5, 4, 7, 6, 9, 8, 11, 10, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26};
    for(int i = 0; i < ALPHABET_SIZE; i++) {
        CU_ASSERT_EQUAL(enigma->plugboardSubstitute[i], expPLugboardSubstitue[i]);
    }
    enigmaFree(enigma);
}

void onlyAlphaTest() {
    Enigma *enigma;

    enigma = enigmaInit("A0 1B 23 c4 5d 76");
    CU_ASSERT_PTR_NULL(enigma);
    enigmaFree(enigma);

    enigma = enigmaInit("A@ |b _C !? ^* &)");
    CU_ASSERT_PTR_NULL(enigma);
    enigmaFree(enigma);
}

void connectionsCountTest() {
    Enigma *enigma = enigmaInit("AB CD EF GH IJ KL MN");
    CU_ASSERT_PTR_NULL(enigma);
    enigmaFree(enigma);

    enigma = enigmaInit("AB CD EF");
    CU_ASSERT_PTR_NULL(enigma);
    enigmaFree(enigma);

    enigma = enigmaInit("");
    CU_ASSERT_PTR_NULL(enigma);
    enigmaFree(enigma);
}

void repeatedConnectionsTest() {
    Enigma *enigma = enigmaInit("AB CD EF GH IJ KL MN");
    CU_ASSERT_PTR_NULL(enigma);
    enigmaFree(enigma);

    enigma = enigmaInit("AB CD BE FG AH IH");
    CU_ASSERT_PTR_NULL(enigma);
    enigmaFree(enigma);
}

void multiConnectionsTest(){
    Enigma *enigma = enigmaInit("AB CDE FG HI JK LM");
    CU_ASSERT_PTR_NULL(enigma);
    enigmaFree(enigma);
}

CU_ErrorCode plugboardInitSuiteFunction() {
    CU_pSuite plugboardInitSuite = NULL;
	plugboardInitSuite = CU_add_suite("Plugboard initialization", NULL, NULL);
	if(plugboardInitSuite == NULL) {
		CU_cleanup_registry();
		return CU_get_error();
	}

    if(CU_add_test(plugboardInitSuite, "Character casing", charCasingTest) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if(CU_add_test(plugboardInitSuite, "Only alpha", onlyAlphaTest) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if(CU_add_test(plugboardInitSuite, "Connections count", connectionsCountTest) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if(CU_add_test(plugboardInitSuite, "Repeated connections", repeatedConnectionsTest) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if(CU_add_test(plugboardInitSuite, "Multi connections", multiConnectionsTest) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    return CU_get_error();
}
