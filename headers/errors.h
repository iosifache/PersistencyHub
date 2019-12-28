#ifndef _ERRORS

#define _ERRORS

#pragma region Configuration

	#define                 SIGN                                -

#pragma endregion

#pragma region Errors

	typedef enum{
		ERROR_PROGRAMMING_GENERIC			= SIGN 100,
		ERROR_PROGRAMMING_INVALID_ARGUMENT		= ERROR_PROGRAMMING_GENERIC SIGN 1,
		ERROR_PROGRAMMING_OPERATION_NOT_COMPLETED	= ERROR_PROGRAMMING_GENERIC SIGN 2
	} ERROR_PROGRAMMING;

	typedef enum{
		ERROR_OPERAING_SYSTEM_GENERIC			= SIGN 200,
		ERROR_OPERAING_SYSTEM_UNABLE_TO_ALLOCATE	= ERROR_OPERAING_SYSTEM_GENERIC SIGN 1,
		ERROR_OPERAING_SYSTEM_UNABLE_TO_OPEN_FOLDER	= ERROR_OPERAING_SYSTEM_GENERIC SIGN 2,
		ERROR_OPERAING_SYSTEM_UNABLE_TO_OPEN_FILE	= ERROR_OPERAING_SYSTEM_GENERIC SIGN 3,
		ERROR_OPERAING_SYSTEM_UNABLE_TO_LOAD_LIB	= ERROR_OPERAING_SYSTEM_GENERIC SIGN 4,
		ERROR_OPERAING_SYSTEM_UNABLE_TO_FIND_FUNC	= ERROR_OPERAING_SYSTEM_GENERIC SIGN 5,
		ERROR_OPERAING_SYSTEM_UNABLE_TO_GET_INFO	= ERROR_OPERAING_SYSTEM_GENERIC SIGN 6,
		ERROR_OPERAING_SYSTEM_UNABLE_TO_RUN_COMMAND	= ERROR_OPERAING_SYSTEM_GENERIC SIGN 7
	} ERROR_OPERAING_SYSTEM;

	typedef enum{
		ERROR_MODULE_MANAGER_GENERIC			= SIGN 300,
		ERROR_MODULE_MANAGER_NOT_FOUND			= ERROR_MODULE_MANAGER_GENERIC SIGN 1
	} ERROR_MODULE_MANAGER;

	typedef enum{
		ERROR_MODULE_GENERIC				= SIGN 400,
		ERROR_MODULE_LOADING_FAILED			= ERROR_MODULE_GENERIC SIGN 1,
		ERROR_MODULE_NOT_EXPLOITABLE			= ERROR_MODULE_GENERIC SIGN 2
	} ERROR_MODULE;

#pragma endregion

#endif