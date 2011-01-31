#pragma once
#include <uuid/uuid.h>

typedef struct
{
	char *name;
	char *table_name;
	// Field Definitions

} component_type;

typedef struct
{
	uuid_t id;
	uuid_t entity_id;
	component_type *type;

	char *data;
} component;
