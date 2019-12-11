#ifndef _ERRORS

#define _ERRORS

#pragma region Errors

	typedef enum{
		ERROR_PROGRAMMING_GENERIC			= 100,
		ERROR_PROGRAMMING_INVALID_ARGUMENT		= ERROR_PROGRAMMING_GENERIC + 1
	} ERROR_PROGRAMMING;

	typedef enum{
		ERROR_OPERAING_SYSTEM_GENERIC			= 200,
		ERROR_OPERAING_SYSTEM_UNABLE_TO_ALLOCATE	= ERROR_OPERAING_SYSTEM_GENERIC + 1,
		ERROR_OPERAING_SYSTEM_UNABLE_TO_OPEN_FOLDER	= ERROR_OPERAING_SYSTEM_GENERIC + 2,
		ERROR_OPERAING_SYSTEM_UNABLE_TO_OPEN_FILE	= ERROR_OPERAING_SYSTEM_GENERIC + 3
	} ERROR_OPERAING_SYSTEM;

#pragma endregion

#endif