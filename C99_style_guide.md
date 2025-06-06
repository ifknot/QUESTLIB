# My Complete C99 Style Guide

## Table of Contents
1. [General Rules](#1-general-rules)
    - [Brace Placement](#brace-placement)
    - [Indentation](#indentation)
    - [Alignment](#alignment)
2. [Control Structures](#2-control-structures)
    - [If-Else Statements](#if-else-statements)
    - [Ternary Operator](#ternary-operator)
    - [Loops](#loops)
    - [Switch Statements](#switch-statements)
3. [Naming Conventions](#3-naming-conventions)
4. [Documentation](#4-documentation)
    - [File Header](#file-header)
    - [Function Docs](#function-docs)
5. [Error Handling](#5-error-handling)
    - [Defensive Checks](#defensive-checks)
    - [Assertions](#assertions)
6. [Memory Management](#6-memory-management)
7. [Generics and Boilerplate](#7-generics-and-boilerplate)
    - [Explicit Boilerplate](#explicit-boilerplate)
    - [Macro Guidelines](#macro-guidelines)
    - [Void* Restrictions](#void-restrictions)
    - [X-Macros](#x-macros)
8. [Full Example](#8-full-example)
9. [License](#9-license)

---

## 1. General Rules

### Brace Placement
```c
void function(void) {
    if (condition) {
        action();
    }
}
```

### Indentation
+ Use 4 spaces (no tabs).
+ Never mix tabs and spaces.

### Alignment
Align related elements:
```c
typedef struct {
    uint8_t  type;   // 1 byte
    uint32_t size;   // 4 bytes
    char*    data;   // 8 bytes
} buffer_t;
```

---

## 2. Control Structures

### If-Else Statements
```c
if (ptr != NULL) {
    safe_operation(ptr);
} else {
    handle_error();
}
```

### Ternary Operator
__Only__ use for very simple assignments where it improves clarity.
```c
// Acceptable
int x = (condition) ? 1 : 0;

// Never do this
int y = (condition1) ? ((condition2) ? a : b) : c;
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

## 3. Naming Conventions

| Element       | Style          | Example          |
|---------------|----------------|------------------|
| Variables     | `snake_case`   | `buffer_size`    |
| Functions     | `snake_case`   | `init_memory()`  |
| Types         | `_t` suffix    | `mem_arena_t`    |
| Constants     | `UPPER_CASE`   | `MAX_BUFFER`     |

Example:
```c
const size_t MAX_RETRIES = 3;
typedef struct { ... } network_socket_t;
size_t calculate_bandwidth(void);
```

---

## 4. Documentation

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

## 5. Error Handling

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

## 6. Memory Management

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

---

## 7. Generics and Boilerplate

C99 lacks generics, but ‘clever’ solutions often cost more in maintenance than they save in typing. When in doubt, prefer verbose boilerplate—it’s easier to debug and optimize.

### Explicit Boilerplate
+ Prefer explicit boilerplate over "clever" macros.
+ Prefer type-specific functions for clarity and debuggability:
```c
// ✅ Preferred
void vec_int_push(vec_int_t *vec, int value);
void vec_float_push(vec_float_t *vec, float value);

// ❌ Avoid: Opaque macro magic
#define VEC_PUSH(vec, val) /* ... */
```

### Macro Guidelines
+ Only use macros for repetitive boilerplate (e.g., X-Macros for enum-string maps).
+ Document expansions and prefix with module name:
```c
/* Expands to stack_int type and methods */
#define DEFINE_STACK_INT STACK_IMPL(int)
```

### Void* Restrictions
+ Never use void* for generic containers (exception: polymorphic callbacks).
+ If required, pair with runtime size checks:
```c
// Allowed only with size parameters
void buffer_op(void *data, size_t elem_size);
```

### X-Macros
Use only for:
+ Enum-to-string conversions
+ Flag set definitions
+ Repetitive struct declarations

Example:

```c
/**
 * @file vector.c
 * @brief 3D vector with X-Macro component
 */

// vec3_components.def
X(x)
X(y)
X(z)

typedef struct {
    #define X(comp) float comp;
    #include "vec3_components.def"
    #undef X
} vec3_t;

float vec3_length(vec3_t v) {
    return sqrtf(
        #define X(comp) + (v.comp * v.comp)
        #include "vec3_components.def"
        #undef X
    );
}
```
Rules:
+ Keep the X-Macro file (<100 lines)
+ Document the expansion pattern at the top
+ Always #undef X after use
+ Never nest X-Macros
---

## 8. Full Example
```c
// ✅ Good: Explicit type-specific API
typedef struct {
    float *data;
    size_t len;
} vec_float_t;

void vec_float_push(vec_float_t *vec, float val) {
    vec->data = realloc(vec->data, (vec->len + 1) * sizeof(float));
    vec->data[vec->len++] = val;
}

// ❌ Bad: Hidden complexity
#define DEFINE_VEC(T) \
    typedef struct { T *data; size_t len; } vec_##T##_t; \
    void vec_##T##_push(vec_##T##_t*, T); \
    /* ... 50 more lines ... */
```

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
