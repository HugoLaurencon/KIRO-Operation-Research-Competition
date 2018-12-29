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
    int iterations = 10000;

    // Meilleur score pour Paris pour rep = 500 (arrete a 243), k = 1, iterations = 1000, 10000 dans le change_loop_randomly, T = 1000/log(i+2)

    //5, 1000, 1000, 100000 pour Nice

    /*
    1) On charge une tres bonne solution pour les boucles. Cela fera office de clustering et c'est mieux qu'un clustering simple.
    2) On passe cette solution a heuristic_change_loop_randomly dont on modifie la fonction pour prendre en compte le change_chains
       (optimiser en rangee pour les indices, ie 0 puis 1 puis ... puis 10 puis 1 puis ...)
    3) On passe la solution obtenue a heuristic_neighbor_multiple_times dont on modifie la fonction pour prendre en compte le change_chains
    4) Repeter mais en ne prenant pas de tres bonne solution au depart et en ne faisant pas heuristic_change_loop_randomly trop longtemps
    */





    //vector<vector<vector<node>>> clusters = clustering(distribution, terminal, D);
    //vector<vector<vector<node>>> solution = heuristic_change_loop_randomly(clusters, distribution, terminal, D, 10000, "resu_pim.txt", true);
    //solution = heuristic_rs_multiple_times(solution, distribution, terminal, D, rep, k, iterations, "resu_pim.txt", true);

    //cout << "Optimisation en recuit simule de la meilleure solution trouvee aleatoirement" << endl;
    //solution = heuristic_neighbor_multiple_times(solution, distribution, terminal, D, rep, k, iterations, "resu_nice.txt", true);



    vector<vector<vector<node>>> solution = read_solution(string("25194_rsm_paris.txt"),distribution,terminal);
    solution = heuristic_neighbor_multiple_times(solution, distribution, terminal, D, 3, 1, 10000, "resu_pim.txt", true);
    cout << cost_solution(solution, D) << endl;


    /*
    cout << " " << endl;
    int cost_before_cc = 0;
    for (int i=0; i<solution.size(); i++) {
        cout << "Le cout du " << i << "eme cluster est " << cost(solution[i], D) << endl;
        cost_before_cc += cost(solution[i], D);
    }
    cout << "Cout avant cc = " << cost_before_cc << endl;
    cout << " " << endl;

    cout << "On change les chaines" << endl;
    solution = change_chains(solution, distribution, terminal, D);
    //write(solution, string("resu_nice_cc.txt"));
    cout << " " << endl;

    int cost_after_cc = 0;
    for (int i=0; i<solution.size(); i++) {
        cout << "Le cout du " << i << "eme cluster est " << cost(solution[i], D) << endl;
        cost_after_cc += cost(solution[i], D);
    }
    cout << "Cout apres cc = " << cost_after_cc << endl;
    */





    /*

    int n = distribution.size() + terminal.size();

    vector<vector<vector<node>>> sol;
    vector<vector<node>> column0;

    vector<node> loop = {0, 8, 2, 12, 6, 3, 7, 5};
    //vector<node> chain_1 = {3, 6};
    vector<node> chain_2 = {2, 11, 9};
    //vector<node> chain_3 = {3, 7};
    //vector<node> chain_4 = {0, 5};

    column0.push_back(loop);
    //sol.push_back(chain_1);
    column0.push_back(chain_2);
    //sol.push_back(chain_3);
    //sol.push_back(chain_4);

    sol.push_back(column0);

    //cout << D[n*12 + 7] << endl;

    //sol = heuristic_rs_multiple_times(sol, distribution, terminal, D, 1, 10, 100000, "resu_grenoble.txt", true);
    sol = heuristic_neighbor_multiple_times(sol, distribution, terminal, D, 1, 10, 1000000, "resu_grenoble.txt");

    cout << cost_solution(sol, D) << endl;

    */








    /*
    vector<vector<vector<node>>> sol;


    vector<int> int_loop_0 = {0, 13};
    vector<int> int_chain_0 = {3, 4, 5, 6, 7, 8, 10, 13, 14};
    vector<node> loop_0;
    vector<node> chain_0;
    for (int i=0; i<int_loop_0.size(); i++) {
        loop_0.push_back(node(int_loop_0[i]));
    }
    for (int i=0; i<int_chain_0.size(); i++) {
        chain_0.push_back(node(int_chain_0[i]));
    }
    vector<vector<node>> column_0;
    column_0.push_back(loop_0);
    column_0.push_back(chain_0);
    sol.push_back(column_0);


    vector<int> int_loop_1 = {1, 34, 36, 33, 30, 29, 37, 39, 38, 40, 45, 47, 51, 52, 55, 60, 58, 54, 44, 35, 46, 41, 32, 27, 28, 26, 24, 20, 22, 17};
    vector<int> int_chain_1 = {9, 11, 12, 15, 16, 18, 19, 21, 23, 25, 31, 42, 43, 48, 49, 50, 53, 56, 57, 59};
    vector<node> loop_1;
    vector<node> chain_1;
    for (int i=0; i<int_loop_1.size(); i++) {
        loop_1.push_back(node(int_loop_1[i]));
    }
    for (int i=0; i<int_chain_1.size(); i++) {
        chain_1.push_back(node(int_chain_1[i]));
    }
    vector<vector<node>> column_1;
    column_1.push_back(loop_1);
    column_1.push_back(chain_1);
    sol.push_back(column_1);


    vector<int> int_loop_2 = {2, 61};
    vector<int> int_chain_2 = {62, 63, 64, 65, 66, 67};
    vector<node> loop_2;
    vector<node> chain_2;
    for (int i=0; i<int_loop_2.size(); i++) {
        loop_2.push_back(node(int_loop_2[i]));
    }
    for (int i=0; i<int_chain_2.size(); i++) {
        chain_2.push_back(node(int_chain_2[i]));
    }
    vector<vector<node>> column_2;
    column_2.push_back(loop_2);
    column_2.push_back(chain_2);
    sol.push_back(column_2);


    sol = change_chains(sol, distribution, terminal, D);
    cout << cost_solution(sol, D) << endl;
    */



    /*
    vector<vector<vector<node>>> clusters;


    vector<vector<node>> column_0;
    vector<node> column_0_node;
    column_0_node.push_back(node(0));
    for (int i=3; i<15; i++) {
        column_0_node.push_back(node(i));
    }
    column_0.push_back(column_0_node);
    clusters.push_back(column_0);


    vector<vector<node>> column_1;
    vector<node> column_1_node;
    column_1_node.push_back(node(1));
    for (int i=15; i<61; i++) {
        column_1_node.push_back(node(i));
    }
    column_1.push_back(column_1_node);
    clusters.push_back(column_1);


    vector<vector<node>> column_2;
    vector<node> column_2_node;
    column_2_node.push_back(node(1));
    for (int i=29; i<61; i++) {
        column_2_node.push_back(node(i));
    }
    column_2.push_back(column_2_node);
    //clusters.push_back(column_2);


    vector<vector<node>> column_3;
    vector<node> column_3_node;
    column_3_node.push_back(node(2));
    for (int i=55; i<68; i++) {
        column_3_node.push_back(node(i));
    }
    column_3.push_back(column_3_node);
    clusters.push_back(column_3);


    vector<vector<vector<node>>> initial_solution = heuristic_change_loop_randomly(clusters, distribution, terminal, D, 1000, "resu_nice.txt");

    cout << "Optimisation en recuit simule de la meilleure solution trouvee aleatoirement" << endl;

    vector<vector<vector<node>>> solution = heuristic_neighbor_multiple_times(initial_solution, distribution, terminal, D, rep, k, "resu_nice.txt");
    */




    /*
    cout << "Fin de la premiere heuristique" << endl << "Passage a la deuxieme" << endl;
    //switch_chain(data,D);
    vector<vector<vector<node>>> data = clustering(distribution, terminal, D);
    int N = 50000;
    int cnt = 0;

    while(cnt<N){
        if (cnt%(N/100)==0) cout << cnt/(N/100) << endl;
        vector<vector<vector<node>>> data_test = data;
        echange_aleat(data_test,distribution, terminal);
        int new_cost = cost_solution(data_test, D);
        if (new_cost< best_cost) {
            best_data = data_test;
            best_cost = new_cost;
            cout << best_cost << endl;
        }
        cnt+=1;
    }

    write(data, string("resu_pim.txt"));

    cout << best_cost << endl;
    */

    return 0;
}
