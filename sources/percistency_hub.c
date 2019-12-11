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