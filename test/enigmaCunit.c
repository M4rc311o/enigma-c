#include <CUnit/Basic.h>
#include <stdlib.h>
#include <stdio.h>

CU_ErrorCode plugboardInitSuiteFunction();
CU_ErrorCode rotorsInitSuiteFunction();
CU_ErrorCode rotorsRotateSuiteFunction();

int main() {
	CU_initialize_registry();
	if(CU_initialize_registry() != CUE_SUCCESS) return CU_get_error();

	if(plugboardInitSuiteFunction() != CUE_SUCCESS) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	if(rotorsInitSuiteFunction() != CUE_SUCCESS) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	if(rotorsRotateSuiteFunction() != CUE_SUCCESS) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();

	return CU_get_error();
}
