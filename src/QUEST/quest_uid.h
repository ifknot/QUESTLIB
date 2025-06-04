


typedef struct private_quest_uid_t quest_uid_t;

/* in imp file
typedef struct {
    quest_size_t uid
} private_quest_uid;

static quest_uid_t quest_global_uid;

*/

void quest_uid_reset();

quest_size_t quest_next_uid();
