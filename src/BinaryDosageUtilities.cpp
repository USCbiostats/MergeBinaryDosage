#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <cmath>
#include <limits>
#include <vector>
#include <algorithm>
#include <Rcpp.h>
#include "SNPClass.h"
#include "Subject.h"
#include "BinaryDosageRead.h"

// [[Rcpp::depends(Rcpp)]]

// [[Rcpp::export]]
Rcpp::DataFrame GetSNP(const std::string &filename, const std::string &snpName, const std::string &chromosome, unsigned int location,
           const std::string &refAllele, const std::string &altAllele) {
  CReadBinaryDosageFile42 rbd;
  Rcpp::StringVector::iterator rstrit;
  Rcpp::NumericVector::iterator rnumit;
  std::vector<CSubject>::const_iterator csubit;
  std::vector<double>::const_iterator dit;
  std::vector<unsigned int> snpIndex;

  if (rbd.Open(filename))
    return 1;

  Rcpp::Rcout << "Number of Subjects:\t" << rbd.NumSubjects() << std::endl;
  Rcpp::StringVector iid(rbd.NumSubjects());
  for (csubit = rbd.Subject().begin(), rstrit = iid.begin(); csubit != rbd.Subject().end(); ++csubit, ++rstrit)
    *rstrit = csubit->SubjectID();

  rbd.FindSNP(snpIndex, snpName, chromosome, location, refAllele, altAllele);
  if (snpIndex.size() == 0)
    return 2;
  if (snpIndex.size() > 1)
    return 3;

  if (rbd.GetSNP(snpIndex[0]))
    return 4;

  Rcpp::NumericVector dosage(rbd.NumSubjects());
  for (dit = rbd.Dosages().begin(), rnumit = dosage.begin(); dit != rbd.Dosages().end(); ++dit, ++rnumit)
    *rnumit = *dit;

  Rcpp::List listOut = Rcpp::List::create(Rcpp::Named("IID") = iid, Rcpp::Named("Dosage") = dosage);
  Rcpp::DataFrame frameout(listOut);
  return frameout;
}
