#ifndef SUBJECT_H
#define SUBJECT_H 1

class CSubject {
protected:
	std::string m_iid;

	void print(std::ostream &_stream) const { _stream << m_iid; }
public:
    CSubject() { m_iid = ""; }
    CSubject(const CSubject &_src) { m_iid = _src.m_iid; }
    CSubject(const std::string &_iid) { m_iid = _iid; }
	~CSubject() {}

	CSubject &operator=(const CSubject &_rhs);
	bool operator==(const CSubject &_rhs) const;
    bool operator==(const std::string &_rhs) const;

	CSubject &Assign(const std::string &_iid) {
		m_iid = _iid;
		return *this;
	}
	void SubjectID(const std::string &_iid) { m_iid = _iid; }
	const std::string &SubjectID() const { return m_iid; }

	friend std::ostream &operator<<(std::ostream &_stream, const CSubject &_rhs) {
		_rhs.print(_stream);
		return _stream;
	}
};

class CFamily {
protected:
    std::string m_fid;
    
    void print(std::ostream &_stream) const { _stream << m_fid; }
public:
    CFamily() { m_fid = ""; }
    CFamily(const CFamily &_src) { m_fid = _src.m_fid; }
    CFamily(const std::string &_fid) { m_fid = _fid; }
    ~CFamily() {}
    
    CFamily &operator=(const CFamily &_rhs);
    bool operator==(const CFamily &_rhs) const;
    
	CFamily &Assign(const std::string &_fid) {
		m_fid = _fid;
		return *this;
	}
    void FamilyID(const std::string &_fid) { m_fid = _fid; }
    const std::string &FamilyID() const { return m_fid; }
    
    friend std::ostream &operator<<(std::ostream &_stream, const CFamily &_rhs) {
        _rhs.print(_stream);
        return _stream;
    }
};

class CGroup {
protected:
    std::string m_gpid;
    
    void print(std::ostream &_stream) const { _stream << m_gpid; }
public:
    CGroup() { m_gpid = ""; }
    CGroup(const CGroup &_src) { m_gpid = _src.m_gpid; }
    CGroup(const std::string &_gpid) { m_gpid = _gpid; }
    ~CGroup() {}
    
    CGroup &operator=(const CGroup &_rhs);
    bool operator==(const CGroup &_rhs) const;
    
	CGroup &Assign(const std::string &_gpid) {
		m_gpid = _gpid;
		return *this;
	}
    void FamilyID(const std::string &_gpid) { m_gpid = _gpid; }
    
    const std::string &GroupID() const { return m_gpid; }
    
    friend std::ostream &operator<<(std::ostream &_stream, const CGroup &_rhs) {
        _rhs.print(_stream);
        return _stream;
    }
};

class CFamilySubject : public CFamily, public CSubject {
protected:
    void print(std::ostream &_stream) const {
		CFamily::print(_stream);
		_stream << '\t';
		CSubject::print(_stream);
	}
public:
    CFamilySubject() : CFamily(), CSubject() {}
    CFamilySubject(const CFamilySubject &_src) : CFamily(_src), CSubject(_src) {}
    CFamilySubject(const CFamily &_src1, const CSubject &_src2) : CFamily(_src1), CSubject(_src2) {}
    CFamilySubject(const std::string &_fid, const CSubject &_src) : CSubject(_src) {
        m_fid = _fid;
    }
    CFamilySubject(const std::string &_fid, const std::string &_iid) : CFamily(_fid), CSubject(_iid) {}
    ~CFamilySubject() {}
 
    CFamilySubject &operator=(const CFamilySubject &_rhs);
	bool operator==(const CFamilySubject &_rhs) const;
	bool operator==(const CSubject &_rhs) const { return CSubject::operator==(_rhs); }
	bool operator==(const CFamily &_rhs) const { return CFamily::operator==(_rhs); }

	CFamilySubject &Assign(const std::string &_fid, const std::string &_iid) {
        CFamily::Assign(_fid);
        CSubject::Assign(_iid);
		return *this;
    }
	CFamilySubject &Assign(const std::string &_fid, const CSubject &_src2) {
        CFamily::Assign(_fid);
        CSubject::operator=(_src2);
		return *this;
    }
	CFamilySubject &Assign(const CFamily &_src1, const CSubject &_src2) {
        CFamily::operator=(_src1);
        CSubject::operator=(_src2);
		return *this;
    }
    
    friend std::ostream &operator<<(std::ostream &_stream, const CFamilySubject &_rhs) {
        _rhs.print(_stream);
        return _stream;
    }
};

class CGroupSubject : public CGroup, public CSubject {
protected:
    void print(std::ostream &_stream) const {
        CGroup::print(_stream);
        _stream << '\t';
        CSubject::print(_stream);
    }
public:
    CGroupSubject() : CGroup(), CSubject() {}
    CGroupSubject(const CGroupSubject &_src) : CGroup(_src), CSubject(_src) {}
    CGroupSubject(const CGroup &_src1, const CSubject &_src2) : CGroup(_src1), CSubject(_src2) {}
    CGroupSubject(const std::string &_gpid, const CSubject &_src) : CSubject(_src) {
        m_gpid = _gpid;
    }
    CGroupSubject(const std::string &_gpid, const std::string &_iid) : CGroup(_gpid), CSubject(_iid) {}
    ~CGroupSubject() {}
    
    CGroupSubject &operator=(const CGroupSubject &_rhs);
    bool operator==(const CGroupSubject &_rhs) const;
    bool operator==(const CSubject &_rhs) const { return CSubject::operator==(_rhs); }
    bool operator==(const CGroup &_rhs) const { return CGroup::operator==(_rhs); }
    
	CGroupSubject &Assign(const std::string &_fid, const std::string &_iid) {
        CGroup::Assign(_fid);
        CSubject::Assign(_iid);
		return *this;
    }
	CGroupSubject &Assign(const std::string &_fid, const CSubject &_src2) {
        CGroup::Assign(_fid);
        CSubject::operator=(_src2);
		return *this;
    }
	CGroupSubject &Assign(const CGroup &_src1, const CSubject &_src2) {
        CGroup::operator=(_src1);
        CSubject::operator=(_src2);
		return *this;
    }
    
    friend std::ostream &operator<<(std::ostream &_stream, const CGroupSubject &_rhs) {
        _rhs.print(_stream);
        return _stream;
    }
};

class CGroupFamilySubject : public CGroup, public CFamilySubject {
protected:
    void print(std::ostream &_stream) const {
        CGroup::print(_stream);
        _stream << '\t';
        CFamilySubject::print(_stream);
    }
public:
    CGroupFamilySubject() : CGroup(), CFamilySubject() {}
    CGroupFamilySubject(const CGroupFamilySubject &_src) : CGroup(_src), CFamilySubject(_src) {}
    CGroupFamilySubject(const CGroup &_src1, const CFamily &_src2, const CSubject &_src3) : CGroup(_src1),CFamilySubject(_src2, _src3) {}
    CGroupFamilySubject(const std::string &_gpid, const CFamilySubject &_src) : CFamilySubject(_src) {
        m_gpid = _gpid;
    }
    CGroupFamilySubject(const std::string &_gpid, const std::string &_fid, const std::string &_iid) : CGroup(_gpid), CFamilySubject(_fid, _iid) {}
    ~CGroupFamilySubject() {}
    
    CGroupFamilySubject &operator=(const CGroupFamilySubject &_rhs);
	bool operator==(const CGroupFamilySubject &_rhs) const;
	bool operator==(const CFamilySubject &_rhs) const { return CFamilySubject::operator==(_rhs); }
	bool operator==(const CGroup &_rhs) const { return CGroup::operator==(_rhs); }

	CGroupFamilySubject &Assign(const std::string &_gpid, const std::string &_fid, const std::string &_iid) {
        CGroup::Assign(_gpid);
        CFamilySubject::Assign(_fid, _iid);
		return *this;
    }
	CGroupFamilySubject &Assign(const std::string &_gpid, const std::string &_fid, const CSubject &_src) {
        CGroup::Assign(_gpid);
        CFamilySubject::Assign(_fid, _src);
		return *this;
	}
	CGroupFamilySubject &Assign(const std::string &_gpid, const CFamilySubject &_src) {
        CGroup::Assign(_gpid);
        CFamilySubject::operator=(_src);
		return *this;
	}
	CGroupFamilySubject &Assign(const CGroup &_src1, const CFamily &_src2, const CSubject &_src3) {
        CGroup::operator=(_src1);
		CFamily::operator=(_src2);
        CSubject::operator=(_src3);
		return *this;
	}
    
    friend std::ostream &operator<<(std::ostream &_stream, const CGroupFamilySubject &_rhs) {
        _rhs.print(_stream);
        return _stream;
    }
};

/*
class CFamilySubject : public CSubject {
protected:
	std::string m_family;
	void print(std::ostream &_stream) const {
		CSubject::print(_stream);
		_stream << '\t' << m_family;
	}
public:
	CFamilySubject();
	CFamilySubject(const CFamilySubject &_src);
	CFamilySubject(const std::string &m_id, const std::string &_family);
	~CFamilySubject() {}

	CFamilySubject &operator=(const CFamilySubject &_rhs);
	bool operator==(const CFamilySubject &_rhs) const;

	void Assign(const std::string &_id, const std::string &_family);
	void Family(const std::string &_family) { m_family = _family; }

	const std::string &Family() const { return m_family; }
};
 */
#endif
