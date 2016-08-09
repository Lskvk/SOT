/* 
 * File:   test_dycors.cpp
 * Author: David Eriksson
 *
 * Created on July 19, 2016, 12:44 PM
 */ 

#include <sot.h>
using namespace sot;

int test_ga_sampling() {

    int dim = 10;
    int maxEvals = 500;  
     
    std::shared_ptr<Problem> data(std::make_shared<UnitBoxProblem<Ackley>>(dim));
    std::shared_ptr<ExpDesign> slhd(std::make_shared<SLHD>(2*(dim+1), dim));
    std::shared_ptr<Surrogate> rbf(std::make_shared<CubicRBF>(maxEvals, dim, data->lBounds(), data->uBounds()));
    std::shared_ptr<Sampling> gaSampling(std::make_shared<GASampling>(data, rbf, 50, 100));
    
    setSeedRandom();
    Optimizer opt(data, slhd, rbf, gaSampling, maxEvals);
    Result res = opt.run();

    // Best value found
    std::cout << res.fBest() << std::endl;

    // Scale back the best solution to the original domain
    std::shared_ptr<Problem> origData(std::make_shared<Ackley>(dim)); // Non-scaled version of the problem

    // Best solution in the original domain
    std::cout << fromUnitBox(res.xBest(), origData->lBounds(), origData->uBounds()).t() << std::endl;

    // Check that we made enough progress and that we are feasible
    if (res.fBest() > -20.0) {
        return (EXIT_FAILURE);
    }
    if (not arma::all(res.xBest() >= data->lBounds())) {
        return (EXIT_FAILURE);
    }    
    if (not arma::all(res.xBest() <= data->uBounds())) {
        return (EXIT_FAILURE);
    }
    
    return EXIT_SUCCESS;
}

int main(int argc, char** argv) {
    return test_ga_sampling();
}