#include "message.h"

void m_enqueue_message(struct Jelly *jelly, union JellyMessage *message)
{
  // place message on the queue
  struct JellyMessageListNode *new_node = malloc(sizeof(struct JellyMessageListNode));
  new_node->message = message;
  new_node->next_message = NULL;

  if (jelly->jelly_message_write_head == NULL) {
    jelly->jelly_message_write_head = new_node;
  } else {
    jelly->jelly_message_write_head->next_message = new_node;
  }
}

void m_process_messages(struct Jelly *jelly)
{
  struct JellyMessageListNode *temp;
  struct JellyMessageListNode *current_message_node;

  u_disable_interrupts(jelly); // critical section, cant get interrupted here
  temp = jelly->jelly_message_read_head;
  jelly->jelly_message_read_head = jelly->jelly_message_write_head;
  jelly->jelly_message_write_head = temp;
  u_enable_interrupts(jelly);

  // process the messages
  while (jelly->jelly_message_read_head != NULL) {
    current_message_node = jelly->jelly_message_read_head;
    switch (current_message_node->message->type) {
    case (PROXIMITY_SENSED):
      // proximity sensed processing
      break;
    case (PROXIMITY_LOST):
      // proximity lost processing
      break;
    case (SET_POSITION):
      p_set_position(jelly, ((struct SetPositionMessage *) current_message_node->message)->position);
      break;
    default:
      // do nothing
      break;
    }

    jelly->jelly_message_read_head = jelly->jelly_message_read_head->next_message;
    free(current_message_node->message);
    free(current_message_node);
  }
}