#ifndef QUEST_CONSTANTS_H
#define QUEST_CONSTANTS_H

#define QUEST_MAX_LEAF     7

#define QUEST_COMPOSITE_MAX_CHILDREN    16

// Connection direction bitflags
#define CONN_N   0x0001
#define CONN_NE  0x0002
#define CONN_E   0x0004
#define CONN_SE  0x0008
#define CONN_S   0x0010
#define CONN_SW  0x0020
#define CONN_W   0x0040
#define CONN_NW  0x0080
#define CONN_UP  0x0100
#define CONN_DOWN 0x0200
#define CONN_STAIR 0x0400  // Special staircase connection

#endif
