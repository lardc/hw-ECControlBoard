#ifndef __COMMUTATOR_H
#define __COMMUTATOR_H

// Includes
#include "stdinc.h"
#include "CommutationTable.h"

// Functions
//
void COMM_DisconnectSimpleRelays();
void COMM_DisconnectBistableRelays();
void COMM_SwitchBistableRelay(uint8_t IndexRelay, bool NewState);
void COMM_SwitchSimpleRelay(uint8_t IndexRelay, bool NewState);

#endif // __COMMUTATOR_H
