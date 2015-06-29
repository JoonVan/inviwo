/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2015 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *********************************************************************************/

/*
The MIT License (MIT)

Copyright (c) 2013 Brandon Amos <http://bamos.github.io>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

// Source:
// http://www.daniweb.com/software-development/cpp/code/427500/calculator-using-shunting-yard-algorithm#
// Author: Jesse Brown
// Modifications: Brandon Amos

#include <inviwo/core/util/shuntingyard.h>

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <math.h>

namespace inviwo {
namespace shuntingyard {

#define isvariablechar(c) (isalpha(c) || c == '_')
TokenQueue Calculator::toRPN(const char* expr, std::map<std::string, double>* vars,
                             std::map<std::string, int> opPrecedence) {
    TokenQueue rpnQueue;
    std::stack<std::string> operatorStack;
    bool lastTokenWasOp = true;

    // In one pass, ignore whitespace and parse the expression into RPN
    // using Dijkstra's Shunting-yard algorithm.
    while (*expr && isspace(*expr)) ++expr;
    while (*expr) {
        if (isdigit(*expr)) {
            // If the token is a number, add it to the output queue.
            char* nextChar = 0;
            double digit = strtod(expr, &nextChar);

            rpnQueue.push(new Token<double>(digit));
            expr = nextChar;
            lastTokenWasOp = false;
        } else if (isvariablechar(*expr)) {
            // If the function is a variable, resolve it and
            // add the parsed number to the output queue.
            if (!vars) throw std::domain_error("Detected variable, but the variable map is null.");

            std::stringstream ss;
            ss << *expr;
            ++expr;
            while (isvariablechar(*expr)) {
                ss << *expr;
                ++expr;
            }
            std::string key = ss.str();
            auto it = vars->find(key);
            if (it == vars->end()) {
                throw std::domain_error("Unable to find the variable '" + key + "'.");
            }
            double val = vars->find(key)->second;

            rpnQueue.push(new Token<double>(val));
            lastTokenWasOp = false;
        } else {
            // Otherwise, the variable is an operator or paranthesis.
            switch (*expr) {
                case '(':
                    operatorStack.push("(");
                    ++expr;
                    break;
                case ')':
                    while (operatorStack.top().compare("(")) {
                        rpnQueue.push(new Token<std::string>(operatorStack.top()));
                        operatorStack.pop();
                    }
                    operatorStack.pop();
                    ++expr;
                    break;
                default: {
                    // The token is an operator.
                    //
                    // Let p(o) denote the precedence of an operator o.
                    //
                    // If the token is an operator, o1, then
                    //   While there is an operator token, o2, at the top
                    //       and p(o1) <= p(o2), then
                    //     pop o2 off the stack onto the output queue.
                    //   Push o1 on the stack.
                    std::stringstream ss;
                    ss << *expr;
                    ++expr;
                    while (*expr && !isspace(*expr) && !isdigit(*expr) && !isvariablechar(*expr) &&
                           *expr != '(' && *expr != ')') {
                        ss << *expr;
                        ++expr;
                    }
                    ss.clear();
                    std::string str;
                    ss >> str;

                    if (lastTokenWasOp) {
                        // Convert unary operators to binary in the RPN.
                        if (!str.compare("-") || !str.compare("+")) {
                            rpnQueue.push(new Token<double>(0));
                        } else {
                            throw std::domain_error("Unrecognized unary operator: '" + str + "'.");
                        }
                    }

                    while (!operatorStack.empty() &&
                           opPrecedence[str] <= opPrecedence[operatorStack.top()]) {
                        rpnQueue.push(new Token<std::string>(operatorStack.top()));
                        operatorStack.pop();
                    }
                    operatorStack.push(str);
                    lastTokenWasOp = true;
                }
            }
        }
        while (*expr && isspace(*expr)) ++expr;
    }
    while (!operatorStack.empty()) {
        rpnQueue.push(new Token<std::string>(operatorStack.top()));
        operatorStack.pop();
    }
    return rpnQueue;
}

double Calculator::calculate(const char* expr, std::map<std::string, double>* vars) {
    // 1. Create the operator precedence map.
    std::map<std::string, int> opPrecedence;
    opPrecedence["("] = -1;
    opPrecedence["<<"] = 1;
    opPrecedence[">>"] = 1;
    opPrecedence["+"] = 2;
    opPrecedence["-"] = 2;
    opPrecedence["*"] = 3;
    opPrecedence["/"] = 3;
    opPrecedence["^"] = 4;

    // 2. Convert to RPN with Dijkstra's Shunting-yard algorithm.
    TokenQueue rpn = toRPN(expr, vars, opPrecedence);

    // 3. Evaluate the expression in RPN form.
    std::stack<double> evaluation;
    while (!rpn.empty()) {
        TokenBase* base = rpn.front();
        rpn.pop();

        Token<std::string>* strTok = dynamic_cast<Token<std::string>*>(base);
        Token<double>* doubleTok = dynamic_cast<Token<double>*>(base);
        if (strTok) {
            std::string str = strTok->val;
            if (evaluation.size() < 2) {
                throw std::domain_error("Invalid equation.");
            }
            double right = evaluation.top();
            evaluation.pop();
            double left = evaluation.top();
            evaluation.pop();
            if (!str.compare("+")) {
                evaluation.push(left + right);
            } else if (!str.compare("*")) {
                evaluation.push(left * right);
            } else if (!str.compare("-")) {
                evaluation.push(left - right);
            } else if (!str.compare("/")) {
                evaluation.push(left / right);
            } else if (!str.compare("<<")) {
                evaluation.push((int)left << (int)right);
            } else if (!str.compare("^")) {
                evaluation.push(pow(left, right));
            } else if (!str.compare(">>")) {
                evaluation.push((int)left >> (int)right);
            } else {
                throw std::domain_error("Unknown operator: '" + str + "'.");
            }
        } else if (doubleTok) {
            evaluation.push(doubleTok->val);
        } else {
            throw std::domain_error("Invalid token.");
        }
        delete base;
    }
    return evaluation.top();
}

}  // namespace

}  // namespace
