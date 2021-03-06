#include "message.h"

// forward declares
void m_free_message_node(struct JellyMessageListNode *message_node);

void m_enqueue_message(struct Jelly *jelly, union JellyMessage *message, bool wake)
{
  // place message on the queue
  struct JellyMessageListNode *new_node = malloc(sizeof(struct JellyMessageListNode));
  new_node->message = message;
  new_node->next_message = NULL;

  if (jelly->jelly_message_write_head == NULL) {
    jelly->jelly_message_write_head = new_node;
  } else {
    struct JellyMessageListNode* tail = jelly->jelly_message_write_head;
    while (tail->next_message != NULL)
      tail = tail->next_message;
    tail->next_message = new_node;
  }

  if (wake)
    jelly_wake(jelly);
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
      proximity_sensed(jelly, ((struct ProximitySensedMessage *) current_message_node->message)->position);
      break;
    case (PROXIMITY_LOST):
      // proximity lost processing
      proximity_lost(jelly, ((struct ProximityLostMessage *) current_message_node->message)->position);
      break;
    case (SET_POSITION):
      p_set_position(jelly, ((struct SetPositionMessage *) current_message_node->message)->position);
      break;
    default:
      // do nothing
      break;
    }

    jelly->jelly_message_read_head = jelly->jelly_message_read_head->next_message;
    m_free_message_node(current_message_node);
  }
}

void m_free_message(union JellyMessage *message)
{
 switch(message->type) {
  case (PROXIMITY_SENSED):
    free(((struct ProximitySensedMessage *) message)->position); // free position
    break;
  case (PROXIMITY_LOST):
    free(((struct ProximityLostMessage *) message)->position); // free position
    break;
  case (SET_POSITION):
    free(((struct SetPositionMessage *) message)->position); // free position
    break;
  default:
    break;
  }
  free(message);
}

void m_free_message_node(struct JellyMessageListNode *message_node)
{
  m_free_message(message_node->message);
  free(message_node);
}
