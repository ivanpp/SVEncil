// flatten version
#include <stdlib.h>
#include <stdio.h>
#include <arm_sve.h>
#define MAX_TIME 10
#define INNER -1.0
#define OUTER 1.0
#ifndef PSIZE
#define PSIZE 100
#endif


#define ERR 1e-3
#define DIFF(x,y) ((x-y)<0? y-x : x-y)
#define FPNEQ(x,y) (DIFF(x,y)>ERR ? 1 : 0)
int test(float32_t ***arr3d, float32_t *arr1d, const int nx, const int ny, const int nz) {
  int i, j, k;
  for (i = 0 ; i < nx; i++) {
    for (j = 0; j < ny; j++)
        for (k = 0; k < nz; k++)
            if (FPNEQ(arr3d[i][j][k],arr1d[i*ny*nz+j*nz+k])) 
            return 0;
  }
  return 1;
}


void setElement(float32_t*** array, float32_t value, size_t x, size_t y, size_t z){
    array[x][y][z] = value;
}


/*3d array*/
void initValues(float32_t ***array, int sx, int sy, int sz, float32_t inner_temp, float32_t outer_temp){
    for(int i=1; i<(sx-1); i++){
        for(int j=1; j<(sy-1); j++){
            for(int k=1; k<(sz-1); k++){
                setElement(array, inner_temp, i, j, k);
            }
        }
    }

    for(int j=0; j<sy; j++){
        for(int k=0; k<sz; k++){
            setElement(array, outer_temp, 0, j, k);
            setElement(array, outer_temp, sx-1, j, k);
        }
    }

    for(int i=0; i<sx; i++){
        for(int k=0; k<sz; k++){
            setElement(array, outer_temp, i, 0, k);
            setElement(array, outer_temp, i, sy-1, k);
        }
    }

    for(int i=0; i<sx; i++){
        for(int j=0; j<sy; j++){
            setElement(array, outer_temp, i, j, 0);
            setElement(array, outer_temp, i, j, sz-1);
        }
    }
}


void stencil_3d_7point(float32_t*** A,float32_t*** B, const int nx, const int ny, const int nz) {
    int i, j, k;

    for(int timestep = 0; timestep < MAX_TIME; ++timestep){  
        for (i = 1; i < nx-1; i++)
            for (j = 1; j < ny-1; j++)
                for (k = 1; k < nz-1; k++)
                    B[i][j][k] = (A[i-1][j  ][k  ]+
                                  A[i  ][j-1][k  ]+
                                  A[i  ][j  ][k-1]+
                                  A[i  ][j  ][k  ]+
                                  A[i  ][j  ][k+1]+
                                  A[i  ][j+1][k  ]+
                                  A[i+1][j  ][k  ])/7.0;
        for (i = 1; i < nx-1; i++)
            for (j = 1; j < ny-1; j++)
                for (k = 1; k < nz-1; k++)
                    A[i][j][k] = B[i][j][k];
    }
}


/*1d array(flatten)*/
void setElement_flatten(float32_t *array, float32_t value, size_t i){
    array[i] = value;
}


void initValues_flatten(float32_t *array, int sx, int sy, int sz, float32_t inner_temp, float32_t outer_temp){
    for(int i=1; i<(sx-1); i++){
        for(int j=1; j<(sy-1); j++){
            for(int k=1; k<(sz-1); k++){
                setElement_flatten(array, inner_temp, i*sy*sz+j*sz+k);
            }
        }
    }

    for(int j=0; j<sy; j++){
        for(int k=0; k<sz; k++){
            setElement_flatten(array, outer_temp, j*sz+k);
            setElement_flatten(array, outer_temp, (sx-1)*sy*sz+j*sz+k);
        }
    }

    for(int i=0; i<sx; i++){
        for(int k=0; k<sz; k++){
            setElement_flatten(array, outer_temp, i*sy*sz+k);
            setElement_flatten(array, outer_temp, i*sy*sz+(sy-1)*sz+k);
        }
    }

    for(int i=0; i<sx; i++){
        for(int j=0; j<sy; j++){
            setElement_flatten(array, outer_temp, i*sy*sz+j*sz);
            setElement_flatten(array, outer_temp, i*sy*sz+j*sz+(sz-1));
        }
    }
}


void stencil_3d_7point_flatten(float32_t* A,float32_t* B, const int nx, const int ny, const int nz) {
    int i, j, k;

    for(int timestep = 0; timestep < MAX_TIME; ++timestep){  
        for (i = 1; i < nx-1; i++)
            for (j = 1; j < ny-1; j++)
                for (k = 1; k < nz-1; k++)
                    B[i*ny*nz+j*nz+k] = (A[(i-1)*ny*nz + j    *nz + k  ]+
                                         A[i*ny*nz     + (j-1)*nz + k  ]+
                                         A[i*ny*nz     + j    *nz + k-1]+
                                         A[i*ny*nz     + j    *nz + k  ]+
                                         A[i*ny*nz     + j    *nz + k+1]+
                                         A[i*ny*nz     + (j+1)*nz + k  ]+
                                         A[(i+1)*ny*nz + j    *nz + k  ])/7.0;
        for (i = 1; i < nx-1; i++)
            for (j = 1; j < ny-1; j++)
                for (k = 1; k < nz-1; k++)
                    A[i*ny*nz + j*nz + k] = B[i*ny*nz + j*nz + k];
    }
}


void test_correctness(const int sx, const int sy, const int sz){
    int i, j, k;
    float32_t ***A = (float32_t ***)malloc(sx*sizeof(float32_t**));
    for (i = 0; i< sy; i++) {
        A[i] = (float32_t **) malloc(sy*sizeof(float32_t *));
        for (j = 0; j < sz; j++) {
            A[i][j] = (float32_t *)malloc(sz*sizeof(float32_t));
        }
    }
    float32_t ***B = (float32_t ***)malloc(sx*sizeof(float32_t**));
    for (i = 0; i< sy; i++) {
        B[i] = (float32_t **) malloc(sy*sizeof(float32_t *));
        for (j = 0; j < sz; j++) {
            B[i][j] = (float32_t *)malloc(sz*sizeof(float32_t));
        }
    }
    // 1d
    float32_t *A1 = (float32_t *)malloc(sx*sy*sz*sizeof(float32_t));
    float32_t *B1 = (float32_t *)malloc(sx*sy*sz*sizeof(float32_t));

    /*test initialization*/
    initValues(A, sx, sy, sz,  INNER, OUTER);
    initValues_flatten(A1, sx, sy, sz, INNER, OUTER);
    if(test(A, A1, sx, sy, sz))
        printf("PASS\n");
    else
        printf("FAIL\n");
    fflush(stdout);
    /*test stencil*/
    stencil_3d_7point(A, B, sx, sy, sz);
    stencil_3d_7point_flatten(A1, B1, sx, sy, sz);
    if(test(A, A1, sx, sy, sz))
        printf("PASS\n");
    else
        printf("FAIL\n");
    fflush(stdout);
    free(A);
    free(A1);
    free(B);
    free(B1);
}


int main() {
    int sx = PSIZE;
    int sy = PSIZE;
    int sz = PSIZE;
    float32_t *A1 = (float32_t *)malloc(sx*sy*sz*sizeof(float32_t));
    float32_t *B1 = (float32_t *)malloc(sx*sy*sz*sizeof(float32_t));
    initValues_flatten(A1, sx, sy, sz, INNER, OUTER);
    stencil_3d_7point_flatten(A1, B1, sx, sy, sz);
    free(A1);
    return 0;
}