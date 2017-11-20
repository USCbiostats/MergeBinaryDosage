#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "SNPClass.h"

// ***********************************************************
// CImputationInfo
// ***********************************************************
CImputationInfo::CImputationInfo() {
    m_group = "";
    m_altFreq = 0.;
    m_maf = 0.;
    m_avgCall = 0.;
    m_rsquared = 0;
}

CImputationInfo::CImputationInfo(const CImputationInfo &_src) {
    m_group = _src.m_group;
    m_altFreq = _src.m_altFreq;
    m_maf = _src.m_maf;
    m_avgCall = _src.m_avgCall;
    m_rsquared = _src.m_rsquared;
}

CImputationInfo::CImputationInfo(const std::string _group, const double _altFreq, const double _maf, const double _avgCall, const double _rsquared) {
    m_group = _group;
    m_altFreq = _altFreq;
    m_maf = _maf;
    m_avgCall = _avgCall;
    m_rsquared = _rsquared;
}

CImputationInfo &CImputationInfo::operator=(const CImputationInfo &_rhs) {
    if (this == &_rhs)
        return *this;
    m_group = _rhs.m_group;
    m_altFreq = _rhs.m_altFreq;
    m_maf = _rhs.m_maf;
    m_avgCall = _rhs.m_avgCall;
    m_rsquared = _rhs.m_rsquared;
    return *this;
}

bool CImputationInfo::operator==(const CImputationInfo &_rhs) const {
    if (this == &_rhs)
        return true;
    return (m_group == _rhs.m_group && m_altFreq == _rhs.m_altFreq && m_maf == _rhs.m_maf && m_avgCall == _rhs.m_avgCall && m_rsquared == _rhs.m_rsquared);
}

CImputationInfo &CImputationInfo::Assign(const std::string _group, const double _altFreq, const double _maf, const double _avgCall, const double _rsquared) {
    m_group = _group;
    m_altFreq = _altFreq;
    m_maf = _maf;
    m_avgCall = _avgCall;
    m_rsquared = _rsquared;
	return *this;
}

// ***********************************************************
// CSNP
// ***********************************************************
CSNP::CSNP() {
	m_name = "";
	m_chromosome = "";
	m_location = 0;
	m_referenceAllele = "";
	m_alternateAllele = "";
}

CSNP::CSNP(const CSNP &_src) {
	m_name = _src.m_name;
	m_chromosome = _src.m_chromosome;
	m_location = _src.m_location;
	m_referenceAllele = _src.m_referenceAllele;
	m_alternateAllele = _src.m_alternateAllele;
}

CSNP::CSNP(const std::string &_name, const std::string &_chromosome, const unsigned int _location, const std::string &_referenceAllele, const std::string &_alternateAllele) {
	m_name = _name;
	m_chromosome = _chromosome;
	m_location = _location;
	m_referenceAllele = _referenceAllele;
	m_alternateAllele = _alternateAllele;
}

CSNP::CSNP(const std::string &_name, const std::string &_chromosome, const unsigned int _location, const std::string &_referenceAllele, const std::string &_alternateAllele, const CImputationInfo &_imputationInfo) : CSNP(_name, _chromosome, _location, _referenceAllele, _alternateAllele) {
    m_imputationInfo.push_back(_imputationInfo);
}

CSNP::CSNP(const std::string &_name, const std::string &_chromosome, const unsigned int _location, const std::string &_referenceAllele, const std::string &_alternateAllele, const std::vector<CImputationInfo> &_imputationInfo) : CSNP(_name, _chromosome, _location, _referenceAllele, _alternateAllele) {
    m_imputationInfo = _imputationInfo;
}

CSNP &CSNP::operator=(const CSNP &_rhs) {
	if (this == &_rhs)
		return *this;
	m_name = _rhs.m_name;
	m_chromosome = _rhs.m_chromosome;
	m_location = _rhs.m_location;
	m_referenceAllele = _rhs.m_referenceAllele;
	m_alternateAllele = _rhs.m_alternateAllele;
    m_imputationInfo = _rhs.m_imputationInfo;
	return *this;
}

bool CSNP::operator==(const CSNP &_rhs) const {
	if (this == &_rhs)
		return true;
    return (m_location == _rhs.m_location && m_name == _rhs.m_name && m_chromosome == _rhs.m_chromosome && m_referenceAllele == _rhs.m_referenceAllele && m_alternateAllele == _rhs.m_alternateAllele);
}

CSNP &CSNP::Assign(const std::string &_name, const std::string &_chromosome, const unsigned int _location, const std::string &_referenceAllele, const std::string &_alternateAllele) {
	m_name = _name;
	m_chromosome = _chromosome;
	m_location = _location;
	m_referenceAllele = _referenceAllele;
	m_alternateAllele = _alternateAllele;
	return *this;
}

CSNP &CSNP::Assign(const std::string &_name, const std::string &_chromosome, const unsigned int _location, const std::string &_referenceAllele, const std::string &_alternateAllele, const CImputationInfo &_imputationInfo) {
    Assign(_name, _chromosome, _location, _referenceAllele, _alternateAllele);
    std::vector<CImputationInfo>().swap(m_imputationInfo);
    m_imputationInfo.push_back(_imputationInfo);
	return *this;
}

CSNP &CSNP::Assign(const std::string &_name, const std::string &_chromosome, const unsigned int _location, const std::string &_referenceAllele, const std::string &_alternateAllele, const std::vector<CImputationInfo> &_imputationInfo) {
    Assign(_name, _chromosome, _location, _referenceAllele, _alternateAllele);
    m_imputationInfo = _imputationInfo;
	return *this;
}
