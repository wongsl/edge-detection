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
	int			vote[1600][181] = {{0},{0}};
	int			maxy, tmptheta, tmprho, theta1, theta2, theta3, rho1, rho2, rho3, count1, count2, count3;
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

 
rho = maxy = maxrho = 0;
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

/*---------------end of hw4 code(gets sgm image)---------------------*/
	for(y = 0; y < ROWS; y++)
	{
		for(x = 0; x < COLS; x++)
		{	
  		if(image[y][x] == 255)
			{	
			for(z = 0; z < 181; z++ )
				{
				theta = z * PI / 180; 
				rho = -1 * (x * sin(theta) -  y * cos (theta)); 
				rho = rho + 800;
				if(rho > maxrho)
					{
					maxrho = rho;
					}
				}
			}
		}
	}
/*---------------------finds the max rho to hard code the vote array size, offset negative values of rho by 800-------------------------------*/

/*-------------------max rho value of 1600 and theta divided into 180 buckets---------------------------------*/


/*finds max rho so i can make appropriate vote array size*/
printf("Maxrho = %f\n", maxrho);

	for(y = 0; y < ROWS; y++)
	{
		for(x = 0; x < COLS; x++)
		{	
  		if(image[y][x] == 255)
			{	
			for(z = 0; z < 181; z++ )
				{
				theta = z * PI / 180; 
				rho = -1 * (x * sin(theta) -  y * cos (theta));	
				rho = rho + 800;
				tmprho = (int)rho;
				tmptheta = (int)(z);	
				vote[tmprho][tmptheta] += 1;			
					}
				}
			}
		}
	

/*---------------------increments votes in vote array-------------------------------*/
/*
for(i = 0; i< maxrho ; i++)
	for(j=0; j<180 ; j++)
		{
		if(vote[i][j] > 100)
		printf("Value of vote[%d][%d] = %d\n", i, j, vote[i][j]);
		}
*/
/*
prints the vote points with more than 100 votes, theshold to find the approx theta and rho values*/


count1 = count2 = count3 = rho1 = rho2 = rho3 = theta1= theta2 = theta3 = 0;

 for(i = 0; i< 1600 ; i++)
 {
	 for(j=0; j<180 ; j++)
		{
		if(vote[i][j] > 100)
		{
			if(j >= 13 && j <= 14)
				{
				rho1 = rho1 + i * vote[i][j];
				theta1 = theta1 + j * vote[i][j]; 
				count1 = count1 + vote[i][j];
			}
			if(j >= 50 && j <=53)
			{
				rho2 = rho2 + i * vote[i][j];
				theta2 = theta2 + j * vote[i][j]; 
				count2 = count2 + vote[i][j];
			}
			if(j >=120 && j<= 135)
			{
				rho3 = rho3 + i * vote[i][j];
				theta3 = theta3 + j * vote[i][j]; 
				count3 = count3 + vote[i][j];
			}
			}
		}
}

rho1 = rho1/count1;
rho2 = rho2/count2;
rho3 = rho3/count3;
theta1 = theta1/count1;
theta2 = theta2/count2;
theta3 = theta3/count3;

/*Finds the 3 rho and 3 theta values to hard code by averagin the values similar to each other*/


printf("rho1 = %d, theta1 = .%d/180pi \n", rho1, theta1);
printf("rho2 = %d, theta2 = .%d/180pi \n", rho2, theta2);
printf("rho3 = %d, theta3 = .%d/180pi \n", rho3, theta3);


	for(i = 0; i < 1600; i++)
	{
		for(z = 0; z < 181; z++)
		{	
  		if((i == rho1 && z == theta1) || (i == rho2 && z == theta2) || (i == rho3 && z == theta3))
			{	
			for(x = 0; x < COLS; x++ )
				{
				/*printf("x = %d", x);*/ 
				y = ((x * sin(z/180 * PI) + (i-800)) / (cos (z/180 * PI ))); 
				/*printf("image[%d][%d]\n", y, x);
				*/
				/*if(y < 0)
					{
					y = y*-1;
					}

				if (x > maxy)
				maxy = y;	
				*/image[y][x] = 255;
				}
			}
		}
	}
/*replaces and finds the x and y values and replaces them to white pixels infinite lines*/
/*
	for(i = 0; i < rho7; i++)
	{
		for(z = 0; z < 11; z++)
		{	
  		if((i == rho1 && z == 1) || (i == rho3 && z == 3) || (i == rho7 && z == 7))
			{	
			for(y = 0; y < ROWS; y++ )
				{ 
				x = -1* (-1 * y * cos(z/10 * PI) + rho) / (sin (z/10 * PI)); 
				if(x < 0)
					{
					x = x*-1;
					}

				if (x > maxy)
					{
					maxy = x;
					}	
				image[y][x] = 255; 
				}
			}
		}
	}
*/
printf("maxy = %d\n", maxy);



	
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
		fwrite( image[i], 1, COLS, fp );

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
