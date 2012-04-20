#include <iostream>
#include <cmath>
#include <string>

#include "GeneratorInterface/CosmicRayInterface/interface/CosmicRayHadronizer.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"

#include "CLHEP/Random/RandomEngine.h"
#include "CLHEP/Random/RandFlat.h"

#include <HepMC/GenCrossSection.h>
#include <HepMC/GenEvent.h>
#include <HepMC/HeavyIon.h>
#include <HepMC/PdfInfo.h>
#include <HepMC/Units.h>

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
using namespace edm;
using namespace std;
using namespace gen;

extern "C"
{
  float gen::rangen_()
  {
    double a = gFlatDistribution_->fire();
    return a;
  }
}

extern "C"
{
  double gen::drangen_(int *idummy)
  {
    double a = gFlatDistribution_->fire();
    return a;
  }
}

CosmicRayHadronizer::CosmicRayHadronizer(const ParameterSet &pset) :
  BaseHadronizer(pset),
  pset_(pset),
  gSeed(pset.getParameter<int>("seed")),
  gBeamMomentum(pset.getParameter<double>("beammomentum")),
  gTargetMomentum(pset.getParameter<double>("targetmomentum")),
  gBeamID(pset.getParameter<int>("beamid")),
  gTargetID(pset.getParameter<int>("targetid")),
  gHEModel(pset.getParameter<int>("model")),
  gParamFileName(pset.getParameter<string>("paramFileName")),
  gNEvent(0),
  gImpactParameter(0)
{
  // Default constructor

  setbuf(stdout,NULL); /* set output to unbuffered */

  Service<RandomNumberGenerator> rng;
  if ( ! rng.isAvailable())
    {
      throw cms::Exception("Configuration")
        << "XXXXXXX requires the RandomNumberGeneratorService\n"
        "which is not present in the configuration file.  You must add the service\n"
        "in the configuration file or remove the modules that require it.";
    }

  gFlatDistribution_.reset(new CLHEP::RandFlat(rng->getEngine(),  0., 1.));

  crmc_init_f_(gSeed,gBeamMomentum,gTargetMomentum,gBeamID,
               gTargetID,gHEModel,gParamFileName.c_str());

  //produces<HepMCProduct>();
}


//_____________________________________________________________________
CosmicRayHadronizer::~CosmicRayHadronizer()
{
  // destructor
  gFlatDistribution_.reset();
}

//_____________________________________________________________________
bool CosmicRayHadronizer::generatePartonsAndHadronize()
{
 crmc_f_(gNParticles,gImpactParameter,
         gPartID[0],gPartPx[0],gPartPy[0],gPartPz[0],
         gPartEnergy[0],gPartMass[0]);

  HepMC::GenEvent* evt = new HepMC::GenEvent();

  evt->set_event_number(gNEvent++);
  //evt->set_signal_process_id(20); //an integer ID uniquely specifying the signal process (i.e. MSUB in Pythia)

  //create event structure;
  HepMC::GenVertex* theVertex = new HepMC::GenVertex();
  evt->add_vertex( theVertex );

  //number of beam particles
  int nBeam = gTargetID + gBeamID;
  if (gTargetID == 207) // fix for lead wrong ID
    ++nBeam;
  if (gBeamID == 207)
    ++nBeam;

  for(int i = 0; i < gNParticles; i++)
    {
     if (gPartEnergy[i]*gPartEnergy[i] + 1e-9 < gPartPy[i]*gPartPy[i] + gPartPx[i]*gPartPx[i] + gPartPz[i]*gPartPz[i])
       cerr << "momentum off  Id:" << gPartID[i] << "(" << i << ") " << sqrt(fabs(gPartEnergy[i]*gPartEnergy[i] - (gPartPy[i]*gPartPy[i] + gPartPx[i]*gPartPx[i] + gPartPz[i]*gPartPz[i]))) << endl;
       //status 1 means final particle
      theVertex->add_particle_out( new HepMC::GenParticle( HepMC::FourVector(gPartPx[i], gPartPy[i], gPartPz[i], gPartEnergy[i]), gPartID[i], (i < nBeam)?4:1)); //beam particles status = 4, final status =1;
    }

  if (nBeam > 2)
    {

      HepMC::HeavyIon ion(-1, //cevt_.koievt, // FIXME // Number of hard scatterings
                          cevt_.npjevt,                // Number of projectile participants
                          cevt_.ntgevt,                // Number of target participants
                          cevt_.kolevt,                // Number of NN (nucleon-nucleon) collisions
                          cevt_.npnevt + cevt_.ntnevt, // Number of spectator neutrons
                          cevt_.nppevt + cevt_.ntpevt, // Number of spectator protons
                          -1, //c2evt_.ng1evt, //FIXME // Number of N-Nwounded collisions
                          -1, //c2evt_.ng2evt, //FIXME // Number of Nwounded-N collisons
                          -1, //c2evt_.ikoevt, //FIXME // Number of Nwounded-Nwounded collisions
                          cevt_.bimevt,                // Impact Parameter(fm) of collision
                          cevt_.phievt,                // Azimuthal angle of event plane
                          c2evt_.fglevt,               // eccentricity of participating nucleons
                          hadr5_.sigineaa);            // nucleon-nucleon inelastic
      evt->set_heavy_ion(ion);
    }
  //evt->print();
  event().reset(evt);
  return true;
}

//_____________________________________________________________________
bool CosmicRayHadronizer::hadronize()
{
   return false;
}

bool CosmicRayHadronizer::decay()
{
   return true;
}

bool CosmicRayHadronizer::residualDecay()
{
   return true;
}

void CosmicRayHadronizer::finalizeEvent(){
    return;
}

void CosmicRayHadronizer::statistics(){
    return;
}

const char* CosmicRayHadronizer::classname() const
{
   return "gen::CosmicRayHadronizer";
}

bool CosmicRayHadronizer::declareStableParticles ( const std::vector<int> )
{
 return true;
 }

bool CosmicRayHadronizer::initializeForInternalPartons()
{
 return true;
 }
