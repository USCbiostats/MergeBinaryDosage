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
void ReadCreatedFile(const std::string &filename) {
	CReadBinaryDosageFile42 rbd;
	std::vector<double>::const_iterator dit;

	if (rbd.Open(filename.c_str())) {
		std::cout << rbd.ErrorMessage() << std::endl;
	}
	std::cout << "Number of subjects:\t" << rbd.NumSubjects() << std::endl;
	std::cout << "Number of SNPs:    \t" << rbd.NumSNPs() << std::endl;
	std::cout << "Number of Groups:  \t" << rbd.NumGroups() << std::endl;
	if (rbd.NumGroups() > 1) {
	  std::cout << "\tGroup Sizes" << std::endl;
	  for (std::vector<unsigned int>::const_iterator uit = rbd.GroupSize().begin(); uit != rbd.GroupSize().end(); ++uit)
	    std::cout << "\t\t" << *uit << std::endl;
	}

	std::cout << "Subject options:   \t" << std::hex << std::setw(4) << rbd.SubjectOptions() << std::endl;
	std::cout << "First 5 subjects:" << std::endl;
	for (std::vector<CSubject>::const_iterator subit = rbd.Subject().begin(); subit != rbd.Subject().begin() + 5; ++subit)
  	std::cout << '\t' << subit->SubjectID() << std::endl;
	std::cout << "Last 5 subjects:" << std::endl;
	for (std::vector<CSubject>::const_iterator subit = rbd.Subject().end() - 5; subit != rbd.Subject().end(); ++subit)
	  std::cout << '\t' << subit->SubjectID() << std::endl;

	std::cout << "SNP options:       \t" << std::hex << std::setw(4) << rbd.SNPOptions() << std::dec << std::endl;

	rbd.GetFirst();
	std::cout << "First SNP\tName\tChromosome\tLocation\tRefAllele\tAltAllele" << std::endl;
	std::cout << "\t\t" << rbd.SNP()[0].Name() << '\t' << rbd.SNP()[0].Chromosome()
           << '\t' << rbd.SNP()[0].Location() << "\t\t" << rbd.SNP()[0].ReferenceAllele()
           << "\t\t" << rbd.SNP()[0].AlternateAllele() << std::endl;
	std::cout << "\tAltFreq\tMAF\tAvgCall\tRsquared" << std::endl;
	for (std::vector<CImputationInfo>::const_iterator impit = rbd.SNP()[0].ImputationInfo().begin(); impit != rbd.SNP()[0].ImputationInfo().end(); ++impit)
	  std::cout << '\t' << impit->AltFreq() << '\t' << impit->MAF() << '\t' << impit->AvgCall() << '\t' << impit->RSquared() << std::endl;
	std::cout << "First 100 dosages:" << std::endl;
	for (dit = rbd.Dosages().begin(); dit != rbd.Dosages().begin() + 100; ++dit) {
		std::cout << '\t' << *dit;
	}
	std::cout << std::endl;
	std::cout << "Last 100 dosages:";
	for (dit = rbd.Dosages().end() - 100; dit != rbd.Dosages().end(); ++dit) {
		std::cout << '\t' << *dit;
	}
	std::cout << std::endl;

	rbd.GetSNP(rbd.NumSNPs() - 1);
	std::cout << "Last SNP\tName\tChromosome\tLocation\tRefAllele\tAltAllele" << std::endl;
	std::cout << "\t\t" << rbd.SNP()[rbd.NumSNPs() - 1].Name() << '\t' << rbd.SNP()[rbd.NumSNPs() - 1].Chromosome()
           << '\t' << rbd.SNP()[rbd.NumSNPs() - 1].Location() << "\t\t" << rbd.SNP()[rbd.NumSNPs() - 1].ReferenceAllele()
           << "\t\t" << rbd.SNP()[rbd.NumSNPs() - 1].AlternateAllele() << std::endl;
	std::cout << "\tAltFreq\tMAF\tAvgCall\tRsquared" << std::endl;
	for (std::vector<CImputationInfo>::const_iterator impit = rbd.SNP()[rbd.NumSNPs() - 1].ImputationInfo().begin(); impit != rbd.SNP()[rbd.NumSNPs() - 1].ImputationInfo().end(); ++impit)
	  std::cout << '\t' << impit->AltFreq() << '\t' << impit->MAF() << '\t' << impit->AvgCall() << '\t' << impit->RSquared() << std::endl;
	std::cout << "First 100 dosages:";
	for (dit = rbd.Dosages().begin(); dit != rbd.Dosages().begin() + 100; ++dit) {
		std::cout << '\t' << *dit;
	}
	std::cout << std::endl;
	std::cout << "Last 100 dosages:";
	for (dit = rbd.Dosages().end() - 100; dit != rbd.Dosages().end(); ++dit) {
		std::cout << '\t' << *dit;
	}
	std::cout << std::endl;
}

// [[Rcpp::export]]
void ReadInterestingStuff(const std::string &filename) {
    unsigned int x[8];
  char y[16];

    std::ifstream infile(filename.c_str(), std::ios::in | std::ios::binary);
    if (!infile.good()) {
        std::cout << "Unable to open interesting stuff" << std::endl;
        return;
    }
	std::cout << "\nInteresting stuff" << std::endl;
    infile.seekg(8);
    infile.read((char *)x, sizeof(x));
	std::cout << "Number of subjects:\t" << x[0] << std::endl;
	std::cout << "Number of SNPs:    \t" << x[1] << std::endl;
	std::cout << "Number of Groups:  \t" << x[2] << std::endl;
	std::cout << "Subject options:   \t" << x[3] << std::endl;
	std::cout << "SNP options:       \t" << x[4] << std::endl;
	std::cout << "Subject Data Start:\t" << x[5] << std::endl;
	std::cout << "SNP Data start:    \t" << x[6] << std::endl;
	std::cout << "Dosage Data start: \t" << x[7] << std::endl;

	infile.seekg(x[5]);
    infile.read((char *)x, 2*sizeof(unsigned int));
  std::cout << "Subject array size:\t" << x[0] << std::endl;
  std::cout << "Family array size: \t" << x[1] << std::endl;

	infile.read(y, 16);
	for (unsigned int ui = 0; ui < 16; ++ui)
	  std::cout << (unsigned int)y[ui] << " : "  << y[ui] << std::endl;

	infile.seekg(x[6]);
	infile.read((char *)x, 4*sizeof(unsigned int));
	std::cout << "Size of SNP Names: \t" << x[0] << std::endl;
	std::cout << "Size of Chromosome:\t" << x[1] << std::endl;
	std::cout << "Size of Ref Allele:\t" << x[2] << std::endl;
	std::cout << "Size of Alt Allele:\t" << x[3] << std::endl;

		infile.close();
}
void WriteHeader(std::ostream &_stream) {
	char magicWord[8] = { 'b', 'o', 's', 'e', 0x0, 0x4, 0x0, 0x2 };
	_stream.write(magicWord, 8);
}

int WriteSubjectData(std::ofstream &_stream, std::vector<CReadBinaryDosageFile42 *> &_rbdv) {
	unsigned int startSubjects, startSNPs;
	unsigned int fidSize = 0;
	unsigned int iidSize = 0;
    char tab = '\t';
	std::vector<CReadBinaryDosageFile42 *>::const_iterator rbdit;
	std::vector<CSubject>::const_iterator csubit;

	startSubjects = (unsigned int)_stream.tellp();
	_stream.write((char *)&iidSize, sizeof(unsigned int));
	_stream.write((char *)&fidSize, sizeof(unsigned int));
	for (rbdit = _rbdv.begin(); rbdit != _rbdv.end(); ++rbdit) {
		for (csubit = (*rbdit)->Subject().begin(); csubit != (*rbdit)->Subject().end(); ++csubit) {
			_stream.write(csubit->SubjectID().c_str(), csubit->SubjectID().length());
            _stream.write(&tab, 1);
		}
	}
	startSNPs = (unsigned int)_stream.tellp();
//	std::cout << "Start SNPs:\t" << startSNPs << std::endl;
  _stream.seekp(28);
  _stream.write((char *)&startSubjects, sizeof(unsigned int));
  _stream.seekp(32);
  _stream.write((char *)&startSNPs, sizeof(unsigned int));
  iidSize = startSNPs - startSubjects - 8;
  _stream.seekp(startSubjects);
  _stream.write((char *)&iidSize, sizeof(unsigned int));
  _stream.seekp(startSNPs);
  _stream.seekp(startSubjects + 4);
  _stream.write((char *)&fidSize, sizeof(unsigned int));
  _stream.seekp(startSNPs);

	return 0;
}

int WriteImputationInfo(std::ofstream &_stream, std::vector<CReadBinaryDosageFile42 *> &_rbdv) {
	unsigned int darraySize;
	unsigned int numGroups = 0;
	std::vector<CReadBinaryDosageFile42 *>::const_iterator rbdit;
	std::vector<double> imputeInfo;
	std::vector<double>::iterator dit;
	std::vector<CImputationInfo>::const_iterator cimpit;
	std::vector<unsigned int>::iterator grpit;
	std::vector<unsigned int> groupSizes;
	unsigned int startDosages;

	groupSizes.push_back(0);
	for (rbdit = _rbdv.begin(); rbdit != _rbdv.end(); ++rbdit) {
		groupSizes.push_back((*rbdit)->NumGroups());
		numGroups += (*rbdit)->NumGroups();
	}

	darraySize = _rbdv[0]->NumSNPs() * numGroups;
	imputeInfo.resize(darraySize);

//	for (grpit = groupSizes.begin(); grpit != groupSizes.end(); ++grpit)
//		std::cout << *grpit << std::endl;

	grpit = groupSizes.begin() + 1;
	for (rbdit = _rbdv.begin(); rbdit != _rbdv.end(); ++rbdit, ++grpit) {
		dit = imputeInfo.begin() + *(grpit - 1);
		for (std::vector<CSNP>::const_iterator snpit = (*rbdit)->SNP().begin(); snpit != (*rbdit)->SNP().end(); ++snpit, dit += (numGroups - *grpit)) {
			for (cimpit = snpit->ImputationInfo().begin(); cimpit != snpit->ImputationInfo().end(); ++cimpit, ++dit)
				*dit = cimpit->AltFreq();
		}
	}
	_stream.write((char *)&imputeInfo[0], darraySize*sizeof(double));
	grpit = groupSizes.begin() + 1;
	for (rbdit = _rbdv.begin(); rbdit != _rbdv.end(); ++rbdit, ++grpit) {
		dit = imputeInfo.begin() + *(grpit - 1);
		for (std::vector<CSNP>::const_iterator snpit = (*rbdit)->SNP().begin(); snpit != (*rbdit)->SNP().end(); ++snpit, dit += (numGroups - *grpit)) {
			for (cimpit = snpit->ImputationInfo().begin(); cimpit != snpit->ImputationInfo().end(); ++cimpit, ++dit)
				*dit = cimpit->MAF();
		}
	}
	_stream.write((char *)&imputeInfo[0], darraySize*sizeof(double));
	grpit = groupSizes.begin() + 1;
	for (rbdit = _rbdv.begin(); rbdit != _rbdv.end(); ++rbdit, ++grpit) {
		dit = imputeInfo.begin() + *(grpit - 1);
		for (std::vector<CSNP>::const_iterator snpit = (*rbdit)->SNP().begin(); snpit != (*rbdit)->SNP().end(); ++snpit, dit += (numGroups - *grpit)) {
			for (cimpit = snpit->ImputationInfo().begin(); cimpit != snpit->ImputationInfo().end(); ++cimpit, ++dit)
				*dit = cimpit->AvgCall();
		}
	}
	_stream.write((char *)&imputeInfo[0], darraySize*sizeof(double));
	grpit = groupSizes.begin() + 1;
	for (rbdit = _rbdv.begin(); rbdit != _rbdv.end(); ++rbdit, ++grpit) {
		dit = imputeInfo.begin() + *(grpit - 1);
		for (std::vector<CSNP>::const_iterator snpit = (*rbdit)->SNP().begin(); snpit != (*rbdit)->SNP().end(); ++snpit, dit += (numGroups - *grpit)) {
			for (cimpit = snpit->ImputationInfo().begin(); cimpit != snpit->ImputationInfo().end(); ++cimpit, ++dit)
				*dit = cimpit->RSquared();
		}
	}
	_stream.write((char *)&imputeInfo[0], darraySize*sizeof(double));
	startDosages = (unsigned int)_stream.tellp();
	_stream.seekp(36);
	_stream.write((char *)&startDosages, sizeof(unsigned int));
	_stream.seekp(startDosages);
	return 0;
}

int WriteDosages(std::ofstream &_stream, std::vector<CReadBinaryDosageFile42 *> &_rbdv) {
	std::vector<CReadBinaryDosageFile42 *>::const_iterator rbdit;
	unsigned int ui, uj;
//	unsigned int dosageNeg, addNeg;
	std::vector<unsigned int> dataSizeV;
	std::vector<unsigned int>::iterator uit;
	unsigned int dataSize;
	unsigned int fullDataSize;
	const short *sdata1, *sdata2;
//	std::vector<std::streampos>::const_iterator cposit;

//	std::cout << "Calculating size" << std::endl;
	dataSizeV.resize(_rbdv.size());
	for (ui = 0; ui < _rbdv[0]->NumSNPs(); ++ui) {
//		std::cout << "Calculated size" << std::endl;
		fullDataSize = 0;
		uit = dataSizeV.begin();
		for (rbdit = _rbdv.begin(); rbdit != _rbdv.end(); ++rbdit, ++uit) {
			if (ui == 0)
				(*rbdit)->GetFirst();
			else
				(*rbdit)->GetNext();
			dataSize = (*rbdit)->NumSubjects();
			sdata1 = &((*rbdit)->SDosages()[0]);
			sdata2 = sdata1 + dataSize;
			for (uj = 0; uj < (*rbdit)->NumSubjects(); ++uj, ++sdata1) {
				if (*sdata1 & 0x8000) {
					++dataSize;
					if (*sdata2 & 0x8000) {
						dataSize += 2;
						sdata2 += 2;
					}
					++sdata2;
				}
			}
//			std::cout << '\t' << 2 * dataSize;
			fullDataSize += 2 * dataSize;
			*uit = 2 * dataSize;
		}
//		std::cout << std::endl;
//		std::cout << "Full data size:\t" << fullDataSize << std::endl;
//		std::cout << "Writing dosages" << std::endl;
		_stream.write((char *)&fullDataSize, sizeof(unsigned int));
		for (rbdit = _rbdv.begin(); rbdit != _rbdv.end(); ++rbdit) {
			dataSize = 2 * (*rbdit)->NumSubjects();
//			std::cout << '\t' << dataSize;
			sdata1 = &((*rbdit)->SDosages()[0]);
			_stream.write((char *)sdata1, dataSize);
		}
//		std::cout << std::endl;
//		std::cout << "Writing additional data" << std::endl;
		uit = dataSizeV.begin();
		for (rbdit = _rbdv.begin(); rbdit != _rbdv.end(); ++rbdit, ++uit) {
			dataSize = *uit - 2 * (*rbdit)->NumSubjects();
//			std::cout << '\t' << dataSize;
			sdata1 = &((*rbdit)->SDosages()[0]) + (*rbdit)->NumSubjects();
			_stream.write((char *)sdata1, dataSize);
		}
//		std::cout << std::endl << std::endl;
	}
	/*
	std::cout << "Known size" << std::endl;
	for (ui = 0; ui < 3; ++ui) {
		for (rbdit = _rbdv.begin(); rbdit != _rbdv.end(); ++rbdit) {
			std::cout << '\t' << (*rbdit)->SNPIndex()[ui + 1] - (*rbdit)->SNPIndex()[ui] - 4;
		}
		std::cout << std::endl;
	}
	*/
	return 0;
}
int WriteSNPData(std::ofstream &_stream, std::vector<CReadBinaryDosageFile42 *> &_rbdv) {
	unsigned int startSNPs;
	unsigned int nameSize = 0;
	unsigned int chromosomeSize = 0;
	unsigned int locationSize = 0;
	unsigned int refAlleleSize = 0;
	unsigned int altAlleleSize = 0;
	unsigned int location = 0;
	char tab = '\t';
	std::vector<CReadBinaryDosageFile42 *>::const_iterator rbdit;
	std::vector<CSNP>::const_iterator csnpit;
	std::streampos currentPos;

	startSNPs = (unsigned int)_stream.tellp();
	_stream.write((char *)&nameSize, sizeof(unsigned int));
	_stream.write((char *)&chromosomeSize, sizeof(unsigned int));
	_stream.write((char *)&refAlleleSize, sizeof(unsigned int));
	_stream.write((char *)&altAlleleSize, sizeof(unsigned int));
//	std::cout << "After size:\t" << _stream.tellp() << std::endl;

	rbdit = _rbdv.begin();
	if ((*rbdit)->SNP().begin()->Name() != "") {
		for (csnpit = (*rbdit)->SNP().begin(); csnpit != (*rbdit)->SNP().end(); ++csnpit) {
			_stream.write(csnpit->Name().c_str(), csnpit->Name().length());
			_stream.write(&tab, 1);
		}
		nameSize = (unsigned int)(_stream.tellp()) - startSNPs - 16;
	}

	if (((*rbdit)->SNPOptions() & 0x00000008) == 0) {
		for (csnpit = (*rbdit)->SNP().begin(); csnpit != (*rbdit)->SNP().end(); ++csnpit) {
			_stream.write(csnpit->Chromosome().c_str(), csnpit->Chromosome().length());
			_stream.write(&tab, 1);
		}
	}
	else {
//		std::cout << "Single Chromosome:\t-" << (*rbdit)->SNP().begin()->Chromosome().c_str() << '-' << std::endl;
		_stream.write((*rbdit)->SNP().begin()->Chromosome().c_str(), (*rbdit)->SNP().begin()->Chromosome().length());
		_stream.write(&tab, 1);
	}
//	std::cout << "After chromosome:\t" << _stream.tellp() << std::endl;
	chromosomeSize = (unsigned int)(_stream.tellp()) - startSNPs - 16 - nameSize;

	for (csnpit = (*rbdit)->SNP().begin(); csnpit != (*rbdit)->SNP().end(); ++csnpit) {
	  location = csnpit->Location();
	  _stream.write((char *)&location, sizeof(unsigned int));
	}
	locationSize = (*rbdit)->NumSNPs() * sizeof(unsigned int);
//	_stream.write((char *)&(*rbdit)->SNP()[0].Location()[0], locationSize);

	for (csnpit = (*rbdit)->SNP().begin(); csnpit != (*rbdit)->SNP().end(); ++csnpit) {
		_stream.write(csnpit->ReferenceAllele().c_str(), csnpit->ReferenceAllele().length());
		_stream.write(&tab, 1);
	}
	refAlleleSize = (unsigned int)(_stream.tellp()) - startSNPs - 16 - nameSize - chromosomeSize - locationSize;

	for (csnpit = (*rbdit)->SNP().begin(); csnpit != (*rbdit)->SNP().end(); ++csnpit) {
		_stream.write(csnpit->AlternateAllele().c_str(), csnpit->AlternateAllele().length());
		_stream.write(&tab, 1);
	}
	altAlleleSize = (unsigned int)(_stream.tellp()) - startSNPs - 16 - nameSize - chromosomeSize - locationSize - refAlleleSize;

	currentPos = _stream.tellp();
	_stream.seekp(startSNPs);
	_stream.write((char *)&nameSize, sizeof(unsigned int));
	_stream.write((char *)&chromosomeSize, sizeof(unsigned int));
	_stream.write((char *)&refAlleleSize, sizeof(unsigned int));
	_stream.write((char *)&altAlleleSize, sizeof(unsigned int));
	_stream.seekp(currentPos);

	WriteImputationInfo(_stream, _rbdv);

	/*
	endSNPs = (unsigned int)_stream.tellp();
	std::cout << "Start SNPs:\t" << startSNPs << std::endl;
	std::cout << "Start Dosages:\t" << endSNPs << std::endl;
	std::cout << "Name size:\t" << nameSize << std::endl;
	std::cout << "Chromosome size:\t" << chromosomeSize << std::endl;
	std::cout << "Location size:\t" << locationSize << std::endl;
	std::cout << "RefAllele size:\t" << refAlleleSize << std::endl;
	std::cout << "AltAllele size:\t" << altAlleleSize << std::endl;
	*/

	return 0;
}

int WriteGeneralInfo(std::ofstream &_stream, std::vector<CReadBinaryDosageFile42 *> &_rbdv) {
	unsigned int numSubjects = 0;
	unsigned int numSNPs = 0;
	unsigned int numGroups = 0;
	unsigned int subjectOptions = 0;
	unsigned int snpOptions = 0;
	unsigned int startSubjectData = 0;
	unsigned int startSNPData = 0;
	unsigned int startDosageData = 0;
	bool differentOptions = false;
	unsigned int groupSize;

	std::vector<CReadBinaryDosageFile42 *>::iterator rbdit1, rbdit2;
	for (rbdit1 = _rbdv.begin(); rbdit1 != _rbdv.end(); ++rbdit1) {
		numSubjects += (*rbdit1)->NumSubjects();
		numGroups += (*rbdit1)->NumGroups();
		for (rbdit2 = rbdit1 + 1; rbdit2 != _rbdv.end(); ++rbdit2) {
			if ((*rbdit1)->NumSNPs() != (*rbdit2)->NumSNPs() || (*rbdit1)->SubjectOptions() != (*rbdit2)->SubjectOptions() || (*rbdit1)->SNPOptions() != (*rbdit2)->SNPOptions())
				differentOptions = true;
		}
	}
	if (differentOptions) {
		std::cerr << "Files have different opitons" << std::endl;
		return 1;
	}
	numSNPs = (*_rbdv.begin())->NumSNPs();
	subjectOptions = (*_rbdv.begin())->SubjectOptions();
	snpOptions = (*_rbdv.begin())->SNPOptions();
//	startSubjectData = 40 + numGroups * sizeof(unsigned int);
	/*
	std::cout << "Total subjects:\t" << numSubjects << std::endl;
	std::cout << "Total SNPs:\t" << numSNPs << std::endl;
	std::cout << "Total groups:\t" << numGroups << std::endl;
	std::cout << "Start subjects:\t" << startSubjectData << std::endl;
	std::cout << "Start SNPs:\t" << startSNPData << std::endl;
	std::cout << "Start Dosage:\t" << startDosageData << std::endl;
	*/
	_stream.write((char *)&numSubjects, sizeof(unsigned int));
	_stream.write((char *)&numSNPs, sizeof(unsigned int));
	_stream.write((char *)&numGroups, sizeof(unsigned int));
	_stream.write((char *)&subjectOptions, sizeof(unsigned int));
	_stream.write((char *)&snpOptions, sizeof(unsigned int));
	_stream.write((char *)&startSubjectData, sizeof(unsigned int));
	_stream.write((char *)&startSNPData, sizeof(unsigned int));
	_stream.write((char *)&startDosageData, sizeof(unsigned int));
//	std::cout << "Current location:\t" << _stream.tellp() << std::endl;
	for (rbdit1 = _rbdv.begin(); rbdit1 != _rbdv.end(); ++rbdit1) {
		for (std::vector<unsigned int>::const_iterator cuit = (*rbdit1)->GroupSize().begin(); cuit != (*rbdit1)->GroupSize().end(); ++cuit) {
			groupSize = *cuit;
			_stream.write((char *)&groupSize, sizeof(unsigned int));
		}
	}
//	std::cout << "Current location:\t" << _stream.tellp() << std::endl;
	return 0;
}

// [[Rcpp::export]]
int BinaryDosageMerge(Rcpp::CharacterVector &mergeFiles, Rcpp::String outFilename) {
	std::vector<CReadBinaryDosageFile42 *> rbd42v;
	std::vector<std::string> mergeFilenames(mergeFiles.size());
	std::vector<std::string>::const_iterator cstrit;
	std::vector<CReadBinaryDosageFile42 *>::iterator rbdit1, rbdit2;

	std::ofstream outfile(outFilename.get_cstring(), std::ios::out | std::ios::binary);
	if (!outfile.good()) {
		std::cout << "Unable to open output file" << std::endl;
		return 1;
	}
	for (unsigned int ui = 0; ui < mergeFiles.size(); ++ui)
	  mergeFilenames[ui] = mergeFiles[ui];
	//	std::vector<CSNP>::const_iterator snpit;

//	filenames.push_back("C:/COH/Data_20170306/aidgxe.omniA.HRC/aidgxe.omniA.HRC.chr22.bdose");
//	filenames.push_back("C:/COH/Data_20170306/aidgxe.mayo_dlbcl/mayo_dlbcl.chr22.bdose");
//	filenames.push_back("C:/COH/Data_20170306/aidgxe.sf2_fl/sf2_fl.chr22.bdose");
//    filenames.push_back("/Users/jmorr/VCF/subset10_c_42.bdose");
//    filenames.push_back("/Users/jmorr/VCF/subset10_c_42.bdose");


//	rbd42 = new CReadBinaryDosageFile42();
	rbd42v.resize(mergeFilenames.size());
	cstrit = mergeFilenames.begin();
	for (rbdit1 = rbd42v.begin(); rbdit1 != rbd42v.end(); ++rbdit1, ++cstrit) {
		*rbdit1 = new CReadBinaryDosageFile42();
		if ((*rbdit1)->Open(*cstrit))
			std::cerr << (*rbdit1)->ErrorMessage() << std::endl;
//		std::cout << (*rbdit1)->SNP().begin()->ImputationInfo().begin()->AltFreq() << std::endl;
	}

//	for (rbdit1 = rbd42v.begin(); rbdit1 != rbd42v.end(); ++rbdit1)
//		std::cout << (*rbdit1)->NumGroups() << '\t' << (*rbdit1)->NumSubjects() << '\t' << (*rbdit1)->NumSNPs() << std::endl;

	/*
	for (rbdit1 = rbd42v.begin(); rbdit1 != rbd42v.end(); ++rbdit1) {
		for (rbdit2 = rbdit1 + 1; rbdit2 != rbd42v.end(); ++rbdit2) {
			if ((*rbdit1)->SNP()[0] == (*rbdit2)->SNP()[0])
				std::cout << "SNP 1 equal" << std::endl;
			else
				std::cout << "SNP 1 not equal" << std::endl;
		}
	*/
	WriteHeader(outfile);
	WriteGeneralInfo(outfile, rbd42v);
	WriteSubjectData(outfile, rbd42v);
	WriteSNPData(outfile, rbd42v);
	WriteDosages(outfile, rbd42v);
	for (std::vector<CReadBinaryDosageFile42 *>::iterator rbdit = rbd42v.begin(); rbdit != rbd42v.end(); ++rbdit) {
		delete *rbdit;
	}
	outfile.close();
	std::cout << "Complete" << std::endl;

//  ReadInterestingStuff(outfilename.c_str());
//	ReadCreatedFile(outfilename.c_str());
	return 0;
}
