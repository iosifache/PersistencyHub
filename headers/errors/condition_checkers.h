#ifndef _CONDITION_CHECKERS

#define _CONDITION_CHECKERS

#pragma region ConditionCheckers

	/**
	 * @brief Construction that checks a value and, if true, return an error code
	 * 
	 */
	#define        RET_CONDITION_CHECKER(condition, error_code) \
	                                                 if (condition) return error_code;

	/**
	 * @brief Construction that checks a value and, if true, goes to an error label
	 * 
	 */
	#define        GOTO_CONDITION_CHECKER(condition, error_label) \
	                                                 if (condition) goto error_label;

#pragma endregion

#endif
