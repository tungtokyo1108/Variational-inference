
/********************

Copyright 2016-2018 Dang Thanh Tung, Hirohisa Kishino
Fast Computational version of PhyloBayes MPI is free software which replace the sampling algorithm by fast optimization  
algorithm : you can redistribute it and/or modify it under the terms of the GNU General Public License.

**********************/


#include "PoissonDPProfileProcessVI.h"
#include "Random.h"


//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//	* PoissonDPProfileProcess
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

void PoissonDPProfileProcessVI::ToStream(ostream& os)	{

	os << Ncomponent << '\n';
	os << kappa << '\n';
	for (int j=0; j<GetDim(); j++)	{
		os << dirweight[j] << '\t';
	}
	os << '\n';
	os << '\n';
	for (int i=0; i<Ncomponent; i++)	{
		for (int j=0; j<GetDim(); j++)	{
			os << profile[i][j] << '\t';
		}
		os << '\n';
	}

	for (int i=0; i<GetNsite(); i++)	{
		os << alloc[i] << '\t';
	}
	os << '\n';
}

void PoissonDPProfileProcessVI::FromStream(istream& is)	{

	is >> Ncomponent;
	is >> kappa;
	
	for (int i=0; i<GetDim(); i++)	{
		is >> dirweight[i];
	}

	for (int i=0; i<Ncomponent; i++)	{
		for (int j=0; j<GetDim(); j++)	{
			is >> profile[i][j];
		}
	}

	for (int i=0; i<GetNsite(); i++)	{
		is >> alloc[i];
	}

	// CHECK some update here ?
}

/* double PoissonDPProfileProcess::IncrementalDPMove(int nrep)	{

	UpdateOccupancyNumbers();
	int NAccepted = 0;
	int Nrep = (GetNsite() * nrep )/ 10;

	for (int rep=0; rep<Nrep; rep++)	{

		int site = (int) (GetNsite() * rnd::GetRandom().Uniform());

		int bk = alloc[site];
		int h = occupancy[bk] > 1 ? Ncomponent+1 : Ncomponent;

		// make a new mode Nmode <= i < h
		for (int i=Ncomponent; i<h ; i++)	{
			CreateComponent(i);
		}

		RemoveSite(site,bk);

		// Gibbs

		double total = 0;
		double* mModeGibbsGrid = new double[h];
		double* mLogSamplingArray = new double[h];

		double max = 0;
		for (int mode = 0; mode < h; mode++)	{
			// mLogSamplingArray[mode] =  LogStatProb(site,mode);
			mLogSamplingArray[mode] =  DiffLogSampling(mode,site);
			if ((!mode) || (max < mLogSamplingArray[mode]))	{
				max = mLogSamplingArray[mode];
			}
		}
		for (int mode = 0; mode < h; mode++)	{
			double temp=0;
			if (occupancy[mode])	{
				temp = occupancy[mode] * exp(mLogSamplingArray[mode] - max);
			}
			else	{
				temp = kappa * exp(mLogSamplingArray[mode] - max);
			}
			total += temp;
			mModeGibbsGrid[mode] = total;
		}

		double q = total * rnd::GetRandom().Uniform();
		int mode = 0;
		while ( (q > mModeGibbsGrid[mode]) && (mode < h)) mode++;
		if (mode == h)	{
			cerr << "error in switch mode integral\n";
			exit(1);
		}

		int Accepted = (mode != bk);

		if (Accepted)	{
			NAccepted++;
		}

		AddSite(site,mode);

		if (mode >= Ncomponent)	{
			if (mode > Ncomponent)	{
				SwapComponents(mode, Ncomponent);
			}
			Ncomponent++;
		}
		if (! occupancy[bk])	{
			if (bk != Ncomponent-1)	{
				SwapComponents(bk,Ncomponent-1);
			}
			Ncomponent--;
		}

		delete[] mModeGibbsGrid;
		delete[] mLogSamplingArray;
	}
	return ((double) NAccepted) / GetNsite() / nrep * 10;
}
*/
