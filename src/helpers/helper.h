using namespace std;
using namespace rapidjson;

/*std::vector<std::vector<int>> convert_array_to_vector_vector(int** __currentBoard);*/
rapidjson::Document openJsonFile(string filepath);
std::vector<Card> convertJsonToVectorDeck(string northernpath);
bool check_if_ignore_card(int typeId);

void halt_execution(std::string message);
void write_results_to_file (std::string filename, std::string results);
void write_results_to_file (std::string filename, std::string header, std::string results);
void print_cmd_heading(std::string app_name);

void print_usage(char *argv[]);


#include "../helpers/helper.cpp"