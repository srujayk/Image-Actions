/**
 * recover.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Recovers JPEGs from a forensic image.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

int main(void)
{
	FILE* input = fopen("card.raw", "r");	

	if(input == NULL)
	{	
			fclose(input);
			fprintf(stderr, "Couldn't open the card.\n");
			return 1;
	}
	
	uint8_t jpgsig1[4] = {0xff, 0xd8, 0xff, 0xe0};
	uint8_t jpgsig2[4] = {0xff, 0xd8, 0xff, 0xe1};
	uint8_t buffer[512];
	uint8_t jpgsig[4];
	fread(buffer, 512, 1, input);
	
	int count = 0;
	int init = 0;
	FILE* output;

	while(fread(buffer, 512, 1, input) > 0)
	{
			for(int i = 0; i < 4; i++)
			{
					jpgsig[i] = buffer[i];
			}

			if((memcmp(jpgsig1, jpgsig, 4) == 0 ) || (memcmp(jpgsig2, jpgsig, 4) == 0))
			{
                    if(init == 0)
					{
					    char filename[8];
					    sprintf(filename, "%03d.jpg", count);
						output = fopen(filename, "w");
						fwrite(buffer, sizeof(buffer), 1, output);
						init = 1;
					}
					if(init == 1)
					{
					    char filename[8];
					    sprintf(filename, "%03d.jpg", count);
						fclose(output);
						output = fopen(filename, "w");
						fwrite(buffer, sizeof(buffer), 1, output);
						count++;
					}
			}
			else
			{
					if(init == 1)
					{
							fwrite(buffer, sizeof(buffer), 1, output);
					}
			}
	}
		
    if(output != NULL)
    {
      fclose(output);
    }

	fclose(input);
	return 0;
}