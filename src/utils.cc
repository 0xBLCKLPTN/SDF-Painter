
#include "../include/utils.hh"

char* read_file(const char* file_path) {
    FILE* file = fopen(file_path, "r");
    if (!file) {
        fprintf(stderr, "Failed to open file: %s\n", file_path);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buffer = (char*)malloc(length + 1);
    if (!buffer) {
        fprintf(stderr, "Failed to allocate memory for file: %s\n", file_path);
        fclose(file);
        return NULL;
    }

    fread(buffer, 1, length, file);
    buffer[length] = '\0';
    fclose(file);

    printf("File with shaders has been read\n");
    return buffer;
}


time_t get_file_modification_time(const char* file_path) {
  struct stat file_stat;
  if (stat(file_path, &file_stat) == 0) {
      return file_stat.st_mtime;
  }
  return 0;
}