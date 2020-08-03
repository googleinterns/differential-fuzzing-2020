#include "testutils.cpp"
#include <dirent.h>

bool runTest(std::string file_name, std::string& buffer)
{
    std::string libyaml_error_string;

    std::string libyaml_final_output = parseLibyaml(file_name, libyaml_error_string);

    if(!libyaml_error_string.empty())
    {
        libyaml_final_output = libyaml_error_string;
    }

    std::string yamlcpp_final_output;

    std::string yamlcpp_error_msg;

    try
    {   
        std::ifstream stream_input_to_yamlcpp;

        stream_input_to_yamlcpp.open(file_name);

        std::string input_to_yamlcpp;

        std::string temp_stream_parser_line;

        while(std::getline(stream_input_to_yamlcpp, temp_stream_parser_line))
        {
            input_to_yamlcpp += temp_stream_parser_line + "\n";
        }


        std::string::size_type prev = 0, current = 0;

        while ((current = input_to_yamlcpp.find("\n---", current)) != std::string::npos)
        {

            YAML::Node node = YAML::Load(input_to_yamlcpp.substr(prev, current-prev));

            yamlcpp_final_output += parseYamlCppNode(node, yamlcpp_error_msg);

            prev = ++current;
        }

        YAML::Node node = YAML::Load(input_to_yamlcpp.substr(prev, current-prev));

        yamlcpp_final_output += parseYamlCppNode(node, yamlcpp_error_msg);

        if(!yamlcpp_error_msg.empty())
        {
            yamlcpp_final_output = yamlcpp_error_msg;
        }
    }
    catch (const std::exception& err)
    {
        yamlcpp_final_output = "ERROR";
    }

    bool test = compareStringsCustom(libyaml_final_output, yamlcpp_final_output, buffer);

    return test;
}

// ---------------------------------------------------------------------------------
// --------------------------- tipical test helper ---------------------------------
// ---------------------------------------------------------------------------------


bool tipicalPositiveTest(std::string name)
{
    std::string buffer = "";

    std::string full_name = ("../examples/" + name);

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

bool tipicalNegativeTest(std::string name)
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
            if(ent->d_name[0] != '.')
            {
                if (tipicalPositiveTest(std::string(ent->d_name)))
                {
                    myfile << std::string(ent->d_name) << std::endl;
                }
            }
        }
        closedir (dir);
    }

    myfile.close();

    return 0;
}
