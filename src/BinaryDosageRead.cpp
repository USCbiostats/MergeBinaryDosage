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
#include "SNPClass.h"
#include "Subject.h"
#include "BinaryDosageRead.h"

// **********************************************
// CReadFile
// **********************************************
CReadFile::CReadFile() {
    m_filename = "";
    m_errorMessage = "";
}

int CReadFile::FileOpenError() {
	m_errorMessage = "Unable to open file:\t" + m_filename;
	return 1;
}
int CReadFile::FileReadError() {
	m_errorMessage = "Error reading from file:\t" + m_filename;
	return 1;
}

int CReadFile::OpenFile(const std::string &_filename) {
    m_infile.close();
    m_filename = _filename;
    m_infile.open(m_filename.c_str(), std::ios::in);
	if (!m_infile.good())
		return FileOpenError();

    return 0;
}

// **********************************************
// CReadBinaryFile
// **********************************************
int CReadBinaryFile::OpenFile(const std::string &_filename) {
	m_infile.close();
	m_filename = _filename;
	m_infile.open(m_filename.c_str(), std::ios::in | std::ios::binary);
	if (!m_infile.good())
		return FileOpenError();

	return 0;
}

// **********************************************
// CBinaryDosageVersion
// **********************************************
CBinaryDosageVersion::CBinaryDosageVersion() : CReadBinaryFile() {
	m_version = 0;
	m_revision = 0;
}

int CBinaryDosageVersion::FileTypeError() {
	m_errorMessage = "File is not of the right type";
	return 1;
}
int CBinaryDosageVersion::VersionError() {
	m_errorMessage = "Unsupported version";
	return 1;
}

int CBinaryDosageVersion::CheckFileType() {
	const char magicWord[4] = { 'b', 'o', 's', 'e' };
	char readMagicWord[4];
	int magicWordLength = 4;

	m_infile.read(readMagicWord, magicWordLength);
	if (!m_infile.good() || std::strncmp(readMagicWord, magicWord, magicWordLength))
		return FileTypeError();
	return 0;
}
int CBinaryDosageVersion::CheckVersion() {
	const unsigned int revisions[4][2] = { { 1, 2 }, { 2, 1}, { 1, 2 }, { 1, 2 } };

	if ((m_version < 1 || m_version > 4) || (m_revision < revisions[m_version - 1][0] && m_revision > revisions[m_version - 1][1]))
		return VersionError();
	return 0;
}

int CBinaryDosageVersion::ReadVersion() {
	char readVersion[4];
	int versionLength = 4;

	m_infile.read(readVersion, versionLength);
	if (!m_infile.good() || readVersion[0] || readVersion[2])
		return VersionError();
	m_version = readVersion[1];
	m_revision = readVersion[3];

	return CheckVersion();
}
int CBinaryDosageVersion::ReadHeader() {
	if (CheckFileType() || ReadVersion())
		return 1;
	return 0;
}

int CBinaryDosageVersion::Open(const std::string &_filename) {
	if (CReadBinaryFile::OpenFile(_filename))
		return 1;
	return ReadHeader();
}

// **********************************************
// CReadBinaryFile4
// **********************************************
CReadBinaryDosageFile4::CReadBinaryDosageFile4() : CBinaryDosageVersion() {
	m_subjectOptions = 0;
	m_numSubjects = 0;
	m_startSubjects = 0;
    m_currentSNP = 0;
	m_SNPOptions = 0;
	m_numSNPs = 0;
	m_startSNPs = 0;
	m_numGroups = 0;
	m_startGroups = 0;
	m_startDosages = 0;
}

int CReadBinaryDosageFile4::FileAlreadyOpenError() {
	m_errorMessage = "File already open";
	return 1;
}

int CReadBinaryDosageFile4::CheckVersion() {
	if (m_version != 4)
		return VersionError();
	return 0;
}

void CReadBinaryDosageFile4::ClearVectors() {
	std::vector<CSubject>().swap(m_subject);
	std::vector<CFamilySubject>().swap(m_familySubject);
	std::vector<CSNP>().swap(m_SNP);
	std::vector<std::streampos>().swap(m_snpIndex);
	std::vector<unsigned int>().swap(m_groupSize);
	std::vector<short>().swap(m_sdosages);
	std::vector<double>().swap(m_dosages);
	std::vector<double>().swap(m_p0);
	std::vector<double>().swap(m_p1);
	std::vector<double>().swap(m_p2);
}
void CReadBinaryDosageFile4::AssignVectors() {
	if (m_subjectOptions & 0x00000001)
		m_familySubject.resize(m_numSubjects);
	else
		m_subject.resize(m_numSubjects);
	m_groupSize.resize(m_numGroups);
	m_sdosages.resize(m_numSubjects);
	m_dosages.resize(m_numSubjects);
	m_SNP.resize(m_numSNPs);
}

int CReadBinaryDosageFile4::ReadStringArray(std::vector<std::string> &_dest, const unsigned int _readSize) {
	std::istringstream iss;
	std::vector<char> readString;

	if (_readSize == 0)
		return 0;
	readString.resize(_readSize);
	m_infile.read(&readString[0], _readSize);
	iss.str(&readString[0]);
	for (std::vector<std::string>::iterator sit = _dest.begin(); sit != _dest.end(); ++sit)
		iss >> *sit;
	if (!m_infile.good() || iss.fail())
		return FileReadError();
	return 0;
}
int CReadBinaryDosageFile4::ReadDoubleArray(std::vector<double> &_dest) {
	m_infile.read((char *)&_dest[0], _dest.size() * sizeof(double));
	if (!m_infile.good())
		return FileReadError();
	return 0;
}

void CReadBinaryDosageFile4::AssignSubjects(const std::vector<std::string> &_iid) {
	CSubject sub;
	std::vector<CSubject>::iterator subit;

	subit = m_subject.begin();
	for (std::vector<std::string>::const_iterator sit = _iid.begin(); sit != _iid.end(); ++sit, ++subit)
		*subit = sub.Assign(*sit);
}
void CReadBinaryDosageFile4::AssignFamilySubjects(const std::vector<std::string> &_iid, const std::vector<std::string> &_fid) {
	CFamilySubject subFam;
	std::vector<CFamilySubject>::iterator subit;
	std::vector<std::string>::const_iterator sit, fit;

	subit = m_familySubject.begin();
	for (sit = _iid.begin(), fit = _fid.begin(); sit != _iid.end(); ++sit, ++fit, ++subit)
		*subit = subFam.Assign(*sit, *fit);
}
void CReadBinaryDosageFile4::AssignSNPs(const std::vector<std::string> &_snpName, const std::vector<std::string> &_chromosome, const std::vector<unsigned int> &_location,
	const std::vector<std::string> &_refAllele, const std::vector<std::string> &_altAllele) {
	std::vector<std::string>::const_iterator sit1, sit2, sit3, sit4;
	std::vector<unsigned int>::const_iterator uit;
	std::vector<CSNP>::iterator snpit;

	sit1 = _snpName.begin();
	sit2 = _chromosome.begin();
	sit3 = _refAllele.begin();
	sit4 = _altAllele.begin();
	uit = _location.begin();
	if (m_SNPOptions & 0x00000008) {
		for (snpit = m_SNP.begin(); snpit != m_SNP.end(); ++snpit, ++sit1, ++sit3, ++sit4, ++uit)
			snpit->Assign(*sit1, *sit2, *uit, *sit3, *sit4);
	}
	else {
		for (snpit = m_SNP.begin(); snpit != m_SNP.end(); ++snpit, ++sit1, ++sit2, ++sit3, ++sit4, ++uit)
			snpit->Assign(*sit1, *sit2, *uit, *sit3, *sit4);
	}
}

int CReadBinaryDosageFile4::ReadGroups() {
	m_infile.read((char *)&m_groupSize[0], m_numGroups * sizeof(unsigned int));
	if (!m_infile.good())
		return FileReadError();
	return ReadSubjects();
}
int CReadBinaryDosageFile4::ReadSubjects(const bool _subjectsOnly) {
	std::vector<std::string> iid, fid;
	unsigned int iidSize, fidSize;

	m_infile.seekg(m_startSubjects);
	iid.resize(m_numSubjects);
	fid.resize(((m_subjectOptions & 0x00000001) ? m_numSubjects : 0));
	m_infile.read((char *)&iidSize, sizeof(unsigned int));
	m_infile.read((char *)&fidSize, sizeof(unsigned int));
	if (ReadStringArray(iid, iidSize) || ReadStringArray(fid, fidSize))
		return 1;
	(m_subjectOptions & 0x00000001) ? AssignFamilySubjects(iid, fid) : AssignSubjects(iid);
	return (_subjectsOnly ? 0 : ReadSNPs());
}
int CReadBinaryDosageFile4::ReadSNPs() {
	unsigned int arraySize[4];
	std::vector<std::string> snpName, chromosome, refAllele, altAllele;
	std::vector<unsigned int> location;

	m_infile.read((char *)arraySize, sizeof(arraySize));
	snpName.resize(m_numSNPs);
	chromosome.resize(((m_SNPOptions & 0x00000008) ? 1 : m_numSNPs));
	if (ReadStringArray(snpName, arraySize[0]) || ReadStringArray(chromosome, arraySize[1]))
		return 1;
	location.resize(m_numSNPs);
	m_infile.read((char *)&location[0], m_numSNPs * sizeof(unsigned int));
	refAllele.resize(m_numSNPs);
	altAllele.resize(m_numSNPs);
	if (ReadStringArray(refAllele, arraySize[2]) || ReadStringArray(altAllele, arraySize[3]))
		return 1;
	AssignSNPs(snpName, chromosome, location, refAllele, altAllele);
	return ReadImputationInfo();
}
int CReadBinaryDosageFile4::ReadImputationInfo() {
	std::vector<CImputationInfo> imputeInfo;
	std::vector<double> afreq, maf, avgCall, rsq;
	std::vector<double>::const_iterator dit1, dit2, dit3, dit4;

	afreq.resize(m_numSNPs * m_numGroups);
	maf.resize(m_numSNPs * m_numGroups);
	avgCall.resize(m_numSNPs * m_numGroups);
	rsq.resize(m_numSNPs * m_numGroups);
	if (ReadDoubleArray(afreq) || ReadDoubleArray(maf) || ReadDoubleArray(avgCall) || ReadDoubleArray(rsq))
		return 1;
	imputeInfo.resize(m_numGroups);
	dit1 = afreq.begin();
	dit2 = maf.begin();
	dit3 = avgCall.begin();
	dit4 = rsq.begin();
	for (std::vector<CSNP>::iterator snpit = m_SNP.begin(); snpit != m_SNP.end(); ++snpit) {
		for (std::vector<CImputationInfo>::iterator impit = imputeInfo.begin(); impit != imputeInfo.end(); ++impit, ++dit1, ++dit2, ++dit3, ++dit4)
			impit->Assign("", *dit1, *dit2, *dit3, *dit4);
		snpit->ImputationInfo(imputeInfo);
	}
	return IndexSNPs();
}
int CReadBinaryDosageFile4::ReadFileInfo() {
	ClearVectors();
	if (!m_infile.good())
		return 1;
	m_infile.read((char *)&m_numSubjects, sizeof(unsigned int));
	m_infile.read((char *)&m_numSNPs, sizeof(unsigned int));
	m_infile.read((char *)&m_numGroups, sizeof(unsigned int));
	m_infile.read((char *)&m_subjectOptions, sizeof(unsigned int));
	m_infile.read((char *)&m_SNPOptions, sizeof(unsigned int));
	m_infile.read((char *)&m_startSubjects, sizeof(unsigned int));
	m_infile.read((char *)&m_startSNPs, sizeof(unsigned int));
	m_startGroups = 40;
	m_infile.read((char *)&m_startDosages, sizeof(unsigned int));
	if (!m_infile.good())
		return FileReadError();
	AssignVectors();
	return ReadGroups();
}

int CReadBinaryDosageFile4::GoToFirst() {
	m_infile.clear();
	m_infile.seekg(m_startDosages);
	if (!m_infile.good())
		return FileReadError();
	return 0;
}

int CReadBinaryDosageFile4::GetSubjects(const std::string &_filename) {
	if (m_infile.is_open())
		return FileAlreadyOpenError();
	if (OpenFile(_filename))
		return 1;
	if (CheckFileType() || ReadVersion()) {
		m_infile.close();
		return 1;
	}
	m_infile.read((char *)&m_numSubjects, sizeof(unsigned int));
	m_infile.seekg(8, std::ios::cur);
	m_infile.read((char *)&m_subjectOptions, sizeof(unsigned int));
	m_infile.seekg(4, std::ios::cur);
	m_infile.read((char *)&m_startSubjects, sizeof(unsigned int));
	if (m_subjectOptions & 0x00000001)
		m_familySubject.resize(m_numSubjects);
	else
		m_subject.resize(m_numSubjects);
	if (ReadSubjects(true)) {
		m_infile.close();
		return 1;
	}
	m_infile.close();
	return 0;
}
int CReadBinaryDosageFile4::GetFirst(const bool _process) {
    m_currentSNP = 0;
    if (GoToFirst())
        return 1;
    return ((_process ? ReadSNP() : SkipSNP()));
}
int CReadBinaryDosageFile4::GetNext(const bool _process) {
    ++m_currentSNP;
    if (m_currentSNP >= m_numSNPs)
        return 1;
    return ((_process ? ReadSNP() : SkipSNP()));
}
int CReadBinaryDosageFile4::GetSNP(const unsigned int _n) {
  if (_n >= m_numSNPs)
    return 1;
  m_currentSNP = _n;
  m_infile.seekg(m_snpIndex[_n]);
  return ReadSNP();
}

unsigned int CReadBinaryDosageFile4::FindSNPbyName(std::vector<unsigned int> &_index, const std::string &_name) {
    std::vector<unsigned int>().swap(_index);
    for (std::vector<CSNP>::const_iterator snpit = m_SNP.begin(); snpit != m_SNP.end(); ++snpit) {
        if (snpit->Name() == _name)
            _index.push_back((unsigned int)(snpit - m_SNP.begin()));
    }
    return (unsigned int)_index.size();
}
unsigned int CReadBinaryDosageFile4::FindSNP(std::vector<unsigned int> &_index, const std::string &_name,  const std::string &_chromosome, const unsigned int _location, const std::string &_refAllele, const std::string &_altAllele) {
    CSNP snp(_name, _chromosome, _location, _refAllele, _altAllele);
    std::vector<unsigned int>().swap(_index);
    for (std::vector<CSNP>::const_iterator snpit = m_SNP.begin(); snpit != m_SNP.end(); ++snpit) {
        if (*snpit == snp)
            _index.push_back((unsigned int)(snpit - m_SNP.begin()));
    }
    return (unsigned int)_index.size();
}
unsigned int CReadBinaryDosageFile4::FindSNPbyLocation(std::vector<unsigned int> &_index, const std::string &_chromosome, const unsigned int _location) {
    std::vector<unsigned int>().swap(_index);
    for (std::vector<CSNP>::const_iterator snpit = m_SNP.begin(); snpit != m_SNP.end(); ++snpit) {
        if (snpit->Location() == _location && snpit->Chromosome() == _chromosome)
            _index.push_back((unsigned int)(snpit - m_SNP.begin()));
    }
    return (unsigned int)_index.size();
}

unsigned int CReadBinaryDosageFile4::FindSubjects(std::vector<unsigned int> &_index, const std::vector<std::string> &_iidv) {
    std::vector<CSubject>::const_iterator subit;
    std::vector<unsigned int>().swap(_index);
    for (std::vector<std::string>::const_iterator sid = _iidv.begin(); sid != _iidv.end(); ++sid) {
        subit = std::find(m_subject.begin(), m_subject.end(), *sid);
        if (subit != m_subject.end())
            _index.push_back((unsigned int)(subit - m_subject.begin()));
        else
            _index.push_back(0xffffffff);
    }
    return (int)_iidv.size();
}
unsigned int CReadBinaryDosageFile4::FindSubjects(std::vector<unsigned int> &_index, const std::vector<std::string> &_fidv,const std::vector<std::string> &_iidv) {
    std::vector<CFamilySubject>::const_iterator subit;
    std::vector<std::string>::const_iterator sit1, sit2;
    CFamilySubject subToFind;
    unsigned int found;

    found = 0;
    std::vector<unsigned int>().swap(_index);
    if (_fidv.size() != _iidv.size())
        return 0xffffffff;
    sit2 = _iidv.begin();
    for (sit1 = _fidv.begin(); sit1 != _fidv.end(); ++sit1, ++sit2) {
        subToFind.Assign(*sit1, *sit2);
        subit = std::find(m_familySubject.begin(), m_familySubject.end(), subToFind);
        if (subit != m_familySubject.end()) {
            _index.push_back((unsigned int)(subit - m_familySubject.begin()));
            ++found;
        } else {
            _index.push_back(0xffffffff);
        }
    }
    return found;
}

int CReadBinaryDosageFile4::ReadHeader() {
	if (CBinaryDosageVersion::ReadHeader())
		return 1;
	return (ReadFileInfo());
}

// **********************************************
// CReadBinaryFile41
// **********************************************
int CReadBinaryDosageFile41::CheckVersion() {
    if (CReadBinaryDosageFile4::CheckVersion() || m_revision != 1)
        return VersionError();
    return 0;
}
int CReadBinaryDosageFile41::ReadSNP() {
    std::cout << "Read 4.1" << std::endl;
	return 0;
}

int CReadBinaryDosageFile41::SkipSNP() {
    std::cout << "Skip 4.1" << std::endl;
    return 0;
}

// **********************************************
// CReadBinaryFile42
// **********************************************
int CReadBinaryDosageFile42::CheckVersion() {
    if (CReadBinaryDosageFile4::CheckVersion() || m_revision != 2)
        return VersionError();
    return 0;
}

void CReadBinaryDosageFile42::AssignVectors() {
    CReadBinaryDosageFile4::AssignVectors();
	m_sdosages.resize(4 * m_numSubjects);
    m_p0.resize(m_numSubjects);
    m_p1.resize(m_numSubjects);
    m_p2.resize(m_numSubjects);
	m_snpIndex.resize(m_numSNPs);
}
int CReadBinaryDosageFile42::IndexSNPs() {
	*(m_snpIndex.begin()) = m_infile.tellg();
	GetFirst(false);
	for (std::vector<std::streampos>::iterator spit = m_snpIndex.begin() + 1; spit != m_snpIndex.end(); ++spit) {
		*spit = m_infile.tellg();
		GetNext(false);
	}
	if (!m_infile.good())
		return FileReadError();
	return 0;
}
int CReadBinaryDosageFile42::ReadSNP() {
  unsigned int snpSize;
  std::vector<double>::iterator dit, p0it, p1it, p2it;
  std::vector<short>::const_iterator sit1, sit2;

  m_infile.read((char *)&snpSize, sizeof(unsigned int));
  std::fill(m_dosages.begin(), m_dosages.end(), std::numeric_limits<double>::quiet_NaN());
  std::fill(m_p0.begin(), m_p0.end(), std::numeric_limits<double>::quiet_NaN());
  std::fill(m_p1.begin(), m_p1.end(), std::numeric_limits<double>::quiet_NaN());
  std::fill(m_p2.begin(), m_p2.end(), std::numeric_limits<double>::quiet_NaN());
  if (snpSize > 0) {
    m_infile.read((char *)&m_sdosages[0], snpSize);
    dit = m_dosages.begin();
    p0it = m_p0.begin();
    p1it = m_p1.begin();
    p2it = m_p2.begin();
    sit2 = m_sdosages.begin() + m_numSubjects;
    for (sit1 = m_sdosages.begin(); sit1 != m_sdosages.begin() + m_numSubjects; ++sit1, ++dit, ++p0it, ++p1it, ++p2it) {
      if (*sit1 == 20001)
        continue;
      *dit = (double)(*sit1 & 0x7fff) / 10000.;
      if (*sit1 & 0x8000) {
        *p1it = (double)(*sit2 & 0x7fff) / 10000.;
        if (*sit2 & 0x8000) {
          ++sit2;
          *p0it = (double)(*sit2 & 0x7fff) / 10000.;
          ++sit2;
          *p2it = (double)(*sit2 & 0x7fff) / 10000.;
        } else {
          *p2it = (1. - *p1it) / 2.;
          *p0it = 1. - *p1it - *p2it;
        }
        ++sit2;
      }
      else {
        if (*dit > 1.) {
          *p0it = 0.;
          *p2it = *dit - 1.;
          *p1it = 1. - *p2it;
        } else {
          *p2it = 0.;
          *p1it = *dit;
          *p0it = 1. - *dit;
        }
      }
    }
  }
  if (!m_infile.good())
    return FileReadError();
  return 0;
}

int CReadBinaryDosageFile42::SkipSNP() {
    unsigned int snpSize;

    m_infile.read((char *)&snpSize, sizeof(unsigned int));
    m_infile.seekg(snpSize, std::ios::cur);
    if (!m_infile.good())
        return FileReadError();
    return 0;
}
/*
int GetBinaryDosageVersion(const std::string &_filename, int &_version, int &_subversion, std::string &_errorMessage) {
	std::ifstream infile;
	const char bdoseHeader[4] = { 'b', 'o', 's', 'e' };
	char readHeader[8];

	_version = 0;
	_subversion = 0;
	infile.open(_filename, std::ios::in | std::ios::binary);
	if (!infile.good()) {
        _errorMessage = "Unable to open file:\t" + _filename;
		return 1;
	}
	infile.read(readHeader, 8);
	infile.close();
	if (std::memcmp(readHeader, bdoseHeader, 4)) {
        _errorMessage = _filename + " is not a binary dosage file";
		return 1;
	}
	if (readHeader[4] != 0 || readHeader[6] != 0) {
		CERR << "Unknown version" << std::endl;
		return 1;
	}
	_version = readHeader[5];
	_subversion = readHeader[7];
	if (_version < 1 || _version > 4 || _version == 2) {
		CERR << "Unknown version" << std::endl;
		return 1;
	}
	if (_subversion < 1 || _subversion > 2) {
		CERR << "Unknown version" << std::endl;
		return 1;
	}

	return 0;
}


// ********************************************************************************************
//
// Reading in the binary dosage file information
//
// ********************************************************************************************
const char CReadBinaryDosage::m_magicWord[4] = { 'b', 'o', 's', 'e' };

CReadBinaryDosage::CReadBinaryDosage() {
	m_numSubjects = 0;
	m_numSNPs = 0;
	m_numGroups = 0;
	m_subjectOptions = 0;
	m_SNPOptions = 0;
}

CReadBinaryDosage::~CReadBinaryDosage() {
	m_infile.close();
}

int CReadBinaryDosage::CheckVersion() {
	char mw[4];

	m_infile.read(mw, 4);
	if (std::memcmp(mw, m_magicWord, 4)) {
		CERR << "File does not appear to be a binary dosage file" << std::endl;
		return 1;
	}
	return 0;
}

int CReadBinaryDosage::ReadHeader() {
	return CheckVersion();
}
// ********************************************************************************************
// Version 4
// ********************************************************************************************
CReadBinaryDosage4::CReadBinaryDosage4() : CReadBinaryDosage() {
	m_subjectStart = 0;
	m_SNPStart = 0;
	m_SNPNameSize = 0;
	m_chromosomeSize = 0;
	m_refAlleleSize = 0;
	m_altAlleleSize = 0;
	m_dosageStart = 0;
	m_subjectIDSize = 0;
	m_familyIDSize = 0;
}

int CReadBinaryDosage4::CheckVersion() {
	char vn[2];
	const char evn[2] = { 0x0, 0x4 };

	if (CReadBinaryDosage::CheckVersion())
		return 1;
	m_infile.read(vn, 2);
	if (std::memcmp(vn, evn, 2)) {
		CERR << "File is not version 4.x" << std::endl;
		return 1;
	}
	return 0;
}

int CReadBinaryDosage4::ReadHeader() {
	if (CReadBinaryDosage::ReadHeader())
		return 1;
	m_infile.read((char *)&m_numSubjects, sizeof(unsigned int));
	m_infile.read((char *)&m_numSNPs, sizeof(unsigned int));
	m_infile.read((char *)&m_numGroups, sizeof(unsigned int));
	m_infile.read((char *)&m_subjectOptions, sizeof(unsigned int));
	m_infile.read((char *)&m_SNPOptions, sizeof(unsigned int));
	m_infile.read((char *)&m_subjectStart, sizeof(unsigned int));
	m_infile.read((char *)&m_SNPStart, sizeof(unsigned int));
	m_infile.read((char *)&m_dosageStart, sizeof(unsigned int));
	std::vector<unsigned int>().swap(m_groupSize);
	m_groupSize.resize(m_numGroups);
	m_infile.read((char *)&m_groupSize[0], m_numGroups * sizeof(unsigned int));

	if (m_infile.fail()) {
		CERR << "Error reading header information" << std::endl;
		return 1;
	}

	return 0;
}

int CReadBinaryDosage4::ReadSubjects() {
	std::vector<char> idArray;
	std::istringstream iss;
	unsigned int ui;

	std::vector<std::string>().swap(m_subjectID);
	std::vector<std::string>().swap(m_familyID);

	m_infile.seekg(m_subjectStart);
	m_infile.read((char *)&m_subjectIDSize, sizeof(unsigned int));
	m_infile.read((char *)&m_familyIDSize, sizeof(unsigned int));

	idArray.resize(m_subjectIDSize);
	m_infile.read(&idArray[0], m_subjectIDSize);
	m_subjectID.resize(m_numSubjects);
	iss.str(&idArray[0]);
	for (ui = 0; ui < m_numSubjects; ++ui)
		iss >> m_subjectID[ui];

	if (m_familyIDSize > 0) {
		std::vector<char>().swap(idArray);
		idArray.resize(m_familyIDSize);
		iss.str(&idArray[0]);
		iss.clear();
		for (ui = 0; ui < m_numSubjects; ++ui)
			iss >> m_familyID[ui];
	}
	//  Rcpp::Rcout << m_infile.tellg() << std::endl;

	return 0;
}

int CReadBinaryDosage4::ReadSNPInfo() {
	std::vector<char> idArray;
	std::istringstream iss;
	std::string rchr;
	unsigned int ui;

	std::vector<std::string>().swap(m_SNPName);
	std::vector<std::string>().swap(m_chromosome);
	std::vector<std::string>().swap(m_refAllele);
	std::vector<std::string>().swap(m_altAllele);
	std::vector<unsigned int>().swap(m_location);
	std::vector<double>().swap(m_altFreq);
	std::vector<double>().swap(m_maf);
	std::vector<double>().swap(m_avgCall);
	std::vector<double>().swap(m_rSquared);

	m_infile.seekg(m_SNPStart);
	m_infile.read((char *)&m_SNPNameSize, sizeof(unsigned int));
	m_infile.read((char *)&m_chromosomeSize, sizeof(unsigned int));
	m_infile.read((char *)&m_refAlleleSize, sizeof(unsigned int));
	m_infile.read((char *)&m_altAlleleSize, sizeof(unsigned int));

	if (m_SNPNameSize > 0) {
		std::vector<char>().swap(idArray);
		idArray.resize(m_SNPNameSize);
		m_infile.read(&idArray[0], m_SNPNameSize);
		iss.str(&idArray[0]);
		iss.clear();
		m_SNPName.resize(m_numSNPs);
		for (ui = 0; ui < m_numSNPs; ++ui)
			iss >> m_SNPName[ui];
	}

	if (m_chromosomeSize > 0) {
		std::vector<char>().swap(idArray);
		idArray.resize(m_chromosomeSize);
		m_infile.read(&idArray[0], m_chromosomeSize);
		iss.str(&idArray[0]);
		iss.clear();
		m_chromosome.resize(m_numSNPs);
		if (m_SNPOptions & 0x0008) {
			iss >> rchr;
			for (ui = 0; ui < m_numSNPs; ++ui)
				m_chromosome[ui] = rchr;
		}
		else {
			for (ui = 0; ui < m_numSNPs; ++ui)
				iss >> m_chromosome[ui];
		}
	}

	m_location.resize(m_numSNPs);
	m_infile.read((char *)&m_location[0], m_numSNPs * sizeof(unsigned int));

	if (m_refAlleleSize > 0) {
		std::vector<char>().swap(idArray);
		idArray.resize(m_refAlleleSize);
		m_infile.read(&idArray[0], m_refAlleleSize);
		iss.str(&idArray[0]);
		iss.clear();
		m_refAllele.resize(m_numSNPs);
		for (ui = 0; ui < m_numSNPs; ++ui)
			iss >> m_refAllele[ui];
	}

	if (m_altAlleleSize > 0) {
		std::vector<char>().swap(idArray);
		idArray.resize(m_altAlleleSize);
		m_infile.read(&idArray[0], m_altAlleleSize);
		iss.str(&idArray[0]);
		iss.clear();
		m_altAllele.resize(m_numSNPs);
		for (ui = 0; ui < m_numSNPs; ++ui)
			iss >> m_altAllele[ui];
	}

	m_altFreq.resize(m_numSNPs);
	m_infile.read((char *)&m_altFreq[0], m_numSNPs * sizeof(double));
	m_maf.resize(m_numSNPs);
	m_infile.read((char *)&m_maf[0], m_numSNPs * sizeof(double));
	m_avgCall.resize(m_numSNPs);
	m_infile.read((char *)&m_avgCall[0], m_numSNPs * sizeof(double));
	m_rSquared.resize(m_numSNPs);
	m_infile.read((char *)&m_rSquared[0], m_numSNPs * sizeof(double));

	//  Rcpp::Rcout << m_infile.tellg() << std::endl;

	return 0;
}

// Read the the amount of memory used for each SNP dosase information
int CReadBinaryDosage4::ReadSNPDataSize() {
	unsigned int ui;

	std::vector<unsigned int>().swap(m_SNPDataSize);
	m_SNPDataSize.resize(m_numSNPs);

	m_infile.seekg(m_dosageStart);
	for (ui = 0; ui < m_numSNPs; ++ui) {
		m_infile.read((char *)&m_SNPDataSize[ui], sizeof(unsigned int));
		m_infile.seekg(m_SNPDataSize[ui], std::ios::cur);
	}
	return 0;
}

int CReadBinaryDosage4::ReadFileInfo(const std::string &binaryDosageFilename) {
	m_infile.close();
	m_infile.clear();

	m_infile.open(binaryDosageFilename.c_str(), std::ios::in | std::ios::binary);
	if (!m_infile.good()) {
		CERR << "Unable to open file:\t" << binaryDosageFilename << std::endl;
		return 1;
	}

	if (ReadHeader())
		return 1;
	if (ReadSubjects())
		return 1;
	if (ReadSNPInfo())
		return 1;
	//	std::cout << "Subjects:\t" << std::setw(8) << m_numSubjects << std::endl
	//		<< "SNPs:\t\t" << std::setw(8) << m_numSNPs << std::endl
	//		<< "Groups:\t\t" << std::setw(8) << m_numGroups << std::endl;

	std::vector<double>().swap(m_dosage);
	m_dosage.resize(m_numSubjects);

	return 0;
}
// ********************************************************************************************
// Version 4.1
// ********************************************************************************************
int CReadBinaryDosage41::CheckVersion() {
	char svn[2];
	const char esvn[2] = { 0x0, 0x1 };

	if (CReadBinaryDosage4::CheckVersion())
		return 1;
	m_infile.read(svn, 2);
	if (std::memcmp(svn, esvn, 2)) {
		CERR << "File is not version 4.1" << std::endl;
		return 1;
	}
	return 0;
}

int CReadBinaryDosage41::ReadFileInfo(const std::string &binaryDosageFilename) {
	if (CReadBinaryDosage4::ReadFileInfo(binaryDosageFilename))
		return 1;

	if (ReadSNPDataSize())
		return 1;

	std::vector<short>().swap(m_readData);
	m_readData.resize(m_numSubjects);

	return 0;
}

int CReadBinaryDosage41::GetFirst(bool process) {
	m_infile.clear();
	m_infile.seekg(m_dosageStart);
	m_infile.read((char *)&m_readData[0], m_numSubjects + m_numSubjects);
	//	for (unsigned int ui = 0; ui < 10; ++ui)
	//		std::cout << std::setw(8) << m_readData[ui];
	//	std::cout << std::endl;
	if (m_infile.fail())
		return 1;
	return 0;
}
// ********************************************************************************************
// Version 4.2
// ********************************************************************************************
int CReadBinaryDosage42::CheckVersion() {
	char svn[2];
	const char esvn[2] = { 0x0, 0x2 };

	if (CReadBinaryDosage4::CheckVersion())
		return 1;
	m_infile.read(svn, 2);
	if (std::memcmp(svn, esvn, 2)) {
		CERR << "File is not version 4.2" << std::endl;
		return 1;
	}
	return 0;
}

int CReadBinaryDosage42::ReadFileInfo(const std::string &binaryDosageFilename) {
	if (CReadBinaryDosage4::ReadFileInfo(binaryDosageFilename))
		return 1;

	if (ReadSNPDataSize())
		return 1;

	std::vector<short>().swap(m_readData);
	m_readData.resize(4 * m_numSubjects);

	return 0;
}

int CReadBinaryDosage42::GetFirst(bool process) {
	m_infile.clear();
	m_infile.seekg(m_dosageStart + sizeof(unsigned int));
	m_infile.read((char *)&m_readData[0], m_SNPDataSize[0]);
	if (m_infile.fail())
		return 1;
	return 0;
}
*/
