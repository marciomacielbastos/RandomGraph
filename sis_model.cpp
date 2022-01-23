//#include "sis_model.h"

//Sis_model::Sis_model() {

//}

//void Sis_model::set_graph(Graph g) {
//    this->g = g;
//    restart();
//}

//void Sis_model::set_probability_to_infect(double p) {
//    this->probability_to_infect = p;
//}

//void Sis_model::set_mean_infectious_interval(double t) {
//    this->lambda = t;
//}


//bool Sis_model::infectious_is_empty() {
//    return this->infectious.empty();
//}

//bool Sis_model::try_infect(unsigned long int node_id) {
//    unsigned long int time;
//    Uniform uniform;
//    Poisson poisson(this->lambda);
//    bool did_infect = false;
//    if (this->probability_to_infect > uniform.random()) {
//        time = poisson.randint();
//        if (time > 0) {
//            this->infectious_time[node_id] = time;
//            did_infect = true;
//        }
//    }
//    return did_infect;
//}

//std::list<unsigned long int> Sis_model::infect() {
//    unsigned long int time;
//    std::list<unsigned long int> infecteds;
//    std::vector<std::vector<unsigned long int>> adj_matrix = this->g.get_adj_matrix();
//    for (auto i : this->infectious) {
//        for (auto s : adj_matrix[i]){
//            if (this->infectious_time[s] == 0) {
//                if(try_infect(s)) {
//                    infecteds.push_back(s);
//                }
//            }
//        }
//    }
//    return infecteds;
//}

////bool Sis_model::try_cure(unsigned long int node_id) {
////    Uniform uniform = Uniform();
////    bool did_cure = false;
////    if (this->probability_to_cure > uniform.random()){
////        this->infectious_days[node_id] = true;
////        did_cure = true;
////    }
////    return did_cure;
////}

//void Sis_model::cure(){
//    unsigned long int node_id;
//    std::list<unsigned long int> still_infectious;
//    while (!this->infectious.empty()) {
//        node_id = this->infectious.back();
//        this->infectious.pop_back();
//        this->infectious_time[node_id]--;
//        if ( this->infectious_time[node_id] > 0) {
//            still_infectious.push_back(node_id);
//        }
//    }
//    this->infectious = still_infectious;
//}

////void Sis_model::cure(){
////    unsigned long int node_id;
////    std::list<unsigned long int> still_infectious;
////    while (!this->infectious.empty()) {
////        node_id = this->infectious.back();
////        this->infectious.pop_back();
////        if ( this->infectious_time[node_id] > 0) {
////            this->infectious_time[node_id]--;
////            still_infectious.push_back(node_id);
////        }
////    }
////    this->infectious = still_infectious;
////}

//void Sis_model::update_infectious(std::list<unsigned long int> infecteds) {
//    this->infectious.splice(this->infectious.end(), infecteds);
//}

//void Sis_model::randomly_infect() {
//    Uniform u;
//    Poisson p(this->lambda);
//    unsigned long int node_id = u.randint(this->g.get_N());
//    unsigned long int time;
//    if (this->infectious_time[node_id] == 0) {
//        time = p.randint();
//        if (time > 0) {
//            this->infectious.push_back(node_id);
//            this->infectious_time[node_id] = time;
//        }
//    }
//}

//void Sis_model::run_dynamics() {
//    /* First create a list of infecteds, they should mix with the current
//       infectious population before the cure phase*/
//    std::list<unsigned long int> infecteds = infect();
//    /* Update infectious list, removing those that have been cured */
//    cure();
//    /* Add the new infecteds to the list of (still) infectious elements */
//    update_infectious(infecteds);
//}

//void Sis_model::record_number_of_infecteds() {
//    this->infectious_record.push_back(this->infectious.size());
//}

//std::vector<unsigned long> Sis_model::get_infectious_record() {
//    return this->infectious_record;
//}

//void Sis_model::restart() {
//    std::vector<unsigned long int> infectious_time(this->g.get_N(), 0);
//    this->infectious_time = infectious_time;
//    while (!this->infectious.empty()) {
//        this->infectious.pop_back();
//    }
//    while (!this->infectious_record.empty()) {
//        this->infectious_record.pop_back();
//    }
//}

//void Sis_model::save(std::string filepath, std::string separator) {
//    std::ofstream myfile;
//    myfile.open (filepath);
//    for(unsigned long int i = 0;  i < this->infectious_record.size(); i++ ){
//        if (i == this->infectious_record.size() - 1) {
//            myfile << this->infectious_record[i];
//        }
//        else {
//            myfile << this->infectious_record[i] << separator;
//        }
//    }
//    myfile.close();
//}
