#include <CUnit/Basic.h>
#include "../inc/enigma.h"
#include "../inc/enigmaErrors.h"

void rotorSubTest() {
    Enigma *enigma;
    enigma = enigmaInit("AB CD EF GH IJ KL", "III II I", "A A A", "A A A");
    CU_ASSERT_PTR_NOT_NULL(enigma);

    char expRotorIIISub[] = {1, 3, 5, 7, 9, 11, 2, 15, 17, 19, 23, 21, 25, 13, 24, 4, 8, 22, 6, 0, 10, 12, 20, 18, 16, 14};
    char expRotorIIIinverseSub[] = {19, 0, 6, 1, 15, 2, 18, 3, 16, 4, 20, 5, 21, 13, 25, 7, 24, 8, 23, 9, 22, 11, 17, 10, 14, 12};

    char expRotorIISub[] = {0, 9, 3, 10, 18, 8, 17, 20, 23, 1, 11, 7, 22, 19, 12, 2, 16, 6, 25, 13, 15, 24, 5, 21, 14, 4};
    char expRotorIIinverseSub[] = {0, 9, 15, 2, 25, 22, 17, 11, 5, 1, 3, 10, 14, 19, 24, 20, 16, 6, 4, 13, 7, 23, 12, 8, 21, 18};

    const int rotorI[] = {4, 10, 12, 5, 11, 6, 3, 16, 21, 25, 13, 19, 14, 22, 24, 7, 23, 20, 18, 15, 0, 8, 1, 17, 2, 9};
    const int rotorIinverse[] = {20, 22, 24, 6, 0, 3, 5, 15, 21, 25, 1, 4, 2, 10, 12, 19, 7, 23, 18, 11, 17, 8, 13, 16, 14, 9};


 for(int j = 0; j < ROTOR_COUNT; j++) {
        for(int i = 0; i < ALPHABET_SIZE; i++) {
            switch (j) {
                case 0:
                    CU_ASSERT_EQUAL(enigma->rotors[j].rotorSubstitute[i], expRotorIIISub[i]);
                    CU_ASSERT_EQUAL(enigma->rotors[j].rotorInverseSubstitute[i], expRotorIIIinverseSub[i]);
                    break;
                case 1:
                    CU_ASSERT_EQUAL(enigma->rotors[j].rotorSubstitute[i], expRotorIISub[i]);
                    CU_ASSERT_EQUAL(enigma->rotors[j].rotorInverseSubstitute[i], expRotorIIinverseSub[i]);
                    break;
                case 2:
                  CU_ASSERT_EQUAL(enigma->rotors[j].rotorSubstitute[i], rotorI[i]);
                  CU_ASSERT_EQUAL(enigma->rotors[j].rotorInverseSubstitute[i], rotorIinverse[i]);
                    break;
            }
        }
    }

    enigmaFree(enigma);
}

void rotorPositionTest(){
Enigma *enigma;

  enigma = enigmaInit("AB CD EF GH IJ KL", "I II III", "1 A2 ?", "A A A");
    CU_ASSERT_PTR_NULL(enigma);
    CU_ASSERT_EQUAL(getLastEnigmaError(), ENIGMA_ROTORS_INVALID_ROTOR_POSITION);


 enigma = enigmaInit("AB CD EF GH IJ KL", "I II III", "ABD AS AAAA", "A A A");
    CU_ASSERT_PTR_NULL(enigma);
    CU_ASSERT_EQUAL(getLastEnigmaError(), ENIGMA_ROTORS_INVALID_ROTOR_POSITION);
}


void ringPositionTest(){
Enigma *enigma;

  enigma = enigmaInit("AB CD EF GH IJ KL", "I II III", "A A A", "1 A2 ?");
    CU_ASSERT_PTR_NULL(enigma);
    CU_ASSERT_EQUAL(getLastEnigmaError(), ENIGMA_ROTORS_INVALID_RING_POSITION);


 enigma = enigmaInit("AB CD EF GH IJ KL", "I II III", "A A A", "ABD AS AAAA");
    CU_ASSERT_PTR_NULL(enigma);
    CU_ASSERT_EQUAL(getLastEnigmaError(), ENIGMA_ROTORS_INVALID_RING_POSITION);


}

void numOfConnections(){
Enigma *enigma;

enigma = enigmaInit("AB CD EF GH IJ KL", "I II III V", "A A A", "A A A");
    CU_ASSERT_PTR_NULL(enigma);
    CU_ASSERT_EQUAL(getLastEnigmaError(), ENIGMA_ROTORS_TOO_MANY_ROTORS);

enigma = enigmaInit("AB CD EF GH IJ KL", "I II", "A A A", "A A A");
    CU_ASSERT_PTR_NULL(enigma);
    CU_ASSERT_EQUAL(getLastEnigmaError(), ENIGMA_ROTORS_TOO_FEW_ROTORS);


enigma = enigmaInit("AB CD EF GH IJ KL", "I II III V", "A A", "A A A");
    CU_ASSERT_PTR_NULL(enigma);
    CU_ASSERT_EQUAL(getLastEnigmaError(), ENIGMA_ROTORS_TOO_FEW_ROTOR_POSITIONS);

enigma = enigmaInit("AB CD EF GH IJ KL", "I II III V", "A A A", "A A");
    CU_ASSERT_PTR_NULL(enigma);
    CU_ASSERT_EQUAL(getLastEnigmaError(), ENIGMA_ROTORS_TOO_FEW_RING_POSITIONS);

}

void rotorNameTest(){
Enigma *enigma;

enigma = enigmaInit("AB CD EF GH IJ KL", "I X D", "A A A", "A A A");
    CU_ASSERT_PTR_NULL(enigma);
    CU_ASSERT_EQUAL(getLastEnigmaError(), ENIGMA_ROTORS_ROTOR_NAME_NOT_FOUND);

enigma = enigmaInit("AB CD EF GH IJ KL", "I V I", "A A A", "A A A");
    CU_ASSERT_PTR_NULL(enigma);
    CU_ASSERT_EQUAL(getLastEnigmaError(), ENIGMA_ROTORS_ROTOR_ALREADY_USED);


}


CU_ErrorCode rotorsInitSuiteFunction() {
    CU_pSuite rotorsInitSuite = NULL;
    rotorsInitSuite = CU_add_suite("Rotors Initialization", NULL, NULL);

    if(rotorsInitSuite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if(CU_add_test(rotorsInitSuite, "Rotor Substitution", rotorSubTest) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();

    }

    
    if(CU_add_test(rotorsInitSuite, "Rotor Position", rotorPositionTest) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();

    }


    if(CU_add_test(rotorsInitSuite, "Ring Position", ringPositionTest) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();

    }

       if(CU_add_test(rotorsInitSuite, "Connections Count", numOfConnections) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();

    }



       if(CU_add_test(rotorsInitSuite, "Rotor Name", rotorNameTest) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();

    }

    return CU_get_error();

}