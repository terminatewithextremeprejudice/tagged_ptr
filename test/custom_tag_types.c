/** tests for tag types with custom configuration */

#include <stdio.h>
#include <unistd.h>
#include <assert.h>

typedef struct ptr_custom_type {
    int     a;
    float   b;
} ptr_custom_type_t;

#define TAGGED_PTR_NO_BASE_TYPES
#include "../tagged_ptr.h"

TAGGED_PTR_CUSTOM_DEFINE(ptr_custom_type_t, 666, ptr_custom_type);

int
main(void)
{
	
	ptr_custom_type_t p;
	p.a = 1;
	p.b = 666.0;

	tagged_ptr_custom_type_t ptr = tagged_create_ptr_custom_type_t(p);
	assert(tagged_ptr_get_tag(&ptr) == 666);
	assert(tagged_ptr_custom_type_t_getter(&ptr).a == p.a);
	assert(tagged_ptr_custom_type_t_getter(&ptr).b == p.b);

    	return 0;
}
