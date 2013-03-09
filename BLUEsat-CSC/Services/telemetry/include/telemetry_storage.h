/*
 * telemetry_storage.h
 *
 *  Created on: Feb 2, 2013
 *      Author: andyc
 */

#ifndef TELEMETRY_STORAGE_H_
#define TELEMETRY_STORAGE_H_



#define TELEM_STORAGE_IDLIST_SIZE 128

void telemetry_storage_init(TaskToken telemTaskToken);
size_t telemetry_storage_write(TaskToken telemTaskToken, char *buf, size_t nbytes);
size_t telemetry_storage_read(TaskToken telemTaskToken, char *buf, size_t nbytes);

#endif /* TELEMETRY_STORAGE_H_ */