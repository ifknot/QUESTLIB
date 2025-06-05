#include "quest_uid.h" 

#include "quest_types.h"

typedef struct {

    quest_size_t i;

} private_quest_uid_t;

static quest_uid_t uid = { 0 };

quest_size_t quest_next_uid() {
    return  uid.i++;
}
