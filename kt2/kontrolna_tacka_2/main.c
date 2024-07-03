//////////////////////////////////////////////////////////////////////////////
// * File name: main.c
// *
// *
// * Description: Projekat(Sistem za otklanjanje suma iz signala) - Kontrolna tacka 2
// * Course: ADOZ
// * Year: 2023
// *
// * Author: Petar Dimitrijevic RA 207/2018
// * Faculty of Technical Sciences, Novi Sad
// *
//////////////////////////////////////////////////////////////////////////////

#include "stdio.h"
#include "ezdsp5535.h"
#include "aic3204.h"
#include "ezdsp5535_aic3204_dma.h"
#include "WAVheader.h"
#include "iir.h"
#include "fir.h"

//zadatak 1
#include "IIR_Notch_Filters.h"

//zadatak 2
#include "FIR_Filters.h"

static WAV_HEADER outputWAVhdr;
static WAV_HEADER inputWAVhdr;

#define SAMPLE_RATE 48000L
#define GAIN 0
//#define IIR_ORDER 2
#define IIR_ORDER 3


#pragma DATA_ALIGN(InputBufferL,4)
Int16 InputBufferL[AUDIO_IO_SIZE];
#pragma DATA_ALIGN(InputBufferR,4)
Int16 InputBufferR[AUDIO_IO_SIZE];

#pragma DATA_ALIGN(OutputBufferL,4)
Int16 OutputBufferL[AUDIO_IO_SIZE];
#pragma DATA_ALIGN(OutputBufferR,4)
Int16 OutputBufferR[AUDIO_IO_SIZE];

/*Define history buffers and Rd/Wr pointers*/
Int16 History_Buffer_R[HP_LENGTH_128];
Int16 History_Buffer_L[HP_LENGTH_128];
Int16 History_Buffer_Impuls[HP_LENGTH_128];

//zadatak 1
Int16 History_Buffer_XX[2];
Int16 History_Buffer_YY[2];
Int16 History_Buffer_Impuls_X[2];
Int16 History_Buffer_Impuls_Y[2];

Int16 History_Buffer_X[IIR_ORDER][2];
Int16 History_Buffer_Y[IIR_ORDER][2];

Int16 impuls_in[AUDIO_IO_SIZE];
Int16 impuls_out[AUDIO_IO_SIZE];
Int16 impuls_in_fir[AUDIO_IO_SIZE];
Int16 impuls_out_fir[AUDIO_IO_SIZE];

Int16 Output_Buffer_L_temp[AUDIO_IO_SIZE];
Int16 Output_Buffer_R_temp[AUDIO_IO_SIZE];


void main( void )
{   
	int i;
	int k;
	Int32 j;
	Int32 number_of_blocks;

	/* Initialize BSL */
    EZDSP5535_init( );

	printf("\n***Program has Started***\n");
		
    /* Initialise hardware interface and I2C for code */
    aic3204_hardware_init();
	
	aic3204_set_input_filename("../Output2Test.wav");
	aic3204_set_output_filename("../IzlasTest.wav");
	
    /* Initialise the AIC3204 codec */
	aic3204_init();
          
    aic3204_dma_init();
	
	aic3204_read_wav_header(&inputWAVhdr);

    // Set up output WAV header
    outputWAVhdr = inputWAVhdr;

    number_of_blocks = inputWAVhdr.data.SubChunk2Size/
    			(inputWAVhdr.fmt.NumChannels*inputWAVhdr.fmt.BitsPerSample*AUDIO_IO_SIZE/8);

    aic3204_write_wav_header(&outputWAVhdr);


    /*Initialize history buffers to 0 */


    //zadatak 1
    for(i = 0; i < 2; i++){
    	History_Buffer_XX[i] = 0;
    	History_Buffer_YY[i] = 0;
    	History_Buffer_Impuls_X[i] = 0;
    	History_Buffer_Impuls_Y[i] = 0;
    }

    //zadatak 2
    //history bufferi za iir filter
    for(k = 0; k < IIR_ORDER; k++){
        for ( i = 0 ; i < 2 ; i++){
            History_Buffer_X[k][i] = 0;
            History_Buffer_Y[k][i] = 0;
        }
    }

    //history bufferi za fir filter
    for( i = 0; i < HP_LENGTH_128; i++){
    	History_Buffer_R[i] = 0;
    	History_Buffer_L[i] = 0;
    	History_Buffer_Impuls[i] = 0;
    }
    //kreiranje impulsnih signala
    impuls_in_fir[0] = 32767;
    impuls_in[0] = 32767;
    for( i = 1; i < AUDIO_IO_SIZE; i++){
    	impuls_in[i] = 0;
    	impuls_in_fir[i] = 0;
    }


	for(i=0;i<number_of_blocks;i++)
	{
		aic3204_read_block(InputBufferL, InputBufferR);

		for(j = 0; j < AUDIO_IO_SIZE; j++)
		{
			//zadatak 1
			//OutputBufferL[j] = second_order_IIR(InputBufferL[j], notch_2kHz_2nd_order, History_Buffer_XX, History_Buffer_YY);
			//OutputBufferR[j] = second_order_IIR(InputBufferR[j], notch_2kHz_2nd_order, History_Buffer_XX, History_Buffer_YY);
			//impuls_out[j] = second_order_IIR(impuls_in[j], notch_2kHz_2nd_order, History_Buffer_Impuls_X, History_Buffer_Impuls_Y);

			//zadatak 2 fir 77 reda i iir cetvrtog reda
			//Output_Buffer_L_temp[j] = fir_basic(InputBufferL[j], high_pass_4900Hz_76th_order, History_Buffer_L, HP_LENGTH_76);
			//Output_Buffer_R_temp[j] = fir_basic(InputBufferR[j], high_pass_4900Hz_76th_order, History_Buffer_R, HP_LENGTH_76);
			//impuls_out_fir[j] = fir_basic(impuls_in_fir[j], high_pass_4900Hz_76th_order, History_Buffer_Impuls, HP_LENGTH_76);
			//OutputBufferL[j] = fourth_order_IIR(Output_Buffer_L_temp[j], notch_2kHz_4th_order, History_Buffer_X, History_Buffer_Y);
			//OutputBufferR[j] = fourth_order_IIR(Output_Buffer_R_temp[j], notch_2kHz_4th_order, History_Buffer_X, History_Buffer_Y);

			//zadatak 2 fir 129 reda i iir sestog reda, ovim filtriranjem se dobija konacan signal
			Output_Buffer_L_temp[j] = fir_basic(InputBufferL[j], high_pass_4900Hz_128th_order, History_Buffer_L, HP_LENGTH_128);
			Output_Buffer_R_temp[j] = fir_basic(InputBufferR[j], high_pass_4900Hz_128th_order, History_Buffer_R, HP_LENGTH_128);
			//impuls_out_fir[j] = fir_basic(impuls_in_fir[j], high_pass_4900Hz_128th_order, History_Buffer_Impuls, HP_LENGTH_128);
			OutputBufferL[j] = sixth_order_IIR(Output_Buffer_L_temp[j], notch_2kHz_6th_order, History_Buffer_X, History_Buffer_Y);
			OutputBufferR[j] = sixth_order_IIR(Output_Buffer_R_temp[j], notch_2kHz_6th_order, History_Buffer_X, History_Buffer_Y);
		}
		aic3204_write_block(OutputBufferL, OutputBufferR);
	}
    	
	/* Disable I2S and put codec into reset */ 
    aic3204_disable();

    printf( "\n***Program has Terminated***\n" );
	SW_BREAKPOINT;
}

