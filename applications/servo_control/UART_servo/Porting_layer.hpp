#pragma once

#include "ch.h"
#include "hal.h"
#include "chprintf.h" //just for debugging

//Macro for checking the size of structs
#define STATIC_SIZE_CHECK(type, length)    static_assert(sizeof(struct type)== length, "invalid size of " #type)

#define GET_SYS_TIME() chVTGetSystemTime()
