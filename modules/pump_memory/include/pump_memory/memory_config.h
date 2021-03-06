#ifndef PUMP_MEMORY_CONFIG_H
#define PUMP_MEMORY_CONFIG_H

// pump_memory version info
#ifndef PUMP_MEMORY_VERSION_MAJOR
#   define PUMP_MEMORY_VERSION_MAJOR 0
#endif // !PUMP_MEMORY_VERSION_MAJOR
#ifndef PUMP_MEMORY_VERSION_MINOR
#   define PUMP_MEMORY_VERSION_MINOR 0
#endif // !PUMP_MEMORY_VERSION_MINOR
#ifndef PUMP_MEMORY_VERSION_PATCH
#   define PUMP_MEMORY_VERSION_PATCH 9
#endif // !PUMP_MEMORY_VERSION_PATCH
#ifndef PUMP_MEMORY_VERSION_BUILD
#   define PUMP_MEMORY_VERSION_BUILD 0
#endif // !PUMP_MEMORY_VERSION_BUILD
#define __PUMP_MEMORY_VERSION_STR(str_) #str_
#define __PUMP_MEMORY_VERJOIN(major_,minor_,patch_,build_) __PUMP_MEMORY_VERSION_STR(major_.minor_.patch_.build_)
#define PUMP_MEMORY_VERSION __PUMP_MEMORY_VERJOIN(PUMP_MEMORY_VERSION_MAJOR,PUMP_MEMORY_VERSION_MINOR,PUMP_MEMORY_VERSION_PATCH,PUMP_MEMORY_VERSION_BUILD)

#endif // !PUMP_MEMORY_CONFIG_H
