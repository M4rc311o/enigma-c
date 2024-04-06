#include <CUnit/Basic.h>
#include <stdlib.h>
#include "../inc/enigma.h"
#include "../inc/enigmaErrors.h"

CU_ErrorCode rotorsInitSuiteFunction() {
    CU_pSuite rotorsInitSuite = NULL;
    rotorsInitSuite = CU_add_suite("Rotors initialization", NULL, NULL);
    if(rotorsInitSuite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Add rotors inti test cases

    return CU_get_error();
}