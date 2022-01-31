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
            if (!lands[i].is_attaking && lands[i].side != 0 && lands[i].side != 1 && i != j) {
                add_soldiers(soldiers_n, max_soldiers, soldiers_ptr,
                             &lands[i], &lands[j]);
            }
        }
       
    }
    
}
