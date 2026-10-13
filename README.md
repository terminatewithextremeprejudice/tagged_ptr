# tagged_ptr

## QUICK NOTES
Tags themselves are 16-bit in length by default, they reside at the lower bits of the tagged_ptr,
while the value itself is stored in the higher bits of the struct, or in a case of custom types 
and data types that are longer than 32-bit in length separate secondary variable is used to store value.
For data types lesser than 64-bit in length, tag is packed in the lower bits of the given value; 
to save up some space. 
When using data packing, tags reserve 32-bit of data on 64-bit platforms and 16-bit on 32-bit ones.
When data packing is disabled entire size of the struct is utilised instead.

## DOCUMENTATION 

By default tagged_ptr provides support for following data types out of the box:
- intptr_t    
- int8_t      
- int16_t 
- int32_t
- int64_t 
- float 
- double 
        
Base types have predefined unique tags defined on tagged_ptr_type enum as following:
```
TAG_PTR     0
TAG_INT8    1
TAG_INT16   2
TAG_INT32   3
TAG_INT64   4
TAG_FLOAT   5
TAG_DOUBLE  6
``````

To provide custom definition of tags just define TAGGED_PTR_NO_TAG_ENUM before including the header.
```    
// i.e. it should look like this 
#include ...
#include ...
#define TAGGED_PTR_NO_TAG_ENUM 
#include "tagged_ptr.h"
```

If you wish to do this you need to define base tags you need to provide your own definitions instead 
or this won't compile.

To remove tagging of base data types altogether, 
just define TAGGED_PTR_NO_BASE_TYPES before including the header.
```
// i.e. it should look like this 
#include ...
#include ...
#define TAGGED_PTR_NO_BASE_TYPES
#include "tagged_ptr.h"

    Or just redefine:
    #define TAGGED_PTR_TYPE_MAP(XX)					                            \
	    XX(TAG_PTR,		intptr_t, ..., ..., ...)                                \
	    XX(TAG_INT8,	int8_t,	  ..., ..., ...)                                \
	    XX(TAG_INT16,	int16_t,  ..., ..., ...)                                \
	    XX(TAG_INT32,	int32_t,  ..., ..., ...)                                \
	    XX(TAG_INT64,	int64_t,  ..., ..., ...)                                \
	    XX(TAG_FLOAT,	float,	  ..., ..., ...)                                \
	    XX(TAG_DOUBLE,	double,	  ..., ..., ...)
    #endif

```

Extending tagged data types thru TAGGED_PTR_CUSTOM_TYPE_MAP macro is also possible:
```
typedef struct custom_type {
	uintptr_t	a;
	void*		b;
} custom_type_t;

#define TAGGED_PTR_CUSTOM_TYPE_MAP(XX)						\
    XX(TAG_CUSTOM_TYPE, custom_type_t, custom_type_t)
```        
