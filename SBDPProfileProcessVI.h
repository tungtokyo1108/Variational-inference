
/********************

Copyright 2016-2018 Dang Thanh Tung, Hirohisa Kishino
Fast Computational version of PhyloBayes MPI is free software which replace the sampling algorithm by fast optimization  
algorithm : you can redistribute it and/or modify it under the terms of the GNU General Public License.

**********************/


#ifndef SBDPPROFILEVI_H
#define SBDPPROFILEVI_H

#include <cmath>
#include "DPProfileProcessVI.h"
#include <gsl/gsl_sf_psi.h>

const int refnmodemaxVI = 100;

// general superclass for all finite process mixtures on site-specific profiles
class SBDPProfileProcessVI: public virtual DPProfileProcessVI	{

	using MixtureProfileProcessVI::LogStatPrior;

	public:

	SBDPProfileProcessVI() : DPProfileProcessVI(), nmodemax(refnmodemaxVI), V(0), maxweighterror(0) {}
	virtual ~SBDPProfileProcessVI(){}
  
        double* kappa_alpha;
        double* kappa_beta;
        double ELBOWeight();
        double ELBOkappa();
        double ELBO_Weight;
        double ELBO_kappa;
       
        virtual double GetSBDPParameter(int k);
        // virtual double GetPCAT(int cat) = 0;
        // virtual double GetTotalSitePCAT() = 0;

	protected:

	virtual void DrawProfileFromPrior();

	double GetMaxWeightError() {return maxweighterror;}
	void ResetMaxWeightError() {maxweighterror = 0;}

	virtual void Create(int innsite, int indim);
	virtual void Delete();

	virtual int GetNmodeMax() {return GetNsite() > nmodemax ? nmodemax : GetNsite();}
	virtual void SetNmodeMax(int n) {nmodemax = n;}

	virtual double IncrementalDPMove(int nrep, double epsilon) = 0;

	double IncrementalDPMove(int nrep)	{
		cerr << "inc move deactivated\n";
		exit(1);
	}

	double GetWeightEnt()	{
		double tot = 0;
		for (int k=0; k<GetNcomponent(); k++)	{
			if (weight[k] > 1e-8)	{
				tot -= weight[k] * log(weight[k]);
			}
		}
		return tot;
	}

	int GetNDisplayedComponent()	{
		return GetNOccupiedComponent();
	}

	int GetLastOccupiedComponent()	{
		int kmax = 0;
		for (int i=0; i<GetNsite(); i++)	{
			if (kmax < alloc[i])	{
				kmax = alloc[i];
			}
		}
		return kmax;
	}

	int GetNCutoff(double cutoff)	{
		int n = (int) (GetNOccupiedComponent() * (1 - cutoff));
		int k = GetLastOccupiedComponent();
		int tot = occupancy[k];
		while (k && (tot < n))	{
			k--;
			if (k)	{
				tot += occupancy[k];
			}
		}
		return k;
	}
		
	virtual void SwapComponents(int cat1, int cat2);

	// double LogAllocPrior();
	double LogIntegratedAllocProb();

	// void ShedTail();

	// redefined
	void SampleAlloc();

	void IncrementalSampleAlloc();

	void SampleWeights();
	void ResampleWeights();
        

	// void ResampleLastWeight();
	double MoveOccupiedCompAlloc(int nrep = 1);
	double MoveAdjacentCompAlloc(int nrep = 1);

	double LogStatPrior();

	/*
	double totweight;
	double cumulProduct;
	*/
       
	int nmodemax;
        /*double* kappa_alpha;
        double* kappa_beta;
       
        virtual double GetSBDPParameter(int k);
        virtual double GetPCAT(int cat) = 0;*/

        double MoveKappa(double tuning, int nrep);
        double Pkappa;
	double* V;
	double* weight;
        double* V_P;
        double* weightVI;
        
        double maxweighterror;
};

#endif

