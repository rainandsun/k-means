#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* strtok() */
#include <sys/types.h> /* open() */
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h> /* getopt() */
int _debug;
#include "dbscan.h"
/*---< usage() >------------------------------------------------------------*/
static void usage(char *argv0, float threshold) {
char *help =
"Usage: %s [switches] -i filename -n num_clusters\n"
" -i filename : file containing data to be clustered\n"
" -b : input file is in binary format (default no)\n"
" -n num_Neighbors: number of clusters (K must > 1)\n"
" -t threshold : threshold value (default %.4f)\n"
" -o : output timing results (default no)\n"
" -d : enable debug mode\n";
"-r: the radius\n";
fprintf(stderr, help, argv0, threshold);
exit(-1);
}
/*---< main() >-------------------------------------------------------------*/
int main(int argc, char **argv) {
int opt;
extern char *optarg;
extern int optind;
int i, j;
float radius=2.23;
int Mints=2;
int isBinaryFile, is_output_timing;
int numClusters, numCoords, numObjs;
int *membership; /* [numObjs] */
char *filename;
float **objects; /* [numObjs][numCoords] data objects */


double timing, io_timing, clustering_timing;

/* some default values */
_debug = 0;


isBinaryFile = 0;
is_output_timing = 0;
filename = NULL;
while ( (opt=getopt(argc,argv,"p:i:n:r:t:abdo"))!= EOF) {
switch (opt) {
case 'i': filename=optarg;
break;
case 'b': isBinaryFile = 1;
break;
case 'n': Mints = atoi(optarg);
break;
case 'r':radius=atof(optarg);
break;
case 'o': is_output_timing = 1;
break;
case 'd': _debug = 1;
break;

}
}

if (is_output_timing) io_timing = wtime();
/* read data points from file ------------------------------------------*/
objects = file_read(isBinaryFile, filename, &numObjs, &numCoords);
if (objects == NULL) exit(1);


if (is_output_timing) {
timing = wtime();
io_timing = timing - io_timing;
clustering_timing = timing;
}
/* start the timer for the core computation -----------------------------*/
/* membership: the cluster id for each data object */
membership=Dbscan(objects,numObjs,numCoords,radius,Mints);
if (is_output_timing) {
timing = wtime();
clustering_timing = timing - clustering_timing;
}
/* output: the coordinates of the cluster centres ----------------------*/
file_write(filename,numObjs,membership);
free(membership);

free(objects[0]);
free(objects);
/*---- output performance numbers ---------------------------------------*/
if (is_output_timing) {
io_timing += wtime() - timing;
printf("\nPerforming **** Regular DBSCAN (sequential version) ****\n");
printf("Input file: %s\n", filename);
printf("numObjs = %d\n", numObjs);
printf("numCoords = %d\n", numCoords);

printf("I/O time = %10.4f sec\n", io_timing);
printf("Computation timing = %10.4f sec\n", clustering_timing);
}

return 0;
}

