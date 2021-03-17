#ifndef __SETTINGS_H_
#define __SETTINGS_H_

#include <stdlib.h>

namespace neat {
    struct Settings {
        float toKill = 0.25;
        float similarity = 3;
        int maxStagnation = 15;
        float crossoverRate = 0.75;
        float weightMutationRate = 0.8;
        float weightRandomRate = 0.1;
        float weightChangeRate = 0.9;
        float sigmoidMult = 4.9;
        float interspeciesCrossoverRate = 0.001;
        float addNodeRate = 0.03;
        float addLinkRate = 0.03;
        float similarityCoefExcess = 1;
        float similarityCoefDisjoint = 1;
        float similarityCoefWeight = 0.4;
        float mutationChangeWeight = 0.1;
        int maxMutations = 10;
        int minMutations = 5;
        float doMutation = 0.9;
        float maxMutationWeight = 1.0;

        static bool doRand(float value) {
            return rand() % ((int)(1.0 / value) * 1000) < 1000;
        }
    };
}


#endif // __SETTINGS_H_
