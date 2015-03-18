#include "dbscan.h"
#include <stdlib.h>
#include <string.h>
#include<stdio.h>
#include <math.h>
#include <malloc.h>
float ** GlobalObjs;
int * Globalkey;
int ** NeighborsMatrix;
int * ClusterIDS;
int *Visited;
int numOBJS=0;

float GetDistance(float* point1,float* point2,int numCoords)
{
   int i;
   float dist=0;
   for(i=0;i<numCoords;i++)
   {
     dist+=(point1[i]-point2[i])*(point1[i]-point2[i]);
   }
   dist=sqrt(dist);
   return dist;
}

int** GetNeighbors(float** Objects,int numObjs,int numCoords,float radius)
{
      int ** NeighborsM;
      int i,j;
      NeighborsM=(int **)malloc(numObjs*sizeof(int *));
      NeighborsM[0]=(int*)malloc(numObjs*numObjs*sizeof(int));
      memset(NeighborsM[0],-1,numObjs*numObjs*sizeof(int));
      for(i=1;i<numObjs;i++)
      {
         NeighborsM[i]=NeighborsM[i-1]+numObjs;
      }
      for(i=0;i<numObjs;i++)
      {
         int location=0;
         NeighborsM[i][i]=i;

         for(j=i+1;j<numObjs;j++)
         {
           float dist=0;
           dist=GetDistance(Objects[i],Objects[j],numCoords);
           if(dist<=radius)
           {
               NeighborsM[i][j]=j;
               NeighborsM[j][i]=i;
           }
         }
      }
      return NeighborsM;
}

int* KeyValue(int numObjs,int Mints)
{
   int i,j;
   int* keys=(int *)calloc(1,numObjs*sizeof(int));
   for(i=0;i<numObjs;i++)
   {
       int sum=0;
       for(j=0;j<numObjs;j++)
       {
          if(NeighborsMatrix[i][j]!=-1)
                  sum++;
       }
       if(sum>=Mints)
       {
            keys[i]=1;
       }
   }
   return keys;
}

void KeyPointCluster(int PointID,int ClusterID)
{
   int i,j;
   //if(!Globalkey[PointID]) return;
   
  
     for(i=0;i<numOBJS;i++)
     {
       
          if(NeighborsMatrix[PointID][i]==-1)
             continue;
          if(!Visited[NeighborsMatrix[PointID][i]])
          {
                ClusterIDS[NeighborsMatrix[PointID][i]]=ClusterID;
                Visited[NeighborsMatrix[PointID][i]]=1;
                if(Globalkey[NeighborsMatrix[PointID][i]])
                {
                    KeyPointCluster(NeighborsMatrix[PointID][i],ClusterID);
                    
                }
          }
      
     } 
     
  
     
      
}

int* Dbscan(float** Objects,int numObjs,int numCoords,float radius,int Mints)
{
   int* Vis=(int*)calloc(1,numObjs*sizeof(int));
   int* ID=(int*)malloc(numObjs*sizeof(int));
   
   int i,j=0;
   int clusterID=0;
   ClusterIDS=ID;
   memset(ClusterIDS,-1,numObjs*sizeof(int));
   Visited=Vis;
   numOBJS=numObjs;
   GlobalObjs=Objects;
  
   NeighborsMatrix=GetNeighbors(Objects,numObjs,numCoords,radius);
   
  // for(i=0;i<numObjs;i++)
  // {  for(j=0;j<numObjs;j++)
  //      printf("%d\t",NeighborsMatrix[i][j]);
 //     printf("\n");
 //  }
   Globalkey=KeyValue(numObjs,Mints);
  // for(i=0;i<numObjs;i++)
    // printf("\n%d\t",Globalkey[i]);
   for(i=0;i<numObjs;i++)
   {
     
       if(!Visited[i] && Globalkey[i])
       {
            // printf("%d\n",i);
             ClusterIDS[i]=clusterID;
             Visited[i]=1;
             
             KeyPointCluster(i,clusterID);
            
             clusterID++;
       }
     
   }
   
   printf("the number of clusters is :%d\n",clusterID);
   free(NeighborsMatrix[0]);
   free(NeighborsMatrix);
   free(Globalkey);
   free(Vis);
  return ClusterIDS;
}




