#ifndef __GLOBAL_H
#define __GLOBAL_H

// Definitions
// 
#define	SCCI_TIMEOUT_TICKS			1000	// ������� ���������� SCCI (� ��)
#define	BCCIM_TIMEOUT_TICKS			1000	// ������� ���������� ������ BCCI (� ��)
#define EP_WRITE_COUNT				0		// ���������� �������� ��� ������
#define EP_COUNT					1		// ���������� �������� ��� ������
#define ENABLE_LOCKING				FALSE	// ������ NV ��������� �������
#define XCCI_EP_SIZE				2000	// ������ ������� ��� ����������� EP

// ��������� ���������
#define TIME_LED_BLINK				500		// ������� ����������� (� ��)
#define TIME_SLAVE_STATE_UPDATE		100		// �������� ������ ��������� ������� ����� (� ��)

#define MAX_SLAVE_NODES				8		// ������������ ����� slave-�����

#endif //  __GLOBAL_H
