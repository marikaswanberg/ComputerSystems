#include <time.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

// This function generates a random array of floats of length SIZE,
// within the real interval [-BOUND, BOUND]
double* generate_random_list(const long int SIZE, const int BOUND){
  assert(SIZE > 0 && "Size must be positive");
  assert(BOUND > 0 && "Bound must be positive");
  // We need to allocate a new array because we need the array past the function call.
  double* random_array = (double*) malloc(SIZE*sizeof(double));
  if (!random_array){
    perror("malloc failed");
    exit(-1);
  }
  double random_number;
  for(int i = 0; i < SIZE; i++){
    random_number = ((double)rand()/RAND_MAX*(2*BOUND)) - BOUND;
    random_array[i] = random_number;
  }
  return random_array;
}

// Update location by velocity, one time-step
void update_coords(double* x, double* y, double* z, double* vx, double* vy, double* vz, const long int SIZE){
  for(int i = 0; i < SIZE; i++){
    x[i] += vx[i];
    y[i] += vy[i];
    z[i] += vz[i];
  }
}


// Returns the average runtime for the update_coords function in microseconds
double timeit(double* x, double* y, double* z, double* vx, double* vy, double* vz, const long int SIZE, const long int ITERS){
  struct timespec start, end;
  double time = 0.0;
  clock_gettime(CLOCK_MONOTONIC, &start);
  
  for(int i = 0; i < ITERS; i++){
    update_coords(x, y, z, vx, vy, vz, SIZE);
    time += (end.tv_nsec - start.tv_nsec);
    
  }
  clock_gettime(CLOCK_MONOTONIC, &end);
  time = (end.tv_sec - start.tv_sec)*1000000.0; // conversion to microseconds
  time += (end.tv_nsec - start.tv_nsec)*0.001;
  printf("%f\n", time);
  return time/(ITERS*SIZE);
}

// Returns the sum of the elements in a list
double sum(double* list, const long int size){
  double sum_total = 0.0;
  for(int i=0; i<size; i++){
    sum_total += list[i];
  }
  return sum_total;
}

int main(int argc, char* argv[]){
  if (argc != 3){
    printf("Required arguments: vector_length and iterations_num");
    return -1;
  }

  const long int SIZE = atoi(argv[1]);
  const long int ITERS = atoi(argv[2]);

  srand(SIZE);

  double* x = generate_random_list(SIZE, 1000);
  double* y = generate_random_list(SIZE, 1000);
  double* z = generate_random_list(SIZE, 1000);
  double* vx = generate_random_list(SIZE, 1);
  double* vy = generate_random_list(SIZE, 1);
  double* vz = generate_random_list(SIZE, 1);

  double average_time = timeit(x,y,z,vx,vy,vz, SIZE, ITERS);

  double chksum = sum(x,SIZE) + sum(y, SIZE) + sum(z, SIZE);
  printf("Mean time per coordinate: %fus\n", average_time);
  printf("Final checksum is: %f\n", chksum);
  
  // free the memory from the dynamically-allocated arrays
  free(x);
  free(y);
  free(z);
  free(vx);
  free(vy);
  free(vz);
  
  return 0;
}

