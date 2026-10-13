/** tests for tag types at their default configuration */

#include "../tagged_ptr.h"

#include <stdio.h>
#include <unistd.h>
#include <assert.h>

int 
main(void)
{
	int	ptr_value = 666;
	int*	ptrx = &ptr_value;
	double	doublex = 666.0f;
	float	floatx = 666.0f;
	int64_t	int64x = 666;
	int32_t	int32x = 666;
	int16_t	int16x = 666;
	int8_t	int8x = 66;

	tagged_intptr_t ptr	= tagged_create_intptr_t((intptr_t)&ptrx);
	tagged_int8_t	ptr_int8 = tagged_create_int8_t(int8x);
	tagged_int16_t	ptr_int16 = tagged_create_int16_t(int16x);
	tagged_int32_t	ptr_int32 = tagged_create_int32_t(int32x);
	tagged_int64_t	ptr_int64 = tagged_create_int64_t(int64x);
	tagged_float	ptr_float = tagged_create_float(floatx);
	tagged_double   ptr_double = tagged_create_double(doublex);

    	assert(tagged_ptr_get_tag(&ptr) == TAG_PTR);
    	assert(tagged_ptr_get_tag(&ptr_int8) == TAG_INT8);
    	assert(tagged_ptr_get_tag(&ptr_int16) == TAG_INT16);
    	assert(tagged_ptr_get_tag(&ptr_int32) == TAG_INT32);
    	assert(tagged_ptr_get_tag(&ptr_int64) == TAG_INT64);
    	assert(tagged_ptr_get_tag(&ptr_float) == TAG_FLOAT);
    	assert(tagged_ptr_get_tag(&ptr_double) == TAG_DOUBLE);
    	assert(tagged_ptr_int8_t_getter(&ptr_int8) == int8x);
    	assert(tagged_ptr_int16_t_getter(&ptr_int16) == int16x);
    	assert(tagged_ptr_int32_t_getter(&ptr_int32) == int32x);
    	assert(tagged_ptr_int64_t_getter(&ptr_int64) == int64x);

	printf("ptr %p %p %d			%d \n", &ptr, ptrx, *ptrx, tagged_ptr_get_tag((tagged_t*)&ptr));
	printf("ptr %p %p %d			%d \n", &ptr_int8, &int8x, tagged_ptr_int8_t_getter(&ptr_int8), tagged_ptr_get_tag((tagged_t*)&ptr_int8));
	printf("ptr %p %p %d			%d \n", &ptr_int16, &ptr_int16, tagged_ptr_int16_t_getter(&ptr_int16), tagged_ptr_get_tag((tagged_t*)&ptr_int16));
	printf("ptr %p %p %d			%d \n", &ptr_int32, &ptr_int32, tagged_ptr_int32_t_getter(&ptr_int32), tagged_ptr_get_tag((tagged_t*)&ptr_int32));
	printf("ptr %p %p %lld			%d \n", &ptr_int64, &ptr_int64, tagged_ptr_int64_t_getter(&ptr_int64), tagged_ptr_get_tag((tagged_t*)&ptr_int64));
	printf("ptr %p %p %lf			%d \n", &ptr_float, &ptr_float, tagged_ptr_float_getter(&ptr_float), tagged_ptr_get_tag((tagged_t*)&ptr_float));
	printf("ptr %p %p %lf			%d \n", &ptr_double, &ptr_double, tagged_ptr_double_getter(&ptr_double), tagged_ptr_get_tag((tagged_t*)&ptr_double));


    return 0;
}
