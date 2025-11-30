#include "activities.h"

int compare_processes(const void *a, const void *b) {
    return strcmp(((ProcessInfo *)a)->name, ((ProcessInfo *)b)->name);
}

void activity_generator() {
    ProcessInfo *proc_list;
    int count;
    get_more_process_info(&proc_list, &count);

    if (count == 0) {
        printf("No processes.\n");
        return;
    }

    qsort(proc_list, count, sizeof(ProcessInfo), compare_processes);

    int background_found = 0;
    for (int i = 0; i < count; i++) {
        const char *state_str;
        switch (proc_list[i].state) {
            case 0:
                state_str = "Running";
                break;
            case 1:
                state_str = "Terminated";
                break;
            case 2:
                state_str = "Stopped";
                break;
            default:
                state_str = "Unknown";
        }

        if (proc_list[i].state != 1) {
            background_found = 1;
            printf("%d : %s - %s\n", proc_list[i].pid, proc_list[i].name, state_str);
        }
    }

    if (!background_found) {
        printf("No processes.\n");
    }

    free(proc_list);
    cleanup_finished_processes();
}
