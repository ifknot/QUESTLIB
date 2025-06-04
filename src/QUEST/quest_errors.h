#ifndef QUEST_ERRORS_H
#define QUEST_ERRORS_H

typedef enum {
    QUEST_SUCCESS = 0,
    QUEST_ITEM_LIST_FULL,
    QUEST_ITEM_LIST_EMPTY
} quest_error_t;

// human readable error translations
char quest_error_messages[3][23] = {};

#endif
