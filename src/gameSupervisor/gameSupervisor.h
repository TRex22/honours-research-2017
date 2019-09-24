#include "../definitions.h"

#include <stdio.h>
#include <cstdio>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <omp.h>
#include <time.h>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>
#include <random>
#include <limits.h>

#include "../helpers/rng.cpp"

/*Actual supervisor stuff*/
/*Enum*/
#include "playerEnum.h"
#include "rulesEnum.h"
#include "locationEnum.h"

/*Classes*/
#include "card.h"
#include "attackPoints.h"
#include "classFunctions.cpp"
#include "playerInfo.h"
#include "gameState.h"

/*rules*/
#include "roleSupervisor.cpp"
#include "boardFunctions.cpp"

/*Gameplay and AI stuff*/
#include "../helpers/tree.h"
#include "../AI/AI.h"
#include "gameFunctions.cpp"
