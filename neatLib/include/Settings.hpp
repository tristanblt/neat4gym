#ifndef __SETTINGS_H_
#define __SETTINGS_H_

#include <stdlib.h>

namespace neat {
    struct Settings {
        float toKill = 0.5;
        float similarity = 2;
        int maxStagnation = 15;
        float crossoverRate = 0;
        float weightMutationRate = 0.8;
        float weightRandomRate = 0.1;
        float weightChangeRate = 0.9;
        float sigmoidMult = 2;
        float interspeciesCrossoverRate = 0.001;
        float addNodeRate = 0.03;
        float addLinkRate = 0.03;
        float similarityCoefExcess = 1;
        float similarityCoefDisjoint = 1;
        float similarityCoefWeight = 0.4;
        float mutationChangeWeight = 0.1;
        int minMutations = 10;
        // float doMutation = 0.9;
        float maxMutationWeight = 0.1;

        static bool doRand(float value) {
            return value == 0 ? false : rand() % ((int)(1.0 / value) * 1000) < 1000;
        }
    };
}


#endif // __SETTINGS_H_
