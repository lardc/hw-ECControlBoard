// Header
#include "DUTLibrary.h"

static const DL_DUTConfiguration DUTConfigArray[] = {
		{A1, OneInput, OneOutput, NoSupply, VDC, Triac},
		{A1, OneInput, OneOutput, NoSupply, IDC, Triac},
		{A1, OneInput, OneOutput, NoSupply, VDC, DualMOSFET},
		{A1, OneInput, OneOutput, NoSupply, IDC, DualMOSFET},

		{I1, OneInput, OneOutput, NoSupply, VDC, Triac},
		{I1, OneInput, OneOutput, NoSupply, IDC, Triac},
		{I1, OneInput, OneOutput, NoSupply, VAC, Triac},
		{I1, OneInput, OneOutput, NoSupply, VDC, SingleMOSFET_IGBT},
		{I1, OneInput, OneOutput, NoSupply, IDC, SingleMOSFET_IGBT},

		{B1, OneInput, OneOutput, NoSupply, VDC, Triac},
		{B1, OneInput, OneOutput, NoSupply, IDC, Triac},
		{B1, OneInput, OneOutput, NoSupply, VAC, Triac},
		{B1, OneInput, OneOutput, NoSupply, VDC, SingleMOSFET_IGBT},
		{B1, OneInput, OneOutput, NoSupply, IDC, SingleMOSFET_IGBT},

		{B2, OneInput, OneOutput, NoSupply, VDC, Triac},
		{B2, OneInput, OneOutput, NoSupply, IDC, Triac},
		{B2, OneInput, OneOutput, NoSupply, VAC, Triac},
		{B2, OneInput, OneOutput, NoSupply, VDC, SingleMOSFET_IGBT},
		{B2, OneInput, OneOutput, NoSupply, IDC, SingleMOSFET_IGBT},

		{B5, OneInput, OneOutput, SingleDCSupply, VDC, SingleMOSFET_IGBT},

		{V1, OneInput, OneOutput, NoSupply, VDC, Triac},
		{V1, OneInput, OneOutput, NoSupply, IDC, Triac},
		{V1, OneInput, OneOutput, NoSupply, VDC, SingleMOSFET_IGBT},
		{V1, OneInput, OneOutput, NoSupply, IDC, SingleMOSFET_IGBT},

		{V2, OneInput, OneOutput, NoSupply, VDC, Triac},
		{V2, OneInput, OneOutput, NoSupply, IDC, Triac},
		{V2, OneInput, OneOutput, NoSupply, VDC, SingleMOSFET_IGBT},
		{V2, OneInput, OneOutput, NoSupply, IDC, SingleMOSFET_IGBT},
};
//------------------------------------------------
