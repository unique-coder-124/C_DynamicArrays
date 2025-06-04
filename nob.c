#define NOB_IMPLEMENTATION
#include "nob.h"

#ifndef PATH_MAX
  #ifdef _WIN32
    #define PATH_MAX MAX_PATH
  #else
    #define PATH_MAX 4096
  #endif
#endif

const char *program_name = "main";
const char *src_file = "main.c";
const char *header_file = "DynamicArrays.h";
const char *obj_name = "main.o";
const char *pp_file = "main.i";

const char *build_dir_base = "build";
const char *dist_dir_base = "dist";

const char *get_exe_ext(void) {
  #ifdef _WIN32
    return ".exe";
  #else
    return "";
  #endif
}

bool build_target(bool is_debug) {
  Nob_Cmd cmd = {0};
  bool success = true;

  const char *target_type = is_debug ? "debug" : "release";
  nob_log(NOB_INFO, "Building target: %s", target_type);

  char build_dir[PATH_MAX];
  snprintf(build_dir, PATH_MAX, "%s/%s", build_dir_base, target_type);

  char dist_dir[PATH_MAX];
  snprintf(dist_dir, PATH_MAX, "%s/%s", dist_dir_base, target_type);

  char obj_path[PATH_MAX];
  snprintf(obj_path, PATH_MAX, "%s/%s", build_dir, obj_name);

  char pp_path[PATH_MAX];
  snprintf(pp_path, PATH_MAX, "%s/%s", build_dir, pp_file);

  char exe_name[256];
  snprintf(exe_name, sizeof(exe_name), "%s%s", program_name, get_exe_ext());
  char exe_path[PATH_MAX];
  snprintf(exe_path, PATH_MAX, "%s/%s", dist_dir, exe_name);

  if (!nob_mkdir_if_not_exists(build_dir_base)) return false;
  if (!nob_mkdir_if_not_exists(dist_dir_base)) return false;
  if (!nob_mkdir_if_not_exists(build_dir)) return false;
  if (!nob_mkdir_if_not_exists(dist_dir)) return false;

  if (is_debug) {

    const char *preprocess_dependencies[] = {src_file, header_file};
    int needs_preprocess = nob_needs_rebuild(pp_path, preprocess_dependencies, NOB_ARRAY_LEN(preprocess_dependencies));
    if (needs_preprocess < 0) return false;

    if (needs_preprocess > 0) {
      nob_cmd_append(&cmd, "clang");
      nob_cmd_append(&cmd, "-std=gnu11", "-Wall", "-Wformat");
      nob_cmd_append(&cmd, "-g3", "-gdwarf-4", "-O0");
      nob_cmd_append(&cmd, "-E", src_file, "-o", pp_path);
      if (!nob_cmd_run_sync_and_reset(&cmd)) {
        nob_log(NOB_ERROR, "Preprocessing failed for %s", target_type);
        return false;
      }
      nob_log(NOB_INFO, "Preprocessed %s to %s", src_file, pp_path);
    } else {
      nob_log(NOB_INFO, "Preprocessed file %s is up to date.", pp_path);
    }

    int needs_compile = nob_needs_rebuild(obj_path, (const char*[]){pp_path}, 1);
    if (needs_compile < 0) return false;

    if (needs_compile > 0) {
      nob_cmd_append(&cmd, "clang");
      nob_cmd_append(&cmd, "-std=gnu11", "-Wall", "-Wformat");
      nob_cmd_append(&cmd, "-g3", "-gdwarf-4", "-O0");
      nob_cmd_append(&cmd, "-c", pp_path, "-o", obj_path);
      if (!nob_cmd_run_sync_and_reset(&cmd)) {
        nob_log(NOB_ERROR, "Compilation failed for %s", target_type);
        return false;
      }
      nob_log(NOB_INFO, "Compiled %s to %s", pp_path, obj_path);
    } else {
      nob_log(NOB_INFO, "Object file %s is up to date.", obj_path);
    }
  } else {

    const char *compile_dependencies[] = {src_file, header_file};
    int needs_compile = nob_needs_rebuild(obj_path, compile_dependencies, NOB_ARRAY_LEN(compile_dependencies));
    if (needs_compile < 0) return false;

    if (needs_compile > 0) {
      nob_cmd_append(&cmd, "clang");
      nob_cmd_append(&cmd, "-std=gnu11", "-Wall", "-Wformat");
      nob_cmd_append(&cmd, "-O3");
      nob_cmd_append(&cmd, "-c", src_file, "-o", obj_path);
      if (!nob_cmd_run_sync_and_reset(&cmd)) {
        nob_log(NOB_ERROR, "Compilation failed for %s", target_type);
        return false;
      }
      nob_log(NOB_INFO, "Compiled %s to %s", src_file, obj_path);
    } else {
      nob_log(NOB_INFO, "Object file %s is up to date.", obj_path);
    }
  }

  const char *link_dependencies[] = {obj_path};
  int needs_link = nob_needs_rebuild(exe_path, link_dependencies, NOB_ARRAY_LEN(link_dependencies));
  if (needs_link < 0) return false;

  if (needs_link > 0) {
    nob_cmd_append(&cmd, "clang");
    nob_cmd_append(&cmd, "-std=gnu11", "-Wall", "-Wformat");
    
    if (is_debug) {
      nob_cmd_append(&cmd, "-g3", "-gdwarf-4", "-O0");
    } else {
      nob_cmd_append(&cmd, "-O3");
      nob_cmd_append(&cmd, "-s");
    }
    
    nob_cmd_append(&cmd, obj_path, "-o", exe_path);
    if (!nob_cmd_run_sync_and_reset(&cmd)) {
      nob_log(NOB_ERROR, "Linking failed for %s", target_type);
      return false;
    }
    nob_log(NOB_INFO, "Linked %s to %s", obj_path, exe_path);
  } else {
    nob_log(NOB_INFO, "Executable %s is up to date.", exe_path);
  }

  nob_cmd_free(cmd);
  nob_log(NOB_INFO, "%s build successful: %s", target_type, exe_path);
  return true;
}

void clean_targets(void) {
  nob_log(NOB_INFO, "Cleaning build artifacts...");

  char path_to_delete[PATH_MAX];
  const char *target_types[] = {"release", "debug"};
  const char *exe_ext_val = get_exe_ext();

  for (size_t i = 0; i < NOB_ARRAY_LEN(target_types); ++i) {

    snprintf(path_to_delete, PATH_MAX, "%s/%s/%s", build_dir_base, target_types[i], obj_name);
    if (nob_file_exists(path_to_delete)) nob_delete_file(path_to_delete);

    snprintf(path_to_delete, PATH_MAX, "%s/%s/%s", build_dir_base, target_types[i], pp_file);
    if (nob_file_exists(path_to_delete)) nob_delete_file(path_to_delete);

    char exe_name_tmp[256];
    snprintf(exe_name_tmp, sizeof(exe_name_tmp), "%s%s", program_name, exe_ext_val);
    snprintf(path_to_delete, PATH_MAX, "%s/%s/%s", dist_dir_base, target_types[i], exe_name_tmp);
    if (nob_file_exists(path_to_delete)) nob_delete_file(path_to_delete);
  }

  nob_log(NOB_INFO, "Clean finished. (Note: Empty directories might remain)");
}

void print_usage(const char *nob_program_name) {
  nob_log(NOB_INFO, "Usage: %s [release | debug | clean | all]", nob_program_name);
  nob_log(NOB_INFO, "  release: Build the release version (default)");
  nob_log(NOB_INFO, "  debug:   Build the debug version");
  nob_log(NOB_INFO, "  all:     Build both release and debug versions");
  nob_log(NOB_INFO, "  clean:   Remove build and distribution artifacts");
}

int main(int argc, char **argv) {
  NOB_GO_REBUILD_URSELF(argc, argv);

  const char *program = nob_shift_args(&argc, &argv);

  if (argc == 0) {
    if (!build_target(false)) return 1;
  } else {
    const char *subcommand = nob_shift_args(&argc, &argv);

    if (strcmp(subcommand, "release") == 0) {
      if (!build_target(false)) return 1;
    } else if (strcmp(subcommand, "debug") == 0) {
      if (!build_target(true)) return 1;
    } else if (strcmp(subcommand, "all") == 0) {
      nob_log(NOB_INFO, "Building all targets...");
      if (!build_target(false)) return 1;
      if (!build_target(true)) return 1;
      nob_log(NOB_INFO, "All targets built.");
    } else if (strcmp(subcommand, "clean") == 0) {
      clean_targets();
    } else {
      print_usage(program);
      nob_log(NOB_ERROR, "Unknown subcommand: %s", subcommand);
      return 1;
    }
  }

  return 0;
}
