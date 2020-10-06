#ifndef __DUT_LIB_H
#define __DUT_LIB_H

#include "stdinc.h"

// Types
typedef enum __DL_Case
{
	A1 = 0,
	I1 = 1,
	I6 = 2,
	B1 = 3,
	B2 = 4,
	B5 = 5,
	V1 = 6,
	V2 = 7,
	V104 = 8,
	V108 = 9,
	L1 = 10,
	L2 = 11,
	D1 = 12,
	D2 = 13,
	D192 = 14

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
	VDC = 0,
	VAC = 1,
	IDC = 2

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
