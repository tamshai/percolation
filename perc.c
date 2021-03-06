#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>
#include "mtwister.h"

/* Site percolation for square LxL lattice with
 periodic boundary conditions */

#define L 256 /* Linear dimension */
#define N (L*L)
#define EMPTY (-N-1)

#define STACKSIZE 100

#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))

/* 
For non-root occupied sites, contains the label 
for the site's parent in the tree
Root sites are marked with a negative value equal 
to minus the size of the cluster
For unoccupied states the value is EMPTY
*/
int ptr[N];          /* Array of pointers */

/*
Contains a list of the nearest neighbors, c
hange this to work with a different topology
*/
int nn[N][4];        /* Nearest neighbors */

/*
Contains the random occupation order  
*/
int order[N];        /* Occupation order */

/* Difference vector from the site to the cluster root */
int dx[N];
int dy[N];

/* Number of occupied sites in the 
configuration in which percolation first occurs */
int nx;
int ny;

/*
Sets up the nearest neighbors according to the chosen topology
periodical boundary conditions
*/
void boundaries()
{
    int i;

    for (i=0; i<N; i++) {
        nn[i][0] = (i+1)%N; /* Right */
        nn[i][1] = (i+N-1)%N; /* Left */
        nn[i][2] = (i+L)%N; /* Down */
        nn[i][3] = (i+N-L)%N; /* Up */
        if (i%L==0) nn[i][1] = i+L-1; /* First column */
        if ((i+1)%L==0) nn[i][0] = i-L+1; /* Last column */
    }
}

/*
Generates the random order in which the sites are occupied, 
randomly permuting the integers from 0 to N-1
*/
void permutation(MTRand *r)
{
    int i,j;
    int temp;

    for (i=0; i<N; i++) order[i] = i; 
    for (i=0; i<N; i++) {dx[i] = 0, dy[i] =0;}; 
    
    nx = 0;
    ny = 0;
    
    for (i=0; i<N; i++) {

        /* Permute the current integer with a randomly
         chosen integer from further in the list */
        double drand = genRand(r);
        j = i + (N-i)*drand;
        j = j % N;
        assert(0 <= drand && drand <= 1);
        assert(0 <= j && j < N);

        temp = order[i]; /* Swap i and j */
        order[i] = order[j];
        order[j] = temp;
    }
}

/* Finds the root pointer given a site 
and compresses the paths and displacements */
int findroot(int i)
{
    int r; /* Root */
    int sp=0; /* Stack pointer */
    int stack[STACKSIZE];

    /* Stack of displacements */
    int dx_s[STACKSIZE], dy_s[STACKSIZE];
    /* Cumulative displacement */ 
    int dx_cum = 0, dy_cum = 0;  

    r = i;

    while (ptr[r]>=0) {
        
        dx_s[sp] = dx[r];
        dy_s[sp] = dy[r];
        stack[sp] = r;
        r = ptr[r];
        sp++;
    }

    while (sp) {
        sp--;
        dx_cum += dx_s[sp];
        dy_cum += dy_s[sp];

        dx[stack[sp]] = dx_cum;
        dy[stack[sp]] = dy_cum;
        ptr[stack[sp]] = r;
    }
    return r;
}



/* Populate the lattice anc check for percolation */ 
void percolate()
{
    int i,j;
    int s1,s2;
    int r1,r2;
    
    for (i=0; i<N; i++) ptr[i] = EMPTY; 

    for (i=0; i<N; i++) {

        /* Occupy the next site randomly  */ 
        r1 = s1 = order[i]; 
        ptr[s1] = -1;       

        for (j=0; j<4; j++) {   
            s2 = nn[s1][j];     
            
            if (ptr[s2]!=EMPTY) { 
                r2 = findroot(s2);  
                
                /* Merge the clusters */
                if (r2!=r1) {               
                    if (ptr[r1]>ptr[r2]) {  
                        /* Increase the size of 
                        the amalgamated cluster */
                        ptr[r2] += ptr[r1]; 
                        
                        findroot(s1);
                        
                        ptr[r1] = r2;       
                        
                        /* Update the displacements */
                        findroot(s2);
                        dx[r1] = - dx[s1] + dx[s2];
                        dy[r1] = - dy[s1] + dy[s2];
                        
                        /* Unit vector between the two sites */
                        switch (j) {
                            case 0:
                                dx[r1] += + 1;    
                                break;
                            case 1:
                                dx[r1] += - 1;      
                                break;
                            case 2: 
                                dy[r1] += + 1;        
                                break;
                            case 3:  
                                dy[r1] += - 1;        
                                break;
                        }
                        

                        r1 = r2;

                    } else {
                        ptr[r1] += ptr[r2];
                        
                        findroot(s2);
                        
                        ptr[r2] = r1;

                        /* Update the displacements */
                        
                        findroot(s1);
                        dx[r2] = - dx[s2] + dx[s1];
                        dy[r2] = - dy[s2] + dy[s1];

                        /* Find the unit vector between 
                        the two sites */
                        switch (j) {
                            case 0:
                                dx[r2] += - 1;   
                                break;
                            case 1:
                                dx[r2] += + 1;  
                                break;
                            case 2:
                                dy[r2] += - 1;        
                                break;
                            case 3:
                                dy[r2] += + 1;        
                                break;
                        }
                        
                    }
                }
                else {
                /* Internal bond, therefore we can test
                 for percolation */

                    int dx1 = 0, dy1 = 0, dx2 = 0, dy2 = 0;

                    findroot(s1);
                    findroot(s2);
                    dx1 = dx[s1];
                    dx2 = dx[s2];
                    dy1 = dy[s1];
                    dy2 = dy[s2];

                    /* Unit vector of the newly added bond */
                    switch (j) {
                        case 0:
                            dx1 += -1;   
                            break;
                        case 1:
                            dx1 += +1;  
                            break;
                        case 2:
                            dy1 += -1;        
                            break;
                        case 3:
                            dy1 += +1;        
                            break;
                    }

                    /* Check for a multiply connected cluster */
                    if (!nx && abs(dx1-dx2) != 0) {
                        nx = i+1;
                    }
                    if (!ny && abs(dy1-dy2) != 0) {
                        ny = i+1;
                    }

                    /* Stop the calculation when percolation has
                    occurred in both axis */
                    if (nx && ny)
                        return;
                }
            }
        }
    }

    /* Should never happen since the system percolates
    always before all the sites are occupied */
    return;
}


int main()
{

    /* Number of Monte Carlo samples */
    int NUM_SIM = 1e7;

    /* Pseudo-random number generator*/
    MTRand r = seedRand(42);


    /* Percolation probabilities as a function 
    of the number of occupied sites */
    int RLh[N], RLb[N];
    for (int i = 0; i< N; i++) {
        RLh[i] = 0;
        RLb[i] = 0;
    }
    
    /* Simulation */
    boundaries();
    for (int j = 0; j < NUM_SIM; j++) {
        permutation(&r);
        percolate();

        for (int i = nx-1; i < N; i++)
                RLh[i] += 1;
        for (int i = MAX(nx-1, ny-1); i < N; i++)
                RLb[i] += 1;
    }

    /* Print the probabilities into std output*/
    for (int i = 0; i < N; i++) {
        printf("%i %.17g %.17g\n", i+1, 
            (double)RLh[i] / NUM_SIM, 
            (double)RLb[i] / NUM_SIM);
    }
}
