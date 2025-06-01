# Complete C99 Style Guide

## Table of Contents
1. [General Rules](#1-general-rules)
2. [Control Structures](#2-control-structures)
3. [Naming Conventions](#3-naming-conventions)
4. [Documentation](#4-documentation)
5. [Error Handling](#5-error-handling)
6. [Memory Management](#6-memory-management)
7. [Full Example](#7-full-example)

---

## Section 1. General Rules

### Brace Placement

```c
void function(void) {
    if (condition) {
        action();
    }
}
```
### Indentation
Use 4 spaces (no tabs)

### Align related elements:

```c
typedef struct {
    uint8_t  type;   // 1 byte
    uint32_t size;   // 4 bytes
    char*    data;   // 8 bytes
} buffer_t;
```

---

## Section 2. Control Structures

### If-Else Statements
```c
if (ptr != NULL) {
    safe_operation(ptr);
} else {
    handle_error();
}
```

### Loops
```c
for (size_t i = 0; i < MAX_ITER; i++) {
    if (should_break(i)) {
        break;
    }
    process(i);
}
```

### Switch Statements
```c
switch (cmd) {
    case CMD_READ: {
        read_data();
        break;
    }
    case CMD_WRITE: {
        write_data();
        break;
    }
    default: {
        invalid_command();
        break;
    }
}
```

---

## Section 3. Naming Convention

| Element       | Style          | Example          |
|---------------|----------------|------------------|
| Variables     | `snake_case`   | `buffer_size`    |
| Functions     | `snake_case`   | `init_memory()`  |
| Types         | `_t` suffix    | `mem_arena_t`    |
| Constants     | `UPPER_CASE`   | `MAX_BUFFER`     |

### Example
```c
const size_t MAX_RETRIES = 3;
typedef struct { ... } network_socket_t;
size_t calculate_bandwidth(void);
```

---

## Section 4. Documentation

### File Header
```c
/**
 * @file memory.c
 * @brief Arena allocator implementation
 * @note Thread-safe for single producer/consumer
 */
 ```

 ### Function Docs
 ```c
 /**
  * @brief Allocates aligned memory
  * @param size Bytes to allocate (>0)
  * @param alignment Power-of-two (e.g., 16, 64)
  * @return void* or NULL on failure
  * @warning alignment must be ≥ sizeof(void*)
  */
 void* aligned_alloc(size_t size, size_t alignment);
 ```

---

## Section 5. Error Handling

### Defensive Checks
```c
int safe_divide(int a, int b, int* result) {
    if (result == NULL) return E_INVALID_PTR;
    if (b == 0) {
#ifndef NDEBUG
        fprintf(stderr, "Division by zero\n");
#endif
        return E_DIV_ZERO;
    }
    *result = a / b;
    return SUCCESS;
}
```

### Assertions
```c
void critical_op(critical_t* ctx) {
    assert(ctx != NULL && "Context cannot be NULL");
    assert(ctx->magic == CTX_MAGIC && "Invalid context");
    // ...
}
```

---

## Section 6. Memory Management

### Allocation Pattern
```c
typedef struct {
    size_t capacity;
    void** items;
} collection_t;

collection_t* new_collection(size_t size) {
    collection_t* col = malloc(sizeof(*col));
    if (!col) return NULL;

    *col = (collection_t){
        .capacity = size,
        .items = calloc(size, sizeof(void*))
    };

    if (!col->items) {
        free(col);
        return NULL;
    }
    return col;
}
```

## Section 7. Full Example

```c
/**
 * @file vector.c
 * @brief 3D vector implementation
 * @defgroup math_vectors Vector Math
 * @{
 */

#include <math.h>
#include <stdbool.h>

#define VECTOR_EPSILON 0.0001f

typedef struct {
    float x;
    float y;
    float z;
} vec3_t;

/**
 * @brief Normalizes a 3D vector
 * @param v Input vector to normalize
 * @return Normalized vector or zero vector if invalid
 */
vec3_t vec3_normalize(vec3_t v) {
    float len = sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);

    if (len > VECTOR_EPSILON) {
        vec3_t result = {
            .x = v.x / len,
            .y = v.y / len,
            .z = v.z / len
        };
        return result;
    }

    return (vec3_t){0};
}

/** @} */ // End math_vectors
```

---

## License

Copyright 2025 Jeremy Simonn Thornton

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
