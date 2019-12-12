#ifndef _PLATFORM_DEPENDENT

#define _PLATFORM_DEPENDENT

#pragma region RequiredLibraries

	#include <stdbool.h>

#pragma endregion

#pragma region Configuration

	#define                 MAX_FILES_IN_FOLDER                 16
	#define                 DT_REG                              8

#pragma endregion

#pragma region TypeDefs

	typedef                 int (*FUNC_PTR)(void);

#pragma endregion

#pragma region ExportedFunctions

	/**
	 * @brief Save all filenames from the provided path inside a array of strings, platform-dependent
	 * 
	 * @param path_to_folder The path to the folder
	 * @param file_count The number of finded files
	 * @param filenames Array containing all the filenames, max MAX_FILES_IN_FOLDER
	 * @return int Zero if success, non-zero if error
	 */
	int _save_all_filenames(const char *path_to_folder, int *file_count, char **filenames);

	/**
	 * @brief Get the number of bits of the current operating system, platform-dependent
	 * 
	 * @return int Number of bits of the current operating system
	 */
	int _get_architecture_bits();

	/**
	 * @brief Check if the current program is runned by a root user, platform-dependent
	 * 
	 * @return int Zero if success, non-zero if error
	 */
	bool _check_root();

	/**
	 * @brief Load a library in the process memory, platform-dependent
	 * 
	 * @param lib_name The name of the library that need to be loaded
	 * @param handle The platform-independend handle
	 * @return int Zero if success, non-zero if error
	 */
	int _load_library(const char *lib_name, void **handle);

	/**
	 * @brief Get a pointer to a function contained by the library, platform-dependent
	 * 
	 * @param handle The platform-independend handle
	 * @param func The name of the function that need to be referenced
	 * @return int Zero if success, non-zero if error
	 */
	int _get_function_pointer(void *handle, const char *func_name, FUNC_PTR *func_ptr, char *error);

	/**
	 * @brief Close a handle to a library, platform-dependent
	 * 
	 * @param handle The platform-independend handle
	 * @return int Zero if success, non-zero if error
	 */
	int _close_handle(void **handle);

#pragma endregion

#endif