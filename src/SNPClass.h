#ifndef SNPCLASS_H
#define SNPCLASS_H 1

class CImputationInfo {
protected:
    std::string m_group;
    double m_altFreq;
    double m_maf;
    double m_avgCall;
    double m_rsquared;
public:
    CImputationInfo();
    CImputationInfo(const CImputationInfo &_src);
    CImputationInfo(const std::string _group, const double _altFreq, const double _maf, const double _avgCall, const double _rsquared);
    ~CImputationInfo() {};
    
    CImputationInfo &operator=(const CImputationInfo &_rhs);
    bool operator==(const CImputationInfo &_rhs) const;
	CImputationInfo &Assign(const std::string _group, const double _altFreq, const double _maf, const double _avgCall, const double _rsquared);

    void Group(const std::string &_group) { m_group = _group; }
    void AltFreq(const double _altFreq) { m_altFreq = _altFreq; }
    void MAF(const double _maf) { m_maf = _maf; }
    void AvgCall(const double _avgCall) { m_avgCall = _avgCall; }
    void RSquared(const double _rsquared) { m_rsquared = _rsquared; }
    
    const std::string &Group() const { return m_group; }
    double AltFreq() const { return m_altFreq; }
    double MAF() const { return m_maf; }
    double AvgCall() const { return m_avgCall; }
    double RSquared() const { return m_rsquared; }

	friend std::ostream&operator<<(std::ostream &_stream, const CImputationInfo &_imputationInfo) {
		if (_imputationInfo.m_group != "")
			_stream << _imputationInfo.m_group << '\t';
		_stream << _imputationInfo.m_altFreq << '\t' << _imputationInfo.m_maf << '\t' << _imputationInfo.m_avgCall<< '\t' << _imputationInfo.m_rsquared;
		return _stream;
	}
};

class CSNP {
protected:
	std::string m_name;
	std::string m_chromosome;
	unsigned int m_location;
	std::string m_referenceAllele;
	std::string m_alternateAllele;
    std::vector<CImputationInfo> m_imputationInfo;
public:
	CSNP();
	CSNP(const CSNP &_src);
	CSNP(const std::string &_name, const std::string &_chromosome, const unsigned int _location, const std::string &_referenceAllele, const std::string &_alternateAllele);
    CSNP(const std::string &_name, const std::string &_chromosome, const unsigned int _location, const std::string &_referenceAllele, const std::string &_alternateAllele, const CImputationInfo &_imputationInfo);
    CSNP(const std::string &_name, const std::string &_chromosome, const unsigned int _location, const std::string &_referenceAllele, const std::string &_alternateAllele, const std::vector<CImputationInfo> &_imputationInfo);
	~CSNP() {}

	CSNP &operator=(const CSNP &_rhs);
    bool operator==(const CSNP &_rhs) const;
	CSNP &Assign(const std::string &_name, const std::string &_chromosome, const unsigned int _location, const std::string &_referenceAllele, const std::string &_alternateAllele);
	CSNP &Assign(const std::string &_name, const std::string &_chromosome, const unsigned int _location, const std::string &_referenceAllele, const std::string &_alternateAllele, const CImputationInfo &_imputationInfo);
	CSNP &Assign(const std::string &_name, const std::string &_chromosome, const unsigned int _location, const std::string &_referenceAllele, const std::string &_alternateAllele, const std::vector<CImputationInfo> &_imputationInfo);
    
	void Name(const std::string &_name) { m_name = _name; }
	void Chromosome(const std::string &_chromosome) { m_chromosome = _chromosome; }
	void Location(const unsigned int _location) { m_location = _location; }
	void ReferenceAllele(const std::string &_referenceAllele) { m_referenceAllele = _referenceAllele; }
	void AlternateAllele(const std::string &_alternateAllele) { m_alternateAllele = _alternateAllele; }
    void ImputationInfo(const std::vector<CImputationInfo> &_imputationInfo) { m_imputationInfo = _imputationInfo; }
    void AddImputationInfo(const CImputationInfo &_imputationInfo) { m_imputationInfo.push_back(_imputationInfo); }

	const std::string &Name() const { return m_name; }
	const std::string &Chromosome() const { return m_chromosome; }
	unsigned int Location() const { return m_location; }
	const std::string &ReferenceAllele() const { return m_referenceAllele; }
	const std::string &AlternateAllele() const { return m_alternateAllele; }
    const std::vector<CImputationInfo> &ImputationInfo() const { return m_imputationInfo; }

	friend std::ostream &operator<<(std::ostream &_stream, const CSNP &_snp) {
		if (_snp.m_name != "")
			_stream << _snp.m_name << '\t';
		_stream << _snp.m_chromosome << '\t' << _snp.m_location << '\t' << _snp.m_referenceAllele << '\t' << _snp.m_alternateAllele;
		return _stream;
	}
};
#endif
