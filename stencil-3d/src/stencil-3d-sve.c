#include <stdlib.h>
#include <stdio.h>
#include <arm_sve.h>
typedef float real_t;
typedef real_t*** arr_t;
#define MAX_TIME 10
#define INNER -1.0
#define OUTER 1.0
#ifndef PSIZE
#define PSIZE 100
#endif

void setElement(float32_t*** array, float32_t value, uint32_t x, uint32_t y, uint32_t z){
    array[x][y][z] = value;
}


void initValues(float32_t ***array, uint32_t sx, uint32_t sy, uint32_t sz, float32_t inner_temp, float32_t outer_temp){
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


void stencil_3d_7point(float32_t ***A, float32_t ***B, uint32_t nx, uint32_t ny, uint32_t nz) {
    uint32_t i, j, k;
    float32_t seven = 7.0;

    // define vector A, B and predicate 
    svfloat32_t vecA;
    svfloat32_t vecB;
    svbool_t predicate;

    for(uint32_t timestep = 0; timestep < MAX_TIME; ++timestep){  
        for (i = 1; i < nx-1; i++)
            for (j = 1; j < ny-1; j++)
                for (k = 1; k < nz-1; k+=svcntw()){
                    predicate = svwhilelt_b32_s32(k, nz-1);

                    // load first A vector to vector B
                    vecB = svld1_f32(predicate, &A[i-1][j ][k ]);

                    // load other A vectors and add it to vector B
                    vecA = svld1_f32(predicate, &A[i  ][j-1][k  ]);
                    vecB = svadd_f32_m(predicate, vecB, vecA);
                    vecA = svld1_f32(predicate, &A[i  ][j  ][k-1]);
                    vecB = svadd_f32_m(predicate, vecB, vecA);
                    vecA = svld1_f32(predicate, &A[i  ][j  ][k  ]);
                    vecB = svadd_f32_m(predicate, vecB, vecA);
                    vecA = svld1_f32(predicate, &A[i  ][j  ][k+1]);
                    vecB = svadd_f32_m(predicate, vecB, vecA);
                    vecA = svld1_f32(predicate, &A[i  ][j+1][k  ]);
                    vecB = svadd_f32_m(predicate, vecB, vecA);
                    vecA = svld1_f32(predicate, &A[i+1][j  ][k  ]);
                    vecB = svadd_f32_m(predicate, vecB, vecA);

                    // divide
                    vecB = svdiv_n_f32_m(predicate, vecB, seven);

                    // store vecB back
                    svst1_f32(predicate, &B[i][j][k], vecB);
                }
    for (i = 1; i < nx-1; i++)
        for (j = 1; j < ny-1; j++)
            for (k = 1; k < nz-1; k+=svcntw()){
                predicate = svwhilelt_b32_s32(k, nz-1);
                // load B and store to A
                vecB = svld1_f32(predicate, &B[i][j][k]);
                svst1_f32(predicate, &A[i][j][k], vecB);
            }
    }
}   


int main() {
    uint32_t i, j, k;
    uint32_t sz = PSIZE;
    uint32_t sx = PSIZE;
    uint32_t sy = PSIZE;
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

    float32_t inner = INNER;
    float32_t outer = OUTER;
    initValues(A, sx, sy, sz,  inner, outer);
    stencil_3d_7point(A, B,  sz, sy, sz);
    free(A);

    printf("Execute successfully\n");
    return 0;
}

