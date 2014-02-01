/***
DEVSIM
Copyright 2013 Devsim LLC

This file is part of DEVSIM.

DEVSIM is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, version 3.

DEVSIM is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with DEVSIM.  If not, see <http://www.gnu.org/licenses/>.
***/

#ifndef DEVICE_HH
#define DEVICE_HH
#include "MathEnum.hh"
#include <cstddef>
#include <string>
#include <vector>
#include <map>
#include <complex>

class PermutationEntry;
typedef std::map<size_t, PermutationEntry> PermutationMap;

namespace dsMath {
template <typename T> class RowColVal;
typedef std::vector<RowColVal<double> > RealRowColValueVec;
typedef std::pair<int, double> RHSEntry;
typedef std::vector<RHSEntry> RHSEntryVec;
}

/**
    A device maintains all of the regions and boundary regions within it
*/
class Contact;
typedef Contact *ContactPtr;
typedef const Contact *ConstContactPtr;

class Interface;
typedef Interface *InterfacePtr;
typedef const Interface *ConstInterfacePtr;

class Coordinate;
typedef Coordinate *CoordinatePtr;

class Region;
typedef Region *RegionPtr;

class Device
{
   public:
      ~Device();

      typedef std::vector<CoordinatePtr> CoordinateList_t;
      typedef std::map<std::string, RegionPtr> RegionList_t;
      typedef std::map<std::string, ContactPtr> ContactList_t;
      typedef std::map<std::string, InterfacePtr> InterfaceList_t;

      /// Coordinate index to an interface pointer
      typedef std::map<size_t, std::vector<InterfacePtr> > CoordinateIndexToInterface_t;
      typedef std::map<size_t, std::vector<ContactPtr> > CoordinateIndexToContact_t;

      Device(std::string, size_t);

      /// If we ever add a delete method.  Interfaces would need to be removed automatically when their underlying regions are removed.
      void AddRegion(const RegionPtr &);

      void Update(const std::vector<double> &/*result*/);
      void ACUpdate(const std::vector<std::complex<double> > &/*result*/);
      void NoiseUpdate(const std::string &/*output*/, const std::vector<size_t> &/*permvec*/, const std::vector<std::complex<double> > &/*result*/);

      void UpdateContacts();
      // Need to be careful with accessors and stuff
      // maintaining constness of contact
      void AddContact(const ContactPtr &);

      void AddInterface(const InterfacePtr &);

      void AddCoordinateList(const CoordinateList_t &);
      void AddCoordinate(CoordinatePtr);

      const std::string &GetName() const
      {
          return deviceName;
      }

      void SetBaseEquationNumber(size_t);
      size_t GetBaseEquationNumber();
      size_t CalcMaxEquationNumber();

    void ContactAssemble(dsMath::RealRowColValueVec &, dsMath::RHSEntryVec &, PermutationMap &, dsMathEnum::WhatToLoad, dsMathEnum::TimeMode);

    void InterfaceAssemble(dsMath::RealRowColValueVec &, dsMath::RHSEntryVec &, PermutationMap &, dsMathEnum::WhatToLoad, dsMathEnum::TimeMode);

    void RegionAssemble(dsMath::RealRowColValueVec &, dsMath::RHSEntryVec &, dsMathEnum::WhatToLoad, dsMathEnum::TimeMode);

    double GetAbsError() const
    {
        return absError;
    }

    double GetRelError() const
    {
        return relError;
    }

    const CoordinateList_t &GetCoordinateList() const
    {
        return coordinateList;
    }

    const RegionList_t &GetRegionList() const
    {
        return regionList;
    }

    const InterfaceList_t &GetInterfaceList() const
    {
        return interfaceList;
    }

    const ContactList_t &GetContactList() const
    {
        return contactList;
    }

    RegionPtr GetRegion(const std::string &);

    InterfacePtr GetInterface(const std::string &) const ;

    ContactPtr GetContact(const std::string &) const;

    size_t GetNumberOfInterfacesOnCoordinate(const Coordinate &);
    size_t GetNumberOfContactsOnCoordinate(const Coordinate &);

    //// We guarantee that this list is sorted by pointer address to the interface
    const CoordinateIndexToInterface_t &GetCoordinateIndexToInterface() const
    {
        return coordinateIndexToInterface;
    }

    //// We guarantee that this list is sorted by pointer address to the contact
    const CoordinateIndexToContact_t   &GetCoordinateIndexToContact() const
    {
        return coordinateIndexToContact;
    }

    void BackupSolutions(const std::string &);
    void RestoreSolutions(const std::string &);

    size_t GetDimension() const
    {
      return dimension;
    }

    void SignalCallbacksOnInterface(const std::string &/*str*/, const Region *) const;

   private:
      Device();
      Device (const Device &);
      Device &operator= (const Device &);

      std::string deviceName;
      size_t dimension;

      RegionList_t regionList;

      ContactList_t contactList;
      InterfaceList_t interfaceList;

      CoordinateList_t coordinateList;

      CoordinateIndexToInterface_t coordinateIndexToInterface;
      CoordinateIndexToContact_t   coordinateIndexToContact;

      size_t baseeqnnum; // base equation number for this region

      double relError;
      double absError;
};

#endif
