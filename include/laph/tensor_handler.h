#ifndef TENSOR_HANDLER_H
#define TENSOR_HANDLER_H

#include <vector>
#include <set>
#include "multi_compare.h"
#include <stdexcept>
#include "tensor.h

typedef unsigned int uint;

typedef pichi::Tensor DataType;


// ************************************************************************
// *                                                                      *
// *  The classes "ContractionInfo", "TensorInfo", and "TensorHandler"    *
// *  are defined in this file.  Here, an object of class "TensorInfo"    *
// *  is a product of "atomic tensors" of some type plus various          *
// *  contractions of the indices.  The atomic (indivisible) tensors      *
// *  must all be of the same type.  Each index must have the same        *
// *  common range.  For example, let T0 be a rank 3 atomic tensor,       *
// *  T1 be a rank 2 atomic tensor, and T2 be another rank 3 atomic       *
// *  tensor, then a "TensorInfo" object can be formed to represent       *
// *  T0_ijk T1_lm T2_nop using the constructor                           *
// *                                                                      *
// *     vector<T> atomics(3);                                            *
// *     atomics[0]=T0; atomics[1]=T1; atomics[2]=T2;                     *
// *     TensorInfo TT(atomics);                                          *
// *                                                                      *
// *  The class "TensorInfo" is templated for the type "T" of atomic      *
// *  tensors; the only requirements for the atomic tensor class          *
// *  is that                                                             *
// *     - a copy constructor, the operator = is defined                  *
// *     - a default constructor                                          *
// *     - a member getRank() const returns the rank of the atomic tensor *
// *     - the operations ==, !=, < are available                         *
// *                                                                      *
// *  Atomic tensors in an object of class "TensorInfo" are stored in     *
// *  a vector, so each atomic tensor can be specified by its location    *
// *  number in the vector: 0,1,...  Another kind of "TensorInfo"         *
// *  object includes contractions of some or all of the indices.         *
// *  For example, the tensor  T0_ijk T1_lm T2_jno has a contraction      *
// *  (summation) over "j" of index 1 of tensor 0 with index 0 of         *
// *  tensor 2.  Note that index numbers and tensor numbers are always    *
// *  zero offset.  This tensor is constructed in the following way:      *
// *                                                                      *
// *     vector<T> atomics(3);                                            *
// *     atomics[0]=T0; atomics[1]=T1; atomics[2]=T2;                     *
// *     int tensor1_number=0; int index1_number=1;                       *
// *     int tensor2_number=2; int index2_number=0;                       *
// *                                                                      *
// *     ContractionInfo c0(tensor1_number,index1_number,                 *
// *                        tensor2_number,index2_number);                *
// *     set<ContractionInfo> contractions;                               *
// *     contractions.insert(c0);                                         *
// *     TensorInfo TT(atomics,contractions);                             *
// *                                                                      *
// *  The class "ContractionInfo" is used to specify the requested        *
// *  contractions or summations over indices.  The order of the          *
// *  two indices in a "ContractionInfo" does not matter; they            *
// *  are stored in a sorted order, and the use of a "set" ensures        *
// *  that the order of the contractions does not matter.                 *
// *                                                                      *
// *  Note that two "TensorInfo" objects can be combined to create        *
// *  a new larger object:                                                *
// *                                                                      *
// *     TensorInfo TT1(...), TT2(...);                                   *
// *     TensorInfo TT3(TT1,TT2);                                         *
// *                                                                      *
// *  Additional contractions can be added to a tensor TT1 using          *
// *                                                                      *
// *     TensorInfo T1(...);                                              *
// *     set<ContractionInfo> added(...);                                 *
// *     TensorInfo T2(T1,added);                                         *
// *                                                                      *
// *  Keep in mind that "ContractionInfo" and "TensorInfo" are            *
// *  informational classes only.  They do not perform the actual         *
// *  contractions, nor do they store data, nor allow access to the       *
// *  tensor elements.  For this, you need an object of the class         *
// *  "TensorHandler", described below.                                   *
// *                                                                      *
// ************************************************************************


class ContractionInfo
{
   uint m_code;

   ContractionInfo();

 public:

   ContractionInfo(uint tensor1_number, uint index1_number,
                   uint tensor2_number, uint index2_number);

   ContractionInfo(const ContractionInfo& in) 
    : m_code(in.m_code) {}

   ContractionInfo& operator=(const ContractionInfo& in)
    {m_code=in.m_code; return *this;}

   ~ContractionInfo(){}

   uint getTensor1Number() const
    {return (m_code>>24);}

   uint getIndex1Number() const
    {return ((m_code>>16)&0xFFu);}

   uint getTensor2Number() const
    {return ((m_code>>8)&0xFFu);}

   uint getIndex2Number() const
    {return (m_code&0xFFu);}

   bool operator==(const ContractionInfo& rhs) const
    {return (m_code==rhs.m_code);}

   bool operator!=(const ContractionInfo& rhs) const
    {return (m_code!=rhs.m_code);}

   bool operator<(const ContractionInfo& rhs) const
    {return (m_code<rhs.m_code);}

 private:

   void do_shift(uint shift)   // add "shift" to both tensor numbers
    {uint i2=m_code&0xFFu; 
     m_code>>=8; uint t2=m_code&0xFFu;
     m_code>>=8; uint i1=m_code&0xFFu;
     uint t1=m_code>>8; 
     m_code=t1+shift;
     m_code<<=8; m_code|=i1;
     m_code<<=8; m_code|=t2+shift;
     m_code<<=8; m_code|=i2;}
     
   uint getTensor1Index1() const
    {return m_code>>16;}
   
   uint getTensor2Index2() const
    {return m_code&0xFFFFu;}

   template<typename T> friend class TensorInfo;

};


inline ContractionInfo::ContractionInfo(uint tensor1_number, uint index1_number,
                                        uint tensor2_number, uint index2_number)
{
 if ((tensor1_number>255)||(tensor2_number>255)||(index1_number>255)||(index2_number>255))
    throw(std::invalid_argument("Unsupported ContractionInfo"));
 if ((tensor1_number==tensor2_number)&&(index1_number==index2_number))
    throw(std::invalid_argument("Invalid ContractionInfo"));
 if ((tensor1_number<tensor2_number)||((tensor1_number==tensor2_number)&&(index1_number<index2_number))){
    m_code=tensor1_number;
    m_code<<=8; m_code|=index1_number;
    m_code<<=8; m_code|=tensor2_number;
    m_code<<=8; m_code|=index2_number;}
 else{
    m_code=tensor2_number;
    m_code<<=8; m_code|=index2_number;
    m_code<<=8; m_code|=tensor1_number;
    m_code<<=8; m_code|=index1_number;}
}

 // *************************************************


template <typename T>
class TensorInfo
{

   std::vector<T> m_atomics;
   std::set<ContractionInfo> m_contracts;

  public:

   TensorInfo(){}

   TensorInfo(const T& atom);

   TensorInfo(const std::vector<T>& atoms);

   TensorInfo(const std::vector<T>& atoms, const std::set<ContractionInfo>& contractions);

   TensorInfo(const TensorInfo& in1, const TensorInfo& in2);

   TensorInfo(const TensorInfo& in, const std::set<ContractionInfo>& add_contractions);
    
   TensorInfo(const TensorInfo& in);
    
   TensorInfo& operator=(const TensorInfo& in);
    
   ~TensorInfo(){}

   void addAtomic(const T& atomic);

   void addContraction(const ContractionInfo& ci);

   void addContraction(uint tensor1_number, uint index1_number,
                       uint tensor2_number, uint index2_number);
  
   uint getNumberOfAtomics() const
    {return m_atomics.size();}
   
   uint getRank() const;
   
   uint getRank(uint tensor_number) const
    {return m_atomics.at(tensor_number).getRank();}
   
   T getAtomicInfo(uint tensor_number) const
    {return m_atomics.at(tensor_number);}
   
   const std::set<ContractionInfo>& getContractions() const
    {return m_contracts;}


   bool operator==(const TensorInfo& rhs) const
    {return LaphEnv::multiEqual(m_atomics,rhs.m_atomics) 
         && LaphEnv::multiEqual(m_contracts,rhs.m_contracts);}

   bool operator!=(const TensorInfo& rhs) const
    {return LaphEnv::multiNotEqual(m_atomics,rhs.m_atomics) 
         || LaphEnv::multiNotEqual(m_contracts,rhs.m_contracts);}

   bool operator<(const TensorInfo& rhs) const
    {return (LaphEnv::multiLessThan(m_atomics,rhs.m_atomics)
         ||((LaphEnv::multiEqual(m_atomics,rhs.m_atomics))
            &&(LaphEnv::multiLessThan(m_contracts,rhs.m_contracts))));}

  
  private:
  
    bool add_contraction(const ContractionInfo& ci);
    
};


// ***********************************

template <typename T>
TensorInfo<T>::TensorInfo(const T& atom)
     : m_atomics(1,atom)
{}

template <typename T>
TensorInfo<T>::TensorInfo(const std::vector<T>& atoms)
     : m_atomics(atoms)
{}

template <typename T>
TensorInfo<T>::TensorInfo(const std::vector<T>& atoms, 
                          const std::set<ContractionInfo>& contractions)
     : m_atomics(atoms)
{
 for (std::set<ContractionInfo>::const_iterator
      it=contractions.begin();it!=contractions.end();it++)
    if (!add_contraction(*it))
        throw(std::invalid_argument("Invalid TensorInfo"));
}

template <typename T>
TensorInfo<T>::TensorInfo(const TensorInfo<T>& in1, const TensorInfo<T>& in2) 
     : m_atomics(in1.m_atomics), m_contracts(in1.m_contracts)
{
 m_atomics.insert(m_atomics.end(),in2.m_atomics.begin(),in2.m_atomics.end());
 uint shift=in1.getNumberOfAtomics();
 for (std::set<ContractionInfo>::const_iterator 
      it=in2.m_contracts.begin();it!=in2.m_contracts.end();it++){
    ContractionInfo ci(*it); ci.do_shift(shift);
    m_contracts.insert(ci);}
}

template <typename T>
TensorInfo<T>::TensorInfo(const TensorInfo<T>& in, 
                          const std::set<ContractionInfo>& add_contractions)
     : m_atomics(in.m_atomics), m_contracts(in.m_contracts)
{
 for (std::set<ContractionInfo>::const_iterator
      it=add_contractions.begin();it!=add_contractions.end();it++)
    if (!add_contraction(*it)) 
        throw(std::invalid_argument("Invalid TensorInfo"));
}
    
template <typename T>
TensorInfo<T>::TensorInfo(const TensorInfo<T>& in) 
   : m_atomics(in.m_atomics), m_contracts(in.m_contracts)
{}
    
template <typename T>
TensorInfo<T>& TensorInfo<T>::operator=(const TensorInfo<T>& in)
{
 m_atomics=in.m_atomics;
 m_contracts=in.m_contracts;
 return *this;
}

template <typename T>
void TensorInfo<T>::addAtomic(const T& atomic)
{
 m_atomics.push_back(atomic);
}

template <typename T>
void TensorInfo<T>::addContraction(const ContractionInfo& ci)
{
 if (!add_contraction(ci))
    throw(std::invalid_argument("Invalid contraction being added"));
}

template <typename T>
void TensorInfo<T>::addContraction(uint tensor1_number, uint index1_number,
                                   uint tensor2_number, uint index2_number)
{
 if (!add_contraction(ContractionInfo(tensor1_number,index1_number,
                                      tensor2_number,index2_number)))
    throw(std::invalid_argument("Invalid contraction being added"));
}
 
template <typename T>
uint TensorInfo<T>::getRank() const
{
 uint rank=0;
 for (typename std::vector<T>::const_iterator it=m_atomics.begin();it!=m_atomics.end();it++)
    rank+=it->getRank();
 return rank-2*m_contracts.size();
}
 
  
    //  Adds a new contraction if valid; returns true if successful,
    //  false otherwise.  A contraction is valid if the numbers of its tensors
    //  are valid, the indices of the tensors are valid, and the two
    //  tensor/index combinations are not involved in any other contractions.

template <typename T>
bool TensorInfo<T>::add_contraction(const ContractionInfo& ci)
{
 for (std::set<ContractionInfo>::const_iterator 
      it=m_contracts.begin();it!=m_contracts.end();it++){
    if (ci.m_code==it->m_code) return true;
    if (ci.getTensor1Index1()==it->getTensor1Index1()) return false;
    if (ci.getTensor2Index2()==it->getTensor2Index2()) return false;}
 uint natomics=getNumberOfAtomics();
 uint t1=ci.getTensor1Number();
 uint t2=ci.getTensor2Number();
 uint i1=ci.getIndex1Number();
 uint i2=ci.getIndex2Number();
 if ((t1>=natomics)||(t2>=natomics)) return false;
 if ((i1>=getRank(t1))||(i2>=getRank(t2))) return false;
 m_contracts.insert(ci);
 return true;
}

// *******************************************************************



template <typename K, typename T>
class TensorHandler
{

   K* m_atomic_handler;
   uint m_index_range;
   std::map<TensorInfo<T>,DataType > m_storage;
  
       // Prevent copying ... handler might contain large
       // amounts of data

   TensorHandler(const TensorHandler&);
   TensorHandler& operator=(const TensorHandler&);


 public:

   TensorHandler(K* atomhandler);

   const DataType& getData(const TensorInfo<T>& in);

   void removeData(const TensorInfo<T>& in) {m_storage.erase(in);}

   void clearData() {m_storage.clear();}
      
   ~TensorHandler(){}
   
};


template <typename K, typename T>
TensorHandler<K,T>::TensorHandler(K* atomhandler)
    :  m_atomic_handler(atomhandler), 
       m_index_range(atomhandler->getIndexRange())
{}



template <typename K, typename T>
const DataType& TensorHandler<K,T>::getData(const TensorInfo<T>& key)
{
   // first, check to see if data for requested key is already in memory
  
 typename std::map<TensorInfo<T>,DataType >::const_iterator 
    it=m_storage.find(key);
 if (it!=m_storage.end()) return it->second;

 DataType result;
 const std::set<ContractionInfo>& contractions=key.getContractions();

   // if not, check to see if key is atomic, then use the underlying
   // atomic handler to get the data
 
 if (key.getNumberOfAtomics()==1){
    try{
       result=m_atomic_handler->getData(key.getAtomicInfo(0));}
    catch(...){}}



 std::pair<typename std::map<TensorInfo<T>,DataType >::iterator,bool> 
    rt=m_storage.insert(make_pair(key,result));
 if (!(rt.second)) 
    throw(std::runtime_error("TensorHandler: Could not insert into storage"));
 return (rt.first)->second;
}


/*
template <typename K, typename T>
DataType TensorHandler<K,T>::get_atomic_data(const TensorInfo<T>& key)




template <typename K, typename T>
std::set<



template <typename K, typename T>
void TensorHandler<K,T>::do_a_contraction(DataType& array, uint index1, uint index2)
{
*/

// ***************************************************************
#endif  
