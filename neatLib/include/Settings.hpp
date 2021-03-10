#ifndef __SETTINGS_H_
#define __SETTINGS_H_

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
    };
}


#endif // __SETTINGS_H_
