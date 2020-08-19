#include "libyaml_utils.h"
#include "yamlcpp_utils.h"
#include "comparison_utils.h"
#include "utils.h"
#include <dirent.h>

bool runTest(std::string file_name, std::string& buffer)
{
    std::string libyaml_error_string;

    std::vector<YAML::Node> libyaml_final_output_nodes = 
        normalizeLibyaml(parseLibyaml(file_name, &libyaml_error_string), &libyaml_error_string);

    std::string yamlcpp_error_msg;

    std::vector<YAML::Node> parsed_nodes= parseYamlCpp(file_name, &yamlcpp_error_msg);

    // normalizeYamlCpp(&parsed_nodes, &yamlcpp_error_msg);

    if( (!libyaml_error_string.empty() || !yamlcpp_error_msg.empty()))
    {
        return libyaml_error_string == yamlcpp_error_msg;
    }

    return compareMultipleNodes(&libyaml_final_output_nodes, &parsed_nodes);
}

// ---------------------------------------------------------------------------------
// --------------------------- typical test helper ---------------------------------
// ---------------------------------------------------------------------------------


bool typicalPositiveTest(std::string name)
{
    std::string buffer = "";

    std::string full_name = ("../examples/" + name);

    bool return_me = runTest(full_name, buffer);

    return !return_me;
}

bool typicalNegativeTest(std::string name)
{
    std::string buffer = "";

    std::string full_name = ("../examples/" + name);

    bool return_me = runTest(full_name, buffer);

    return return_me;
}

// ---------------------------------------------------------------------------------
// -------------------------------------- main -------------------------------------
// ---------------------------------------------------------------------------------

int main(int argc, char* args[])
{
    std::string path;
    if (argc >0)
    {
        std::string path = args[0];
    }
    else
    {
        //../../yaml-test-suite/test/
        //../examples/
        path = "../examples/";
    }

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
                    std::cout << "../examples/" << std::string(ent->d_name) << std::endl;
                    myfile << "../examples/" << std::string(ent->d_name) << std::endl;
                }
            }
        }
        closedir (dir);
    }
    return 0;
}
