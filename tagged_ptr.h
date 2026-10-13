/* tagged_ptr - simple pointer tagging implementation
                no warranty implied; use at your own risk.*/

#ifndef TAGGED_PTR_H_
#define TAGGED_PTR_H_ 

#ifdef __cplusplus
#extern "C" {
#endif

#if defined(__GNUC__) || defined(__clang__)
#   pragma GCC diagnostic push
#endif

#include <stdint.h>
#include <stddef.h>

#if !defined(TAGGED_PTR_ASSERT)
#   include <assert.h>
#   define TAGGED_PTR_ASSERT(x) assert(x)
#endif 

#if _WIN32 || _WIN64
#if _WIN64
#define ARCH64
#else
#define ARCH32
#endif
#endif

#if __GNUC__
#if __x86_64__ || __ppc64__
#define ARCH64
#else
#define ARCH32
#endif
#endif

#ifdef TAGGED_PTR 
#   define TAGGED_PTR_DEF static 
#else 
#   define TAGGED_PTR_DEF extern
#endif 

#if defined(TAGGED_PTR_CANARY_ENABLED)
#   ifndef TAGGED_PTR_CANARY
#       define TAGGED_PTR_CANARY    0xDEADBEEF
#   endif
#   define TAGGED_PTR_CANARY_FIELDS     \
        unsigned long canary;
#else
#   define TAGGED_PTR_CANARY
#   define TAGGED_PTR_CANARY_FIELDS
#endif

#define ARG9_(a1, a2, a3, a4, a5, a6, a7, a8, a9, ...) a9
#define NARGS(...) ARG9_(__VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1)
#define S8_(first, ...) first; S7_(__VA_ARGS__)
#define S7_(first, ...) first; S6_(__VA_ARGS__)
#define S6_(first, ...) first; S5_(__VA_ARGS__)
#define S5_(first, ...) first; S4_(__VA_ARGS__)
#define S4_(first, ...) first; S3_(__VA_ARGS__)
#define S3_(first, ...) first; S2_(__VA_ARGS__)
#define S2_(first, ...) first; S1_(__VA_ARGS__)
#define S1_(first) first;
#define PASTE3(a, b, c) PASTE3_(a, b, c)
#define PASTE3_(a, b, c) a ## b ## c
#define MM_INVOKE_B( macro, args )	macro args
#define MM_APPLY_1( macroname, a1 ) \
    MM_INVOKE_B( macroname, (a1) )
#define MM_APPLY_2( macroname, a1, a2 ) \
    MM_INVOKE_B( macroname, (a1) ) \
    MM_APPLY_1( macroname, a2 )
#define MM_APPLY_3( macroname, a1, a2, a3 ) \
    MM_INVOKE_B( macroname, (a1) ) \
    MM_APPLY_2( macroname, a2, a3 )
#define MM_CONCAT__( a, b )     a ## b
#define MM_CONCAT_( a, b )      MM_CONCAT__( a, b )
#define MM_CONCAT( a, b )       MM_CONCAT_( a, b )
#define MM_APPLY_3( macroname, a1, a2, a3 ) \
    MM_INVOKE_B( macroname, (a1) ) \
    MM_APPLY_2( macroname, a2, a3 )
#define MM_INVOKE( macro, args )	macro args
#define MM_APPLY( macroname, ... )									\
    MM_INVOKE(												\
        MM_CONCAT( MM_APPLY_, NARGS( __VA_ARGS__ ) ),							\
        ( macroname, __VA_ARGS__ )									\
    )
#define WITH_L_PREFIX_( lit )     tagged_##lit
#define PREFIX( ... )	MM_APPLY( WITH_L_PREFIX_, __VA_ARGS__ )
#define STRUCT_CREATE(name, ...)									\
   typedef struct name name;										\
   struct name {											\
		PASTE3(S, NARGS(__VA_ARGS__), _)(__VA_ARGS__)						\
   };

#define TAGGED_PTR_DEFINE(t, tag, s, setter, getter)							\
	typedef s PREFIX(t);										\
													\
	static tagged_##t	setter(int, t);	    							\
	static t			getter(tagged_##t* ptr);					\
													\
	TAGGED_PTR_DEF tagged_##t									\
	tagged_create_##t (t value)		    							\
	{												\
		return setter(tag, value);								\
	}

#define TAGGED_PTR_CUSTOM_DEFINE(t, tag, s)								\
	STRUCT_CREATE(PREFIX(t),									\
			intptr_t primary,								\
			t secondary)									\
													\
	extern t											\
	tagged_##t##_getter(tagged_##t* ptr)								\
	{												\
		TAGGED_PTR_ASSERT(ptr != NULL);								\
													\
		return ptr->secondary;									\
	}												\
													\
	extern void											\
	tagged_##t##_setter(tagged_##t* ptr, t value)							\
	{												\
		TAGGED_PTR_ASSERT(ptr != NULL);								\
													\
		ptr->secondary = value;									\
	}												\
													\
	extern tagged_##t										\
	tagged_create_##t (void* dst, t value)								\
	{												\
		return (PREFIX(t)) { tag, value };							\
	}

#define TAGGED_PTR_BASE_FIELDS                          \
    intptr_t primary;

#ifndef TAGGED_PTR_EXTENDED_FIELDS                      
#   define TAGGED_PTR_EXTENDED_FIELDS                   \
        double secondary;
#endif

#if !defined(TAGGED_PTR_NO_BASE_TYPES)
typedef struct {
    TAGGED_PTR_BASE_FIELDS
} tagged_t;
#endif 

typedef struct {
    TAGGED_PTR_BASE_FIELDS
    TAGGED_PTR_EXTENDED_FIELDS
} tagged_e_t;

#ifndef TAGGED_PTR_NO_BASE_TYPES
#   ifdef TAGGED_PTR_NO_DATA_PACKING
#   else
#       if defined(ARCH64)
#       define TAGGED_PTR_TYPE_MAP(XX)							                    \
	XX(TAG_PTR,	intptr_t, tagged_t,     tagged_ptr_intptr_t_setter, tagged_ptr_intptr_t_getter)     \
	XX(TAG_INT8,	int8_t,	  tagged_t,     tagged_ptr_int8_t_setter,   tagged_ptr_int8_t_getter)	    \
	XX(TAG_INT16,	int16_t,  tagged_t,     tagged_ptr_int16_t_setter,  tagged_ptr_int16_t_getter)      \
	XX(TAG_INT32,	int32_t,  tagged_t,     tagged_ptr_int32_t_setter,  tagged_ptr_int32_t_getter)      \
	XX(TAG_INT64,	int64_t,  tagged_e_t,   tagged_ptr_int64_t_setter,  tagged_ptr_int64_t_getter)      \
	XX(TAG_FLOAT,	float,	  tagged_e_t,   tagged_ptr_float_setter,    tagged_ptr_float_getter)        \
	XX(TAG_DOUBLE,	double,	  tagged_e_t,   tagged_ptr_double_setter,   tagged_ptr_double_getter)
#       else 
#       define TAGGED_PTR_TYPE_MAP(XX)							                    \
	XX(TAG_PTR,	intptr_t, tagged_t,     tagged_ptr_intptr_t_setter, tagged_ptr_intptr_t_getter)     \
	XX(TAG_INT8,	int8_t,	  tagged_t,     tagged_ptr_int8_t_setter,   tagged_ptr_int8_t_getter)	    \
	XX(TAG_INT16,	int16_t,  tagged_t,     tagged_ptr_int16_t_setter,  tagged_ptr_int16_t_getter)      \
	XX(TAG_INT32,	int32_t,  tagged_e_t,   tagged_ptr_int32_t_setter,  tagged_ptr_int32_t_getter)      \
	XX(TAG_INT64,	int64_t,  tagged_e_t,   tagged_ptr_int64_t_setter,  tagged_ptr_int64_t_getter)      \
	XX(TAG_FLOAT,	float,	  tagged_e_t,   tagged_ptr_float_setter,    tagged_ptr_float_getter)        \
	XX(TAG_DOUBLE,	double,	  tagged_e_t,   tagged_ptr_double_setter,   tagged_ptr_double_getter)
#       endif
#   endif
#endif

#if !defined(TAGGED_PTR_NO_TAG_ENUM)
typedef enum {
  TAGGED_PTR_UNKNOWN_HANDLE = 0,
#ifdef TAGGED_PTR_TYPE_MAP
#   define XX(uc, lc, cx, zx, vx) uc,
        TAGGED_PTR_TYPE_MAP(XX)
#   undef XX
#endif
#ifdef TAGGED_PTR_CUSTOM_TYPE_MAP
#	define XX(uc, lc, cx) uc,
		TAGGED_PTR_CUSTOM_TYPE_MAP(XX)
#	undef XX
#endif
  TAGGED_PTR_TYPE_MAX
} tagged_ptr_type;
#endif

#if defined TAGGED_PTR_TYPE_MAP && !defined TAGGED_PTR_NO_BASE_TYPES
#define XX(tag, type, struct, setter, getter)						\
	    TAGGED_PTR_DEFINE(type, tag, struct, setter, getter)
    TAGGED_PTR_TYPE_MAP(XX)
#undef XX
#endif
#ifdef TAGGED_PTR_CUSTOM_TYPE_MAP
#	define XX(tag, type, struct)									\
		TAGGED_PTR_CUSTOM_DEFINE(type, tag, struct)
	TAGGED_PTR_CUSTOM_TYPE_MAP(XX)
#	undef XX
#endif

#if !defined(TAGGED_PTR_NO_BASE_TYPES)
#   define TAGGED_PTR_TAG_BITS 0xffff
#else
#   if defined(ARCH64)
#       define TAGGED_PTR_TAG_BITS 0xffffffffffffffff;
#   else 
#       define TAGGED_PTR_TAG_BITS 0xffffffff;
#   endif
#endif

TAGGED_PTR_DEF int
tagged_ptr_get_tag(void* tagged)
{
    TAGGED_PTR_ASSERT(tagged != NULL);

	return ((tagged_t*)tagged)->primary & TAGGED_PTR_TAG_BITS;
}

TAGGED_PTR_DEF void
tagged_ptr_set_tag(void* ptr, int16_t tag)
{
    TAGGED_PTR_ASSERT(ptr != NULL);

    ((tagged_t*)ptr)->primary &= TAGGED_PTR_TAG_BITS;
    ((tagged_t*)ptr)->primary |= tag;
}

#if !defined(TAGGED_PTR_NO_BASE_TYPES)

TAGGED_PTR_DEF inline tagged_intptr_t
tagged_ptr_intptr_t_setter(int tag, intptr_t value)
{
	return (tagged_intptr_t){ (intptr_t)value << 16 | tag };
}

TAGGED_PTR_DEF inline intptr_t
tagged_ptr_intptr_t_getter(tagged_intptr_t* ptr)
{
	TAGGED_PTR_ASSERT(ptr != NULL);

	return *(intptr_t*)ptr->primary >> 16;
}

TAGGED_PTR_DEF inline tagged_int8_t
tagged_ptr_int8_t_setter(int tag, int8_t value)
{
	return (tagged_int8_t){ (intptr_t)value << 56 | tag };
}

TAGGED_PTR_DEF inline int8_t
tagged_ptr_int8_t_getter(tagged_int8_t* ptr)
{
	TAGGED_PTR_ASSERT(ptr != NULL);

	return ((*ptr).primary >> 56);
}

TAGGED_PTR_DEF inline tagged_int16_t
tagged_ptr_int16_t_setter(int tag, int16_t value)
{
	return (tagged_int16_t){ (intptr_t)value << 48 | tag };
}

TAGGED_PTR_DEF inline int16_t
tagged_ptr_int16_t_getter(tagged_int16_t* ptr)
{
	TAGGED_PTR_ASSERT(ptr != NULL);

	return ((*ptr).primary >> 48);
}

TAGGED_PTR_DEF inline tagged_int32_t
tagged_ptr_int32_t_setter(int tag, int32_t value)
{
#if defined(ARCH64)
	return (tagged_int32_t){ (intptr_t)value << 32 | tag };
#else 
    return (tagged_int32_t){ tag, value };
#endif
}

TAGGED_PTR_DEF inline int32_t
tagged_ptr_int32_t_getter(tagged_int32_t* ptr)
{
#if defined(ARCH64)
	return ((*ptr).primary >> 32);
#else 
    return ptr->secondary;
#endif 
}

TAGGED_PTR_DEF inline tagged_int64_t
tagged_ptr_int64_t_setter(int tag, int64_t value)
{
	tagged_int64_t result = (tagged_int64_t){ tag, 0.0f };

	__asm__("movq\t{%1, %0|%0, %1}" : "+x"(result.secondary) : "x"(value));
	return result;
}

TAGGED_PTR_DEF inline int64_t
tagged_ptr_int64_t_getter(tagged_int64_t* ptr)
{
	TAGGED_PTR_ASSERT(ptr != NULL);

	int64_t dest;

    	__asm__("movq\t{%1, %0|%0, %1}" : "+r"(dest) : "x"((*ptr).secondary));
	return dest;
}

TAGGED_PTR_DEF inline tagged_float
tagged_ptr_float_setter(int tag, float value)
{
	tagged_float result = (tagged_float){ tag, 0.0f };

	__asm__("movss\t{%1, %0|%0, %1}" : "+x"(result.secondary) : "x"(value));
	return result;
}

TAGGED_PTR_DEF inline float
tagged_ptr_float_getter(tagged_float* ptr)
{
	TAGGED_PTR_ASSERT(ptr != NULL);

	float dest;

    	__asm__("movss\t{%1, %0|%0, %1}" : "+x"(dest) : "x"(ptr->secondary));
	return dest;
}

TAGGED_PTR_DEF inline tagged_double
tagged_ptr_double_setter(int tag, double value)
{
	tagged_double result = (tagged_double){ tag, value };
	return result;
}

TAGGED_PTR_DEF inline double
tagged_ptr_double_getter(tagged_double* ptr)
{
	TAGGED_PTR_ASSERT(ptr != NULL);

	return (double)(ptr->secondary);
}
#endif

#undef ARCH32
#undef ARCH64

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif

#ifdef __cplusplus
}
#endif

#endif /* TAGGED_PTR_H_ */

