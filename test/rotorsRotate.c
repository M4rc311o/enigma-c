#include <CUnit/Basic.h>
#include <stdlib.h>
#include "../inc/enigma.h"
#include "../inc/enigmaErrors.h"

static Enigma *enigma = NULL;

int initSuite() {
    enigma = enigmaInit("AB CD EF GH IJ KL MN", "I II III", "A A A", "A A A");
    if(enigma == NULL) return 1;
    return 0;
}

int cleanSuite() {
    if(enigmaFree(enigma) != ENIGMA_SUCCESS) return 1;
    return 0;
}

CU_ErrorCode rotorsRotateSuiteFunction() {
    CU_pSuite rotorsRotateSuite = NULL;
    rotorsRotateSuite = CU_add_suite("Rotors rotation", initSuite, cleanSuite);
    if(rotorsRotateSuite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Add rotors rotation test cases, testing one step, turnovers and correct looping of positions
    // Currently no exposed function to do these tests

    return CU_get_error();
}
