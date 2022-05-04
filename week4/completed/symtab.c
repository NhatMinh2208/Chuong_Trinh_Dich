/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"

void freeObject(Object* obj);
void freeScope(Scope* scope);
void freeObjectList(ObjectNode *objList);
void freeReferenceList(ObjectNode *objList);

SymTab* symtab;
Type* intType;
Type* charType;

/******************* Type utilities ******************************/

Type* makeIntType(void) {
  Type* type = (Type*) malloc(sizeof(Type));
  type->typeClass = TP_INT;
  return type;
}

Type* makeCharType(void) {
  Type* type = (Type*) malloc(sizeof(Type));
  type->typeClass = TP_CHAR;
  return type;
}

Type* makeArrayType(int arraySize, Type* elementType) {
  Type* type = (Type*) malloc(sizeof(Type));
  type->typeClass = TP_ARRAY;
  type->arraySize = arraySize;
  type->elementType = elementType;
  return type;
}

Type* duplicateType(Type* type) {
  // TODO
  Type* type1 = (Type*) malloc(sizeof(Type));
  type1->typeClass = type->typeClass;
  type1->arraySize = type->arraySize;
  type1->elementType = type->elementType;
  return type1;
}

int compareType(Type* type1, Type* type2) {
  // TODO
  if(type1->typeClass == type2->typeClass){
    if(type1->typeClass == TP_ARRAY){
      if(type1->arraySize == type2->arraySize && compareType(type1->elementType,type2->elementType)){
        return 1;
      }
      return 0;
    }
    return 1;
  }
  return 0;
}

void freeType(Type* type) {
  // TODO
  if(type->typeClass == TP_ARRAY){
    freeType(type->elementType);
    free(type);
  }
  else free(type);
}

/******************* Constant utility ******************************/

ConstantValue* makeIntConstant(int i) {
  // TODO
  ConstantValue* constant = (ConstantValue*)malloc(sizeof(ConstantValue));
  constant->type = TP_INT;
  constant->intValue = i;
  return constant;
}

ConstantValue* makeCharConstant(char ch) {
  // TODO
  ConstantValue* constant = (ConstantValue*)malloc(sizeof(ConstantValue));
  constant->type = TP_CHAR;
  constant->charValue = ch;
  return constant;
}

ConstantValue* duplicateConstantValue(ConstantValue* v) {
  // TODO
  ConstantValue* constant = (ConstantValue*)malloc(sizeof(ConstantValue));
  constant->type = v->type;
  if(v->type == TP_INT){
    constant->intValue = v->intValue;
  }
  if(v->type == TP_CHAR){
    constant->charValue = v->charValue;
  }
  return constant;
}

/******************* Object utilities ******************************/

Scope* createScope(Object* owner, Scope* outer) {
  Scope* scope = (Scope*) malloc(sizeof(Scope));
  scope->objList = NULL;
  scope->owner = owner;
  scope->outer = outer;
  return scope;
}

Object* createProgramObject(char *programName) {
  Object* program = (Object*) malloc(sizeof(Object));
  strcpy(program->name, programName);
  program->kind = OBJ_PROGRAM;
  program->progAttrs = (ProgramAttributes*) malloc(sizeof(ProgramAttributes));
  program->progAttrs->scope = createScope(program,NULL);
  symtab->program = program;

  return program;
}

Object* createConstantObject(char *name) {
  // TODO
  Object* constant = (Object*) malloc(sizeof(Object));
  strcpy(constant->name,name);
  constant->kind = OBJ_CONSTANT;
  constant->constAttrs = (ConstantAttributes*) malloc(sizeof(ConstantAttributes));
  return constant;
}

Object* createTypeObject(char *name) {
  // TODO
  Object* type = (Object*) malloc(sizeof(Object));
  strcpy(type->name,name);
  type->kind = OBJ_TYPE;
  type->typeAttrs = (TypeAttributes*) malloc(sizeof(TypeAttributes));
  return type;
}

Object* createVariableObject(char *name) {
  // TODO
  Object* var = (Object*) malloc(sizeof(Object));
  strcpy(var->name,name);
  var->kind = OBJ_VARIABLE;
  var->varAttrs = (VariableAttributes*) malloc(sizeof(VariableAttributes));
  var->varAttrs->type = NULL;
  var->varAttrs->scope = createScope(var,symtab->currentScope);
  return var;
}

Object* createFunctionObject(char *name) {
  // TODO
  Object* f = (Object*) malloc(sizeof(Object));
  strcpy(f->name,name);
  f->kind = OBJ_FUNCTION;
  f->funcAttrs = (FunctionAttributes*) malloc(sizeof(FunctionAttributes));
  f->funcAttrs->paramList = NULL;
  f->funcAttrs->returnType = NULL;
  f->funcAttrs->scope = createScope(f,symtab->currentScope);
  return f;
}

Object* createProcedureObject(char *name) {
  // TODO
  Object* proc = (Object*) malloc(sizeof(Object));
  strcpy(proc->name,name);
  proc->kind = OBJ_PROCEDURE;
  proc->procAttrs = (ProcedureAttributes*) malloc(sizeof(ProcedureAttributes));
  proc->procAttrs->paramList = NULL;
  proc->procAttrs->scope = createScope(proc,symtab->currentScope);
  return proc;
}

Object* createParameterObject(char *name, enum ParamKind kind, Object* owner) {
  // TODO
  Object* para = (Object*) malloc(sizeof(Object));
  strcpy(para->name,name);
  para->kind = OBJ_PARAMETER;
  para->paramAttrs = (ParameterAttributes*) malloc(sizeof(ParameterAttributes));
  para->paramAttrs->kind = kind;
  para->paramAttrs->function = owner;
  return para;
}

void freeObject(Object* obj) {
  // TODO
  switch (obj->kind)
  {
  case OBJ_PROGRAM:
    freeScope(obj->progAttrs->scope);
    free(obj->progAttrs);
    free(obj);
    break;
  case OBJ_CONSTANT:
    free(obj->constAttrs->value);
    free(obj->constAttrs);
    free(obj);
    break;
  case OBJ_TYPE:
    freeType(obj->typeAttrs->actualType);
    free(obj->typeAttrs);
    free(obj);
    break;
  case OBJ_VARIABLE:
    freeType(obj->varAttrs->type);
    //freeScope(obj->varAttrs->scope);
    free(obj->varAttrs);
    free(obj);
    break;
  case OBJ_FUNCTION:
    freeType(obj->funcAttrs->returnType);
    freeReferenceList(obj->funcAttrs->paramList);
    //freeScope(obj->funcAttrs->scope);
    free(obj->funcAttrs);
    free(obj);
    break;
  case OBJ_PROCEDURE:
    freeReferenceList(obj->procAttrs->paramList);
    //freeScope(obj->procAttrs->scope);
    free(obj->procAttrs);
    free(obj);
    break;
  case OBJ_PARAMETER:
    freeType(obj->paramAttrs->type);
    freeObject(obj->paramAttrs->function);
    free(obj->paramAttrs);
    free(obj);
    break;
  default:
    break;
  }
}

void freeScope(Scope* scope) {
  // TODO
  freeObjectList(scope->objList);
  //freeObject(scope->owner);
  free(scope);
}

void freeObjectList(ObjectNode *objList) {
  // TODO
  ObjectNode* temp = objList;
  while(temp!=NULL){
    ObjectNode* temp1 = temp;
    temp=temp->next;
    freeObject(temp1->object);
    free(temp1);
  }
}

void freeReferenceList(ObjectNode *objList) {
  // TODO
  ObjectNode* temp = objList;
  while(temp!=NULL){
    ObjectNode* temp1 = temp;
    temp=temp->next;
    free(temp1);
  }
}

void addObject(ObjectNode **objList, Object* obj) {
  ObjectNode* node = (ObjectNode*) malloc(sizeof(ObjectNode));
  node->object = obj;
  node->next = NULL;
  if ((*objList) == NULL) 
    *objList = node;
  else {
    ObjectNode *n = *objList;
    while (n->next != NULL) 
      n = n->next;
    n->next = node;
  }
}

Object* findObject(ObjectNode *objList, char *name) {
  // TODO
  while (objList != NULL) {
    if (strcmp(objList->object->name, name) == 0) 
      return objList->object;
    else objList = objList->next;
  }
  return NULL;
}

/******************* others ******************************/

void initSymTab(void) {
  Object* obj;
  Object* param;
  
  symtab = (SymTab*) malloc(sizeof(SymTab));
  symtab->globalObjectList = NULL;
  
  obj = createFunctionObject("READC");
  obj->funcAttrs->returnType = makeCharType();
  addObject(&(symtab->globalObjectList), obj);
  
  obj = createFunctionObject("READI");
  obj->funcAttrs->returnType = makeIntType();
  addObject(&(symtab->globalObjectList), obj);
  
  obj = createProcedureObject("WRITEI");
  param = createParameterObject("i", PARAM_VALUE, obj);
  param->paramAttrs->type = makeIntType();
  addObject(&(obj->procAttrs->paramList),param);
  addObject(&(symtab->globalObjectList), obj);
  
  obj = createProcedureObject("WRITEC");
  param = createParameterObject("ch", PARAM_VALUE, obj);
  param->paramAttrs->type = makeCharType();
  addObject(&(obj->procAttrs->paramList),param);
  addObject(&(symtab->globalObjectList), obj);
  
  obj = createProcedureObject("WRITELN");
  addObject(&(symtab->globalObjectList), obj);
  
  intType = makeIntType();
  charType = makeCharType();
}

void cleanSymTab(void) {
  freeObject(symtab->program);
  freeObjectList(symtab->globalObjectList);
  free(symtab);
  freeType(intType);
  freeType(charType);
}

void enterBlock(Scope* scope) {
  symtab->currentScope = scope;
}

void exitBlock(void) {
  symtab->currentScope = symtab->currentScope->outer;
}

void declareObject(Object* obj) {
  if (obj->kind == OBJ_PARAMETER) {
    Object* owner = symtab->currentScope->owner;
    switch (owner->kind) {
    case OBJ_FUNCTION:
      addObject(&(owner->funcAttrs->paramList), obj);
      break;
    case OBJ_PROCEDURE:
      addObject(&(owner->procAttrs->paramList), obj);
      break;
    default:
      break;
    }
  }
 
  addObject(&(symtab->currentScope->objList), obj);
}


