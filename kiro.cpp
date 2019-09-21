#include "kiro.h"

int read_file(string distances_file, string nodes_file, vector<int> &D, vector<node> &distribution, vector<node> &terminal){
    string path_distance = string("../KIRO/Data/") + distances_file;
    string path_nodes = string("../KIRO/Data/") + nodes_file;
    ifstream distances(path_distance);
    ifstream nodes(path_nodes);
    if(distances && nodes){
        int test = 0;
        int n = 0;
        while(test != 2){
            int d_;
            distances >> d_;
            D.push_back(d_);
            n++;
            if (d_ == 0) test++;
        }
        n -= 2;
        for(int i = 0; i < n*n-n-2; i++){
            int d_;
            distances >> d_;
            D.push_back(d_);
        }
        for(int i = 0; i < n; i++){
            double x;
            double y;
            string type;
            nodes >> x >> y >> type;
            bool t = false;
            if(type == string("terminal")) t = true;
            if (t){
                node N(x, y, t, i);
                terminal.push_back(N);
            }
            else {
                node N(x, y, t, i);
                distribution.push_back(N);
            }
        }
        distances.close();
        nodes.close();
        return(n);
    }
    else cerr << "pb ouverture" << endl;
}



void write(vector<vector<vector<node>>> V, string name){
    string file_name = string("../") + name;
    ofstream fichier(file_name.c_str(), ios::out|ios::trunc);
    if (fichier){
        for(int i = 0; i < V.size(); i++){
            for(int j = 0; j < V[i].size(); j++){
                if(j == 0) fichier << "b";
                else fichier << "c";
                for(int k = 0; k < V[i][j].size(); k++){
                    fichier << " " << V[i][j][k].get_indice();
                }
                fichier << endl;
            }
        }
    }
    fichier.close();
}



vector<vector<vector<node>>> read_solution(string name, vector<node> distributions, vector<node> terminaux){
    string file_name = string("../KIRO/") + name;
    vector<vector<vector<node>>> data;
    ifstream fichier(file_name.c_str());
    if (fichier){
        string ligne;
        vector<vector<node>> distribution;
        int cnt = 0;
        while(getline(fichier,ligne)){
            cout << endl;
            vector<node> chaine;
            if (ligne[0] == 'b'){
                if (cnt > 0){
                    data.push_back(distribution);
                    distribution.clear();
                }
                cnt++;
            }
            auto it = ligne.begin() + 2;
            string n("");
            while(it != ligne.end() && it != ligne.end() + 1){
                while((*it == '0'||*it == '1'||*it == '2'||*it == '3'||*it == '4'||*it == '5'||*it == '6'||*it == '7'||*it == '8'||*it == '9')&& it != ligne.end() && it != ligne.end() + 1){
                    cout << (*it);
                    n += (*it);
                    it++;
                }
                cout << " ";
                it++;
                if (!n.empty()){
                    int number = stoi(n,nullptr);
                    n = string("");
                    if (number<distributions.size()) chaine.push_back(distributions[number]);
                    else chaine.push_back(terminaux[number-distributions.size()]);
                }
            }
            distribution.push_back(chaine);
        }
        fichier.close();
        data.push_back(distribution);
        return(data);
    }
    else{
        cerr << "probleme de lecture" << endl;
    }
}



bool node::is_in(vector<node> V){
    for(int i = 0; i < V.size(); i++){
        if(indice == V[i].get_indice()) return true;
    }
    return false;
}



int cost(vector<vector<node>> C, vector<int> lenght){
    int result = 0;
    int n = sqrt(lenght.size());
    for (int j = 0; j < C.size(); j++){
        if (j == 0)
            result += lenght[C[j][0].get_indice() + C[j][C[j].size()-1].get_indice()*n];
        for(int i = 1; i < C[j].size(); i++){
            result += lenght[C[j][i].get_indice() + C[j][i-1].get_indice()*n];
        }
    }

    return result;
}



int cost_solution(vector<vector<vector<node>>> C, vector<int> lenght){
    int result = 0;
    for(int i = 0; i < C.size(); i++) result += cost(C[i],lenght);
    return result;
}



vector<vector<vector<node>>> clustering(vector<node> nodes_d, vector<node> nodes_t, vector<int> distances) {
    int n = nodes_d.size() + nodes_t.size();
    vector<int> index_clusters;
    for (int i = 0; i < nodes_t.size(); i++) {
        int index = 0;
        int min_distance = distances[n*nodes_t[i].get_indice() + nodes_d[0].get_indice()];
        for (int j=0; j<nodes_d.size(); j++) {
            if (distances[n*nodes_t[i].get_indice() + nodes_d[j].get_indice()] < min_distance) {
                min_distance = distances[n*nodes_t[i].get_indice() + nodes_d[j].get_indice()];
                index = j;
            }
        }
        index_clusters.push_back(index);
    }
    vector<vector<vector<node>>> data_new(nodes_d.size());
    vector<vector<node>> data(nodes_d.size());
    for (int i = 0; i < nodes_d.size(); i++) {
        data[i].push_back(nodes_d[i]);
        data_new[i].push_back(data[i]);
    }
    for (int i = 0; i < nodes_t.size(); i++) {
        data[index_clusters[i]].push_back(nodes_t[i]);
    }
    for (int i = 0; i < nodes_d.size(); i++) {
        for (int j = 0; j < data[i].size()-1; j++) {
            int distance_min = distances[n*data[i][j].get_indice() + data[i][j+1].get_indice()];
            int index = j+1;
            for (int k = j+2; k < data[i].size(); k++) {
                if (distances[n*data[i][j].get_indice() + data[i][k].get_indice()] < distance_min) {
                    distance_min = distances[n*data[i][j].get_indice() + data[i][k].get_indice()];
                    index = k;
                }
            }
            data[i].insert(data[i].begin()+j+1, data[i][index]);
            data[i].erase(data[i].begin()+index+1);
        }
        if (data[i].size() <= 30) {
            for (int j = 0; j < data[i].size()-1; j++) {
                data_new[i][0].push_back(data[i][j+1]);
            }
        }
        else {
            for (int j = 0; j < 29 ; j++) {
                data_new[i][0].push_back(data[i][j+1]);
            }
            for (int j = 30; j < data[i].size(); j++) {
                if (data_new[i][data_new[i].size()-1].size() >= 5) {
                    vector<node> new_chain;
                    int distance_min = distances[n*data_new[i][0][0].get_indice() + data[i][j].get_indice()];
                    int index = 0;
                    for (int k = 1; k < 30; k++) {
                        if (distances[n*data_new[i][0][k].get_indice() + data[i][j].get_indice()] < distance_min) {
                            distance_min = distances[n*data_new[i][0][k].get_indice() + data[i][j].get_indice()];
                            index = k;
                        }
                    }
                    new_chain.push_back(data_new[i][0][index]);
                    new_chain.push_back(data[i][j]);
                    data_new[i].push_back(new_chain);
                }
                else {
                    int cost1 = distances[n*data_new[i][data_new[i].size()-1][data_new[i][data_new[i].size()-1].size()-1].get_indice() + data[i][j].get_indice()];
                    int cost2 = distances[n*data_new[i][0][0].get_indice() + data[i][j].get_indice()];
                    int index = 0;
                    for (int k=1; k<data_new[i][0].size(); k++) {
                        if (distances[n*data_new[i][0][k].get_indice() + data[i][j].get_indice()] < cost2) {
                            cost2 = distances[n*data_new[i][0][k].get_indice() + data[i][j].get_indice()];
                            index = k;
                        }
                    }
                    if (cost1 < cost2) {
                        data_new[i][data_new[i].size()-1].push_back(data[i][j]);
                    }
                    else {
                        vector<node> new_chain;
                        new_chain.push_back(data_new[i][0][index]);
                        new_chain.push_back(data[i][j]);
                        data_new[i].push_back(new_chain);
                    }
                }
            }
        }
    }
    return data_new;
}



vector<vector<vector<node>>> change_chains(vector<vector<vector<node>>> initial_solution, vector<node> distribution, vector<node> terminal, vector<int> distances) {
    int n = distribution.size() + terminal.size();
    vector<vector<vector<node>>> new_solution;
    for (int i = 0; i < initial_solution.size(); i++) {
        vector<vector<node>> column;
        column.push_back(initial_solution[i][0]);
        new_solution.push_back(column);
    }
    vector<node> chain;
    for (int i = 0; i < initial_solution.size(); i++) {
        for (int j = 1; j < initial_solution[i].size(); j++) {
            for (int k = 1; k < initial_solution[i][j].size(); k++) {
                chain.push_back(initial_solution[i][j][k]);
            }
        }
    }
    if (chain.size() >= 1) {
        for (int j = 0; j < chain.size()-1; j++) {
            int distance_min = distances[n*chain[j].get_indice() + chain[j+1].get_indice()];
            int index = j+1;
            for (int k = j+2; k < chain.size(); k++) {
                if (distances[n*chain[j].get_indice() + chain[k].get_indice()] < distance_min) {
                    distance_min = distances[n*chain[j].get_indice() + chain[k].get_indice()];
                    index = k;
                }
            }
            chain.insert(chain.begin()+j+1, chain[index]);
            chain.erase(chain.begin()+index+1);
        }
    }
    int magic = 0;
    for (int j = 0; j < chain.size(); j++) {
        if (new_solution[magic][new_solution[magic].size()-1].size() >= 6) {
            vector<node> new_chain;
            int distance_min = distances[n*new_solution[0][0][0].get_indice() + chain[j].get_indice()];
            magic = 0;
            int index = 0;
            for (int p=0; p<new_solution.size(); p++) {
                for (int k=0; k<new_solution[p][0].size(); k++) {
                    if (distances[n*new_solution[p][0][k].get_indice() + chain[j].get_indice()] < distance_min) {
                        distance_min = distances[n*new_solution[p][0][k].get_indice() + chain[j].get_indice()];
                        index = k;
                        magic = p;
                    }
                }
            }
            new_chain.push_back(new_solution[magic][0][index]);
            new_chain.push_back(chain[j]);
            new_solution[magic].push_back(new_chain);
        }
        else {
            int cost1 = distances[n*new_solution[magic][new_solution[magic].size()-1][new_solution[magic][new_solution[magic].size()-1].size()-1].get_indice() + chain[j].get_indice()];
            int cost2 = distances[n*new_solution[0][0][0].get_indice() + chain[j].get_indice()];
            int mag = 0;
            int index = 0;
            for (int p = 0; p < new_solution.size(); p++) {
                for (int k = 0; k < new_solution[p][0].size(); k++) {
                    if (distances[n*new_solution[p][0][k].get_indice() + chain[j].get_indice()] < cost2) {
                        cost2 = distances[n*new_solution[p][0][k].get_indice() + chain[j].get_indice()];
                        index = k;
                        mag = p;
                    }
                }
            }
            if (cost1 < cost2) {
                new_solution[magic][new_solution[magic].size()-1].push_back(chain[j]);
            }
            else {
                magic = mag;
                vector<node> new_chain;
                new_chain.push_back(new_solution[magic][0][index]);
                new_chain.push_back(chain[j]);
                new_solution[magic].push_back(new_chain);
            }
        }
    }
    return new_solution;
}



vector<vector<vector<node>>> neighbor(vector<vector<vector<node>>> best_data, int n, vector<node> distribution, vector<node> terminal, vector<int> distances, int indice, bool CC) {
    vector<vector<vector<node>>> data_new_change = best_data;
    int cost_change;
    vector<vector<vector<node>>> data_new_remove = best_data;
    int cost_remove;
    vector<vector<vector<node>>> data_new_add = best_data;
    int cost_add;
    vector<node> previous_loop_elements;
    for (int j = 0; j < best_data[indice][0].size(); j++) {
        previous_loop_elements.push_back(best_data[indice][0][j]);
    }
    vector<node> previous_chains_elements;
    for (int i = 1; i < best_data[indice].size(); i++) {
        for (int j = 1; j < best_data[indice][i].size(); j++) {
            previous_chains_elements.push_back(best_data[indice][i][j]);
        }
    }
    if (previous_chains_elements.size() >= 1) {
        vector<vector<node>> new_column_change;
        vector<node> loop_change = previous_loop_elements;
        vector<node> chains_change = previous_chains_elements;
        default_random_engine generator(random_device{}());
        int index_loop_change = uniform_int_distribution<>(1,previous_loop_elements.size()-1)(generator);
        int index_chains_change = uniform_int_distribution<>(0,previous_chains_elements.size()-1)(generator);
        loop_change.push_back(chains_change[index_chains_change]);
        chains_change.push_back(loop_change[index_loop_change]);
        loop_change.erase(loop_change.begin()+index_loop_change);
        chains_change.erase(chains_change.begin()+index_chains_change);
        for (int j = 0; j < loop_change.size()-1; j++) {
            int distance_min = distances[n*loop_change[j].get_indice() + loop_change[j+1].get_indice()];
            int index = j + 1;
            for (int k = j+2; k < loop_change.size(); k++) {
                if (distances[n*loop_change[j].get_indice() + loop_change[k].get_indice()] < distance_min) {
                    distance_min = distances[n*loop_change[j].get_indice() + loop_change[k].get_indice()];
                    index = k;
                }
            }
            loop_change.insert(loop_change.begin()+j+1, loop_change[index]);
            loop_change.erase(loop_change.begin()+index+1);
        }
        for (int j = 0; j < chains_change.size()-1; j++) {
            int distance_min = distances[n*chains_change[j].get_indice() + chains_change[j+1].get_indice()];
            int index = j + 1;
            for (int k = j+2; k < chains_change.size(); k++) {
                if (distances[n*chains_change[j].get_indice() + chains_change[k].get_indice()] < distance_min) {
                    distance_min = distances[n*chains_change[j].get_indice() + chains_change[k].get_indice()];
                    index = k;
                }
            }
            chains_change.insert(chains_change.begin()+j+1, chains_change[index]);
            chains_change.erase(chains_change.begin()+index+1);
        }
        new_column_change.push_back(loop_change);
        for (int j = 0; j < chains_change.size(); j++) {
            if (new_column_change[new_column_change.size()-1].size() >= 6) {
                vector<node> new_chain;
                int distance_min = distances[n*new_column_change[0][0].get_indice() + chains_change[j].get_indice()];
                int index = 0;
                for (int k=1; k<loop_change.size(); k++) {
                    if (distances[n*new_column_change[0][k].get_indice() + chains_change[j].get_indice()] < distance_min) {
                        distance_min = distances[n*new_column_change[0][k].get_indice() + chains_change[j].get_indice()];
                        index = k;
                    }
                }
                new_chain.push_back(new_column_change[0][index]);
                new_chain.push_back(chains_change[j]);
                new_column_change.push_back(new_chain);
            }
            else {
                int cost1 = distances[n*new_column_change[new_column_change.size()-1][new_column_change[new_column_change.size()-1].size()-1].get_indice() + chains_change[j].get_indice()];
                int cost2 = distances[n*new_column_change[0][0].get_indice() + chains_change[j].get_indice()];
                int index = 0;
                for (int k=1; k<new_column_change[0].size(); k++) {
                    if (distances[n*new_column_change[0][k].get_indice() + chains_change[j].get_indice()] < cost2) {
                        cost2 = distances[n*new_column_change[0][k].get_indice() + chains_change[j].get_indice()];
                        index = k;
                    }
                }
                if (cost1 < cost2) {
                    new_column_change[new_column_change.size()-1].push_back(chains_change[j]);
                }
                else {
                    vector<node> new_chain;
                    new_chain.push_back(new_column_change[0][index]);
                    new_chain.push_back(chains_change[j]);
                    new_column_change.push_back(new_chain);
                }
            }
        }
        data_new_change[indice] = new_column_change;
        if (CC) {
            cost_change = cost_solution(change_chains(data_new_change, distribution, terminal, distances), distances);
        }
        else {
            cost_change = cost_solution(data_new_change, distances);
        }
    }
    else {
        cost_change = INT_MAX;
    }
    if (previous_loop_elements.size() >= 3) {
        vector<vector<node>> new_column_remove;
        vector<node> loop_remove = previous_loop_elements;
        vector<node> chains_remove = previous_chains_elements;
        default_random_engine generator(random_device{}());
        int index_loop_remove = uniform_int_distribution<>(1,previous_loop_elements.size()-1)(generator);
        chains_remove.push_back(loop_remove[index_loop_remove]);
        loop_remove.erase(loop_remove.begin()+index_loop_remove);
        for (int j = 0; j < loop_remove.size()-1; j++) {
            int distance_min = distances[n*loop_remove[j].get_indice() + loop_remove[j+1].get_indice()];
            int index = j+1;
            for (int k = j+2; k < loop_remove.size(); k++) {
                if (distances[n*loop_remove[j].get_indice() + loop_remove[k].get_indice()] < distance_min) {
                    distance_min = distances[n*loop_remove[j].get_indice() + loop_remove[k].get_indice()];
                    index = k;
                }
            }
            loop_remove.insert(loop_remove.begin()+j+1, loop_remove[index]);
            loop_remove.erase(loop_remove.begin()+index+1);
        }
        for (int j = 0; j < chains_remove.size()-1; j++) {
            int distance_min = distances[n*chains_remove[j].get_indice() + chains_remove[j+1].get_indice()];
            int index = j+1;
            for (int k = j+2; k < chains_remove.size(); k++) {
                if (distances[n*chains_remove[j].get_indice() + chains_remove[k].get_indice()] < distance_min) {
                    distance_min = distances[n*chains_remove[j].get_indice() + chains_remove[k].get_indice()];
                    index = k;
                }
            }
            chains_remove.insert(chains_remove.begin()+j+1, chains_remove[index]);
            chains_remove.erase(chains_remove.begin()+index+1);
        }
        new_column_remove.push_back(loop_remove);
        for (int j = 0; j < chains_remove.size(); j++) {
            if (new_column_remove[new_column_remove.size()-1].size() >= 6) {
                vector<node> new_chain;
                int distance_min = distances[n*new_column_remove[0][0].get_indice() + chains_remove[j].get_indice()];
                int index = 0;
                for (int k = 1; k < loop_remove.size(); k++) {
                    if (distances[n*new_column_remove[0][k].get_indice() + chains_remove[j].get_indice()] < distance_min) {
                        distance_min = distances[n*new_column_remove[0][k].get_indice() + chains_remove[j].get_indice()];
                        index = k;
                    }
                }
                new_chain.push_back(new_column_remove[0][index]);
                new_chain.push_back(chains_remove[j]);
                new_column_remove.push_back(new_chain);
            }
            else {
                int cost1 = distances[n*new_column_remove[new_column_remove.size()-1][new_column_remove[new_column_remove.size()-1].size()-1].get_indice() + chains_remove[j].get_indice()];
                int cost2 = distances[n*new_column_remove[0][0].get_indice() + chains_remove[j].get_indice()];
                int index = 0;
                for (int k = 1; k < new_column_remove[0].size(); k++) {
                    if (distances[n*new_column_remove[0][k].get_indice() + chains_remove[j].get_indice()] < cost2) {
                        cost2 = distances[n*new_column_remove[0][k].get_indice() + chains_remove[j].get_indice()];
                        index = k;
                    }
                }
                if (cost1 < cost2) {
                    new_column_remove[new_column_remove.size()-1].push_back(chains_remove[j]);
                }
                else {
                    vector<node> new_chain;
                    new_chain.push_back(new_column_remove[0][index]);
                    new_chain.push_back(chains_remove[j]);
                    new_column_remove.push_back(new_chain);
                }
            }
        }
        data_new_remove[indice] = new_column_remove;
        if (CC) {
            cost_remove = cost_solution(change_chains(data_new_remove, distribution, terminal, distances), distances);
        }
        else {
            cost_remove = cost_solution(data_new_remove, distances);
        }
    }
    else {
        cost_remove = INT_MAX;
    }
    if ((previous_loop_elements.size() <= 29) && (previous_chains_elements.size() >= 2)) {
        vector<vector<node>> new_column_add;
        vector<node> loop_add = previous_loop_elements;
        vector<node> chains_add = previous_chains_elements;
        default_random_engine generator(random_device{}());
        int index_chains_add = uniform_int_distribution<>(0,previous_chains_elements.size()-1)(generator);
        loop_add.push_back(chains_add[index_chains_add]);
        chains_add.erase(chains_add.begin()+index_chains_add);
        for (int j = 0; j < loop_add.size()-1; j++) {
            int distance_min = distances[n*loop_add[j].get_indice() + loop_add[j+1].get_indice()];
            int index = j+1;
            for (int k = j+2; k < loop_add.size(); k++) {
                if (distances[n*loop_add[j].get_indice() + loop_add[k].get_indice()] < distance_min) {
                    distance_min = distances[n*loop_add[j].get_indice() + loop_add[k].get_indice()];
                    index = k;
                }
            }
            loop_add.insert(loop_add.begin()+j+1, loop_add[index]);
            loop_add.erase(loop_add.begin()+index+1);
        }
        for (int j = 0; j < chains_add.size()-1; j++) {
            int distance_min = distances[n*chains_add[j].get_indice() + chains_add[j+1].get_indice()];
            int index = j+1;
            for (int k = j+2; k < chains_add.size(); k++) {
                if (distances[n*chains_add[j].get_indice() + chains_add[k].get_indice()] < distance_min) {
                    distance_min = distances[n*chains_add[j].get_indice() + chains_add[k].get_indice()];
                    index = k;
                }
            }
            chains_add.insert(chains_add.begin()+j+1, chains_add[index]);
            chains_add.erase(chains_add.begin()+index+1);
        }
        new_column_add.push_back(loop_add);
        for (int j = 0; j < chains_add.size(); j++) {
            if (new_column_add[new_column_add.size()-1].size() >= 6) {
                vector<node> new_chain;
                int distance_min = distances[n*new_column_add[0][0].get_indice() + chains_add[j].get_indice()];
                int index = 0;
                for (int k = 1; k < loop_add.size(); k++) {
                    if (distances[n*new_column_add[0][k].get_indice() + chains_add[j].get_indice()] < distance_min) {
                        distance_min = distances[n*new_column_add[0][k].get_indice() + chains_add[j].get_indice()];
                        index = k;
                    }
                }
                new_chain.push_back(new_column_add[0][index]);
                new_chain.push_back(chains_add[j]);
                new_column_add.push_back(new_chain);
            }
            else {
                int cost1 = distances[n*new_column_add[new_column_add.size()-1][new_column_add[new_column_add.size()-1].size()-1].get_indice() + chains_add[j].get_indice()];
                int cost2 = distances[n*new_column_add[0][0].get_indice() + chains_add[j].get_indice()];
                int index = 0;
                for (int k = 1; k < new_column_add[0].size(); k++) {
                    if (distances[n*new_column_add[0][k].get_indice() + chains_add[j].get_indice()] < cost2) {
                        cost2 = distances[n*new_column_add[0][k].get_indice() + chains_add[j].get_indice()];
                        index = k;
                    }
                }
                if (cost1 < cost2) {
                    new_column_add[new_column_add.size()-1].push_back(chains_add[j]);
                }
                else {
                    vector<node> new_chain;
                    new_chain.push_back(new_column_add[0][index]);
                    new_chain.push_back(chains_add[j]);
                    new_column_add.push_back(new_chain);
                }
            }
        }
        data_new_add[indice] = new_column_add;
        if (CC) {
            cost_add = cost_solution(change_chains(data_new_add, distribution, terminal, distances), distances);
        }
        else {
            cost_add = cost_solution(data_new_add, distances);
        }
    }
    else {
        cost_add = INT_MAX;
    }
    if (cost_change <= min(cost_remove, cost_add)) {
        return data_new_change;
    }
    else if (cost_remove <= cost_add) {
        return data_new_remove;
    }
    else {
        return data_new_add;
    }
}



vector<vector<vector<node>>> heuristic_rs_multiple_times(vector<vector<vector<node>>> initial_solution, vector<node> distribution, vector<node> terminal, vector<int> D, int rep, int k, int iterations, string name, bool CC) {
    int n = distribution.size() + terminal.size();
    vector<vector<vector<node>>> best_data = initial_solution;
    vector<vector<vector<node>>> current_data = initial_solution;
    int best_cost;
    if (CC) {
        best_cost = cost_solution(change_chains(best_data, distribution, terminal, D), D);
    }
    else {
        best_cost = cost_solution(best_data, D);
    }
    int current_cost;
    if (CC) {
        current_cost = cost_solution(change_chains(current_data, distribution, terminal, D), D);
    }
    else {
        current_cost = cost_solution(current_data, D);
    }
    cout << best_cost << endl;
    for (int l = 0; l < rep; l++) {
        for (int indice = 0; indice < best_data.size(); indice++) {
            vector<vector<vector<node>>> initial_data = best_data;
            for (int j = 0; j < k; j++) {
                vector<vector<vector<node>>> best_data_j = initial_data;
                vector<vector<vector<node>>> current_data_j = initial_data;
                int best_cost_j;
                int current_cost_j;
                if (CC) {
                    best_cost_j = cost_solution(change_chains(best_data_j, distribution, terminal, D), D);
                    current_cost_j = cost_solution(change_chains(current_data_j, distribution, terminal, D), D);
                }
                else {
                    best_cost_j = cost_solution(best_data_j, D);
                    current_cost_j = cost_solution(current_data_j, D);
                }
                for (int i = 0; i < iterations; i++) {
                    vector<vector<vector<node>>> new_data = neighbor(current_data_j, n, distribution, terminal, D, indice);
                    int new_cost;
                    if (CC) {
                        new_cost = cost_solution(change_chains(new_data, distribution, terminal, D), D);
                    }
                    else {
                        new_cost = cost_solution(new_data, D);
                    }
                    float T = 1000/log(i+2);
                    float probability = min(float(1), exp(-(new_cost-current_cost)/T));
                    std::random_device rd;
                    std::mt19937 gen(rd());
                    std::uniform_real_distribution<> dis(0.0, 1.0);
                    if (dis(gen) < probability) {
                        current_cost_j = new_cost;
                        current_data_j = new_data;
                        if (current_cost_j < best_cost_j) {
                            best_cost_j = new_cost;
                            best_data_j = new_data;
                        }
                    }
                }
                if (best_cost_j < best_cost) {
                    best_data = best_data_j;
                    best_cost = best_cost_j;
                    cout << best_cost << " et k = " << j << endl;
                    if (CC) {
                        write(change_chains(best_data, distribution, terminal, D), string(name));
                    }
                    else {
                        write(best_data, string(name));
                    }
                }
            }
            cout << "indice : " << indice << endl;
        }
        cout << "REP = " << l << endl;
    }
    cout << best_cost << endl;
    return best_data;
}

