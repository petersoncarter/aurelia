#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// simulator headers
#include "jelly_manager.h"
#include "input.h"
#include "render.h"

// device headers
#include "../device/proximity.h"
#include "../device/message.h"
#include "../device/network.h"
#include "../device/jelly.h"

#define SLEEP_TIME 10000 // 100 fps i guess

/*void test_event(void)
{
  struct JellyEvent* proximity_event = jm_create_event(PROXIMITY);
  proximity_event->dst_addr = 0;
  struct ProximitySensedMessage *proximity_message = malloc(sizeof(struct ProximitySensedMessage));
  proximity_message->type = PROXIMITY_SENSED;
  proximity_message->position = malloc(sizeof(struct Position));
  proximity_message->position->x = jelly_threads[0]->jelly->position->x;
  proximity_message->position->y = jelly_threads[0]->jelly->position->y;
  proximity_event->message = (union JellyMessage*) proximity_message;
  jm_queue_event(proximity_event, true);
}*/

void send_test_message(void)
{
  struct Jelly *jelly_zero = jelly_threads[1]->jelly;
  struct JellyNetworkPacket *packet = malloc(sizeof(struct JellyNetworkPacket));
  packet->src_addr = 0x00;
  packet->dst_addr = BROADCAST_ADDRESS;
  packet->timestamp = 1;
  packet->sequence_num = 1;
  packet->port_address = 0x00010000;
  packet->hops = 0;

  // add the message
  packet->payload.type = PAYLOAD_PROXIMITY_SENSED;
  ((struct JellyNetworkProximitySensedPayload*) &packet->payload)->x_pos = jelly_zero->position->x;
  ((struct JellyNetworkProximitySensedPayload*) &packet->payload)->y_pos = jelly_zero->position->y;
  n_packet_available_isr(jelly_zero, packet);
}

int main(int argc, char **argv) {
  srand(time(NULL));
  // Initialize the networking
  // n_network_init();

  // Initialize the jellys
  jm_manager_init();

  // Initialize the main window
  r_init_display();
  i_init_input();

  // init the input
  proximity_init();

  // send test message
  send_test_message();

  for(;;) {
    // lazy sleeping, could actually try to hit fps but i'm lazy
    usleep(SLEEP_TIME);

    // actually do the run loop stuff
    i_process_input();
    r_render();
  }
}
