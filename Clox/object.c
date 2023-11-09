#include <stdio.h>
#include <string.h>

#include "memory.h"
#include "object.h"
#include "value.h"
#include "vm.h"

#define ALLOCATE_OBJ(type, objectType) \
	(type*)allocateObject(sizeof(type), objectType)

static Obj* allocateObject(size_t size, ObjType type) {
	Obj* object = (Obj*)reallocate(NULL, 0, size);
	object->type = type;

	object->next = vm.objects;
	vm.objects = object;
	return object;
}

static ObjString* allocateString(char* chars, int length) {ObjString* string = ALLOCATE(ObjString, sizeof(ObjString) + (length + 1) * sizeof(char) + 1);
	string->obj.type = OBJ_STRING;
	string->obj.next = vm.objects;
	string->length = length;
	memcpy(string->chars, chars, length + 1);
	
	vm.objects = (Obj*)string;
	return string;
}

ObjString* takeString(char* chars, int length) {
	ObjString* result = allocateString(chars, length);
	FREE_ARRAY(char, chars, length + 1);
	return result;
}

ObjString* copyString(const char* chars, int length) {
	char* heapChars = ALLOCATE(char, length + 1);
	memcpy(heapChars, chars, length);
	heapChars[length] = '\0';
	return allocateString(heapChars, length);
}

void printObject(Value value) {
	ObjType type = OBJ_TYPE(value);
	switch (type) {
		case OBJ_STRING:
			printf("%s", AS_CSTRING(value));
			break;
	}
}