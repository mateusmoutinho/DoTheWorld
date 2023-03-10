

#ifdef __linux__

bool private_dtw_verify_if_add(const int expected_type, int d_type){
    if (expected_type == DTW_FILE_TYPE  && d_type == DT_REG) {
        return true;
    }

    if (expected_type == DTW_FOLDER_TYPE && d_type == DT_DIR) {
        return true;
    }

    if (expected_type == DTW_ALL_TYPE) {
      
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

struct DtwStringArray * dtw_list_basic(const char *path,int expected_type,bool concat_path,bool add_end_bar_to_dir){

    DIR *dir;
    struct dirent *entry;

    //array of directories
    struct DtwStringArray *dirs = dtw_constructor_string_array();
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
    
        if (private_dtw_verify_if_add(expected_type,entry->d_type)) {
            
            
            if(concat_path){
                //allocates memory for the directory
                char *generated_dir = (char*)malloc(strlen(path) + strlen(entry->d_name) + 2);
                sprintf(generated_dir, "%s/%s", path, entry->d_name);
                
                dirs->add_string(dirs, generated_dir);
                free(generated_dir);

            }
            else{
                dirs->add_string(dirs, entry->d_name);
                
            }

            i++;
        }
    }

    if(expected_type == DTW_FOLDER_TYPE && add_end_bar_to_dir){
        for(int i = 0; i < dirs->size; i++){
            char *dir = dirs->strings[i];
            char *new_dir = (char*)malloc(strlen(dir) + 1);
            //concat '/' to the end of the directory
            sprintf(new_dir, "%s/", dir);
            free(dirs->strings[i]);
            dirs->strings[i] = new_dir;
        }
    }
    closedir(dir);

    return dirs;
}

#endif