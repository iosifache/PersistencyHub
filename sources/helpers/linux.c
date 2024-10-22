#ifdef __linux__

#pragma region RequiredLibraries

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <dlfcn.h>
	#include <dirent.h>
	#include <unistd.h>
	#include <pwd.h>
	#include <fcntl.h>
	#include <sys/types.h>
	#include <sys/utsname.h>
	#include "../../headers/helpers/platform_dependent.h"
	#include "../../headers/helpers/helpers.h"
	#include "../../headers/errors/codes.h"

#pragma endregion

#pragma region ForwardDeclarations

	ssize_t readlink(const char *pathname, char *buf, size_t bufsiz);

	FILE *popen(const char *command, const char *type);

	int pclose(FILE *stream);

#pragma endregion

#pragma region FunctionDefinitions

#pragma region System

int _get_architecture_bits(){

	return (sizeof(void *) * 8);

}

int _is_root(){

	return (geteuid() == 0);

}

int _get_user_detail(USER_INFO requested, char **storage){

	uid_t uid;
	struct passwd *pw;
	char *info = NULL;
	int ret_val;

	// Get path for UID
	uid = getuid();
	pw = getpwuid(uid);
	if (pw == NULL)
		return ERROR_OPERATING_SYSTEM_UNABLE_TO_GET_INFO;

	// Copy the path and free the allocated structure
	switch (requested){
		case USERNAME:
			info = pw->pw_name;
			break;
		case HOME_PATH:
			info = pw->pw_dir;
			break;
	}
	ret_val = copy_string(storage, info, 0);

	// Return
	return ret_val;

}

int _get_username(char **username){

	return _get_user_detail(USERNAME, username);

}

int _get_user_home_directory(char **path){

	return _get_user_detail(HOME_PATH, path);

}

int _get_executable_path(char **buffer){

	int path_length;

	// Allocate buffer
	*buffer = (char *)calloc(1, MAX_PATH_LENGTH);
	if (*buffer == NULL)
		return ERROR_OPERATING_SYSTEM_UNABLE_TO_ALLOCATE;

	// Get path
	path_length = readlink(SELF_EXECUTABLE_PATH, *buffer, MAX_PATH_LENGTH);
	if (path_length < 0)
		return ERROR_OPERATING_SYSTEM_UNABLE_TO_GET_INFO;
	(*buffer)[path_length] = '\0';

	// Return
	return 0;

}

int _execute_command(char *command, char *output){

	FILE *pipe;
	char *new_command;
	char buffer[LINE_BUFFER_SIZE];

	// Get the new command
	copy_string(&new_command, command, strlen(STDERR_TO_STDOUT) + 1);
	strcat(new_command, STDERR_TO_STDOUT);

	// Open the command for reading
	pipe = popen(new_command, "r");
	if (pipe == NULL)
		return ERROR_OPERATING_SYSTEM_UNABLE_TO_RUN_COMMAND;

	// Read the output a line at a time
	while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
		strcat(output, buffer);
	}

	// Close
	pclose(pipe);

	// Free
	free(new_command);

	// Return
	return 0;

}

#pragma endregion

#pragma region SharedLibraries

int _load_library(const char *lib_name, void **handle){

	// Open library and verify it
	*handle = dlopen(lib_name, RTLD_LAZY | RTLD_GLOBAL);

	// Return
	return 0;

}

int _get_function_pointer(void *handle, const char *func_name, FUNC_PTR *func_ptr, char *error){

	// Get symbol and init error
	*func_ptr = (FUNC_PTR)dlsym(handle, func_name);
	error = dlerror();

	// Return
	return 0;

}

int _unlink_library(void **handle){

	// Close library handle
	dlclose(handle);
	*handle = NULL;

	// Return
	return 0;

}

#pragma endregion

#pragma region FileSystem

int _change_working_directory(const char *path){

	int ret_val;

	// Change directory
	ret_val = chdir(path);
	if (ret_val == -1)
		return ERROR_OPERATING_SYSTEM_UNABLE_TO_CHANGE_DIR;

	// Return
	return 0;

}

int _get_all_files_from_folder(const char *path_to_folder, int *file_count, char **filenames, int max_files, const char *extension){

	DIR *folder;
	struct dirent *file;
	char *ext_ptr;
	int index = 0;
	int ext_length, error;

	// Open folder
	folder = opendir(path_to_folder);
	if (folder == NULL)
		return ERROR_OPERATING_SYSTEM_UNABLE_TO_OPEN_FOLDER;

	// Get extension length
	if (extension != NULL)
		ext_length = strlen(extension);

	// Get all files
	while ((file = readdir(folder)) != NULL){

		// Verify if regular file
		if (file->d_type == DT_REG){

			// Verify number of files
			if (index >= max_files)
				break;

			// Verify extension
			if (extension != NULL){
				ext_ptr = strstr(file->d_name, extension);
				if (ext_ptr == NULL || strlen(ext_ptr) != ext_length)
					continue;
			}

			// Allocate the new filename
			error = copy_string(&filenames[index], file->d_name, 0);
			if (error != 0)
				return error;

			// Go to next element from array
			index++;

		}

	}
	*file_count = index;

	// Free the allocated folder
	free(folder);

	// Return
	return 0;

}

int _has_privilege(const char *path, PRIVILEGE priv){

	int mode;

	// Get the standard privilege
	switch (priv){
		case WRITE:
			mode = W_OK;
			break;
		case READ:
			mode = R_OK;
			break;
		case EXECUTE:
			mode = X_OK;
			break;
		default:
			return ERROR_PROGRAMMING_INVALID_ARGUMENT;
	}

	// Check privileges and return
	int ret;
	ret = access(path, mode);
	return (ret == 0);

}

#pragma endregion

#pragma endregion

#endif