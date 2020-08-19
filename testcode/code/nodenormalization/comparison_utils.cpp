#include "comparison_utils.h"

void disectSequenceNode(YAML::Node* disect_me, std::stack <YAML::Node>* data_save_stack, 
    std::stack <char>* additional_info_stack)
{
    for (int i = disect_me->size() - 1; i >= 0; i--) 
    {
        data_save_stack->push((*disect_me)[i]);
        additional_info_stack->push('L');
    }
}

void disectMapNode(YAML::Node* disect_me, std::stack <YAML::Node>* data_save_stack,
    std::stack <char>* additional_info_stack)
{
    std::stack <YAML::const_iterator> loca_iterators_temp_stack;

    for (YAML::const_iterator it = disect_me->begin(); it != disect_me->end(); ++it) 
    {
        loca_iterators_temp_stack.push(it);
    }

    while (!loca_iterators_temp_stack.empty())
    {
        YAML::const_iterator it = loca_iterators_temp_stack.top();
        loca_iterators_temp_stack.pop();

        data_save_stack->push(it->second);
        additional_info_stack->push('V');
        data_save_stack->push(it->first);
        additional_info_stack->push('K');
    }
}

bool compareSingleNode
    (const YAML::Node* compare_me_one,const YAML::Node* compare_me_two)
{
    std::stack <YAML::Node> iteration_list_stack_one;

    std::stack <char> additional_info_stack_one;
    additional_info_stack_one.push(' ');

    iteration_list_stack_one.push(*compare_me_one);
    additional_info_stack_one.push('U');

    // second set of comparison

    std::stack <YAML::Node> iteration_list_stack_two;

    std::stack <char> additional_info_stack_two;
    additional_info_stack_two.push(' ');

    iteration_list_stack_two.push(*compare_me_two);
    additional_info_stack_two.push('U');

    while (!iteration_list_stack_one.empty() && !iteration_list_stack_two.empty())
    {
        YAML::Node base_iterator_one = iteration_list_stack_one.top();
        YAML::Node base_iterator_two = iteration_list_stack_two.top();

        if (additional_info_stack_two.top() != additional_info_stack_one.top())
        {
            return false;
        }

        iteration_list_stack_one.pop();

        additional_info_stack_one.pop();

        iteration_list_stack_two.pop();

        additional_info_stack_two.pop();

        if ((base_iterator_one.Tag() != "?" && base_iterator_one.Tag() != "!" && 
            base_iterator_one.Tag() != "") || (base_iterator_two.Tag() != "?" && 
            base_iterator_two.Tag() != "!" && base_iterator_two.Tag() != ""))
        {
            if (base_iterator_one.Tag() != base_iterator_two.Tag())
            {
                return false;
            }
        }

        if ((base_iterator_one.Type() == YAML::NodeType::Null) && 
            (base_iterator_two.Type() == YAML::NodeType::Null))
        {
            if ((additional_info_stack_two.top() == 'U' && additional_info_stack_one.top() == 'U') ||
                (additional_info_stack_two.top() != 'U' && additional_info_stack_one.top() != 'U'))
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else if ((base_iterator_one.Type() == YAML::NodeType::Scalar) && 
            (base_iterator_two.Type() == YAML::NodeType::Scalar))
        {
            if (base_iterator_one.as<std::string>() != base_iterator_two.as<std::string>())
            {
                return false;
            }
        }
        else if ((base_iterator_one.Type() == YAML::NodeType::Sequence) && 
            (base_iterator_two.Type() == YAML::NodeType::Sequence))
        {
            disectSequenceNode
                (&base_iterator_one, &iteration_list_stack_one, &additional_info_stack_one);
            disectSequenceNode
                (&base_iterator_two, &iteration_list_stack_two, &additional_info_stack_two);
        }
        else if ((base_iterator_one.Type() == YAML::NodeType::Map) && 
            (base_iterator_two.Type() == YAML::NodeType::Map))
        {
            disectMapNode
                (&base_iterator_one, &iteration_list_stack_one, &additional_info_stack_one);
            disectMapNode
                (&base_iterator_two, &iteration_list_stack_two, &additional_info_stack_two);
        }
        else if ((base_iterator_one.Type() == YAML::NodeType::Undefined) && 
            (base_iterator_two.Type() == YAML::NodeType::Undefined))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    return iteration_list_stack_one.empty() && iteration_list_stack_two.empty();
}

bool compareMultipleNodes
    (const std::vector<YAML::Node>* compare_me_one,const std::vector<YAML::Node>* compare_me_two)
{
    std::vector<YAML::Node>::const_iterator iterator_one = compare_me_one->begin();
    std::vector<YAML::Node>::const_iterator iterator_two = compare_me_two->begin();

    while (iterator_one != compare_me_one->end() && iterator_two != compare_me_two->end() &&
        compareSingleNode(&(*iterator_one), &(*iterator_two)))
    {
        iterator_one++;
        iterator_two++;
    }

    return (iterator_one == compare_me_one->end() && iterator_two == compare_me_two->end());
}