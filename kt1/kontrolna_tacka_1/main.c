//////////////////////////////////////////////////////////////////////////////
// * File name: main.c
// *                                                                          
// * Description:  Main function.
// *                                                                          
// * Copyright (C) 2011 Texas Instruments Incorporated - http://www.ti.com/ 
// * Copyright (C) 2011 Spectrum Digital, Incorporated
// *                                                                          
// *                                                                          
// *                                                                          
//////////////////////////////////////////////////////////////////////////////

#include "stdio.h"
#include "ezdsp5535.h"
#include "ezdsp5535_i2c.h"
#include "aic3204.h"
#include "ezdsp5535_aic3204_dma.h"
#include "ezdsp5535_i2s.h"
#include "WAVheader.h"
#include "fir.h"
#include "echo.h"

#include "low_pass_34th_order.h"
#include "low_pass_120th_order.h"

static WAV_HEADER outputWAVhdr;
static WAV_HEADER inputWAVhdr;

//za zadatke 2 i 3 koristimo 48kHz sampling_rate, a za zadatak 4 koristimo 16kHz zbog delay funkcije
//#define SAMPLE_RATE 48000L
#define SAMPLE_RATE 16000L
#define GAIN 0

#pragma DATA_ALIGN(InputBufferL,4)
Int16 InputBufferL[AUDIO_IO_SIZE];
#pragma DATA_ALIGN(InputBufferR,4)
Int16 InputBufferR[AUDIO_IO_SIZE];

#pragma DATA_ALIGN(OutputBufferL,4)
Int16 OutputBufferL[AUDIO_IO_SIZE];
#pragma DATA_ALIGN(OutputBufferR,4)
Int16 OutputBufferR[AUDIO_IO_SIZE];


/* TO DO: Define history buffers and Rd/Wr pointers*/
/* Your code here */
Int16 impuls_in[AUDIO_IO_SIZE];
Int16 impuls_out[AUDIO_IO_SIZE];
Int16 history_buffer_L[AUDIO_IO_SIZE];
Int16 history_buffer_R[AUDIO_IO_SIZE];
Int16 history_buffer_impuls[AUDIO_IO_SIZE];
Uint16 p_state_L;
Uint16 p_state_R;
Uint16 p_state_impuls;
/*
 *
 *  main( )
 *
 */
void main( void )
{   
	int i, j;
	Int32 number_of_blocks;
	/* Initialize BSL */
	EZDSP5535_init( );


	/* Initialise hardware interface and I2C for code */
	aic3204_hardware_init();

	aic3204_set_input_filename("../17.wav");
	aic3204_set_output_filename("../Output2v2.wav");

	/* Initialise the AIC3204 codec */
	aic3204_init();

	aic3204_dma_init();

	aic3204_read_wav_header(&inputWAVhdr);

	// Set up output WAV header
	outputWAVhdr = inputWAVhdr;

	number_of_blocks = inputWAVhdr.data.SubChunk2Size/
				(inputWAVhdr.fmt.NumChannels*inputWAVhdr.fmt.BitsPerSample*AUDIO_IO_SIZE/8);

	aic3204_write_wav_header(&outputWAVhdr);

	set_sampling_frequency_and_gain(SAMPLE_RATE, GAIN);

    /* TO DO: Initialize history buffers to 0 */
    /* Your code here */
	impuls_in[0] = 16000;
	for(i = 0; i < AUDIO_IO_SIZE; i++){
		impuls_in[i] = 16000;
	}

	//zadatak 2 i 3,
	for(i = 0; i < LP_LENGTH_34; i++){
		history_buffer_L[i] = 0;
		history_buffer_R[i] = 0;
		history_buffer_impuls[i] = 0;
	}

	//zadatak 4, poziv inita za echo i delay
	echo_init(12000, 16384);


	for(i = 0; i < number_of_blocks; ++i)
	{
		aic3204_read_block(InputBufferL, InputBufferR);

		/* TO DO: Invoke filtering routine */
		/* Your code here */

		for(j = 0; j < AUDIO_IO_SIZE; j++)
		{
			//zadatak 2
			OutputBufferL[j] = fir_circular(InputBufferL[j], low_pass_1500Hz_34th_order, history_buffer_L, LP_LENGTH_34, &p_state_L);
			OutputBufferR[j] = fir_circular(InputBufferR[j], low_pass_1500Hz_34th_order, history_buffer_R, LP_LENGTH_34, &p_state_R);
			//impuls_out[j] = fir_circular(impuls_in[j], low_pass_1500Hz_34th_order, history_buffer_impuls, LP_LENGTH_34, &p_state_impuls);

			//zadatak 3
			//OutputBufferL[j] = fir_circular(InputBufferL[j], low_pass_1500Hz_120th_order, history_buffer_L, LP_LENGTH_120, &p_state_L);
			///OutputBufferR[j] = fir_circular(InputBufferR[j], low_pass_1500Hz_120th_order, history_buffer_R, LP_LENGTH_120, &p_state_R);
			//impuls_out[j] = fir_circular(impuls_in[j], low_pass_1500Hz_120th_order, history_buffer_impuls, LP_LENGTH_120, &p_state_impuls);

			//zadatak 4
			//OutputBufferL[j] = echo(InputBufferL[j]);
			//OutputBufferR[j] = echo(InputBufferR[j]);
		}
		aic3204_write_block(OutputBufferL, OutputBufferR);
	}

	/* Disable I2S and put codec into reset */ 
    aic3204_disable();

    printf( "\n***Program has Terminated***\n" );
	SW_BREAKPOINT;
}

