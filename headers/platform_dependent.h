#ifndef _PLATFORM_DEPENDENT

#define _PLATFORM_DEPENDENT

#pragma region RequiredLibraries

	#include <stdbool.h>

#pragma endregion

#pragma region Configuration

	#define                 MAX_FILES_IN_FOLDER                 16
	#define                 DT_REG                              8

#pragma endregion

#pragma region ExportedFunctions

	/**
	 * @brief Save all filenames from the provided path inside a array of strings
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

#pragma endregion

#endif