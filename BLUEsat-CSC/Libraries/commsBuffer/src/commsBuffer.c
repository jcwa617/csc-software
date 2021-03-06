#include "commsBuffer.h"
#include "debug.h"
// TaskToken globtoken;
extern TaskToken         sharedTaskToken;
/*
 *  Bit Stuffing Functions
 * ---------------------
 * */
UnivRetCode stuffBufMSBtoLSB (char * inputBuff, unsigned int input_size, buffer * outputBuff)
{
   UnivRetCode result = URC_FAIL;
   char temp;
   buffer input;
   if (inputBuff==NULL || outputBuff == NULL ||input_size==0)
   {
         return result;
   }
   if (initBuffer(&input, inputBuff, input_size) == URC_FAIL)return result;
   //vDebugPrint(sharedTaskToken, "%1x\n\r",&outputBuff->buff[outputBuff->index] , NO_INSERT, NO_INSERT);
   while (bitPopMSBtoLSB (&input, &temp, sizeof (char))==URC_SUCCESS)
      {
	   //vDebugPrint(sharedTaskToken, "%d\n\r",temp, NO_INSERT, NO_INSERT);
         outputBuff->connectedOnes = (temp==0)?0: outputBuff->connectedOnes+1;
         if (bitPushMSBtoLSB (outputBuff, temp)== URC_FAIL)return result;
         //vDebugPrint(sharedTaskToken, "%1x\n\r",&outputBuff->buff[outputBuff->index] , NO_INSERT, NO_INSERT);
         if ( outputBuff->connectedOnes > PatternLimit)
            {
               outputBuff->connectedOnes = 0; // Take into account the 1 to be added after this if block
               if (bitPushMSBtoLSB (outputBuff, 0)== URC_FAIL)return result;
            }

      }
   return URC_SUCCESS;
}

UnivRetCode stuffBufLSBtoMSB (char * inputBuff, unsigned int input_size, buffer * outputBuff)
{
   UnivRetCode result = URC_FAIL;
   char temp;
   buffer input;
   if (inputBuff==NULL || outputBuff == NULL ||input_size==0)
   {
         return result;
   }

   if (initBuffer(&input, inputBuff, input_size) == URC_FAIL)return result;

   while (bitPopLSBtoMSB (&input, &temp, sizeof (char))==URC_SUCCESS)
      {

         outputBuff->connectedOnes = (temp==0)?0: outputBuff->connectedOnes+1;
         if (bitPushLSBtoMSB (outputBuff, temp)== URC_FAIL)return result;
         if ( outputBuff->connectedOnes > PatternLimit)
            {
               outputBuff->connectedOnes = 0; // Take into account the 1 to be added after this if block
               if (bitPushLSBtoMSB (outputBuff, 0)== URC_FAIL)return result;
            }
      }
   return URC_SUCCESS;
}

UnivRetCode pushBuf (char * inputBuff, unsigned int input_size, buffer * outputBuff)
{
   UnivRetCode result = URC_FAIL;
   char temp;
   buffer input;
   if (inputBuff==NULL || outputBuff == NULL ||input_size==0)
   {
         return result;
   }
   if (initBuffer(&input, inputBuff, input_size) == URC_FAIL)return result;
   while (bitPopLSBtoMSB (&input, &temp, sizeof (char))==URC_SUCCESS)
      {
         if (bitPushLSBtoMSB (outputBuff, temp)== URC_FAIL)return result;
      }
   return URC_SUCCESS;
}

UnivRetCode initBuffer(buffer * input, char * buff, unsigned int size)
{
   if (input == NULL || buff == NULL ||size == 0) return URC_FAIL;
   input->buff = buff;
   input->buff_size = size;
   input->byte_pos = 0;
   input->index = 0;
   input->connectedOnes=0;
   return URC_SUCCESS;
}

UnivRetCode bitPopLSBtoMSB (buffer* buff, char * out, unsigned int size)
{
   UnivRetCode result = URC_FAIL;
   char temp  = 0;
   if (size == 0||out ==NULL ||buff==NULL)
      {
         return result;
      }
   if (buff->index>=buff->buff_size)
      {
         return result;
      }
   temp = buff->buff[buff->index];
   *out = (temp& (LSB_bit_mask<<buff->byte_pos++))?1:0;

   if (buff->byte_pos%8==0)
      {
         ++buff->index;
         buff->byte_pos = 0;
      }
   return URC_SUCCESS;
}

UnivRetCode bitPopMSBtoLSB (buffer* buff, char * out, unsigned int size)
{
   UnivRetCode result = URC_FAIL;
   char temp  = 0;
   if (size == 0||out ==NULL ||buff==NULL)
      {
         return result;
      }
   if (buff->index>=buff->buff_size)
      {
         return result;
      }
   temp = buff->buff[buff->index];
   *out = (temp& (MSB_bit_mask>>buff->byte_pos++))?1:0;

   if (buff->byte_pos%8==0)
      {
         ++buff->index;
         buff->byte_pos = 0;
      }
   return URC_SUCCESS;
}

UnivRetCode bitPushLSBtoMSB (buffer* buff, char in)//lsbtomsb
{
   UnivRetCode result = URC_FAIL;
   unsigned int  temp; // Use unsigned int due to left shifting later on
   if (buff==NULL)
      {
         return result;
      }
   if (buff->index>=buff->buff_size)
      {
         return result;
      }
   temp = (in == 0)?0:LSB_bit_mask;
   buff->buff[buff->index] = (buff->buff[buff->index]| (temp<<buff->byte_pos++));
   if (buff->byte_pos%8==0)
      {
         ++buff->index;
         buff->byte_pos = 0;
      }
   return URC_SUCCESS;
}

UnivRetCode bitPushMSBtoLSB (buffer* buff, char in)//lsbtomsb
{
   UnivRetCode result = URC_FAIL;
   unsigned int  temp; // Use unsigned int due to left shifting later on
   if (buff==NULL)
      {
         return result;
      }
   if (buff->index>=buff->buff_size)
      {
         return result;
      }
   temp = (in == 0)?0:MSB_bit_mask;
   buff->buff[buff->index] = (buff->buff[buff->index]| (temp>>buff->byte_pos++));
   if (buff->byte_pos%8==0)
      {
         ++buff->index;
         buff->byte_pos = 0;
      }
   return URC_SUCCESS;
}
