#include "service.h"
#include "debug.h"
#include "Protocols.h"

#define PROCTOCOLS_Q_SIZE 5

//testing procedures:

//crafted/obtained message without bit stuffing, hard-wired FCS

//crafted message without bit stuffing, our calculated FCS should matches the correct value

//particularly nastily crafted message ?

//random messages


/*
 * AX. 25
 */

//#define FLAG_SENDING_PERIOD_IN_MS 1//probably not necessary

#define N_FLAGS_BETWEEN_PACKETS 10

//SSIDs
#define BLUESAT_GS_SSID 0x0
#define BLUESAT_SAT_SSID 0x0

//NOTE : this number is randomly picked, replace it with a proper one
#define MAX_AX25_INFO_FIELD_BYTES 50



#define AX25_CRC_POLYNOMIAL_FLIPED 0x8408 // AX25 crc polynomial reversed bits by bits

void AX25fcsCalc( char input[], unsigned int len,unsigned char *fcsByte0, unsigned char * fcsByte1);




typedef struct {
	enum PROTOCOL_TO_USE_SENDING protocol;
	char * data;
	//size must be in bytes
	unsigned int size;
} protMessage;








static portTASK_FUNCTION(vProtocolsTask, pvParameters);

static TaskToken Proctocols_TaskToken;

void vProtocols_Service_Init(unsigned portBASE_TYPE uxPriority){
	vDebugPrint(Proctocols_TaskToken, "Protocols Task running \r\n", NO_INSERT, NO_INSERT, NO_INSERT);

	Proctocols_TaskToken = ActivateTask(TASK_PROTOCOLS,
			"Protcolos Task",
			SEV_TASK_TYPE,
			uxPriority,
			SERV_STACK_SIZE,
			vProtocolsTask);
	vActivateQueue(Proctocols_TaskToken, PROCTOCOLS_Q_SIZE);

}

static portTASK_FUNCTION(vProtocolsTask, pvParameters){
	(void) pvParameters;
	//UnivRetCode enResult;
	//MessagePacket incoming_packet;
	//struct protBufferItem * protMessage;
	while(1){

			//enResult = enGetRequest(Proctocols_TaskToken, &incoming_packet, portMAX_DELAY);
			//if (enResult != URC_SUCCESS) continue;

			//extract the pointer from the IPC message
			//protMessage = (struct protBufferItem *) incoming_packet.Data;

			//do the actual stuff here

			//print message
			//vDebugPrint(Proctocols_TaskToken, "Protocols Task running \r\n", NO_INSERT, NO_INSERT, NO_INSERT);

			//complete request by passing the status to the sender
			//vCompleteRequest(incoming_packet.Token, URC_SUCCESS);
		}
}







/*
 * This function calculates the FCS checksum based on a MATLAB implementation
 * Which is obtained from :
 *		The Cyclic Redundancy Check (CRC) for AX.25
 *		Bill Newhall, KB2BRD
 *		billnewhall@yahoo.com
 *		Boulder, Colorado
 *
 */
void AX25fcsCalc( char input[], unsigned int len,unsigned char *fcsByte0, unsigned char * fcsByte1){
	//short should be 16bits, change data type if it isn't
	unsigned int inputbit;
	unsigned int inputbyte;
	char ch1,ch2,ch3;
	unsigned short shiftRegister,shiftedOutBit,xorMask;

	ch1 = len/100+'0';
	ch2 = (len%100)/10+'0';
	ch3 = len%10+'0';

	for(inputbyte=0,inputbit=0,shiftRegister=0xFFFF; inputbyte < len;){
		shiftedOutBit = shiftRegister & 0x0001;//shift the rightmost bit out

		shiftRegister = shiftRegister>>1;//shift one bit to the right

		//translate SR=xor(SR, XORMask) and XORMask = ...
		if( (((input[inputbyte] & (0x1<<inputbit))>>inputbit) ^ shiftedOutBit)){
			xorMask = AX25_CRC_POLYNOMIAL_FLIPED;
		}
		else xorMask = 0;

		shiftRegister = shiftRegister ^ xorMask;

		inputbit++;
		if(inputbit == 8){
			inputbit=0;
			inputbyte++;
		}
	}

	//flip and reverse the shift register to get the result

	shiftRegister =~shiftRegister;

	/*
	 * The FCS are transmitted bit 15(leftmost) first
	 *
	 * This ought to be send from left to right(for the whole 16 bits!)
	 * Also note that the modem sends bytes in Reverse
	 * Also note that the ShiftRegister's MSB is the rightmost bit
	 * i.e. no reverse inside bytes
	 */
	(*fcsByte0) = shiftRegister&0x00FF;
	(*fcsByte1) = (shiftRegister&0xFF00)>>8;
	return;
}