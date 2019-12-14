#ifndef _HELPERS

#define _HELPERS

#pragma region RequiredLibraries

	#include "platform_dependent.h"

#pragma endregion

#pragma region Enumerations

	typedef enum{
		ARCHITECTURE_BITS = 0,
		ROOT_CHECK = 1
	} SYSTEM_DETAILS;

#pragma endregion

#pragma region ExportedFunctions

	#pragma region System

	/**	
	 * @brief Get details about system
	 * 
	 * @param detail The specifier about what information is needed
	 * @param result The location where the result is stored
	 * @return int Zero if success, non-zero if error
	 */
	int get_system_details(SYSTEM_DETAILS detail, void *result);

	/**
	 * @brief Get the home path of the current user
	 * 
	 * @param path The path that will be initialised with the user path
	 * @return int Zero if success, non-zero if error
	 */
	int get_user_home_directory(char **path);

	#pragma endregion

	#pragma region SharedLibraries

	/**
	 * @brief Load a library in the process memory
	 * 
	 * @param lib_name The name of the library that need to be loaded
	 * @param handle The platform-independend handle
	 * @return int Zero if success, non-zero if error
	 */
	int load_library(const char *lib_name, void **handle);

	/**
	 * @brief Get a pointer to a function contained by the library
	 * 
	 * @param handle The platform-independend handle
	 * @param func The name of the function that need to be referenced
	 * @return int Zero if success, non-zero if error
	 */
	int get_function_pointer(void *handle, const char *func_name, FUNC_PTR *func_ptr);

	/**
	 * @brief Close a handle to a library
	 * 
	 * @param handle The platform-independend handle
	 * @return int Zero if success, non-zero if error
	 */
	int unlink_library(void **handle);

	#pragma endregion

	#pragma region FileSystem

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
	 * @brief Get the parent folder path
	 * 
	 * @param file_path Full path of a folder
	 * @return int Zero if success, non-zero if error
	 */
	int get_parent_folder_path(char *file_path);

	/**
	 * @brief Change the working directory of the executable
	 * 
	 * @param path The new working directory
	 * @return int Zero if success, non-zero if error
	 */
	int change_working_directory(const char *path);

	/**
	 * @brief Get the full path to a file
	 * 
	 * @param full_path The location where the full path will be saved
	 * @param parent_folder The parent folder of the file
	 * @param filename The target filename
	 * @return int Zero if success, non-zero if error
	 */ 
	int concat_path_with_filename(char **full_path, const char *parent_folder, const char *filename);

	/**
	 * @brief Check the privilege of a file
	 * 
	 * @param path The path to the file
	 * @param priv The privilege that must be checked out
	 * @return int Negative if privilege isn't valid or posivive(and zero) if is valid
	 */
	int has_privilege(const char *path, PRIVILEGE priv);

	/**
	 * @brief Create a file with a defaut content
	 * 
	 * @param path The path of the file
	 * @param content The default content of the file
	 * @return int Zero if success, non-zero if error
	 */
	int create_file(const char *path, const char *content);

	/**
	 * @brief Get the content of a file
	 * 
	 * @param path The path of the file
	 * @param buffer The buffer that will be allocated, containing the content
	 * @return int Zero if success, non-zero if error
	 */
	int get_file_content(const char *path, char **buffer);

	#pragma endregion

	#pragma region StringOperations

	/**
	 * @brief Copy a string in other location
	 * 
	 * @param dest Location that will be allocated and where the string will be stored
	 * @param src Source string
	 * @param surplus Number of bytes that will be allocated besides to the needed memory
	 * @return int Zero if success, non-zero if error
	 */
	int copy_string(char **dest, const char *src, int surplus);

	/**
	 * @brief Remove all occurences of a string
	 * 
	 * @param string The string in which the search will be made
	 * @param find Searched string
	 * @return int Zero if success, non-zero if error
	 */
	int remove_all_substring_occurences(char *string, const char *find);

	#pragma endregion

#pragma endregion

#endif