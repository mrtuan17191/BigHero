#ifndef _UG_CUSTOM_MACROS_H_
#define _UG_CUSTOM_MACROS_H_

#define UG_PROPERTY_TEMPLATE(varType, varName, funName)\
protected: varType varName;\
public: virtual varType get##funName(void);\
public: virtual void set##funName(varType var);

#define UG_PROPERTY_TEMPLATE_REF(varType, varName, funName)\
protected: varType varName;\
public: virtual const varType& get##funName(void);\
public: virtual void set##funName(const varType& var);

#define UG_PROPERTY_TEMPLATE_GETTER(varType, varName, funName)\
protected: varType varName;\
public: virtual varType get##funName(void);

#define UG_PROPERTY_TEMPLATE_GETTER_REFF(varType, varName, funName)\
protected: varType varName;\
public: virtual const varType& get##funName(void);

#define UG_PROPERTY_GETTER(varType, varName, funName)\
protected: varType varName;\
public: virtual varType get##funName(void) const { return varName; }

#define UG_PROPERTY_GETTER_REFF(varType, varName, funName)\
protected: varType varName;\
public: virtual const varType& get##funName(void) const { return varName; }

#define UG_PROPERTY_FULL(varType, varName, funName)\
protected: varType varName;\
public: virtual varType get##funName(void) const { return varName; }\
public: virtual void set##funName(varType var){ varName = var; }

#define UG_PROPERTY_FULL_REFF(varType, varName, funName)\
protected: varType varName;\
public: virtual const varType& get##funName(void) const { return varName; }\
public: virtual void set##funName(const varType& var){ varName = var; }

#define UG_PROPERTY_FULL_BOOL(varName, funName)\
protected: bool varName;\
public: virtual bool is##funName(void) const { return varName; }\
public: virtual void set##funName(bool var){ varName = var; }

#endif
