#ifndef VOWEL_COUNTER
#define VOWEL_COUNTER

#include "Marker.hpp"

class VowelCounter: public Marker
{
    public:
        VowelCounter(std::string config);

        TestResult Mark(std::string file);
        std::string GetFeedback();
    
    private:
        
};

#endif