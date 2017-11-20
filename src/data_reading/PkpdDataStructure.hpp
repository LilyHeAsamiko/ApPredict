/*

Copyright (c) 2005-2017, University of Oxford.
All rights reserved.

University of Oxford means the Chancellor, Masters and Scholars of the
University of Oxford, having an administrative office at Wellington
Square, Oxford OX1 2JD, UK.

This file is part of Chaste.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
 * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.
 * Neither the name of the University of Oxford nor the names of its
   contributors may be used to endorse or promote products derived from this
   software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#ifndef PKPDDATASTRUCTURE_HPP_
#define PKPDDATASTRUCTURE_HPP_

#include "AbstractDataStructure.hpp"

/**
 * Helper class to read in PKPD data
 */
class PkpdDataStructure : public AbstractDataStructure
{
private:
    /** The maximum concentration that has been encountered */
    double mMaxConc;

protected:
    virtual void LoadALine(std::stringstream& rLine)
    {
        double time;
        std::vector<double> concs_at_this_time;

        rLine >> time;
        while (rLine.good())
        {
            double temp;
            rLine >> temp;
            concs_at_this_time.push_back(temp);
            if (temp > mMaxConc)
            {
                mMaxConc = temp;
            }
        }

        mTimes.push_back(time);
        mClinicalDoses.push_back(concs_at_this_time);
    }

    std::vector<double> mTimes;
    std::vector<std::vector<double> > mClinicalDoses;

public:
    PkpdDataStructure(FileFinder& rFileFinder)
            : AbstractDataStructure(),
              mMaxConc(-DBL_MAX)
    {
        LoadDataFromFile(rFileFinder.GetAbsolutePath());
    };

    std::vector<double> GetConcentrationsAtTimeIndex(unsigned index)
    {
        return mClinicalDoses[index];
    }

    unsigned GetNumPatients()
    {
        return mClinicalDoses[0].size();
    }

    double GetMaximumConcentration()
    {
        return mMaxConc;
    }

    std::vector<double> GetConcentrationsForPatient(unsigned patientIndex)
    {
        std::vector<double> conc_through_time_this_patient(mTimes.size());

        for (unsigned t = 0; t < mTimes.size(); t++)
        {
            conc_through_time_this_patient[t] = mClinicalDoses[t][patientIndex];
        }

        return conc_through_time_this_patient;
    }

    std::vector<double> GetTimes()
    {
        return mTimes;
    }
};

#endif // PKPDDATASTRUCTURE_HPP_
