void print_error(const char *msg);

void print_info(const char *msg);

void print_success(const char *msg);

int run_command(const char *cmd);

int file_exists(const char *path);

char* read_meta_key(const char *filepath, const char*key);

int ensure_dir(const char *path);

int run_hook(const char *path,const char* hook);

void parse_dep_array(const char* str,char** deps);

void free_dep_array(char** deps);
