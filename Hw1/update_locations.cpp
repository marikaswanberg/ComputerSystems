#include <iostream>
#include <time.h>

double generate_random_list(const int SIZE, const int BOUND){
  double* random_array = new double[SIZE];
  srand(time(NULL));
  double random_number;
  for(int i = 0; i < size; i++){
    random_number = (rand()%(2*BOUND)) - BOUND;
    random_array[i] = random_number;
  }
  return random_array;
}

int main(){
  double* my_array = generate_random_list(30, 10);

  for(int i = 0; i<30; i++){
    std::cout << my_array[i] + ", ";
    }
  return 0;
}

