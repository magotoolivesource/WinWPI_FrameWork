#pragma once


#define SAFEDELETE(x) if (x) { delete x; x = nullptr; }
#define SAFEARRAYDELETE(x) if (x) { delete[] x; x = nullptr; }
#define SAFEDELETEFOR(x) if (x) { for (auto& item : x) { delete item; } x.clear(); }


#define PI 3.14159265f
#define DEG2RAD 0.0174532925f //0.0174533f
#define RAD2DEG 57.2957795785523f

//#define GETTER(varType, varName) \
//    varType get##funcName() const { \
//        return varName; \
//    }

//#define SETTER(varType, varName) \
//    void set##funcName(varType value) { \
//        varName = value; \
//    }

//#define GETSETER(varType, varName) \
//    GETTER(varType, varName) \
//    SETTER(varType, varName)


#define GETTER(varType, varName, defaultval) \
protected:\
varType m_##varName = defaultval;\
public:\
inline varType Get##varName( ) { return m_##varName; }

#define SETTER(varType, varName, defaultval) \
protected:\
varType m_##varName = defaultval;\
public:\
inline void Set##varName(varType p_val) { m_##varName = p_val; }


#define GETSETER(varType, varName, defaultval) \
protected:\
varType m_##varName = defaultval;\
public:\
inline varType Get##varName( ) { return m_##varName; }\
inline void Set##varName(varType p_val) { m_##varName = p_val; }

