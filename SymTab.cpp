//
// Created 2/5/19
//

#include <iostream>
#include "SymTab.hpp"

SymTab::SymTab(){
    std::map<std::string, std::shared_ptr<TypeDescriptor> > newSymTab;
    scopeStack.push_back(newSymTab);
    _currentScope = 0;
}

void SymTab::setReturnValue(std::shared_ptr<TypeDescriptor> td){
    _returnValue = td;
}

std::shared_ptr<TypeDescriptor> SymTab::getReturnValue(){
    std::shared_ptr<TypeDescriptor> return_ptr = _returnValue;
    _returnValue = nullptr;
    return return_ptr;
}

// push a new symbol table onto the stack and change the pointer
void SymTab::openNewScope(std::vector<std::string> parameterList){
    // this is the map we will push onto the symbol table's scope stack
    std::map<std::string, std::shared_ptr<TypeDescriptor> > newSymTab;
    // this copies the arguements, which are defined in the current scope, into the map that will become the new scope
    for(int i = 0; i < parameterList.size(); i++){
        std::string val_name = parameterList[i];
        newSymTab[val_name] = scopeStack[_currentScope].find(val_name)->second;
    }
    scopeStack.push_back(newSymTab);
    _currentScope++;
}

// should have an optional parameter for a return value?
void SymTab::closeScope(){
    scopeStack.pop_back();
    _currentScope = scopeStack.size() - 1;
}

// used by assignment and function definition statements
void SymTab::setValueFor(std::string varName, std::shared_ptr<TypeDescriptor> typeDescriptorPtr) {
    // Define a variable by setting its initial value.
    std::cout << "Setting value for " << varName << ": ";
    if(typeDescriptorPtr->type() == TypeDescriptor::INTEGER){
        //std::cout << "setValueFor detected an " << typeDescriptorPtr->type() << std::endl;
        //std::shared_ptr<NumberTypeDescriptor> nbr_ptr = std::dynamic_pointer_cast<NumberTypeDescriptor>(typeDescriptorPtr);
        std::shared_ptr<NumberTypeDescriptor> nbr_ptr = std::dynamic_pointer_cast<NumberTypeDescriptor>(typeDescriptorPtr);
        if(nbr_ptr){
            std::cout << nbr_ptr->value.intValue << std::endl;
            //std::shared_ptr<NumberTypeDescriptor> new_ptr = new NumberTypeDescriptor(TypeDescriptor::INTEGER);
            std::shared_ptr<NumberTypeDescriptor> new_ptr;
            new_ptr = std::make_shared<NumberTypeDescriptor>(TypeDescriptor::INTEGER);

            new_ptr->value.intValue = nbr_ptr->value.intValue;
            //symTab[varName] = new_ptr;
            scopeStack[_currentScope][varName] = new_ptr;

        }
    }
    else if(typeDescriptorPtr->type() == TypeDescriptor::STRING){
        //std::shared_ptr<StringTypeDescriptor> str_ptr = std::dynamic_pointer_cast<StringTypeDescriptor>(typeDescriptorPtr);
        std::shared_ptr<StringTypeDescriptor> str_ptr = std::dynamic_pointer_cast<StringTypeDescriptor>(typeDescriptorPtr);
        if(str_ptr){
            std::cout << "... " << str_ptr->stringValue << std::endl;
            std::shared_ptr<StringTypeDescriptor> new_ptr;
            new_ptr = std::make_shared<StringTypeDescriptor>(TypeDescriptor::STRING);
            new_ptr->stringValue = str_ptr->stringValue;
            //symTab[varName] = new_ptr;
            scopeStack[_currentScope][varName] = new_ptr;
        }
    }
    else if(typeDescriptorPtr->type() == TypeDescriptor::NUMBER_ARRAY){
        //std::shared_ptr<StringTypeDescriptor> str_ptr = std::dynamic_pointer_cast<StringTypeDescriptor>(typeDescriptorPtr);
        std::shared_ptr<NumberArrayTypeDescriptor> nbr_ptr = std::dynamic_pointer_cast<NumberArrayTypeDescriptor>(typeDescriptorPtr);
        if(nbr_ptr){

            std::cout << "... vector of size :" << nbr_ptr->intVector.size() << std::endl;

            std::shared_ptr<NumberArrayTypeDescriptor> new_ptr;
            new_ptr = std::make_shared<NumberArrayTypeDescriptor>(TypeDescriptor::NUMBER_ARRAY);
            new_ptr->intVector = nbr_ptr->intVector;
            //symTab[varName] = new_ptr;
            scopeStack[_currentScope][varName] = new_ptr;
        }
    }
    else if(typeDescriptorPtr->type() == TypeDescriptor::STRING_ARRAY){
        //std::shared_ptr<StringTypeDescriptor> str_ptr = std::dynamic_pointer_cast<StringTypeDescriptor>(typeDescriptorPtr);
        std::shared_ptr<StringArrayTypeDescriptor> str_ptr = std::dynamic_pointer_cast<StringArrayTypeDescriptor>(typeDescriptorPtr);
        if(str_ptr){
            std::cout << "... vector of size: " << str_ptr->stringVector.size() << std::endl;
            std::shared_ptr<StringArrayTypeDescriptor> new_ptr;
            new_ptr = std::make_shared<StringArrayTypeDescriptor>(TypeDescriptor::STRING_ARRAY);
            new_ptr->stringVector = str_ptr->stringVector;
            //symTab[varName] = new_ptr;
            scopeStack[_currentScope][varName] = new_ptr;
        }
    }
}

bool SymTab::isDefined(std::string vName) {
    //return symTab.find(vName) != symTab.end();
    return scopeStack[_currentScope].find(vName) != scopeStack[_currentScope].end();
}

std::shared_ptr<TypeDescriptor> SymTab::getValueFor(std::string vName) {
    if( ! isDefined(vName)) {
        std::cout << "SymTab::getValueFor: " << vName << " has not been defined.\n";
        exit(1);
    }

    //std::shared_ptr<TypeDescriptor> ptr = symTab.find(vName)->second;
    std::shared_ptr<TypeDescriptor> ptr = scopeStack[_currentScope].find(vName)->second;

    if(ptr->type() == TypeDescriptor::INTEGER){
        std::shared_ptr<NumberTypeDescriptor> nm_ptr;
        nm_ptr = std::dynamic_pointer_cast<NumberTypeDescriptor>(ptr);
        if(nm_ptr){
            std::cout << "SymTab::getValueFor: " << vName << " contains " << nm_ptr->value.intValue <<".\n" << std::endl;
            return nm_ptr;
        }
    }
    else if(ptr->type() == TypeDescriptor::STRING){
        std::shared_ptr<StringTypeDescriptor> str_ptr;
        str_ptr = std::dynamic_pointer_cast<StringTypeDescriptor>(ptr);
        if(str_ptr){
            std::cout << "SymTab::getValueFor: " << vName << " contains " << str_ptr->stringValue <<".\n" << std::endl;
            return str_ptr;
        }
    }
    else if(ptr->type() == TypeDescriptor::NUMBER_ARRAY){
        std::shared_ptr<NumberArrayTypeDescriptor> num_ary_td;
        num_ary_td = std::dynamic_pointer_cast<NumberArrayTypeDescriptor>(ptr);
        if(num_ary_td){
            std::cout << "SymTab::getValueFor: " << vName << " contains an integer array of size " << num_ary_td->intVector.size() <<".\n" << std::endl;
            return num_ary_td;
        }
    }
    else if(ptr->type() == TypeDescriptor::STRING_ARRAY){
    std::shared_ptr<StringArrayTypeDescriptor> str_ary_td;
    str_ary_td = std::dynamic_pointer_cast<StringArrayTypeDescriptor>(ptr);
    if(str_ary_td){
        std::cout << "SymTab::getValueFor: " << vName << " contains an string array of size " << str_ary_td->stringVector.size() <<".\n" << std::endl;
        return str_ary_td;
    }
    }
    else{
        std::cout << "Problem with SymTab::getValueFor()\n";
    }
}

void printValue(std::shared_ptr<TypeDescriptor>desc) {

    // Use std::dynamic_pointer_cast to down-cast, from TypeDescriptor to 
    // NumberDescriptr. 
    std::shared_ptr<NumberTypeDescriptor> nDesc = std::dynamic_pointer_cast<NumberTypeDescriptor>(desc);
    // std::dynamic_pointer_cast will return a nullptr if
    // desc is not of datatype NumberDescritpr.
    if( nDesc ){
        if( nDesc->type() == TypeDescriptor::INTEGER ){
            std::cout << nDesc->value.intValue;
            return;
        }
        else if( nDesc->type() == TypeDescriptor::BOOL ){
            std::cout << nDesc->value.boolValue;
            return;
        }
        else{
            std::cout << "Misconfigured union type.";
            return;
        }
    }

    std::shared_ptr<StringTypeDescriptor> sDesc = std::dynamic_pointer_cast<StringTypeDescriptor>(desc);

    if( sDesc )
        if ( sDesc->type() == TypeDescriptor::STRING){
            std::cout << sDesc->stringValue;
            return;
        }
    else {
        std::cout << "Misconfigured string type.";
    }

    std::cout << "Problem with SymTab::Print() function.";
}

/*

// function class
Function::Function() : _paramList{{}}, _blockStatements{nullptr} {};
Function::Function(std::vector<std::string> params, Statements* blockStatements) : _paramList{params}, _blockStatements{blockStatements} {};

// functions
void Functions::addFunction(std::string funcName, std::shared_ptr<Function> funcPtr){
    funcTab[funcName] = funcPtr;
}

bool Functions::isDefined(std::string funcName){
    return funcTab.find(funcName) != funcTab.end();
}

std::shared_ptr<Function> Functions::getFunction(std::string funcName){
    std::shared_ptr<Function> ptr = funcTab.find(funcName)->second;
    return ptr;
}

*/
