#include <iostream>
#include <string>
#include <queue>
#include <stack>

int preVal(std::string opComp)
{
	// Associating precedence value with each operator.
	int precedence;
	switch (opComp[0])
	{
	case '(':
		precedence = 5;
		break;
	case '^':
		precedence = 4;
		break;
	case '/':
	case '*':
		precedence = 3;
		break;
	case '+':
	case '-':
		precedence = 2;
		break;
	case ')':
		precedence = 1;
		break;
	default:
		precedence = 0;
	}
	return precedence;
}

int main()
{
	// Initialising variables and obtaining initial equation.
	std::cout << "Enter Input: ";
	std::string input;
	std::getline(std::cin >> std::ws, input);
	std::queue<std::string> original, rpn;
	std::stack<std::string> opStack;

	// Separating string into it's individual constituent components.
	for (int i = 0; i < input.size(); i++)
	{
		if (input[i] > 47 && input[i] < 58) // Check to see if it's a number, if so check for more digits.
		{
			for (int j = i + 1; j < input.size() + 1; j++)
			{
				if (input[j] < 48 || input[j] > 57) // A non-number is found and therefore the end of the number.
				{
					original.push(input.substr(i, j - i));
					i = j - 1;
					break;
				}
			}
		}
		else if (input[i] != ' ') // Obtaining operators that aren't whitespaces.
		{
			std::string temp; // Char needs to be a string.
			original.push(temp.insert(0, 1, input[i]));
		}
	}

	while (!original.empty())
	{
		// Add to rpn if its a number.
		if (original.front()[0] > 47 && original.front()[0] < 58)
		{
			rpn.push(original.front());
			original.pop();
		}
		else
		{
			// If operator stack is empty or the precedence of the original is higher than that of the operator stack top/top is '(', just put onto the operator stack.
			if (opStack.empty() || preVal(opStack.top()) == 5 || preVal(opStack.top()) < preVal(original.front()) || (preVal(original.front()) == 4 && preVal(opStack.top()) == 4))
			{
				opStack.push(original.front());
				original.pop();
			}
			// If its a closing bracket, push all operators onto the final stack until it finds the opening bracket, then remove both brackets.
			else if (preVal(original.front()) == 1)
			{
				while (preVal(opStack.top()) != 5)
				{
					rpn.push(opStack.top());
					opStack.pop();
				}
				opStack.pop();
				original.pop();
			}
			// If precedence of original is lower/equal to the operator top, then pop off operator stack and put to rpn and add operator to operator stack.
			else
			{
				while (!opStack.empty() && preVal(opStack.top()) >= preVal(original.front()))
				{
					rpn.push(opStack.top());
					opStack.pop();
				}
				opStack.push(original.front());
				original.pop();
			}
		}
	}
	// Pushing the remaining operators to the rpn
	while (!opStack.empty())
	{
		rpn.push(opStack.top());
		opStack.pop();
	}


	// Calculation without binary tree
	std::stack<std::string> calStack;
	double val1, val2, result;
	int end = rpn.size();

	for (int i = 0; i < end; i++)
	{
		if (rpn.front()[0] > 47 && rpn.front()[0] < 58)
		{
			calStack.push(rpn.front());
			rpn.pop();
		}
		else
		{
			val1 = std::stod(calStack.top());
			calStack.pop();
			val2 = std::stod(calStack.top());
			calStack.pop();
			switch (rpn.front()[0])
			{
			case '*':
				result = val2 * val1;
				break;
			case '/':
				result = val2 / val1;
				break;
			case '+':
				result = val2 + val1;
				break;
			case '-':
				result = val2 - val1;
				break;
			case '^':
				result = pow(val2, val1);
				break;
			}
			calStack.push(std::to_string(result));
			rpn.pop();
		}
	}
	std::cout << calStack.top();

	return 0;
}