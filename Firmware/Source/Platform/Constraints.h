#ifndef __CONSTRAINTS_H
#define __CONSTRAINTS_H

// Include
#include "stdinc.h"
#include "DataTable.h"
#include "Global.h"

// Definitions
#define NO		0
#define YES		1

#define NODE_ID_MIN					1
#define NODE_ID_MAX					255

#define TIMEOUT_MIN					1000
#define TIMEOUT_MAX					60000
#define TIMEOUT_DEF					5000

#define CTRL_PULSE_LENGH_MIN		5
#define CTRL_PULSE_LENGH_MAX		500
#define CTRL_PULSE_LENGH_DEF		10

// Types
typedef struct __TableItemConstraint
{
	Int16U Min;
	Int16U Max;
	Int16U Default;
} TableItemConstraint;

// Variables
extern const TableItemConstraint NVConstraint[DATA_TABLE_NV_SIZE];
extern const TableItemConstraint VConstraint[DATA_TABLE_WP_START - DATA_TABLE_WR_START];

#endif // __CONSTRAINTS_H
