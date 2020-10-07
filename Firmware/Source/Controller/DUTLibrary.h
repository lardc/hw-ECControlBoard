#ifndef __DUT_LIB_H
#define __DUT_LIB_H

#include "stdinc.h"

// Types
typedef enum __DL_Case
{
	A1 = 1,
	I1 = 2,
	I6 = 3,
	B1 = 4,
	B2 = 5,
	B5 = 6,
	V1 = 7,
	V2 = 8,
	V104 = 9,
	V108 = 10,
	L1 = 11,
	L2 = 12,
	D1 = 13,
	D2 = 14,
	D192 = 15

} DL_Case;

typedef enum __DL_InputPositions
{
	OneInput = 0,
	TwoInputs = 1

} DL_InputPositions;

typedef enum __DL_OutputPositions
{
	OneOutput = 0,
	TwoOutputs = 1,
	ThreeOutputs = 2

} DL_OutputPositions;

typedef enum __DL_AuxPowerSupply
{
	NoSupply = 0,
	SingleDCSupply = 1,
	DoubleDCSupply = 2

} DL_AuxPowerSupply;

typedef enum __DL_InputType
{
	VDC = 1,
	IDC = 2,
	VAC = 3

} DL_InputType;

typedef enum __DL_OutputType
{
	Triac = 0,
	SingleMOSFET_IGBT = 1,
	DualMOSFET = 2

} DL_OutputType;

typedef struct __DL_DUTConfiguration
{
	DL_Case Case;
	DL_InputPositions InputPositionsNum;
	DL_OutputPositions OutputPositionsNum;
	DL_AuxPowerSupply AuxPowerSupply;
	DL_InputType InputType;
	DL_OutputType OutputType;

} DL_DUTConfiguration, *pDL_DUTConfiguration;

#endif // __DUT_LIB_H
