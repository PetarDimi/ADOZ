/*****************************************************************************/
/*                                                                           */
/* FILENAME                                                                  */
/* 	 ezdsp5535_aic3204_dma.c                                                 */
/*                                                                           */
/* DESCRIPTION                                                               */
/*   Setup functions for aic3204 codec on the EZDSP 5535 (Simulator Version) */
/*                                                                           */
/*                                                                           */
/* REVISION                                                                  */
/*   Revision: 1.00	                                                         */
/*   Author  : Dejan Bokan                                                   */
/*****************************************************************************/
#include "tistdtypes.h"
#include <stdio.h>
#include "aic3204.h"
#include "ezdsp5535_aic3204_dma.h"

#pragma DATA_ALIGN(SimulatorInput, 4)
Int16 SimulatorInput[AUDIO_IO_SIZE*4];
#pragma DATA_ALIGN(SimulatorOutput, 4)
Int16 SimulatorOutput[AUDIO_IO_SIZE*4];

void aic3204_dma_init(void)
{
}

void aic3204_read_block(Int16* buffer_left, Int16* buffer_right)
{
	int i;

	if(input_file == NULL)
	{
		printf("ERROR: Invalid input file!\n");
		return;
	}

	int n = fread(SimulatorInput, 1, AUDIO_IO_SIZE*4, input_file);
	n >>= 1;
	for(i = 0; i < AUDIO_IO_SIZE; i++)
	{
		if(i < n)
		{
			buffer_left[i] = SimulatorInput[4*i] | (SimulatorInput[4*i+1]<<8);
			buffer_right[i] = SimulatorInput[4*i+2] | (SimulatorInput[4*i+3]<<8);
		} else
		{
			buffer_left[i] = 0;
			buffer_right[i] = 0;
		}
	}
}

void aic3204_write_block(Int16* buffer_left, Int16* buffer_right)
{	
	int i;

	if(output_file == NULL)
	{
		printf("ERROR: Invalid output file!\n");
		return;
	}

	for(i = 0; i < AUDIO_IO_SIZE; i++)
	{
		SimulatorOutput[4*i] = buffer_left[i] & 0x00FF;
		SimulatorOutput[4*i+1] = buffer_left[i] >> 8;
		SimulatorOutput[4*i+2] = buffer_right[i] & 0x00FF;
		SimulatorOutput[4*i+3] = buffer_right[i] >> 8;
	}
	fwrite(SimulatorOutput, sizeof(Int16), AUDIO_IO_SIZE*4, output_file);
}

void aic3204_write_data(Int16* buffer, Int16 length)
{
	int i;

	if(output_file == NULL)
	{
		printf("ERROR: Invalid output file!\n");
		return;
	}

	for(i = 0; i < AUDIO_IO_SIZE; i++)

	{
		SimulatorOutput[2*i] = buffer[i] & 0x00FF;
		SimulatorOutput[2*i+1] = buffer[i] >> 8;

	}

	fwrite(SimulatorOutput, sizeof(Int16), AUDIO_IO_SIZE*2, output_file);
}




