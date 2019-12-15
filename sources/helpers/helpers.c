#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/utsname.h>
#include "../../headers/library/percistency_hub.h"
#include "../../headers/helpers/helpers.h"
#include "../../headers/errors.h"

#pragma region System

int get_architecture_bits(void *result){

	// Assign the corresponding value of the enum
	switch (_get_architecture_bits()){
		case 32:
			*((ARCHITECTURE *)result) = X32;
			break;
		case 64:
			*((ARCHITECTURE *)result) = X64;
			break;
		default:
			*((ARCHITECTURE *)result) = UNKNOWN;
			break;
	}

	// Return
	return 0;

}

int is_root(void *result){

	if (_is_root()){
		*((ROOTED_STATE *)result) = WITH_ROOT;
	}
	else{
		*((ROOTED_STATE *)result) = WITHOUT_ROOT;
	}

	// Return
	return 0;

}

int get_system_details(SYSTEM_DETAILS detail, void *result){

	int ret_val;

	// Determine what information is needed
	switch(detail){
		case ARCHITECTURE_BITS:
			ret_val = get_architecture_bits(result);
			break;
		case ROOT_CHECK:
			ret_val = is_root(result);
			break;
		default:
			result = NULL;
			ret_val = ERROR_PROGRAMMING_INVALID_ARGUMENT;
	}

	// Return
	return ret_val;

}

int get_user_home_directory(char **path){

	// Return
	return _get_user_home_directory(path);

}

int get_executable_path(char **path){

	// Return
	return _get_executable_path(path);

}

#pragma endregion

#pragma region SharedLibraries

int load_library(const char *lib_name, void **handle){

	// Free the handle if exists
	if (*handle != NULL)
		free(*handle);

	// Open the library and check the operation
	_load_library(lib_name, handle);
	if (*handle == NULL)
		return ERROR_OPERAING_SYSTEM_UNABLE_TO_LOAD_LIB;

	// Return
	return 0;

}

int get_function_pointer(void *handle, const char *func_name, FUNC_PTR *func_ptr){

	char *error = NULL;

	// Checl arguments
	if (handle == NULL || func_name == NULL)
		return ERROR_PROGRAMMING_INVALID_ARGUMENT;

	// Get a pointer to function
	_get_function_pointer(handle, func_name, func_ptr, error);
	if (error != NULL)
		return ERROR_OPERAING_SYSTEM_UNABLE_TO_FIND_FUNC;

	// Return
	return 0;

}

int unlink_library(void **handle){

	// Check parameters
	if (*handle == NULL)
		return 0;

	// Close handle
	_unlink_library(handle);

	// Return
	return 0;

}

#pragma endregion

#pragma region FileSystem

int get_all_files_from_folder(const char *path_to_folder, int *file_count, char ***filenames, int max_files){

	int ret_val;

	// Allocate the filenames array
	*filenames = (char **)calloc(1, max_files * sizeof(char *));
	if (*filenames == NULL)
		return ERROR_OPERAING_SYSTEM_UNABLE_TO_ALLOCATE;

	// Open folder
	ret_val = _get_all_files_from_folder(path_to_folder, file_count, *filenames, max_files);

	// Return
	return ret_val;

}

int get_parent_folder_path(char *file_path){

	int i, length;

	length = strlen(file_path);
	for (i = length - 1; i >= 0; i--)
		if (file_path[i] == PATH_SEPARATOR[0]){
			file_path[i] = '\0';
			break;
		}

	// Return
	return 0;

}

int change_working_directory(const char *path){

	// Check parameters
	if (path == NULL)
		return ERROR_PROGRAMMING_INVALID_ARGUMENT;

	return _change_working_directory(path);

}

int concat_path_with_filename(char **full_path, const char *parent_folder, const char *filename){

	// Check parameters
	if (parent_folder == NULL || filename == NULL)
		return ERROR_PROGRAMMING_INVALID_ARGUMENT;

	// Free if already allocated
	if (*full_path != NULL)
		free(*full_path);

	// Construct path
	copy_string(full_path, parent_folder, strlen(filename) + 1);
	strcat(*full_path, PATH_SEPARATOR);
	strcat(*full_path, filename);

	// Return
	return 0;

}

int has_privilege(const char *path, PRIVILEGE priv){

	// Return
	return _has_privilege(path, priv);

}

int create_file(const char *path, const char *content){

	FILE *file;

	// Check arguments
	if (path == NULL || content == NULL)
		return ERROR_PROGRAMMING_INVALID_ARGUMENT;

	// Create file
	file = fopen(path, "w");
	if (file == NULL)
		return ERROR_OPERAING_SYSTEM_UNABLE_TO_OPEN_FILE;

	// Write content and close
	fwrite(content, strlen(content), 1, file);
	fclose(file);

	// Return
	return 1;

}

int get_file_content(const char *path, char **buffer){

	FILE *file;
	int length;

	// Verify arguments
	if (path == NULL)
		return ERROR_PROGRAMMING_INVALID_ARGUMENT;
	if (*buffer != NULL)
		free(*buffer);

	// Open file
	file = fopen(path, "r");
	if (file == NULL)
		return ERROR_OPERAING_SYSTEM_UNABLE_TO_OPEN_FILE;

	// Get file size
	fseek(file, 0, SEEK_END);
	length = ftell(file);

	// Allocate buffer
	*buffer = (char *)malloc((length + 1) * sizeof(char));
	fseek(file, 0, SEEK_SET);
	fread(*buffer, length, 1, file);
	(*buffer)[length] = '\0';

	// Return
	return 0;

}

#pragma endregion

#pragma region StringOperations

int copy_string(char **dest, const char *src, int surplus){

	// Allocate the destination
	if (*dest != NULL)
		free(dest);
	*dest = (char *)calloc(1, (strlen(src) + surplus + 1) * sizeof(char));
	if (dest == NULL)
		return ERROR_OPERAING_SYSTEM_UNABLE_TO_ALLOCATE;

	// Copy string
	strcpy(*dest, src);

	// Return
	return 0;

}

int remove_all_substring_occurences(char *string, const char *find){

	char *index;
	int length;

	if (string == NULL || find == NULL)
		return ERROR_PROGRAMMING_INVALID_ARGUMENT;

	length = strlen(find);
	while (1){

		index = strstr(string, find);
		if (index != NULL){
			strcpy(index, index + length);
		}
		else
			return 0;

	}

}

#pragma endregion

#pragma region Logging

int init_logger(LOGGER **logger, const char *filename){

	// Verify arguments
	if (filename == NULL)
		return ERROR_PROGRAMMING_INVALID_ARGUMENT;

	// Allocate logger
	if (*logger != NULL)
		free(*logger);
	*logger = (LOGGER *)malloc(sizeof(LOGGER));
	if (*logger == NULL)
		return ERROR_OPERAING_SYSTEM_UNABLE_TO_ALLOCATE;

	// Create file
	(*logger)->handle = fopen(filename, "a");
	if ((*logger)->handle == NULL){
		free(*logger);
		return ERROR_OPERAING_SYSTEM_UNABLE_TO_OPEN_FILE;
	}

	// Return
	return 0;

}

int log_message(LOGGER *logger, const char *message){

	time_t now;
	char *formatted_time;

	// Verify arguments
	if (logger == NULL || message == NULL)
		return ERROR_PROGRAMMING_INVALID_ARGUMENT;

	// Get formatted time
	now = time(0);
	formatted_time = ctime(&now);

	// Print in file
	fprintf(logger->handle, "[+] %s: %s\n", formatted_time, message);

	// Return
	return 0;

}

int free_logger(LOGGER **logger){

	if (*logger != NULL){
		free(*logger);
		*logger = NULL;
	}

	// Return
	return 0;

}

#pragma endregion