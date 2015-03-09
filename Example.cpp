#include "DFA.h"
#include "NFA.h"
#include "EpsilonNFA.h"
#include <iostream>

int main() {
    DFA even_zeroes(
        {"q1", "q2"},            // set of states (Q)
        {"0", "1"},              // alphabet (Sigma)
        {                        // transition table (delta)
            {{"q1", "0"}, "q2"},
            {{"q1", "1"}, "q1"},
            {{"q2", "0"}, "q1"},
            {{"q2", "1"}, "q2"},
        },
        "q1",                    // initial state (q0)
        {"q1"}                   // set of final states (F)
    );

    std::cout << "Does 0 have an even amount of zeroes? ";
    std::cout << (even_zeroes.accepts({"0"}) ? "yes" : "no")
        << std::endl;
    std::cout << "Does 00 have an even amount of zeroes? ";
    std::cout << (even_zeroes.accepts({"0", "0"}) ? "yes" : "no")
        << std::endl;
    std::cout << "Does 000 have an even amount of zeroes? ";
    std::cout << (even_zeroes.accepts({"0", "0", "0"}) ? "yes" : "no")
        << std::endl;
    std::cout << "Does 0000 have an even amount of zeroes? ";
    std::cout << (even_zeroes.accepts({"0", "0", "0", "0"}) ? "yes" : "no")
        << std::endl;

    NFA ends_in_1(
        {"p", "q", "r"},                  // set of states (Q)
        {"0", "1"},                  // alphabet (Sigma)
        {                            // transition table (delta)
            {{"p", "0"}, {"p", "r"}},
            {{"p", "1"}, {"p", "q"}},
            {{"r", "0"}, {"q"}},
        },
        "p",                         // initial state (q0)
        {"q"}                        // set of final states (F)
    );
    
    DFA x = ends_in_1.ssc();

    std::cout << "Does 0 end in a one? ";
    std::cout << (x.accepts({"0"}) ? "yes" : "no")
        << std::endl;
    std::cout << "Does 00 end in a one? ";
    std::cout << (x.accepts({"0", "0"}) ? "yes" : "no")
        << std::endl;
    std::cout << "Does 001 end in a one? ";
    std::cout << (x.accepts({"0", "0", "1"}) ? "yes" : "no")
        << std::endl;
    std::cout << "Does 0010 end in a one? ";
    std::cout << (x.accepts({"0", "0", "1", "0"}) ? "yes" : "no")
        << std::endl;

    even_zeroes.print_dot();
    x.print_dot();
    
    return 0;
}

