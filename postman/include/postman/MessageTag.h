#ifndef MESSAGE_TAG_H
#define MESSAGE_TAG_H

/*
 * Queue declaration message is:
 * 1B tag,
 * 1B persistence flag,
 * 1B durability flag,
 * 255 B name,
 * 255 B bindingKey
 *
 * Queue bind message is:
 * 1B tag,
 * 255B name
 *
 * Message is:
 * 1B tag
 * 8B message size
 * 255B routing key
 * size B message
 */

enum class MessageTag : uint8_t {
  queueDeclare = 01,
  queueBind = 02,
  ack = 03,
  rej = 04,
  collect = 05,
  message = 06
};

#endif  // MESSAGE_TAG_H
