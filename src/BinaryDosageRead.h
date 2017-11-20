#ifndef BINARYDOSAGEREAD_H
#define BINARYDOSAGEREAD_H 1

// int GetBinaryDosageVersion(const std::string &_filename, int &_version, int &_subversion, std::string &_errorMessage);

class CReadFile {
protected:
  std::string m_filename;
  std::string m_errorMessage;
  std::ifstream m_infile;

	virtual int FileOpenError();
	virtual int FileReadError();

	virtual int OpenFile(const std::string &_filename);
public:
  CReadFile();
  virtual ~CReadFile() { m_infile.close(); }
 
	virtual int Open(const std::string &_filename) = 0;
  const std::string &ErrorMessage() const { return m_errorMessage; }
};

class CReadBinaryFile : public CReadFile {
protected:
	virtual int OpenFile(const std::string &_filename);
public:
	CReadBinaryFile() : CReadFile() {}
	virtual ~CReadBinaryFile() {}
};

class CBinaryDosageVersion : public CReadBinaryFile {
protected:
	unsigned int m_version;
	unsigned int m_revision;

	virtual int FileTypeError();
	virtual int VersionError();

	virtual int CheckVersion();
	virtual int CheckFileType();

	virtual int ReadVersion();
	virtual int ReadHeader();
public:
	CBinaryDosageVersion();

	virtual int Open(const std::string &_filename);
  unsigned int Version() const { return m_version; }
  unsigned int Revision() const { return m_revision; }
};

class CReadBinaryDosageFile4 : public CBinaryDosageVersion {
protected:
	unsigned int m_subjectOptions;
	unsigned int m_numSubjects;
	unsigned int m_startSubjects;
	std::vector<CSubject> m_subject;
	std::vector<CFamilySubject> m_familySubject;
	std::vector<unsigned int> m_SNPDataSize;

	unsigned int m_SNPOptions;
	unsigned int m_numSNPs;
	unsigned int m_startSNPs;
  unsigned int m_currentSNP;
	std::vector<CSNP> m_SNP;
	std::vector<std::streampos> m_snpIndex;

	unsigned int m_numGroups;
	unsigned int m_startGroups;
	std::vector <unsigned int> m_groupSize;

	unsigned int m_startDosages;
	std::vector<short> m_sdosages;
	std::vector<double> m_dosages;
	std::vector<double> m_p0;
	std::vector<double> m_p1;
	std::vector<double> m_p2;

	int FileAlreadyOpenError();

	virtual int CheckVersion();

	void ClearVectors();
	virtual void AssignVectors();

	int ReadStringArray(std::vector<std::string> &_dest, const unsigned int _readSize);
	int ReadDoubleArray(std::vector<double> &_dest);

	void AssignSubjects(const std::vector<std::string> &_iid);
	void AssignFamilySubjects(const std::vector<std::string> &_fid, const std::vector<std::string> &_iid);
	void AssignSNPs(const std::vector<std::string> &_snpName, const std::vector<std::string> &_chromosome, const std::vector<unsigned int> &_location,
	const std::vector<std::string> &_refAllele, const std::vector<std::string> &_altAllele);

	int ReadGroups();
	int ReadSubjects(const bool _subjectOnly = false);
	int ReadSNPs();
	int ReadImputationInfo();
	int ReadFileInfo();

	int GoToFirst();

	virtual int ReadHeader();
	virtual int IndexSNPs() { return 0; }
  virtual int ReadSNP() = 0;
  virtual int SkipSNP() = 0;
public:
	CReadBinaryDosageFile4();
	virtual ~CReadBinaryDosageFile4() {}

	int GetSubjects(const std::string &_filename);
	int GetFirst(const bool _process = true);
	int GetNext(const bool _process = true);
	int GetSNP(const unsigned int _n);
	
	unsigned int FindSNPbyName(std::vector<unsigned int> &_index, const std::string &_name);
	unsigned int FindSNPbyLocation(std::vector<unsigned int> &_index, const std::string &_chromosome, const unsigned int _location);
	unsigned int FindSNP(std::vector<unsigned int> &_index, const std::string &_name, const std::string &_chromosome, const unsigned int _location, const std::string &_refAllele, const std::string &_altAllele);
	
	unsigned int FindSubjects(std::vector<unsigned int> &_index, const std::vector<std::string> &_iidv);
	unsigned int FindSubjects(std::vector<unsigned int> &_index, const std::vector<std::string> &_fidv, const std::vector<std::string> &_iidv);
	
	unsigned int SubjectOptions() const { return m_subjectOptions; }
	unsigned int NumSubjects() const { return m_numSubjects; }
	unsigned int SNPOptions() const { return m_SNPOptions; }
	unsigned int NumSNPs() const { return m_numSNPs; }
	unsigned int NumGroups() const { return m_numGroups; }

	const std::vector<CSubject> &Subject() const { return m_subject;  }
	const std::vector<CFamilySubject> &FamilySubject() const { return m_familySubject; }
	const std::vector<CSNP> &SNP() const { return m_SNP; }
	const CSNP &CurrentSNP() const { return m_SNP[m_currentSNP]; }
	const std::vector<std::streampos> &SNPIndex() const { return m_snpIndex; }
	const std::vector<unsigned int> &GroupSize() const { return m_groupSize; }
	const std::vector<short> &SDosages() const { return m_sdosages; }
	const std::vector<double> &Dosages() const { return m_dosages; }
	const std::vector<double> &P0() const { return m_p0; }
	const std::vector<double> &P1() const { return m_p1; }
	const std::vector<double> &P2() const { return m_p2; }
};

class CReadBinaryDosageFile41 : public CReadBinaryDosageFile4 {
protected:
  virtual int CheckVersion();
  virtual int ReadSNP();
  virtual int SkipSNP();
public:
  CReadBinaryDosageFile41() : CReadBinaryDosageFile4() {}
  virtual ~CReadBinaryDosageFile41() {}
};

class CReadBinaryDosageFile42 : public CReadBinaryDosageFile4 {
protected:
  virtual int CheckVersion();
  
  virtual void AssignVectors();
  virtual int IndexSNPs();
  virtual int ReadSNP();
  virtual int SkipSNP();
public:
  CReadBinaryDosageFile42() : CReadBinaryDosageFile4() {}
  virtual ~CReadBinaryDosageFile42() {}
};

#endif
