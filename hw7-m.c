#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ROWS   		 		 250
#define COLS				 250
#define LOGICAL_X_MIN			-4.0
#define LOGICAL_X_MAX			 4.0
#define LOGICAL_Y_MIN			-4.0
#define LOGICAL_Y_MAX			 4.0

void clear( unsigned char image[][COLS] );
int plot_logical_point( float x, float y, unsigned char image[][COLS] );
int plot_physical_point( int x, int y, unsigned char image[][COLS] );
int in_range( int x, int y );
void header( int row, int col, unsigned char head[32] );

int main( int argc, char **argv )
{
	int			i, j, i2, j2;
	float			 x, y, z, z0, r, a, m, p, q, pnormal, qnormal, alpha, L, Ll, Ls, theta;
	float			c, Hx, Hy, Hz, r2;
	FILE			*fp;
	unsigned char	image[ROWS][COLS];
	char *ifile, *ofile;
	unsigned char head[32];
	
	/* Example to show how to do format conversion */
	/* Input image file */
	ifile = "image1.raw";
	/* Output image file */
	ofile = "image.ras";

	/* Clear image buffer */
	clear(image);

	/* Read in a raw image */
	/* Open the file */
/*	if (( fp = fopen( ifile, "rb" )) == NULL )
	{
		fprintf( stderr, "error: couldn't open %s\n", argv[1] );
		exit( 1 );
	}			
*/
	/* Read the file */
/*	for ( i = 0; i < ROWS ; i++ )
		if ( fread( image[i], 1, COLS, fp ) != COLS )
		{
			fprintf( stderr, "error: couldn't read enough stuff\n" );
			exit( 1 );
		}
*/
	/* Close the file */
/*	fclose( fp );
*/
	/* Create a header */
	header(ROWS, COLS, head);

/* Modify this for the homework. Don't forget to write the image buffer out to a local file */
clear(image);

printf("Enter S value in [x,y,z] form\n");
printf("Enter value for x: \n");
scanf("%f", &x);
printf("Enter value for y: \n");
scanf("%f", &y);
printf("Enter value for z: \n");
scanf("%f", &z);
printf("Enter value for r: \n");
scanf("%f", &r);
printf("Enter value for a: \n");
scanf("%f", &a);
printf("Enter value for m: \n");
scanf("%f", &m);


z0 =p = q = pnormal = qnormal = theta = 0;
printf("%f %f %f", x, y, z);

/*i = y values j = x values*/

	for(i2 = 0; i2 < ROWS; i2++ )
	{
		for(j2 = 0; j2< COLS; j2++ )
		{

			i = i2 - ROWS/2;
			j = j2 - COLS/2;
			r2 =sqrt( pow(j , 2.0) + pow(i , 2.0));
			
			if(r2 < r)
			{
			p = ( -1 * j )/ sqrt(pow(r, 2.0) - (pow(j, 2.0) + pow(i,2.0)));	
			/*p = ( -1 * x )/ (z - z0);*/

			q = ( -1 * i )/ sqrt(pow(r, 2.0) - (pow(j, 2.0) + pow(i,2.0)));
			/*q = ( -1 * y )/ (z - z0);*/
			c = sqrt(pow(p, 2.0) + pow(q, 2.0) + 1);
			
			printf("p = %f	q= %f \n", p, q);
		

			pnormal = (-1 *  p) / ( sqrt( pow(p, 2.0) + pow(q, 2.0) + 1));
			qnormal = (-1 * q) / ( sqrt( pow(p, 2.0) + pow(q, 2.0) + 1));	
			printf("pnormal = %f 	qnormal = %f\n", pnormal, qnormal);
			theta = (x * pnormal + y * qnormal + z * 1/ c );	/*angle between source and normal*/
			if(theta > 0)
			{	
			Hx = (0 + x) / sqrt(pow((x + y + (z + 1)), 2.0));
			Hy = (0 + y) / sqrt(pow((x + y + (z + 1)), 2.0)); 
			Hz = (1 + z) / sqrt(pow((x + y + (z + 1)), 2.0)); 
				

			alpha  = acos( pnormal * Hx + qnormal * Hy + 1/c * Hz); 

			Ls = exp(-1 * pow( (alpha / m ), 2.0));    /*alpha not a*/
			Ll = theta;
			L = a * Ll + (1 - a) * Ls;
		/*	printf("theta = %f\n", theta);
		*/	printf("theta = %f Ls = %f Ll = %f L = %f\n", theta, Ls, Ll, L);
			L = L * 255;				/*normalize L*/
			image[i2][j2] = L;
			}
			}
		}
	}

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

int plot_logical_point( float x, float y, unsigned char image[][COLS] )
{
	int	nx, ny;
	float	xc, yc;
	xc = COLS / ((float)LOGICAL_X_MAX - LOGICAL_X_MIN);
	yc = ROWS / ((float)LOGICAL_Y_MAX - LOGICAL_Y_MIN);
	nx = (x - LOGICAL_X_MIN) * xc;
	ny = (y - LOGICAL_Y_MIN) * yc;
	return plot_physical_point( nx, ny, image );
}

int plot_physical_point( int x, int y, unsigned char image[][COLS] )
{
	if (! in_range(x,y) ) return 0;
	return image[y][x] = 255;
}
	int in_range( int x, int y )
	{
		return x >= 0 && x < COLS && y >= 0 && y < ROWS;
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
