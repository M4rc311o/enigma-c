#include <CUnit/Basic.h>
#include <stdlib.h>
#include "../inc/enigma.h"
#include "../inc/enigmaErrors.h"

void encryptionCharTest() {
    char input[] = {'a', 'B', 'c', 'd', 'E', 'f', 'g', 'H', 'i', 'J', 'k', 'L', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
    char expected[] = {'M', 'F', 'K', 'I', 'Q', 'B', 'J', 'V', 'D', 'G', 'C', 'U', 'A', 'S', 'W', 'T', 'E', 'Z', 'N', 'P', 'L', 'H', 'O', 'Y', 'X', 'R'};

    for(int x = 0;x < (sizeof(input) - 1);x++) {
        Enigma *enigma = enigmaInit("AB CD EF GH IJ KL", "I II III", "A B C", "G H I");
        CU_ASSERT_PTR_NOT_NULL(enigma);
        CU_ASSERT_EQUAL(enigmaEncChar(enigma, input[x]), expected[x]);
        enigmaFree(enigma);
    }
}

void encryptionInvalidChar() {
    Enigma *enigma = enigmaInit("AB CD EF GH IJ KL", "I II III", "A B C", "G H I");
    CU_ASSERT_PTR_NOT_NULL(enigma);
    CU_ASSERT_FALSE(enigmaEncChar(enigma, '%'));
    CU_ASSERT_EQUAL(getLastEnigmaError(), ENIGMA_ENC_CHAR_NOT_ALPHA);
    enigmaFree(enigma);
}

void encryptionStrTest() {
    char input[] = "ITISALONGESTABLISHEDFACTTHATAREADERWILLBEDISTRACTEDBYTHEREADABLECONTENTOFAPAGEWHENLOOKINGATITSLAYOUTTHEPOINTOFUSINGLOREMIPSUMISTHATITHASAMOREORLESSNORMALDISTRIBUTIONOFLETTERSASOPPOSEDTOUSINGCONTENTHERECONTENTHEREMAKINGITLOOKLIKEREADABLEENGLISHMANYDESKTOPPUBLISHINGPACKAGESANDWEBPAGEEDITORSNOWUSELOREMIPSUMASTHEIRDEFAULTMODELTEXTANDASEARCHFORLOREMIPSUMWILLUNCOVERMANYWEBSITESSTILLINTHEIRINFANCYVARIOUSVERSIONSHAVEEVOLVEDOVERTHEYEARSSOMETIMESBYACCIDENTSOMETIMESONPURPOSEINJECTEDHUMOURANDTHELIKEITISALONGESTABLISHEDFACTTHATAREADERWILLBEDISTRACTEDBYTHEREADABLECONTENTOFAPAGEWHENLOOKINGATITSLAYOUTTHEPOINTOFUSINGLOREMIPSUMISTHATITHASAMOREORLESSNORMALDISTRIBUTIONOFLETTERSASOPPOSEDTOUSING";
    char expected[] = "EEFKTVZSHVTAJXSDEXPIQFPZDOVCPLJDFYOQGUIAHEAWNUNVVCXCEYDLKKFHWJNIGFOMZQMVBXDBATYUKZQCYLWVHPEODMDQLRYWHPGBYLYNBKAYJZJYCBMCYDWWABCJAQFZQGQJMJLQWICEMNUGMEPQBGKYCYVEOUJKZQJDVNNBBLBYIEYUWXKKEAIGGBGBEEYGVDMMHFEBIHEEAAUKWIALGEKYMGWVRDAYDXZGNJFYSMPENIUHYQKRVHADDZZPUPURTJMCCULVQAZBLVMRQUHPEJROJBBOGVNBVNKRVGFPFJVPWWMYBGNMWQSOGPJGMUNSGXGWLUCJVSYWHNTGBRWEHDTBAFKKNVQMZOWCRFKGAUITTJWKIQRMKINWYILZEPZKLZXOHMVZCQLFNLSPKSBTIDRGVELFMKKZDRNEYZHGIGJVLARBZSBXMIUGGZTZUOBGKTGHPJJHRDGXJGOYLGUYYPPPIJJLEEORLAJHNRGOARKCPQCPTSCLTTVJRRNVEIHGEVLDZMJHFUBXEFWPVPBVHEWQWDINUQEPESKYRTRSMDJZMUSEOTBPKTZKSIFZBCZQIICLWVOITEGXNXBRPQLDWWNUYQUUWCQOLMBHDGPQEEQGVCTCWATDMUPCAHBQSOKMKWYGDGDITDLEWBBFFNNRXFCGURSKVVRDERICEK";
    char encrypted[1000];
    Enigma *enigma = enigmaInit("QR SX VI OC PD KL", "V II IV", "C X Z", "Z Y X");
    CU_ASSERT_PTR_NOT_NULL(enigma);
    enigmaEncStr(enigma, input, encrypted);
    for(int x = 0;x < sizeof(input);x++) {
        CU_ASSERT_EQUAL(encrypted[x], expected[x]);
    }
    enigmaFree(enigma);
}



CU_ErrorCode encryptionSuiteFunction() {
    CU_pSuite encryptionSuite = NULL;
    encryptionSuite = CU_add_suite("Encryption", NULL, NULL);
    if(encryptionSuite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if(CU_add_test(encryptionSuite, "Character encryption test", encryptionCharTest) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if(CU_add_test(encryptionSuite, "Invalid character encryption test", encryptionInvalidChar) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if(CU_add_test(encryptionSuite, "String encryption test", encryptionStrTest) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    return CU_get_error();
}
