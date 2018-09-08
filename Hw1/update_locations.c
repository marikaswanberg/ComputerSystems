#include <time.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

// This function generates a random array of floats of length SIZE,
// within the real interval [-BOUND, BOUND]
double* generate_random_list(const int SIZE, const int BOUND){
  assert(SIZE > 0 && "Size must be positive");
  assert(BOUND > 0 && "Bound must be poitive");
  // We need to allocate a new array because we need the array past the function call.
  double* random_array = (double*) malloc(SIZE*sizeof(double));
  double random_number;
  for(int i = 0; i < SIZE; i++){
    random_number = ((double)rand()/RAND_MAX*(2*BOUND)) - BOUND;
    random_array[i] = random_number;
  }
  return random_array;
}

// Update location by velocity, one time-step
void update_coords(double* x, double* y, double* z, double* vx, double* vy, double* vz, const int SIZE){
  for(int i = 0; i < SIZE; i++){
    x[i] += vx[i];
    y[i] += vy[i];
    z[i] += vz[i];
  }
}



double timeit(double* x, double* y, double* z, double* vx, double* vy, double* vz, const int SIZE, const int ITERS){
  struct timespec start, end;
  double time = 0;
  for(int i = 0; i < ITERS; i++){
    clock_gettime(CLOCK_MONOTONIC, &start);
    update_coords(x, y, z, vx, vy, vz, SIZE);
    clock_gettime(CLOCK_MONOTONIC, &end);
    time += (end.tv_nsec - start.tv_nsec);
  }
  return time/(ITERS*SIZE);
}

int main(int argc, char* argv[]){
  if (argc != 3){
    printf("Required arguments: vector_length and iterations_num");
    return -1;
  }

  const int SIZE = atoi(argv[1]);
  const int ITERS = atoi(argv[2]);

  srand(time(NULL));

  double* x = generate_random_list(SIZE, 1000);
  double* y = generate_random_list(SIZE, 1000);
  double* z = generate_random_list(SIZE, 1000);
  double* vx = generate_random_list(SIZE, 1);
  double* vy = generate_random_list(SIZE, 1);
  double* vz = generate_random_list(SIZE, 1);

  double average_time = timeit(x,y,z,vx,vy,vz, SIZE, ITERS);
  printf("%f", average_time);
  
  return 0;
}

