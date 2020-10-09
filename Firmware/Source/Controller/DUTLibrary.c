// Header
#include "DUTLibrary.h"

// Variables
//
static const DL_DUTConfiguration DUTConfigStorage[] = {
		{A1,	OneInput, OneOutput, NoSupply},
		{I1,	OneInput, OneOutput, NoSupply},
		// I6
		{B1,	OneInput, OneOutput, NoSupply},
		{B2,	OneInput, OneOutput, NoSupply},
		{B5,	OneInput, OneOutput, SingleDCSupply},
		{V1,	OneInput, OneOutput, NoSupply},
		{V2,	OneInput, OneOutput, NoSupply},
		{V104,	OneInput, OneOutput, NoSupply},
		{V108,	OneInput, OneOutput, DoubleDCSupply},
		{L1,	OneInput, OneOutput, NoSupply},
		{L2,	TwoInputs, TwoOutputs, NoSupply},
		{D1,	OneInput, ThreeOutputs, NoSupply},
		{D2,	OneInput, ThreeOutputs, NoSupply},
		{D192,	OneInput, ThreeOutputs, NoSupply}
};

// Forward functions
uint16_t DUTLIB_GetStorageSize();

// Functions
uint16_t DUTLIB_GetStorageSize()
{
	return sizeof(DUTConfigStorage) / sizeof(DUTConfigStorage[0]);
}
//------------------------------------------------

bool DUTLIB_ExtractConfiguration(DL_Case Case, const DL_DUTConfiguration *Result)
{
	uint8_t DevNum = DUTLIB_GetStorageSize();
	for(uint8_t i = 0; i < DevNum; ++i)
	{
		if(Case == DUTConfigStorage[i].Case)
		{
			Result = &DUTConfigStorage[i];
			return true;
		}
	}
	return false;
}
//------------------------------------------------
