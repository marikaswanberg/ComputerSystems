#include <time.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

// This function generates a random array of floats of length SIZE,
// within the real interval [-BOUND, BOUND]
int64_t* generate_random_list(const long int SIZE, const int BOUND){
  assert(SIZE > 0 && "Size must be positive");
  assert(BOUND > 0 && "Bound must be positive");
  // We need to allocate a new array because we need the array past the function call.
  int64_t* random_array = (int64_t*) malloc(SIZE*sizeof(int64_t));
  // Make sure the memory allocation worked
  if (!random_array){
    perror("malloc failed");
    exit(-1);
  }
  int64_t random_number;
  for(int i = 0; i < SIZE; i++){
    random_number = ((int64_t)rand()/RAND_MAX*(2*BOUND)) - BOUND;
    random_array[i] = random_number;
  }
  return random_array;
}

// Update location by velocity, one time-step
void update_coords(int64_t* x, int64_t* y, int64_t* z, int64_t* vx, int64_t* vy, int64_t* vz, const long int SIZE){
  for(int i = 0; i < SIZE; i++){
    x[i] += vx[i];
    y[i] += vy[i];
    z[i] += vz[i];
  }
}


// Returns the average runtime for the update_coords function in microseconds
double timeit(int64_t* x, int64_t* y, int64_t* z, int64_t* vx, int64_t* vy, int64_t* vz, const long int SIZE, const long int ITERS){
  struct timespec start, end;
  double  microseconds = 0.0;
  
  // time before
  clock_gettime(CLOCK_MONOTONIC, &start);
  // Do a lot of iterations
  for(int i = 0; i < ITERS; i++){
    update_coords(x, y, z, vx, vy, vz, SIZE);
  }
  //time after
  clock_gettime(CLOCK_MONOTONIC, &end);
  // Convert from seconds and nanoseconds to microseconds
  microseconds = (end.tv_sec*1000000 + end.tv_nsec*0.001) - (start.tv_sec*1000000 + start.tv_nsec*0.001); 
  return microseconds/(ITERS*SIZE);
}

// Returns the sum of the elements in a list
int64_t sum(int64_t* list, const long int size){
  int64_t sum_total = 0;
  for(int i=0; i<size; i++){
    sum_total += list[i];
  }
  return sum_total;
}

int main(int argc, char* argv[]){
  if (argc != 3){
    perror("Required arguments: vector_length and iterations_num");
    exit(-1);
  }

  const long int SIZE = atoi(argv[1]);
  const long int ITERS = atoi(argv[2]);
  if (SIZE <=0 || ITERS <= 0){
    perror("vector_length and iterations_num must be at least 1");
    exit(-1);
  }

  srand(SIZE); // Set the seed

  int64_t* x = generate_random_list(SIZE, 1000);
  int64_t* y = generate_random_list(SIZE, 1000);
  int64_t* z = generate_random_list(SIZE, 1000);
  int64_t* vx = generate_random_list(SIZE, 1);
  int64_t* vy = generate_random_list(SIZE, 1);
  int64_t* vz = generate_random_list(SIZE, 1);

  double average_time = timeit(x,y,z,vx,vy,vz, SIZE, ITERS);

  int64_t chksum = sum(x,SIZE) + sum(y, SIZE) + sum(z, SIZE);
  printf("Mean time per coordinate: %fus\n", average_time);
  printf("Final checksum is: %li\n", chksum);
  
  // free the memory from the dynamically-allocated arrays
  free(x);
  free(y);
  free(z);
  free(vx);
  free(vy);
  free(vz);
  
  return 0;
}

