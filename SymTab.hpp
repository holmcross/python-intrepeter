//
// Created 2/5/19
//

#ifndef EXPRINTER_SYMTAB_HPP
#define EXPRINTER_SYMTAB_HPP

#include <string>
#include <map>
#include <vector>
#include "Token.hpp"

//class Statements ; // forward declaration for the FunctionBlockStatements in class Function

// This is a flat and integer-based symbol table. It allows for variables to be
// initialized, determines if a give variable has been defined or not, and if
// a variable has been defined, it returns its value.




struct TypeDescriptor {
    // The base class for datatype descriptors.

    // This enum can expand to include additional types.
    enum types {INTEGER, BOOL, STRING, NUMBER_ARRAY, STRING_ARRAY, EMPTY_ARRAY};  

    TypeDescriptor(types type): _type{type} {}

    types &type() { return _type; }

    virtual ~TypeDescriptor() {}

private:
    types _type;
};

struct NumberTypeDescriptor: public TypeDescriptor {
    // Number descriptor includes integer, double, and boolean datatypes.

    NumberTypeDescriptor(types descType):  TypeDescriptor(descType) {}

    union {
	    int intValue;
        //float floatValue;
	    //double doubleValue;
	    short boolValue;
    } value;
};

struct StringTypeDescriptor: public TypeDescriptor {
    // String descriptor includes string datatypes

    StringTypeDescriptor(types descType):  TypeDescriptor(descType) {}
    std::string stringValue;
};

struct NumberArrayTypeDescriptor: public TypeDescriptor {
public:
    NumberArrayTypeDescriptor(types descType):  TypeDescriptor(descType) {}
    void pushValue(int val)  { intVector.push_back(val); }
    void setValue(std::vector<int> vect)  {intVector = vect;}
    std::vector<int> intVector;
};

struct StringArrayTypeDescriptor: public TypeDescriptor {
public:
    StringArrayTypeDescriptor(types descType):  TypeDescriptor(descType) {}
    void pushValue(std::string val)  { stringVector.push_back(val); }
    void setValue(std::vector<std::string> vect)  {stringVector = vect;}
    std::vector<std::string> stringVector;
};

struct EmptyArrayTypeDescriptor: public TypeDescriptor {
    EmptyArrayTypeDescriptor(types descType):  TypeDescriptor(descType) {}
};

class SymTab {
public:
    SymTab();

    void setValueFor(std::string vName, std::shared_ptr<TypeDescriptor>);

    bool isDefined(std::string vName);
    std::shared_ptr<TypeDescriptor> getValueFor(std::string vName);
    //std::shared_ptr<TypeDescriptor> getReturnValue() const {return _returnValue;}
    bool hasReturnValue() const         {return _returnValue != nullptr; }
    std::shared_ptr<TypeDescriptor> getReturnValue();
    void setReturnValue(std::shared_ptr<TypeDescriptor>);


    int currentScope() const {return _currentScope;}
    void openNewScope(std::vector<std::string>);
    void closeScope();

private:
    // returning functions place thier value in here
    std::shared_ptr<TypeDescriptor> _returnValue;
    // this is a stack of symbol tables
    // index 0 contains the gobal scope
    std::vector< std::map<std::string, std::shared_ptr<TypeDescriptor> > > scopeStack;
    int _currentScope;
};

void printValue(std::shared_ptr<TypeDescriptor>);

/*
class Function {
public:
    Function();
    Function(std::vector<std::string> params, Statements* blockStatements);
    Statements*& getStatements()             {return _blockStatements;}
    std::vector<std::string> getParams()    {return _paramList;}
private:
    // parameter list
    std::vector<std::string> _paramList;
    // code block
    Statements* _blockStatements;
};



class Functions {
public:
    void addFunction(std::string funcName, std::shared_ptr<Function>);
    bool isDefined(std::string funcName);
    std::shared_ptr<Function> getFunction(std::string funcName);


private:
    std::map<std::string, std::shared_ptr<Function> > funcTab;
};
*/

#endif //EXPRINTER_SYMTAB_HPP
