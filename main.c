#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATH_LEN 512

typedef struct {
    char input_file[MAX_PATH_LEN];
    char output_file[MAX_PATH_LEN];
    double start_time;
    double duration;
} VideoSegmentTask;

void print_banner() {
    printf("========================================\n");
    printf("       Cibve Video Engine v0.1          \n");
    printf("       High-Performance C Engine        \n");
    printf("========================================\n");
}

int parse_arguments(int argc, char *argv[], VideoSegmentTask *task) {
    if (argc < 5) {
        printf("Usage: %s <input> <output> <start_time> <duration>\n", argv[0]);
        return -1;
    }
    snprintf(task->input_file, sizeof(task->input_file), "%s", argv[1]);
    snprintf(task->output_file, sizeof(task->output_file), "%s", argv[2]);
    task->start_time = atof(argv[3]);
    task->duration = atof(argv[4]);
    return 0;
}

int execute_extraction(VideoSegmentTask *task) {
    char command[1024];
    snprintf(command, sizeof(command), 
             "ffmpeg -ss %.2f -i \"%s\" -t %.2f -c copy \"%s\" -y", 
             task->start_time, task->input_file, task->duration, task->output_file);
    printf("[Cibve] Executing: %s\n", command);
    return system(command);
}

int main(int argc, char *argv[]) {
    print_banner();
    VideoSegmentTask task;
    if (parse_arguments(argc, argv, &task) != 0) return 1;
    
    printf("[Cibve] Processing segment from %.2fs (duration: %.2fs)...\n", 
           task.start_time, task.duration);
           
    if (execute_extraction(&task) == 0) {
        printf("[Cibve] Success: Segment extracted to %s\n", task.output_file);
    } else {
        fprintf(stderr, "[Cibve Error] Failed to extract video segment.\n");
        return 1;
    }
    return 0;
}
