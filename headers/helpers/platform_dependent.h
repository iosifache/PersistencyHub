#ifndef _PLATFORM_DEPENDENT

#define _PLATFORM_DEPENDENT

#pragma region RequiredLibraries

	#include "../library/persistency_hub.h"

#pragma endregion

#pragma region Configuration

	#define        COMMAND_MAX_SIZE                  128
	#define        LINE_BUFFER_SIZE                  64

	#ifdef __linux__

		#define        PATH_SEPARATOR                    "/"
		#define        MAX_PATH_LENGTH                   64
		#define        DT_REG                            8
		#define        SELF_EXECUTABLE_PATH              "/proc/self/exe"

	#elif _WIN32

		#define        PATH_SEPARATOR                    "\\"

	#else

		#error Platform not supported

	#endif

#pragma endregion

#pragma region Enumerations

	typedef enum{
		WRITE,
		READ,
		EXECUTE
	} PRIVILEGE;

#pragma endregion

#pragma region TypeDefs

	typedef                 int (*FUNC_PTR)(ENVIRONMENT *);

#pragma endregion

#pragma region FunctionDeclarations

	#pragma region System

	/**
	 * @brief Get the number of bits of the current operating system, platform-dependent
	 * 
	 * @return int Number of bits of the current operating system
	 */
	int _get_architecture_bits();

	/**
	 * @brief Check if the current program is runned by a root user, platform-dependent
	 * 
	 * @return int Zero if not root, non-zero if root
	 */
	int _is_root();

	/**
	 * @brief Get the home path of the current user
	 * 
	 * @param path The path that will be initialised with the user path
	 * @return int Zero if success, non-zero if error
	 */
	int _get_user_home_directory(char **path);

	/**
	 * @brief Get the executable path
	 * 
	 * @param buffer The buffer where the location will be stored
	 * @return int Zero if success, non-zero if error
	 */
	int _get_executable_path(char **buffer);

	/**
	 * @brief Execute a command and get its output
	 * 
	 * @param command Command to be executed
	 * @param output The output of the command
	 * @return int Zero if success, non-zero if error
	 */
	int _execute_command(char *command, char *output);

	#pragma endregion

	#pragma region SharedLibraries

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
	int _unlink_library(void **handle);

	#pragma endregion

	#pragma region FileSystem

	/**
	 * @brief Change the working directory of the executable
	 * 
	 * @param path The new working directory
	 * @return int Zero if success, non-zero if error
	 */
	int _change_working_directory(const char *path);

	/**
	 * @brief Save all filenames from the provided path inside a array of strings, platform-dependent
	 * 
	 * @param path_to_folder The path to the folder
	 * @param file_count The number of finded files
	 * @param filenames Array containing all the filenames, max MAX_FILES_IN_FOLDER
	 * @param max_files Maximum number of files in that folder
	 * @param extension The extension of the checked files
	 * @return int Zero if success, non-zero if error
	 */
	int _get_all_files_from_folder(const char *path_to_folder, int *file_count, char **filenames, int max_files, const char *extension);

	/**
	 * @brief Check the privilege of a file
	 * 
	 * @param path The path to the file
	 * @param priv The privilege that must be checked out
	 * @return int Negative if privilege isn't valid or posivive(and zero) if is valid
	 */
	int _has_privilege(const char *path, PRIVILEGE priv);

	#pragma endregion

#pragma endregion

#endif