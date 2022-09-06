/**
 * @file timer_manager.cc
 * @author Jamishon
 * @brief Timer manager
 * @version 0.1
 * @date 2022-09-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "skiplist.h"
#include "pthread.h"

class TimerManager {
public:
    TimerManager() {

    }

    ~TimerManager() {

    }

    void AddTimer(Timer* timer) {

    }

    void RemoveTimer(Timer* timer) {

    }

    

private:
    SkipList timer_list_;
    pthread_spinlock_t lock_;

};