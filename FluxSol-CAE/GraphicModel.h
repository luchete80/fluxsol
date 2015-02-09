#include "FluxSol.h"

class GraphicModel:public Model
{

};

class GraphicCFDModel:public CFDModel
{

    public:
        GraphicCFDModel(const std::string s):CFDModel(s){};
};
