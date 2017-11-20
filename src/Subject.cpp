#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "Subject.h"

CSubject &CSubject::operator=(const CSubject &_rhs) {
	if (this == &_rhs)
		return *this;
	m_iid = _rhs.m_iid;
	return *this;
}

bool CSubject::operator==(const CSubject &_rhs) const {
	if (this == &_rhs)
		return true;
	return (m_iid == _rhs.m_iid);
}

bool CSubject::operator==(const std::string &_rhs) const {
    return (m_iid == _rhs);
}

CFamily &CFamily::operator=(const CFamily &_rhs) {
    if (this == &_rhs)
        return *this;
    m_fid = _rhs.m_fid;
    return *this;
}

bool CFamily::operator==(const CFamily &_rhs) const {
    if (this == &_rhs)
        return true;
    return (m_fid == _rhs.m_fid);
}

CGroup &CGroup::operator=(const CGroup &_rhs) {
    if (this == &_rhs)
        return *this;
    m_gpid = _rhs.m_gpid;
    return *this;
}

bool CGroup::operator==(const CGroup &_rhs) const {
    if (this == &_rhs)
        return true;
    return (m_gpid == _rhs.m_gpid);
}

CFamilySubject &CFamilySubject::operator=(const CFamilySubject &_rhs) {
    CFamily::operator=(_rhs);
    CSubject::operator=(_rhs);
    return *this;
}

bool CFamilySubject::operator==(const CFamilySubject &_rhs) const {
	if (this == &_rhs)
		return true;
	return (CSubject::operator==(_rhs) && CFamily::operator==(_rhs));
}

CGroupSubject &CGroupSubject::operator=(const CGroupSubject &_rhs) {
    CGroup::operator=(_rhs);
    CSubject::operator=(_rhs);
    return *this;
}

bool CGroupSubject::operator==(const CGroupSubject &_rhs) const {
    if (this == &_rhs)
        return true;
    return (CSubject::operator==(_rhs) && CGroup::operator==(_rhs));
}

CGroupFamilySubject &CGroupFamilySubject::operator=(const CGroupFamilySubject &_rhs) {
    CGroup::operator=(_rhs);
    CFamilySubject::operator=(_rhs);
    return *this;
}

bool CGroupFamilySubject::operator==(const CGroupFamilySubject &_rhs) const {
	if (this == &_rhs)
		return true;
	return (CSubject::operator==(_rhs) && CFamily::operator==(_rhs) && CGroup::operator==(_rhs));
}

/*
CFamilySubject::CFamilySubject() : CSubject() {
	m_family = "";
}

CFamilySubject::CFamilySubject(const CFamilySubject &_src) : CSubject(_src) {
	m_family = _src.m_family;
}

CFamilySubject::CFamilySubject(const std::string &_id, const std::string &_family) : CSubject(_id) {
	m_family = _family;
}

CFamilySubject &CFamilySubject::operator=(const CFamilySubject &_rhs) {
	if (this == &_rhs)
		return *this;
	CSubject::operator=(_rhs);
	m_family = _rhs.m_family;
	return *this;
}

bool CFamilySubject::operator==(const CFamilySubject &_rhs) const {
	if (this == &_rhs)
		return true;
	return (CSubject::operator==(_rhs) && m_family == _rhs.m_family);
}
*/
