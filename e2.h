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
	char *name;
	char *table_name;
	int num_fields;
	//todo: field definitions
} component_type;

typedef struct {
	uuid_t id;
	uuid_t entity_id;
	ComponentType *type;
	// fields
} component;

typedef struct {
	uuid_t id;
	char *name;
	char *script;
} system;


