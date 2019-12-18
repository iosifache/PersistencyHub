#ifndef _MODULE

#pragma RequiredLibraries

	#include "../../headers/library/persistency_hub.h"

#pragma endregion

#pragma region ExportedFunctions

	/**
	 * @brief Check the compatibility of the module
	 * 
	 * @param env The already setted environment
	 * @return int Zero if not compatible, non-zero if compatible
	 */
	extern int is_compatible(ENVIRONMENT *env);

	/**
	 * @brief Install the module
	 * 
	 * @param env The already setted environment
	 * @return int Zero if success, non-zero if error
	 */
	extern int exploit(ENVIRONMENT *env);

	/**
	 * @brief Check if the module is installed
	 * 
	 * @param env The already setted environment
	 * @return int Zero if not present, non-zero if present
	 */
	extern int is_installed(ENVIRONMENT *env);

	/**
	 * @brief Delete an installed module
	 * 
	 * @param env The already setted environment
	 * @return int Zero if success, non-zero if error
	 */
	extern int delete_installed(ENVIRONMENT *env);

#pragma endregion

#endif