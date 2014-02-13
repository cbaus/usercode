#include <algorithm>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <sstream>
#include <fstream>
#include <math.h>
#include <stdlib.h>


#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>


#include "HepMC/IO_HEPEVT.h"
#include "HepMC/GenParticle.h"
#include <HepMC/HeavyIon.h>
#include "HepMC/HEPEVT_Wrapper.h"
#include "HepMC/IO_GenEvent.h"
#include "HepMC/Units.h"
#include "HepMC/GenEvent.h"


#include <TF1.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TFile.h>
#include <TMath.h>
#include <TParticle.h>
#include <TParticlePDG.h>
#include <TProfile.h>
#include <TRFIOFile.h>

#include "analysisfiles.h"


namespace io = boost::iostreams;
using namespace std;

TParticle* CopyHepMC2ROOT(HepMC::GenParticle* a)
{
  TParticle* b = new TParticle;
  if(!b)
    return 0;
  b->SetPdgCode(a->pdg_id ());

  b->SetStatusCode(a->status ());

  b->SetMomentum(a->momentum ().px (),
		 a->momentum ().py (),
		 a->momentum ().pz (),
		 a->momentum ().e ()
		 );

  b->SetProductionVertex(a->production_vertex ()->position ().x (),
			 a->production_vertex ()->position ().y (),
			 a->production_vertex ()->position ().z (),
			 a->production_vertex ()->position ().t ()
			 );
  return b;
}


int main (int argc, char **argv)
{
  TH1::SetDefaultSumw2();

  set<int> numu;
  numu.insert (12);
  numu.insert (14);
  numu.insert (16);
  numu.insert (13);

  //-------------------DataManager
  DataManager data;

  //lhcf
  double cs = 1;
  bool cs_set = 0;

  //-------------------EVENT LOOP
  while (data.GetNextEvent())
    {
      HepMC::HeavyIon* heavyIonInfo = NULL;
      heavyIonInfo = data.evt->heavy_ion ();
      if(!cs_set && heavyIonInfo && data.energy=="7TeV")
	{
	  cs =  heavyIonInfo->sigma_inel_NN () / 1e9;
	  //cout << "!Cross section determined to be " << cs << endl;
	  cs_set = true;
	}
      //-------------------EVENT SELECTION
      double castor_e = 0;
      double castor_e_pi0 = 0;
      double castor_e_pic = 0;
      double castor_e_bar = 0;
      double castor_e_gam = 0;
      double castor_e_oth = 0;
      HepMC::GenEvent::particle_const_iterator par = data.evt->particles_begin ();
      for (par = data.evt->particles_begin (); par != data.evt->particles_end (); ++par)
	{
	  HepMC::GenParticle* p = (*par);
	  if (p->status () != 1 && p->status () != 2) continue;
	  HepMC::GenVertex* parent_vertex = p->production_vertex();
	  const int id = p->pdg_id ();
	  //const double pt = p->momentum ().perp ();
	  const double e = p->momentum ().e ();
	  const double eta = p->momentum ().eta ();	               
	  //const bool isNuMu = numu.count (id);	               

	  // const double lhcfz = 141.05;
	  // const double theta = p->momentum ().theta ();
	  // const double phi = p->momentum ().phi ();
	  // const double lhcfr = lhcfz * tan(theta);
	  // const double lhcfx = lhcfr * sin(phi);
	  // const double lhcfy = lhcfr * cos(phi); //which way does not really matter since sym


	  TParticle* rootP = 0;
	  //lhcf
	  HepMC::GenVertex::particles_in_const_iterator mother = parent_vertex->particles_in_const_begin ();
	  if(data.energy == "7TeV" && p->status () == 2 && id == 111)//parent_vertex && parent_vertex->particles_in_size () == 1 && (*mother)->pdg_id () == 111)
	    {
	      if (!rootP) rootP = CopyHepMC2ROOT(p);
	      const double pt = p->momentum ().perp ();
	      const double rap = rootP->Y();
                  
	      if(pt < 0.6)
		{
		  data.FillLHCF(rap,pt);
		}
	    }
	  //castor
	  if (p->status () == 1 && -6.6 < eta && eta < -5.2)// && pt > 0.04)// && isNuMu == 0)
	    {
	      if (!rootP) rootP = CopyHepMC2ROOT(p);
	      castor_e += e;

	      if (abs(id) == 22 && mother != parent_vertex->particles_in_const_end () && (*mother)->pdg_id() == 111 ) castor_e_pi0 += e;
	      else if (abs(id) == 22) castor_e_gam += e;
	      else if (abs(id) == 211) castor_e_pic += e;
	      else if (string(rootP->GetPDG()->ParticleClass()) == "Baryon") castor_e_bar += e;
	      else castor_e_oth += e;

	    }
          delete rootP;
	}//PARTICLE LOOP
      data.FillCASTOR(castor_e);
      data.FillCASTORcomposition(castor_e,castor_e_pi0,castor_e_pic,castor_e_bar,castor_e_gam,castor_e_oth);
    }//EVENT LOOP
    
  return 0;
}
