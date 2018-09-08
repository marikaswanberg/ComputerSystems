#include <iostream>
#include <time.h>
#include <string>
#include <assert.h>

// This function generates a random array of floats of length SIZE,
// within the real interval [-BOUND, BOUND]
double* generate_random_list(const int SIZE, const int BOUND){
  assert(SIZE > 0 && "Size must be positive");
  assert(BOUND > 0 && "Bound must be poitive");
  // We need to allocate a new array because we need the array past the function call.
  double* random_array = new double[SIZE];
  std::srand(time(NULL));
  double random_number;
  for(int i = 0; i < SIZE; i++){
    random_number = ((double)std::rand()/RAND_MAX*(2*BOUND)) - BOUND;
    random_array[i] = random_number;
  }
  return random_array;
}

void update_coords(double* x, double* y, double* z, double* vx, double* vy, double* vz){
  return 1;
}




int main(){
  double* my_array = generate_random_list(-10, 20);

  for(int i = 0; i<10; i++){
    std::cout << my_array[i] << std::endl;
    }
  return 0;
}

