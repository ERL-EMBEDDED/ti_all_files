#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>

struct timespec AddTimespecByNs(struct timespec ts, int64_t ns) {
    ts.tv_nsec += ns;

    while (ts.tv_nsec >= 1000000000) {
        ++ts.tv_sec;
        ts.tv_nsec -= 1000000000;
    }

    while (ts.tv_nsec < 0) {
        --ts.tv_sec;
        ts.tv_nsec += 1000000000;
    }

    return ts;
}

void* time_test(void* arg) {
    struct timespec next_wakeup_time_;
    clock_gettime(CLOCK_MONOTONIC, &next_wakeup_time_);

    while (true) {
        next_wakeup_time_ = AddTimespecByNs(next_wakeup_time_, 1000000);  // 1 millisecond
        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &next_wakeup_time_, NULL);
        printf("Raw timespec.tv_nsec: %jd.%09ld\n", next_wakeup_time_.tv_sec, next_wakeup_time_.tv_nsec);
    }

    // Add any necessary cleanup or return statements based on your specific requirements
    return NULL;
}
void runTime(){

    // Create a pthread_t variable to hold the thread ID
    pthread_t time;

    // Create and run the thread_test function in a new thread
    pthread_create(&time, NULL, time_test, NULL);

    // Wait for the thread to finish
    pthread_join(time, NULL);
}

int main() {

    runTime();
    
    return 0;
}
