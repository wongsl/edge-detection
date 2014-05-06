#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ROWS   	       	 		 480 
#define COLS				 640
#define LOGICAL_X_MIN			-4.0
#define LOGICAL_X_MAX			 4.0
#define LOGICAL_Y_MIN			-4.0
#define LOGICAL_Y_MAX			 4.0
#define PI 				3.14159265

void clear( unsigned char image[][COLS] );
void header( int row, int col, unsigned char head[32] );

int main( int argc, char **argv )
{
	int			countWhitePixel, threshold, i, j, x, y, dx, dy, max, min, SGM;
	int			vote[762][10] = {{0},{0}};
	int			tmptheta, tmprho;
	float			scale, z, theta, maxrho, rho;
	FILE			*fp;
	unsigned char	image[ROWS][COLS], tmp[ROWS][COLS];
	char *ifile, *ofile;
	unsigned char head[32];
	
	/* Example to show how to do format conversion */
	/* Input image file */
	ifile = "image.raw";
	/* Output image file */
	ofile = "image1.ras";


	/* Clear image buffer */
	clear(image);

	/* Read in a raw image */
	/* Open the file */
	if (( fp = fopen( ifile, "rb" )) == NULL )
	{
		fprintf( stderr, "error: couldn't open %s\n", argv[1] );
		exit( 1 );
	}			

	/* Read the file */
	for ( i = 0; i < ROWS ; i++ )
		if ( fread( image[i], 1, COLS, fp ) != COLS )
		{
			fprintf( stderr, "error: couldn't read enough stuff\n" );
			exit( 1 );
		}

	/* Close the file */
	fclose( fp );

	/* Create a header */
	header(ROWS, COLS, head);

/* Modify this for the homework. Don't forget to write the image buffer out to a local file */
	for(y = 0;y < ROWS; y++)
	{
		for(x = 0; x < COLS; x++)
		{
		tmp[y][x] = image[y][x];
		}
	}

 
rho = maxrho = 0;
countWhitePixel = dx = dy = SGM = max = min  = 0;
x = y = 1;

	for(y = 1; y < ROWS-1; y++)
	{
		for(x = 1; x < COLS-1; x++)
		{
		/*Sobel operator to pixel*/
		dx = -1 * image[y-1][x-1] - 2 * image[y][x-1] - image[y+1][x-1] + image[y-1][x+1] + 2* image[y][x+1] + image[y+1][x+1];  /* dE/dx */
			
		dy =  -1 * image[y-1][x-1] - 2 * image[y-1][x] - image[y-1][x+1] + image[y+1][x-1] + 2* image[y+1][x] + image[y+1][x+1];   /* dE/dy */		
	
		SGM = dx * dx + dy * dy; /*dx^2 + dy^2*/
			
		if(SGM > max)
		{
		max = SGM; /*to find value to scale*/
		}

		if(SGM < min)
		{
		min = SGM;
		}	

		}
	}
		
		scale = (float)(max) / 255; /*normalizing scale*/

	for(y = 0; y < ROWS; y++)
	{
		for(x = 0; x < COLS; x++)
		{
		dx = -1 * image[y-1][x-1] - 2 * image[y][x-1] - image[y+1][x-1] + image[y-1][x+1] + 2* image[y][x+1] + image[y+1][x+1];  /* dE/dx */
			
		dy =  -1 * image[y-1][x-1] - 2 * image[y-1][x] - image[y-1][x+1] + image[y+1][x-1] + 2* image[y+1][x] + image[y+1][x+1];   /* dE/dy */		
	
		SGM = dx * dx + dy * dy; /*dx^2 + dy^2*/
	
		tmp[y][x] = (float) SGM / scale; /*SGM Image*/
		}
	}
	
	for(y = 0; y < ROWS; y++)
	{
		for(x = 0; x < COLS; x++)
		{
		image[y][x] = tmp[y][x]; /*replace image with SGM image*/
		}
	}



	threshold = 30;


	for(y = 0; y < ROWS; y++)
	{
		for(x = 0; x < COLS; x++)
		{
			if(image[y][x] < threshold)
			{
			image[y][x] = 0;
			}

			else
			{
			countWhitePixel++;
			image[y][x] = 255;
			}
	
		}
	}


	for(y = 0; y < ROWS; y++)
	{
		for(x = 0; x < COLS; x++)
		{	
  		if(image[y][x] == 255)
			{	
			for(z = 0; z < 11; z++ )
				{
				theta = z * PI * .1; 
				rho = -1 * (x * sin(theta) -  y * cos (theta)); 
				if(rho < 0)
					{
					rho = rho * -1;
					}	
				if(rho > maxrho)
					{
					maxrho = rho;
					}
				}
			}
		}
	}


printf("Maxrho = %f\n", maxrho);

	for(y = 0; y < ROWS; y++)
	{
		for(x = 0; x < COLS; x++)
		{	
  		if(image[y][x] == 255)
			{	
			for(z = 0; z < 11; z++ )
				{
				theta = z * PI * .1; 
				rho = -1 * (x * sin(theta) -  y * cos (theta)); 
				if(rho < 0)
					{
					rho = rho * -1;
					}

				tmprho = (int)rho;
				tmptheta = (int)(z);	
				vote[tmprho][tmptheta] += 1;			
					}
				}
			}
		}
	

for(i = 0; i< maxrho ; i++)
	for(j=0; j<10 ; j++)
		{
		if(vote[i][j] > 33)
		printf("Value of vote[%d][%d] = %d\n", i, j, vote[i][j]);
		}




/*
 for(i = 0; i< maxrho ; i++)
	for(j=0; j<10 ; j++)
		{
		if(vote[i][j] > 33)
			{
			if(j = 1)
			{
			
			}
			if(j = 3)
			{
			
			}
			if(j = 7)
			{

			}
			}
		}


 
 */ 

	
	printf("Maxrho = %f\n", maxrho);

	printf("Max : %d \nMin : %d \nscale : %f\n", max, min, scale);

/* Save it into a ras image */
	/* Open the file */
	if (!( fp = fopen( ofile, "wb" )))
	  fprintf( stderr, "error: could not open %s\n", argv[1] ), exit(1);

	/* Write the header */
	fwrite( head, 4, 8, fp );
	 
	/* Write the image */
	for ( i = 0; i < ROWS; i++ ) 
		fwrite( vote[i], 1, tmptheta, fp );

	/* Close the file */
	fclose( fp );	

		return 0;
}

void clear( unsigned char image[][COLS] )
{
	int	i,j;
	for ( i = 0 ; i < ROWS ; i++ )
		for ( j = 0 ; j < COLS ; j++ ) image[i][j] = 0;
}

void header( int row, int col, unsigned char head[32] )
{
	int *p = (int *)head;
	char *ch;
	int num = row * col;

	/* Choose little-endian or big-endian header depending on the machine. Don't modify this */
	/* Little-endian for PC */
	
	*p = 0x956aa659;
	*(p + 3) = 0x08000000;
	*(p + 5) = 0x01000000;
	*(p + 6) = 0x0;
	*(p + 7) = 0xf8000000;

	ch = (char*)&col;
	head[7] = *ch;
	ch ++; 
	head[6] = *ch;
	ch ++;
	head[5] = *ch;
	ch ++;
	head[4] = *ch;

	ch = (char*)&row;
	head[11] = *ch;
	ch ++; 
	head[10] = *ch;
	ch ++;
	head[9] = *ch;
	ch ++;
	head[8] = *ch;
	
	ch = (char*)&num;
	head[19] = *ch;
	ch ++; 
	head[18] = *ch;
	ch ++;
	head[17] = *ch;
	ch ++;
	head[16] = *ch;
	
/*
 * 	// Big-endian for unix
 * 		*p = 0x59a66a95;
 * 			*(p + 1) = col;
 * 				*(p + 2) = row;
 * 					*(p + 3) = 0x8;
 * 						*(p + 4) = num;
 * 							*(p + 5) = 0x1;
 * 								*(p + 6) = 0x0;
 * 									*(p + 7) = 0xf8;
 * 									*/
}
