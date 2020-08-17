#include "utils.h"
#include <dirent.h>

bool runTest(std::string file_name, std::string& buffer)
{
    std::string libyaml_error_string = "";

    std::vector<YAML::Node> libyaml_final_output_nodes = 
        normalizeLibyaml(parseLibyaml(file_name, &libyaml_error_string), &libyaml_error_string);

    std::string libyaml_final_output = normalizeYamlCpp(&libyaml_final_output_nodes, &libyaml_error_string);

    if (!libyaml_error_string.empty())
    {
        libyaml_final_output = libyaml_error_string;
    }

    std::string yamlcpp_error_msg = "";

    std::vector<YAML::Node> parsed_nodes= parseYamlCpp(file_name, &yamlcpp_error_msg);

    std::string yamlcpp_final_output;
    if (!yamlcpp_error_msg.empty())
    {
        std::cout << "Oops" << std::endl;
        yamlcpp_final_output = yamlcpp_error_msg;
    }
    else
    {
        std::cout << "Eeps" << std::endl;
        yamlcpp_final_output = normalizeYamlCpp
                (&parsed_nodes, &yamlcpp_error_msg);

        if (!yamlcpp_error_msg.empty())
        {
            std::cout << "Uups" << std::endl;
            yamlcpp_final_output = yamlcpp_error_msg;
        }
    }

    bool test = compareStringsCustom(libyaml_final_output, yamlcpp_final_output, buffer);

    return test;
}

// ---------------------------------------------------------------------------------
// --------------------------- typical test helper ---------------------------------
// ---------------------------------------------------------------------------------


bool typicalPositiveTest(std::string name)
{
    std::string buffer = "";

    std::string full_name = ("../examples/" + name);

    std::cerr << "+--- test: " + full_name << std::endl;
    
    bool return_me = runTest(full_name, buffer);

    if (!return_me)
    {
        std::cerr << "+--- test: " + full_name << std::endl;
        std::cerr << "+- FAIL" << std::endl;
        std::cerr << buffer << std::endl;
        std::cerr << "+---" << std::endl;
    }

    return !return_me;
}

bool typicalNegativeTest(std::string name)
{
    std::string buffer = "";

    std::string full_name = ("../examples/" + name);

    bool return_me = runTest(full_name, buffer);

    if (return_me)
    {
        std::cerr << "+--- test: " + full_name << std::endl;
        std::cerr << "+- FAIL" << std::endl;
        std::cerr << buffer << std::endl;
        std::cerr << "+---" << std::endl;
    }

    return return_me;
}

// ---------------------------------------------------------------------------------
// -------------------------------------- main -------------------------------------
// ---------------------------------------------------------------------------------
int main()
{

    std::string path = "../examples/";

    DIR *dir;
    struct dirent *ent;

    std::ofstream myfile;

    myfile.open ("autoreport.txt");

    if ((dir = opendir ("../examples/")) != NULL) 
    {
        /* print all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL) 
        {
            if (ent->d_name[0] != '.')
            {
                if (typicalPositiveTest(std::string(ent->d_name)))
                {
                    myfile << "../examples/" << std::string(ent->d_name) << std::endl;
                }
            }
        }
        closedir (dir);
    }
    return 0;
}
