#include <stdio.h>
#include <sys/shm.h>
#include <sys/stat.h>

int main() {
    // Create a shared memory segment
    int segment_id = shmget(IPC_PRIVATE, 1024, S_IRUSR | S_IWUSR);
    if (segment_id == -1) {
        perror("shmget failed");
        return 1;
    }

    // Attach the shared memory segment
    char *shared_memory = (char *)shmat(segment_id, NULL, 0);
    if (shared_memory == (char *)(-1)) {
        perror("shmat failed");
        return 1;
    }

    // Write data to the shared memory segment
    snprintf(shared_memory, 1024, "Hello from shared memory!");

    // Print the data from the shared memory segment
    printf("Data in shared memory: %s\n", shared_memory);

    // Detach the shared memory segment
    if (shmdt(shared_memory) == -1) {
        perror("shmdt failed");
        return 1;
    }

    // Remove the shared memory segment
    if (shmctl(segment_id, IPC_RMID, NULL) == -1) {
        perror("shmctl failed");
        return 1;
    }

    return 0;
}