#include "Object.h"
//------------------------------------------------------------------------------

OBJECT::OBJECT(OBJECT_TYPE Type, const char* Name){
 this->Type = Type;
 this->Name = Name;
}
//------------------------------------------------------------------------------

int OBJECT::Compare(TREE_NODE* Right){
 return Name.Compare(((OBJECT*)Right)->Name);
}
//------------------------------------------------------------------------------
