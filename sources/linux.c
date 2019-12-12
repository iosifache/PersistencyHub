#ifdef __linux__

#include <stdlib.h>
#include <dlfcn.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/utsname.h>
#include "../headers/platform_dependent.h"
#include "../headers/helpers.h"
#include "../headers/errors.h"

int _save_all_filenames(const char *path_to_folder, int *file_count, char **filenames){

	DIR *folder;
	struct dirent *file;
	int index = 0;

	// Open folder
	folder = opendir(path_to_folder);
	if (folder == NULL)
		return ERROR_OPERAING_SYSTEM_UNABLE_TO_OPEN_FOLDER;

	// Get all files
	while ((file = readdir(folder)) != NULL){

		// Verify if regular file
		if (file->d_type == DT_REG){

			// Verify number of files
			if (index >= MAX_FILES_IN_FOLDER)
				break;

			// Allocate the new filename
			copy_string(&filenames[index], file->d_name);
			if (filenames[index] == NULL)
				return ERROR_OPERAING_SYSTEM_UNABLE_TO_ALLOCATE;

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

int _get_architecture_bits(){

	return (sizeof(void *) * 8);

}

bool _check_root(){

	return (geteuid() == 0);

}

int _load_library(const char *lib_name, void **handle){

	// Open library and verify it
	*handle = dlopen(lib_name, RTLD_LAZY);

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

int _close_handle(void **handle){

	// Close library handle
	dlclose(*handle);
	*handle = NULL;

	// Return
	return 0;

}

#endif