#include <assert.h>
float** file_read(int, char*, int*, int*);
int file_write(char*, int,int*);
int** GetNeighbors(float** Objects,int numObjs,int numCoords,float radius);
float GetDistance(float* point1,float* point2,int numCoords);
void KeyPointCluster(int PointID,int ClusterID);
int* Dbscan(float** Objects,int numObjs,int numCoords,float radius,int Mints);
int* KeyValue(int numObjs,int Mints);
extern int _debug;
double wtime(void);
