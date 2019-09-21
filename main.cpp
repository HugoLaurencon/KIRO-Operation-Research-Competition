#include "kiro.h"

int main()
{
    srand(clock());
    vector<int> D;
    vector<node> distribution;
    vector<node> terminal;

    int read = read_file(string("pim/distances.csv"),string("pim/nodes0.csv"), D, distribution, terminal);

    int rep = 500;
    int k = 1;
    int iterations = 1000;

    vector<vector<vector<node>>> clusters = clustering(distribution, terminal, D);
    solution = heuristic_rs_multiple_times(clusters, distribution, terminal, D, rep, k, iterations, "resu_pim.txt", true);

    return 0;
}

