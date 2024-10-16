#include <iostream>
#include <stack>
#include <string>
#include <cctype>
#include <stdexcept>

using namespace std;

// Функция для определения приоритета операций
int precedence(char c) {
    if (c == '+' || c == '-')
        return 1;
    if (c == '*' || c == '/')
        return 2;
    return 0;
}

// Функция для выполнения арифметической операции
int performOperation(char operation, int operand1, int operand2) {
    switch (operation) {
        case '+': return operand1 + operand2;
        case '-': return operand1 - operand2;
        case '*': return operand1 * operand2;
        case '/': 
            if (operand2 == 0) 
                throw runtime_error("Error: Division by zero");
            return operand1 / operand2;
        default: 
            throw runtime_error("Error: Invalid operation");
    }
}

// Функция для преобразования инфиксного выражения в постфиксное
string infixToPostfix(const string &expression) {
    stack<char> operators;
    string postfix = "";

    for (size_t i = 0; i < expression.length(); ++i) {
        char current = expression[i];

        // Пропускаем пробелы
        if (isspace(current)) continue;

        // Если символ является цифрой, обрабатываем целое число
        if (isdigit(current)) {
            while (i < expression.length() && isdigit(expression[i])) {
                postfix += expression[i++];
            }
            postfix += ' ';
            i--; // Вернуться на один символ назад, так как цикл ушёл вперёд
        }
        // Обработка открывающей скобки
        else if (current == '(') {
            operators.push(current);
        }
        // Обработка закрывающей скобки
        else if (current == ')') {
            while (!operators.empty() && operators.top() != '(') {
                postfix += operators.top();
                postfix += ' ';
                operators.pop();
            }
            operators.pop(); // Убираем открывающую скобку
        }
        // Обработка операторов
        else if (current == '+' || current == '-' || current == '*' || current == '/') {
            while (!operators.empty() && precedence(operators.top()) >= precedence(current)) {
                postfix += operators.top();
                postfix += ' ';
                operators.pop();
            }
            operators.push(current);
        } else {
            throw runtime_error("Error: Invalid character in expression");
        }
    }

    // Выталкиваем оставшиеся операторы
    while (!operators.empty()) {
        postfix += operators.top();
        postfix += ' ';
        operators.pop();
    }

    return postfix;
}

// Функция для вычисления постфиксного выражения
int evaluatePostfix(const string &expression) {
    stack<int> operands;

    for (size_t i = 0; i < expression.length(); ++i) {
        char current = expression[i];

        // Пропускаем пробелы
        if (isspace(current)) continue;

        // Если символ является цифрой, собираем число
        if (isdigit(current)) {
            int number = 0;
            while (i < expression.length() && isdigit(expression[i])) {
                number = number * 10 + (expression[i++] - '0');
            }
            operands.push(number);
            i--; // Вернуться на один символ назад, чтобы не пропустить ничего
        }
        // Обработка операторов
        else if (current == '+' || current == '-' || current == '*' || current == '/') {
            if (operands.size() < 2) {
                throw runtime_error("Error: Invalid postfix expression");
            }

            int operand2 = operands.top(); operands.pop();
            int operand1 = operands.top(); operands.pop();

            int result = performOperation(current, operand1, operand2);
            operands.push(result);
        }
    }

    if (operands.size() != 1) {
        throw runtime_error("Error: Invalid postfix expression");
    }

    return operands.top();
}

// Главная функция
int main() {
    try {
        string infixExpression;
        cout << "Enter an infix expression: ";
        getline(cin, infixExpression);

        string postfixExpression = infixToPostfix(infixExpression);
        cout << "Postfix expression: " << postfixExpression << endl;

        int result = evaluatePostfix(postfixExpression);
        cout << "Result: " << result << endl;
    } catch (const exception &e) {
        cerr << e.what() << endl;
    }

    return 0;
}
