#include "unity.h"
#include "Map.h"
#include <stdio.h>
#include "Person.h"
#include "mock_ComparePerson.h"
#include "mock_Hash.h"
#include "List.h"
#include "customAssert.h"

#define getPersonFromBucket(x) ((Person*)((List*)(x))->data)
void setUp(void)
{
}

void tearDown(void)
{
}

void test_mapNew_given_length_of_10_should_create_a_new_Map()
{
	Map *map = mapNew(10);
	
	TEST_ASSERT_NOT_NULL(map);
	TEST_ASSERT_NOT_NULL(map->bucket);
	TEST_ASSERT_EQUAL(10,map->length);
	TEST_ASSERT_EQUAL(0,map->size);
}
void test_mapStore_given_Ali_should_add_it_to_map()
{
	Map *map = mapNew(5);
	Person *person = personNew("Ali",25,70.3);
	
	hash_ExpectAndReturn(person,3);
	// comparePerson_ExpectAndReturn();
	
	mapStore(map,person,comparePerson,hash);
	TEST_ASSERT_NOT_NULL(map->bucket[3]);
	// TEST_ASSERT_EQUAL_STRING("Ali",((Person*)((List*)map->bucket[3])->data)->name);
	TEST_ASSERT_EQUAL_Person("Ali",25,70.3,getPersonFromBucket(map->bucket[3]));
	TEST_ASSERT_EQUAL(1,map->size);

}

void test_mapStore_given_Ali_but_Ali_is_in_the_Map_should_throw_ERR_SAME_ELEMENT_exception()
{
	CEXCEPTION_T e;
	
	Person *person = personNew("Ali",25,70.3);
	Map *map = mapNew(5);
	
	//map->bucket[3]=person  <<<<<<<<<<< this testing is wrong , because the bucket should contain the list rather that the person directly.
	List *list = listNew(person,NULL);
	map->bucket[3]=list;
	hash_ExpectAndReturn(person,3);
	comparePerson_ExpectAndReturn(person,person,1);
	
	Try
	{
		mapStore(map,person,comparePerson,hash);
		TEST_FAIL_MESSAGE("Expect ERR_SAME_ELEMENT exception to be thrown.");
	}
	Catch(e)
	{
		TEST_ASSERT_EQUAL(ERR_SAME_ELEMENT,e);
		TEST_ASSERT_NOT_NULL(map->bucket[3]);
		TEST_ASSERT_EQUAL_Person("Ali",25,70.3,getPersonFromBucket(map->bucket[3]));
	}
}

void test_mapStore_given_Zorro_should_add_it_to_map_when_ali_was_inside_the_map_with_same_value()
{
	Map *map = mapNew(5);
	Person *person = personNew("Ali",25,70.3);
	Person *person2 = personNew("Zorro",30,65.2);
	
	hash_ExpectAndReturn(person,3);
	hash_ExpectAndReturn(person2,3);
	comparePerson_ExpectAndReturn(person,person2,0);
	
	mapStore(map,person,comparePerson,hash);
	mapStore(map,person2,comparePerson,hash);
	TEST_ASSERT_NOT_NULL(map->bucket[3]);
	TEST_ASSERT_EQUAL_Person("Zorro",30,65.2,getPersonFromBucket(map->bucket[3]));
	TEST_ASSERT_EQUAL_Person("Ali",25,70.3,getPersonFromBucket(((List*)map->bucket[3])->next));
	TEST_ASSERT_EQUAL(1,map->size);

}

//************************************************************************
/*
	Given Ali and Ali is not in the map 
	return NULL
*/
void test_mapFind_will_return_NULL_when_specific_bucket_is_NULL()
{
	Map *map = mapNew(5);
	void *returnedData;
	Person *person = personNew("Ali",25,70.3);
	Person *testPerson = personNew("Ali",0,0);
	
	hash_ExpectAndReturn(testPerson,3);
	returnedData = mapFind(map, testPerson, comparePerson,hash);
	TEST_ASSERT_NULL(returnedData);
}
/*
	Given Ali and Ali is in the map 
	return Ali Object
*/
void test_mapFind_will_return_the_specific_object_in_the_map()
{
	Map *map = mapNew(5);
	void *returnedData;
	
	Person *person = personNew("Ali",25,70.3);
	Person *testPerson = personNew("Ali",0,0);
	
	List *list = listNew(person,NULL);
	map->bucket[3]=list;
	hash_ExpectAndReturn(testPerson,3);
	comparePerson_ExpectAndReturn(person,testPerson,1);
	
	returnedData = mapFind(map, testPerson, comparePerson,hash);
	TEST_ASSERT_NOT_NULL(returnedData);
	TEST_ASSERT_EQUAL_Person("Ali",25,70.3,returnedData);
	
}
/*
	Given Ali and Ali is not in the linked list(Not the first location)
	return NULL
*/
void test_mapFind_will_return_NULL_when_the_person_is_not_inside_the_map()
{
	Map *map = mapNew(5);
	void *returnedData;
	Person *person = personNew("Ali",25,70.3);
	Person *person2 = personNew("Zorro",25,70.3);
	Person *testPerson = personNew("Ali",0,0);
	
	List *list = listNew(person2,NULL);
	map->bucket[3]=list;
	
	hash_ExpectAndReturn(testPerson ,3);
	comparePerson_ExpectAndReturn(person2,testPerson ,0);

	returnedData = mapFind(map, testPerson , comparePerson,hash);
	TEST_ASSERT_NULL(returnedData);
}
/*
	Given Ali and Ali is in the linked list 
	return Ali object
*/
void test_mapFind_will_return_the_specific_object_when_the_object_is_being_at_location_2_onwards()
{
	Map *map = mapNew(5);
	void *returnedData;
	Person *person = personNew("Ali",25,70.3);
	Person *person2 = personNew("Zorro",25,70.3);
	Person *testPerson = personNew("Ali",0,0);
	
	/*
		person2->person
	*/
	List *list = listNew(person,NULL);
	List *list2 = listNew(person2,list);
	map->bucket[3]=list2;
	
	hash_ExpectAndReturn(testPerson,3);
	comparePerson_ExpectAndReturn(person2,testPerson,0);
	comparePerson_ExpectAndReturn(person,testPerson,1);

	returnedData = mapFind(map, testPerson, comparePerson,hash);
	TEST_ASSERT_NOT_NULL(returnedData);
	TEST_ASSERT_EQUAL_Person("Ali",25,70.3,returnedData);
}
//************************************************************************
/*
	Given Ali and Ali is not in the map 
	return NULL
*/
void test_mapRemove_will_return_NULL_when_specific_bucket_is_NULL()
{
	Map *map = mapNew(5);
	void *returnedData;
	Person *person = personNew("Ali",25,70.3);
	Person *testPerson = personNew("Ali",0,0);
	
	hash_ExpectAndReturn(testPerson,3);
	returnedData = mapRemove(map, testPerson, comparePerson,hash);
	TEST_ASSERT_NULL(returnedData);
}
/*
	Given Ali and Ali is in the map 
	return Ali Object
*/
void test_mapRemove_will_return_the_specific_object_in_the_map()
{
	Map *map = mapNew(5);
	void *returnedData;
	
	Person *person = personNew("Ali",25,70.3);
	Person *testPerson = personNew("Ali",0,0);
	
	List *list = listNew(person,NULL);
	map->bucket[3]=list;
	map->size++;
	hash_ExpectAndReturn(testPerson,3);
	comparePerson_ExpectAndReturn(person,testPerson,1);
	
	returnedData = mapRemove(map, testPerson, comparePerson,hash);
	TEST_ASSERT_NOT_NULL(returnedData);
	TEST_ASSERT_EQUAL_Person("Ali",25,70.3,returnedData);
	TEST_ASSERT_NULL(map->bucket[3]);
	TEST_ASSERT_EQUAL(0,map->size);
	
}
/*
	Given Ali and Ali is not in the linked list(Not the first location)
	return NULL
*/
void test_mapRemove_will_return_NULL_when_the_person_is_not_inside_the_map()
{
	Map *map = mapNew(5);
	void *returnedData;
	Person *person = personNew("Ali",25,70.3);
	Person *person2 = personNew("Zorro",25,70.3);
	Person *testPerson = personNew("Ali",0,0);
	
	List *list = listNew(person2,NULL);
	map->bucket[3]=list;
	map->size++;
	hash_ExpectAndReturn(testPerson,3);
	comparePerson_ExpectAndReturn(person2,testPerson,0);

	returnedData = mapRemove(map, testPerson, comparePerson,hash);
	TEST_ASSERT_NULL(returnedData);
	TEST_ASSERT_EQUAL(1,map->size);
}
/*
	Given Ali and Ali is in the linked list <- At location 2 onward
	return Ali object
*/
void test_mapRemove_will_return_the_specific_object_when_the_object_is_being_at_location_2_onwards()
{
	Map *map = mapNew(5);
	void *returnedData;
	Person *person = personNew("Ali",25,70.3);
	Person *person2 = personNew("Zorro",25,70.3);
	Person *testPerson = personNew("Ali",0,0);
	
	/*
		person2->person
	*/
	List *list = listNew(person,NULL);
	List *list2 = listNew(person2,list);
	map->bucket[3]=list2;
	map->size++;
	
	hash_ExpectAndReturn(testPerson,3);
	comparePerson_ExpectAndReturn(person2,testPerson,0);
	comparePerson_ExpectAndReturn(person,testPerson,1);

	returnedData = mapRemove(map, testPerson, comparePerson,hash);
	TEST_ASSERT_NOT_NULL(returnedData);
	TEST_ASSERT_EQUAL_Person("Ali",25,70.3,returnedData);
	TEST_ASSERT_NOT_NULL(map->bucket[3]);
	TEST_ASSERT_EQUAL_Person("Zorro",25,70.3,getPersonFromBucket(map->bucket[3]));
	TEST_ASSERT_NULL(((List*)(map->bucket[3]))->next);
	TEST_ASSERT_EQUAL(1,map->size);
}

/*
	Given Ali and Ali is in the linked list <- at the first location 
	return Ali object
*/
void test_mapRemove_will_return_the_specific_object_for_object_being_in_the_first_location()
{
	Map *map = mapNew(5);
	void *returnedData;
	Person *person = personNew("Ali",25,70.3);
	Person *person2 = personNew("Zorro",25,70.3);
	Person *testPerson = personNew("Ali",0,0);
	/*
		person->person2
	*/
	List *list = listNew(person2,NULL);
	List *list2 = listNew(person,list);
	map->bucket[3]=list2;
	map->size++;
	
	hash_ExpectAndReturn(testPerson,3);
	comparePerson_ExpectAndReturn(person,testPerson,1);

	returnedData = mapRemove(map, testPerson, comparePerson,hash);
	TEST_ASSERT_NOT_NULL(returnedData);
	TEST_ASSERT_EQUAL_Person("Ali",25,70.3,returnedData);
	TEST_ASSERT_NOT_NULL(map->bucket[3]);
	TEST_ASSERT_EQUAL_Person("Zorro",25,70.3,getPersonFromBucket(map->bucket[3]));
	TEST_ASSERT_NULL(((List*)(map->bucket[3]))->next);
	TEST_ASSERT_EQUAL(1,map->size);
	
}
/*
	Given Ali and Ali is in the linked list <- between two list 
	
	Zorro->Ali->Kevin
	
	return Ali object
*/
void test_mapRemove_will_return_the_specific_object_which_being_in_between_two_lists()
{
	Map *map = mapNew(5);
	void *returnedData;
	Person *person = personNew("Zorro",25,70.3);
	Person *person2 = personNew("Ali",25,70.3);
	Person *person3 = personNew("Kevin",25,70.3);
	Person *testPerson = personNew("Ali",0,0);
	/*
		person->person2
	*/
	List *list = listNew(person3,NULL);
	List *list2 = listNew(person2,list);
	List *list3 = listNew(person,list2);
	map->bucket[3]=list3;
	map->size++;
	
	hash_ExpectAndReturn(testPerson,3);
	comparePerson_ExpectAndReturn(person,testPerson,0);
	comparePerson_ExpectAndReturn(person2,testPerson,1);

	returnedData = mapRemove(map, testPerson, comparePerson,hash);
	TEST_ASSERT_NOT_NULL(returnedData);
	TEST_ASSERT_EQUAL_Person("Ali",25,70.3,returnedData);
	TEST_ASSERT_NOT_NULL(map->bucket[3]);
	TEST_ASSERT_EQUAL_Person("Zorro",25,70.3,getPersonFromBucket(map->bucket[3]));
	TEST_ASSERT_NOT_NULL(((List*)(map->bucket[3]))->next);
	TEST_ASSERT_EQUAL_Person("Kevin",25,70.3,getPersonFromBucket(((List*)(map->bucket[3]))->next));
	TEST_ASSERT_EQUAL(1,map->size);
	
}










