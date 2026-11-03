/***********************************************************************
 * Module:  IIdentificable.h
 * Author:  gdann
 * Modified: martes, 3 de noviembre de 2026 9:15:20
 * Purpose: Declaration of the class IIdentificable
 ***********************************************************************/

#if !defined(__Proyecto_IIdentificable_h)
#define __Proyecto_IIdentificable_h

class IIdentificable
{
public:
   virtual std::string getClave(void)=0;

protected:
private:

};

#endif