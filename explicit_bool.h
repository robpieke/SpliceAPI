//
// This template class allows for defining explicit boolean
// version operators without C++0x
//
// This code is taken (mostly) verbatim from 
// http://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Safe_bool
// modified so the actual conversion operators is no hidden on the base class 
//
// Usage is like so
//
//
//  class YourClass : 
//    public safe_bool <YourClass> // CRTP idiom
//  {
//  public:
//    // Define boolean conversion
//    operator explicit_bool::type() const {
//      return explicit_bool::get(SomeLogicHere()); 
//    }
//  };
//
//   This will allow
// if (yourClass)
//   but not
// int var = yourClass;

#pragma once

class explicit_bool {
  public:
    typedef void (explicit_bool::*type)() const;
    void this_type_does_not_support_comparisons() const {}
  
    static type get(bool condition) {
        return condition ? &explicit_bool::this_type_does_not_support_comparisons : 0;
    }
};
