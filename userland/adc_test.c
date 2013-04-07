/*
 * main.c
 *
 *  Created on: 01.04.2013
 *      Author: christian-karsch-x
 */

#include <fcntl.h> /* open() O_RDONLY=0 O_RDWR=2 */
#include <unistd.h> /* close() read() write() */
#include <sys/ioctl.h> /* ioctl() */
#include <stdint.h>
#include <stdio.h>
#include <sys/time.h>

#include "../kernel/lpc313x_adc_dev.h"

int
main( void ) {
	int fd;
	char sVal[7]; /* { '0' , 'x' , '3 ' , 'F' , 'F' , '\n' ,'\0' } */
	struct timeval tstart;
	struct timeval tend;
	int i;
	float t;
	float sps;
	

	
	LPC313X_ADC_WRITE wdata = LPC313X_ADC_CONFIG_DEFAULT;
	LPC313X_ADC_READ rdata = { 0 };
	LPC313X_ADC_VALUE retValue;

	printf( "Test old sytle\n\n" );
	printf( "open O_RDWR\n"); fd = open( "/dev/"LPC313X_ADC_DEVNAME , O_RDWR );
	printf( "write \"0\"\n"); write( fd, "0", 2 );
	printf( "close\n"); close( fd );
	printf( "open O_RDONLY\n"); fd = open( "/dev/"LPC313X_ADC_DEVNAME , O_RDONLY );
	printf( "read sVal\n"); read( fd, sVal, sizeof( sVal ) );
	printf( "close\n"); close( fd );
	printf( "CH0: %s", sVal );
	printf( "open O_RDONLY\n"); fd = open( "/dev/"LPC313X_ADC_DEVNAME , O_RDONLY );
	printf( "read sVal\n"); read( fd, sVal, sizeof( sVal ) );
	printf( "close\n"); close( fd );
	printf( "CH0: %s", sVal );

	printf( "\nTest old sytle with multiple read and writes\n\n" );
	printf( "open O_RDWR\n"); fd = open( "/dev/"LPC313X_ADC_DEVNAME , O_RDWR );
	printf( "write \"0\"\n");write( fd, "0", 2 );
	printf( "read sVal\n"); read( fd, sVal, sizeof( sVal ) );
	printf( "CH0: %s", sVal );
	printf( "read sVal\n"); read( fd, sVal, sizeof( sVal ) );
	printf( "CH0: %s", sVal );
	printf( "read sVal\n"); read( fd, sVal, sizeof( sVal ) );
	printf( "CH0: %s", sVal );
	printf( "write \"1\"\n");write( fd, "1", 2 );
	printf( "read sVal\n"); read( fd, sVal, sizeof( sVal ) );
	printf( "CH1: %s", sVal );
	printf( "read sVal\n"); read( fd, sVal, sizeof( sVal ) );
	printf( "CH1: %s", sVal );
	printf( "read sVal\n"); read( fd, sVal, sizeof( sVal ) );
	printf( "CH1: %s", sVal );
	printf( "close\n"); close( fd );

	
	printf( "\nTest new sytle\n\n" );
	
	printf( "open\n"); fd = open( "/dev/"LPC313X_ADC_DEVNAME , O_RDWR );
	printf( "write wdata\n"); write( fd, &wdata, sizeof( wdata ) );
	printf( "read rdata\n"); read( fd, &rdata, sizeof( rdata ) );
	printf( "Value=%i; Channel=%i; Errorcode=%i; Powermode=%i; Legacymode=%i; Debugmode=%i\n", rdata.value,
			rdata.channel, rdata.flags.errorcode, rdata.flags.power, rdata.flags.legacy, rdata.flags.debug );
	printf( "Resulution: CH0=%i; CH1=%i; CH2=%i; CH3=%i\n", rdata.resulution[0], rdata.resulution[1],
			rdata.resulution[2], rdata.resulution[3] );
	printf( "Averageing: CH0=%i; CH1=%i; CH2=%i; CH3=%i\n", rdata.average[0], rdata.average[1], rdata.average[2],
			rdata.average[3] );

	printf( "read rdata\n"); read( fd, &rdata, sizeof( rdata ) );
	printf( "Value=%i; Channel=%i; Errorcode=%i\n", rdata.value, rdata.channel, rdata.flags.errorcode );
	printf( "read rdata\n"); read( fd, &rdata, sizeof( rdata ) );
	printf( "Value=%i; Channel=%i; Errorcode=%i\n", rdata.value, rdata.channel, rdata.flags.errorcode );

	wdata.channel = 1;
	printf( "write wdata (channel = 1)\n"); write( fd, &wdata, sizeof( wdata ) );
	printf( "read rdata\n"); read( fd, &rdata, sizeof( rdata ) );
	printf( "Value=%i; Channel=%i; Errorcode=%i\n", rdata.value, rdata.channel, rdata.flags.errorcode );

	wdata.channel = 3;
	printf( "write wdata (channel = 3)\n"); write( fd, &wdata, sizeof( wdata ) );
	printf( "read rdata\n"); read( fd, &rdata, sizeof( rdata ) );
	printf( "Value=%i; Channel=%i; Errorcode=%i\n", rdata.value, rdata.channel, rdata.flags.errorcode );
	
	printf( "\nTest ioctl\n\n" );
	printf( "ioctl LPC313X_ADC_SET_CHANNEL, 1\n");  ioctl( fd, LPC313X_ADC_SET_CHANNEL, 1 );
	printf( "ioctl LPC313X_ADC_GET_VALUE, 0\n");    retValue.vint = ioctl( fd, LPC313X_ADC_GET_VALUE, 0 );
	printf( "Value=%i; Channel=%i; Errorcode=%i\n", retValue.value, retValue.channel, retValue.flags.errorcode);
	printf( "ioctl LPC313X_ADC_SET_CHANNEL, 3\n");  ioctl( fd, LPC313X_ADC_SET_CHANNEL, 3 );
	printf( "ioctl LPC313X_ADC_GET_VALUE, 0\n");    retValue.vint = ioctl( fd, LPC313X_ADC_GET_VALUE, 0 );
	printf( "Value=%i; Channel=%i; Errorcode=%i\n", retValue.value, retValue.channel, retValue.flags.errorcode);
	

	printf( "\nTest Powermode=ON\n\n");
	wdata.channel = 0;
	wdata.flags.power = LPC313X_ADC_ON;
	printf( "write wdata\n"); write( fd, &wdata, sizeof( wdata ) );
	printf( "ioctl LPC313X_ADC_GET_VALUE, 0\n");    retValue.vint = ioctl( fd, LPC313X_ADC_GET_VALUE, 0 );
	printf( "Value=%i; Channel=%i; Errorcode=%i\n", retValue.value, retValue.channel, retValue.flags.errorcode);	
	
	
	printf( "\nSpeedtest 1: using 'read' one channel (ADC Power: auto)\n\n" );
	wdata.channel = 0;
	wdata.flags.power = LPC313X_ADC_AUTO;
	printf( "write wdata\n"); write( fd, &wdata, sizeof( wdata ) );
	gettimeofday(&tstart, NULL );
	for(i = 0; i < 10000; i++) {
	  read( fd, &rdata, sizeof( rdata ) );
	}
	gettimeofday(&tend, NULL );
	t = (tend.tv_sec - tstart.tv_sec) + (float)(tend.tv_usec - tstart.tv_usec) / 1000000.0;
	t = t / 10000.0;
	sps = 1 / t;
	printf("t=%f SPS=%f\n", t, sps);

	printf( "\nSpeedtest 2: using 'read' two channels (ADC Power: auto)\n\n" );
	wdata.channel = 0;
	wdata.flags.power = LPC313X_ADC_AUTO;
	printf( "write wdata\n"); write( fd, &wdata, sizeof( wdata ) );
	gettimeofday(&tstart, NULL );
	for(i = 0; i < 10000; i+=2) {
	  ioctl( fd, LPC313X_ADC_SET_CHANNEL, 0 );
	  read( fd, &rdata, sizeof( rdata ) );
	  ioctl( fd, LPC313X_ADC_SET_CHANNEL, 1 );
	  read( fd, &rdata, sizeof( rdata ) );  
	}
	gettimeofday(&tend, NULL );
	t = (tend.tv_sec - tstart.tv_sec) + (float)(tend.tv_usec - tstart.tv_usec) / 1000000.0;
	t = t / 10000.0;
	sps = 1 / t;
	printf("t=%f SPS=%f\n", t, sps);
	
	
	printf( "\nSpeedtest 3: using 'ioctl' one channel (ADC Power: auto)\n\n" );
	wdata.channel = 0;
	wdata.flags.power = LPC313X_ADC_AUTO;
	printf( "write wdata\n"); write( fd, &wdata, sizeof( wdata ) );
	gettimeofday(&tstart, NULL );
	for(i = 0; i < 10000; i++) {
	  retValue.vint = ioctl( fd, LPC313X_ADC_GET_VALUE, 0 );
	}
	gettimeofday(&tend, NULL );
	t = (tend.tv_sec - tstart.tv_sec) + (float)(tend.tv_usec - tstart.tv_usec) / 1000000.0;
	t = t / 10000.0;
	sps = 1 / t;
	printf("t=%f SPS=%f\n", t, sps);
	
	printf( "\nSpeedtest 4: using 'ioctl' two channels (ADC Power: auto)\n\n" );
	wdata.channel = 0;
	wdata.flags.power = LPC313X_ADC_AUTO;
	printf( "write wdata\n"); write( fd, &wdata, sizeof( wdata ) );
	gettimeofday(&tstart, NULL );
	for(i = 0; i < 10000; i+=2) {
	  ioctl( fd, LPC313X_ADC_SET_CHANNEL, 0 );
	  retValue.vint = ioctl( fd, LPC313X_ADC_GET_VALUE, 0 );
	  ioctl( fd, LPC313X_ADC_SET_CHANNEL, 1 );
	  retValue.vint = ioctl( fd, LPC313X_ADC_GET_VALUE, 0 );
	}
	gettimeofday(&tend, NULL );
	t = (tend.tv_sec - tstart.tv_sec) + (float)(tend.tv_usec - tstart.tv_usec) / 1000000.0;
	t = t / 10000.0;
	sps = 1 / t;
	printf("t=%f SPS=%f\n", t, sps);
	
	
	
	
	printf( "\nSpeedtest 5: using 'read' one channel (ADC Power: always on)\n\n" );
	wdata.channel = 0;
	wdata.flags.power = LPC313X_ADC_ON;
	printf( "write wdata\n"); write( fd, &wdata, sizeof( wdata ) );
	gettimeofday(&tstart, NULL );
	for(i = 0; i < 10000; i++) {
	  read( fd, &rdata, sizeof( rdata ) );
	}
	gettimeofday(&tend, NULL );
	t = (tend.tv_sec - tstart.tv_sec) + (float)(tend.tv_usec - tstart.tv_usec) / 1000000.0;
	t = t / 10000.0;
	sps = 1 / t;
	printf("t=%f SPS=%f\n", t, sps);

	printf( "\nSpeedtest 6: using 'read' two channels (ADC Power: always on)\n\n" );
	wdata.channel = 0;
	wdata.flags.power = LPC313X_ADC_ON;
	printf( "write wdata\n"); write( fd, &wdata, sizeof( wdata ) );
	gettimeofday(&tstart, NULL );
	for(i = 0; i < 10000; i+=2) {
	  ioctl( fd, LPC313X_ADC_SET_CHANNEL, 0 );
	  read( fd, &rdata, sizeof( rdata ) );
	  ioctl( fd, LPC313X_ADC_SET_CHANNEL, 1 );
	  read( fd, &rdata, sizeof( rdata ) );  
	}
	gettimeofday(&tend, NULL );
	t = (tend.tv_sec - tstart.tv_sec) + (float)(tend.tv_usec - tstart.tv_usec) / 1000000.0;
	t = t / 10000.0;
	sps = 1 / t;
	printf("t=%f SPS=%f\n", t, sps);
	
	
	printf( "\nSpeedtest 7: using 'ioctl' one channel (ADC Power: always on)\n\n" );
	wdata.channel = 0;
	wdata.flags.power = LPC313X_ADC_ON;
	printf( "write wdata\n"); write( fd, &wdata, sizeof( wdata ) );
	gettimeofday(&tstart, NULL );
	for(i = 0; i < 10000; i++) {
	  retValue.vint = ioctl( fd, LPC313X_ADC_GET_VALUE, 0 );
	}
	gettimeofday(&tend, NULL );
	t = (tend.tv_sec - tstart.tv_sec) + (float)(tend.tv_usec - tstart.tv_usec) / 1000000.0;
	t = t / 10000.0;
	sps = 1 / t;
	printf("t=%f SPS=%f\n", t, sps);
	
	printf( "\nSpeedtest 8: using 'ioctl' two channels (ADC Power: always on)\n\n" );
	wdata.channel = 0;
	wdata.flags.power = LPC313X_ADC_ON;
	printf( "write wdata\n"); write( fd, &wdata, sizeof( wdata ) );
	gettimeofday(&tstart, NULL );
	for(i = 0; i < 10000; i+=2) {
	  ioctl( fd, LPC313X_ADC_SET_CHANNEL, 0 );
	  retValue.vint = ioctl( fd, LPC313X_ADC_GET_VALUE, 0 );
	  ioctl( fd, LPC313X_ADC_SET_CHANNEL, 1 );
	  retValue.vint = ioctl( fd, LPC313X_ADC_GET_VALUE, 0 );
	}
	gettimeofday(&tend, NULL );
	t = (tend.tv_sec - tstart.tv_sec) + (float)(tend.tv_usec - tstart.tv_usec) / 1000000.0;
	t = t / 10000.0;
	sps = 1 / t;
	printf("t=%f SPS=%f\n", t, sps);

	printf("close\n"); close( fd );
	
	return 0;
}
