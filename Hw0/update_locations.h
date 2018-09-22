#ifdef UPDATE_LOCS
#define UPDATE_LOCS

// This function generates a random array of floats of length SIZE,
// within the real interval [-BOUND, BOUND]
int64_t* generate_random_list(const long int SIZE, const int BOUND){};

// Update location by velocity, one time-step
void update_coords(int64_t* x, int64_t* y, int64_t* z, int64_t* vx, int64_t* vy, int64_t* vz, const long int SIZE){};

// Returns the average runtime for the update_coords function in microseconds
double timeit(int64_t* x, int64_t* y, int64_t* z, int64_t* vx, int64_t* vy, int64_t* vz, const long int SIZE, const long int ITERS){};

// Returns the sum of the elements in a list
int64_t sum(int64_t* list, const long int size){};

int main(int argc, char* argv[]){};

#endif
