#ifndef _HELPERS

#define _HELPERS

#pragma region Enumerations

	typedef enum{
		ARCHITECTURE_BITS = 0,
		ROOT_CHECK = 1
	} SYSTEM_DETAILS;

#pragma region ExportedFunctions

	/**
	 * @brief Copy a string in other location
	 * 
	 * @param dest Location that will be allocated and where the string will be stored
	 * @param src Source string
	 * @return int Zero if success, non-zero if error
	 */
	int copy_string(char **dest, const char *src);

	/**
	 * @brief Get the all files from folder
	 * 
	 * @param path_to_folder The path to the folder
	 * @param file_count The number of finded files
	 * @param filenames Array containing all the filenames, max MAX_FILES_IN_FOLDER
	 * @param max_files Maximum number of files in that folder
	 * @return int Zero if success, non-zero if error
	 */
	int get_all_files_from_folder(const char *path_to_folder, int *file_count, char ***filenames, int max_files);

	/**
	 * @brief Get details about system
	 * 
	 * @param detail The specifier about what information is needed
	 * @param result The location where the result is stored
	 * @return int Zero if success, non-zero if error
	 */
	int get_system_details(SYSTEM_DETAILS detail, void *result);

#pragma endregion

#endif