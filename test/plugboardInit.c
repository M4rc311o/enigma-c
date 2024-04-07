#include <CUnit/Basic.h>
#include <stdlib.h>
#include "../inc/enigma.h"
#include "../inc/enigmaErrors.h"

void charCasingTest() {
    Enigma *enigma = enigmaInit("AB cd eF Gh ij KL", "I II III", "A A A", "A A A");
    CU_ASSERT_PTR_NOT_NULL(enigma);
    char expPLugboardSubstitue[] = {1, 0, 3, 2, 5, 4, 7, 6, 9, 8, 11, 10, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25};
    for(int i = 0; i < ALPHABET_SIZE; i++) {
        CU_ASSERT_EQUAL(enigma->plugboard.plugboardSubstitute[i], expPLugboardSubstitue[i]);
    }
    enigmaFree(enigma);
}

void onlyAlphaTest() {
    Enigma *enigma;

    enigma = enigmaInit("A0 1B 23 c4 5d 76", "I II III", "A A A", "A A A");
    CU_ASSERT_PTR_NULL(enigma);
    CU_ASSERT_EQUAL(getLastEnigmaError(), ENIGMA_PLUGBOARD_CONNECTION_NOT_ALPHA);

    enigma = enigmaInit("A@ |b _C !? ^* &)", "I II III", "A A A", "A A A");
    CU_ASSERT_PTR_NULL(enigma);
    CU_ASSERT_EQUAL(getLastEnigmaError(), ENIGMA_PLUGBOARD_CONNECTION_NOT_ALPHA);
}

void connectionsCountTest() {
    Enigma *enigma = enigmaInit("AB CD EF GH IJ KL MN", "I II III", "A A A", "A A A");
    CU_ASSERT_PTR_NULL(enigma);
    CU_ASSERT_EQUAL(getLastEnigmaError(), ENIGMA_PLUGBOARD_TOO_MANY_CONNECTIONS);

    enigma = enigmaInit("AB CD EF", "I II III", "A A A", "A A A");
    CU_ASSERT_PTR_NULL(enigma);
    CU_ASSERT_EQUAL(getLastEnigmaError(), ENIGMA_PLUGBOARD_TOO_FEW_CONNECTIONS);

    enigma = enigmaInit("", "I II III", "A A A", "A A A");
    CU_ASSERT_PTR_NULL(enigma);
    CU_ASSERT_EQUAL(getLastEnigmaError(), ENIGMA_PLUGBOARD_TOO_FEW_CONNECTIONS);
}

void repeatedConnectionsTest() {
    Enigma *enigma = enigmaInit("AA CD EF GH IJ KL MN", "I II III", "A A A", "A A A");
    CU_ASSERT_PTR_NULL(enigma);
    CU_ASSERT_EQUAL(getLastEnigmaError(), ENIGMA_PLUGBOARD_CONNECTION_ALREADY_USED);

    enigma = enigmaInit("AB CD BE FG AH IH", "I II III", "A A A", "A A A");
    CU_ASSERT_PTR_NULL(enigma);
    CU_ASSERT_EQUAL(getLastEnigmaError(), ENIGMA_PLUGBOARD_CONNECTION_ALREADY_USED);
}

void multiConnectionsTest() {
    Enigma *enigma = enigmaInit("AB CDE FG HI JK LM", "I II III", "A A A", "A A A");
    CU_ASSERT_PTR_NULL(enigma);
    CU_ASSERT_EQUAL(getLastEnigmaError(), ENIGMA_PLUGBOARD_WRONG_CONNECTION_LENGTH);
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
