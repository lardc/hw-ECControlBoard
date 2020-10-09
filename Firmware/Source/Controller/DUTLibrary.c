// Header
#include "DUTLibrary.h"

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
//------------------------------------------------

uint16_t DUTLIB_GetStorageSize()
{
	return sizeof(DUTConfigStorage) / sizeof(DUTConfigStorage[0]);
}
//------------------------------------------------


