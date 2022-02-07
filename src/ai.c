#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "map.h"
#include "soldier.h"
#include "ai.h"

void check_bot_attack(int lands_n, Land lands[], int* soldiers_n,
                      int* max_soldiers, Soldier **soldiers_ptr) {
    for (int i = 0; i < lands_n; i++) {
        for (int j = 0; j < lands_n; j++) {
            if (lands[i].side != 1 && lands[i].side != 0) {
            if (lands[i].soldiers > lands[j].soldiers + 5 && lands[i].side != lands[j].side && rand()%150 == 0 && !lands[i].is_attaking && lands[j].side != 0) {
                add_soldiers(soldiers_n, max_soldiers, soldiers_ptr, &lands[i], &lands[j]);
                break;
            }

            if (lands[i].soldiers > lands[j].soldiers + 9 && lands[i].side != lands[j].side && rand()%180 == 0 && !lands[i].is_attaking && lands[j].side == 0) {
                add_soldiers(soldiers_n, max_soldiers, soldiers_ptr, &lands[i], &lands[j]);
                break;
            }

            }

           
        }
       
    }
    
}
