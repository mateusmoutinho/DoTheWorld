

#ifdef __linux__

bool private_dtw_verify_if_add(const char *type, int d_type){
    if (strcmp(type,"file") == 0 && d_type == DT_REG) {
        return true;
    }

    if (strcmp(type,"dir") == 0 && d_type == DT_DIR) {
        return true;
    }

    if (strcmp(type,"all") == 0) {
        return true;
    }
    return false;
}
bool private_dtw_verify_if_skip(struct dirent *entry){
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            return true;
        }
        return false;
}

struct DtwStringArray * dtw_list_basic(const char *path,const char* type,bool concat_path){

    DIR *dir;
    struct dirent *entry;

    //array of directories
    struct DtwStringArray *dirs = dtw_create_string_array();
    int i = 0;

    //means that the directory is not found
    if ((dir = opendir(path)) == NULL) {
        return dirs;
    }

    //reads the directory and adds the directories to the array
    while ((entry = readdir(dir)) != NULL) {
        //means is not a directory
        if (private_dtw_verify_if_skip(entry)){
            continue;
        }
    
        if (private_dtw_verify_if_add(type,entry->d_type)) {
            
            
            if(concat_path){
                //allocates memory for the directory
                char *generated_dir = (char*)malloc(strlen(path) + strlen(entry->d_name) + 2);
                sprintf(generated_dir, "%s/%s", path, entry->d_name);
                dtw_add_string(dirs, generated_dir);
                free(generated_dir);

            }
            else{
                dtw_add_string(dirs, entry->d_name);
            }

            i++;
        }
    }

  
    closedir(dir);

    return dirs;
}

#endif