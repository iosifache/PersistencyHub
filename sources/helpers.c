#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/utsname.h>
#include "../headers/percistency_hub.h"
#include "../headers/helpers.h"
#include "../headers/errors.h"

int copy_string(char **dest, const char *src){

	// Allocate the destination
	if (*dest != NULL)
		free(dest);
	*dest = (char *)malloc((strlen(src) + 1) * sizeof(char));
	if (dest == NULL)
		return ERROR_OPERAING_SYSTEM_UNABLE_TO_ALLOCATE;

	// Copy string
	strcpy(*dest, src);

	// Return
	return 0;

}

int get_all_files_from_folder(const char *path_to_folder, int *file_count, char ***filenames, int max_files){

	int ret_val;

	// Allocate the filenames array
	*filenames = (char **)malloc(max_files * sizeof(char *));
	if (*filenames == NULL)
		return ERROR_OPERAING_SYSTEM_UNABLE_TO_ALLOCATE;

	// Open folder
	ret_val = _save_all_filenames(path_to_folder, file_count, *filenames);

	// Return
	return ret_val;

}

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

int check_root(void *result){

	if (_check_root()){
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
			ret_val = check_root(result);
			break;
		default:
			result = NULL;
			ret_val = ERROR_PROGRAMMING_INVALID_ARGUMENT;
	}

	// Return
	return ret_val;

}

int load_library(const char *lib_name, void **handle){

	// Allocate the new handle
	if (*handle != NULL)
		free(*handle);
	*handle = (void *)malloc(sizeof(void *));
	if (*handle == NULL)
		return ERROR_OPERAING_SYSTEM_UNABLE_TO_ALLOCATE;

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

int close_handle(void **handle){

	// Close handle
	_close_handle(handle);

	// Return
	return 0;

}