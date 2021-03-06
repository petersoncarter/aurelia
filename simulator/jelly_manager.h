#ifndef __JELLY_MANAGER_H
#define __JELLY_MANAGER_H

// standard c headers
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <semaphore.h>

// project specific
#include "../device/jelly.h"
#include "../device/message.h"
#include "../device/position.h"
#include "../device/network.h"

typedef uint16_t JellyAddress;

#define NUM_JELLYS 50

enum JellyEventType {
  PROXIMITY,
  NETWORK_MESSAGE,
  TIMER,
  TEARDOWN
};

struct JellyEvent {
  enum JellyEventType type;
  JellyAddress dst_addr;
  void *payload;
  struct JellyEvent* next_event;
};

// jelly threads
struct JellyThread {
  struct Jelly* jelly;
  struct JellyInitFrame *init_frame;
  pthread_t run_thread;
};

// jelly manager event queue
extern struct JellyEvent *jm_event_queue_head;
extern pthread_mutex_t jelly_event_queue_mutex;

// access to the jelly threads
extern struct JellyThread *jelly_threads[];

void jm_manager_init(void);
void jm_queue_event(struct JellyEvent *jelly_event, bool notify);
void jm_queue_notify(void);
struct JellyEvent* jm_create_event(enum JellyEventType event_type);

#endif
