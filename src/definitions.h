#define TRUE 1
#define FALSE 0
#define DEBUG FALSE
#define MAX_DECK_SIZE 15
#define SINGLE_LINE "----------------------\n"
#define DOUBLE_LINE "======================\n\n"

// #pragma omp declare reduction (vec_merge : std::vector<int> : omp_out.insert(omp_out.end(), omp_in.begin(), omp_in.end()))
