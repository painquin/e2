#pragma once


#include <uuid/uuid.h>

/*
   Utility Functions (util.c)
*/
enum LOG_LEVEL
{
	LOG_DBG,
	LOG_MON,
	LOG_ERR
};

extern const char *log_level_names[];
extern void e2log(enum LOG_LEVEL, const char *, ... );


typedef struct {
	uuid_t id;
} Entity;


typedef struct {
	uuid_t id;
	char *name;
	int num_fields;
	//todo: field definitions
} ComponentType;

typedef struct {
	uuid_t id;
	ComponentType *type;
	Entity *entity;
	// fields
} Component;

typedef struct {
	uuid_t id;
	char *name;
	char *script;
} System;


