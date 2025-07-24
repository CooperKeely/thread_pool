#include "../lib/unity/unity.h"
#include "../include/arena.h"

// Basic Tests
void test_init_valid_size(){}
void test_reset_clears_allocations(){}
void test_deinit_destorys_arena(){}

// Edge Cases
void test_allocate_zero_bytes(){}
void test_allocate_exact_remaining_space(){}
void test_allocate_one_byte_over_capacity(){}
void test_allocate_way_over_capacity(){}
void test_maximum_arena_size(){}
void test_minimum_arena_size(){}

// Error Handling
void test_null_arena_parameter(){}
void test_invalid_arena_size(){}
void test_double_deinit(){}

// Memory Safety
void test_write_to_allocated_memory(){}
void test_no_memory_leaks(){}

int main(void){
	UNITY_BEGIN();
	
	// Basic Tests
	RUN_TEST(test_init_valid_size);
	RUN_TEST(test_reset_clears_allocations);
	RUN_TEST(test_deinit_destorys_arena);

	// Edge Cases
	RUN_TEST(test_allocate_zero_bytes);
	RUN_TEST(test_allocate_exact_remaining_space);
	RUN_TEST(test_allocate_one_byte_over_capacity);
	RUN_TEST(test_allocate_way_over_capacity);
	RUN_TEST(test_maximum_arena_size);
	RUN_TEST(test_minimum_arena_size);

	// Error Handeling
	RUN_TEST(test_null_arena_parameter);	
	RUN_TEST(test_invalid_arena_size);	
	RUN_TEST(test_double_deinit);	

	// Memory Safety
	RUN_TEST(test_write_to_allocated_memory);	
	RUN_TEST(test_no_memory_leaks);	

	UNITY_END();

}
