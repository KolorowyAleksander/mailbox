#ifndef MESSAGE_TAG_H
#define MESSAGE_TAG_H

/*
 * Queue declaration message is:
 * 1B tag,
 * 1B persistence flag,
 * 1B durability flag,
 * 255 B name,
 * 255 B bindingKey
 */

/*
 * Message is:
 * 1B tag
 * 8B message size
 * size B message
 */

enum class MessageTag : uint8_t {
  acknowledge = 03,
  queueDeclare = 05,
  message = 06,
  recieve = 07
};

// TODO: decide if this will ever be needed
// template <typename Enumeration>
// auto as_integer(Enumeration const value) ->
//     typename std::underlying_type<Enumeration>::type {
//   return static_cast<typename
//   std::underlying_type<Enumeration>::type>(value);
// }

#endif  // MESSAGE_TAG_H
