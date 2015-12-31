#ifndef YAML_PRIVATE_H_
#define YAML_PRIVATE_H_

#include "yaml.h"

/* The size of the input raw buffer.
 */
#define YAML_INPUT_RAW_BUFFER_SIZE	16384

/* The size of the input buffer.
 * It should be possible to decode the whole raw buffer.
 */
#define YAML_INPUT_BUFFER_SIZE	(YAML_INPUT_RAW_BUFFER_SIZE * 3)

/* The size of the output buffer.
 */
#define YAML_OUTPUT_BUFFER_SIZE	16384

/* The size of the output raw buffer.
 * It should be possible to encode the whole output buffer.
 */
#define YAML_OUTPUT_RAW_BUFFER_SIZE	(YAML_OUTPUT_BUFFER_SIZE * 2 + 2)

/* The size of other stacks and queues.
 */
#define YAML_INITIAL_STACK_SIZE		16
#define YAML_INITIAL_QUEUE_SIZE		16
#define YAML_INITIAL_STRING_SIZE	16

#endif /* !YAML_PRIVATE_H_ */
