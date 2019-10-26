#ifndef __common_h__34805723904562903456203495629034__
#define __common_h__34805723904562903456203495629034__

#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <vector>
#include <set>
#include <map>
#include <list>
#include <algorithm>
#include <memory>


class InvalidIPAddressFormatException
 {
   public:
                             InvalidIPAddressFormatException ( const std::string & input )
                              : m_Input ( input ) { }
   private:
    std::string              m_Input;
    friend std::ostream    & operator <<                   ( std::ostream    & os,
                                                             const InvalidIPAddressFormatException & x )
     {
       return os << x . m_Input;
     }
                                                                  
 };
 
class InvalidIndexException
 {
   public:
                             InvalidIndexException         ( int idx ) 
                              : m_Idx ( idx ) { }
   private:
    int                      m_Idx;                           
    friend std::ostream    & operator <<                   ( std::ostream     & os,
                                                             const InvalidIndexException & x )
     {
       return os << x . m_Idx;
     }
 }; 


class CIPv4
 {
   public:
    //---------------------------------------------------------------------------------------------
                             CIPv4                         ( void )
     {
       m_Addr[0] = m_Addr[1] = m_Addr[2] = m_Addr[3] = 0;
     }
    //---------------------------------------------------------------------------------------------
                             CIPv4                         ( const std::string & src )
     {
       std::istringstream iss ( src );
       if ( !( iss >> *this ) || ! iss . eof () )
        throw InvalidIPAddressFormatException ( src );
     }
    //---------------------------------------------------------------------------------------------
    bool                     operator ==                   ( const CIPv4     & x ) const
     {
       for ( int i = 0; i < 4; i ++ )
        if ( m_Addr[i] != x . m_Addr[i] )
         return false;
       return true;
     }
    //---------------------------------------------------------------------------------------------
    friend std::ostream    & operator <<                   ( std::ostream    & os,
                                                             const CIPv4     & x )
     {
       return os << (std::uint32_t) x . m_Addr[0] << '.'
                 << (std::uint32_t) x . m_Addr[1] << '.'
                 << (std::uint32_t) x . m_Addr[2] << '.'
                 << (std::uint32_t) x . m_Addr[3];
     }
    //---------------------------------------------------------------------------------------------
    friend std::istream    & operator >>                   ( std::istream    & is,
                                                             CIPv4           & x )
     {
       std::uint8_t addr[4];

       for (int i = 0; i < 4; i ++ )
        CIPv4::parseDotNum ( is, addr[i], i > 0 );

       if ( is )
        for ( int i = 0; i < 4; i ++ )
         x . m_Addr[i] = addr[i];
       return is;
     }
    //---------------------------------------------------------------------------------------------
   private:
    static void              parseDotNum                   ( std::istream    & is,
                                                             std::uint8_t    & n,
                                                             bool              dot )
     {
       int  x;
       char c;

       if ( dot &&
            ( ! (is >> c ) || c != '.' ) )
        {
          is . setstate ( std::ios::failbit );
          return;
        }
       if ( ! ( is >> x ) || x < 0 || x > 255 )
        {
          is . setstate ( std::ios::failbit );
          return;
        }
       n = x;
     }
    //---------------------------------------------------------------------------------------------
    std::uint8_t             m_Addr[4];
 };

class CIPv6
 {
   public:
    //---------------------------------------------------------------------------------------------
                             CIPv6                         ( void )
     {
       for ( int i = 0; i < 8; i ++ )
        m_Addr[i] = 0;
     }
    //---------------------------------------------------------------------------------------------
                             CIPv6                         ( const std::string & src )
     {
       std::istringstream iss ( src );
       if ( !( iss >> *this ) || ! iss . eof () )
        throw InvalidIPAddressFormatException ( src );
     }
    //---------------------------------------------------------------------------------------------
    bool                     operator ==                   ( const CIPv6     & x ) const
     {
       for ( int i = 0; i < 8; i ++ )
        if ( m_Addr[i] != x . m_Addr[i] )
         return false;
       return true;
     }
    //---------------------------------------------------------------------------------------------
    friend std::ostream    & operator <<                   ( std::ostream    & os,
                                                             const CIPv6     & x )
     {
       std::ios::fmtflags oldFlags = os . flags ();
       os << std::hex;
       for ( int i = 0; i < 8; i ++ )
        {
          if ( i > 0 ) os << ':';
          os << x . m_Addr[i];
        }
       os . flags ( oldFlags );
       return os;
     }
    //---------------------------------------------------------------------------------------------
    friend std::istream    & operator >>                   ( std::istream    & is,
                                                             CIPv6           & x )
     {
       std::uint16_t addr[8];
       std::ios::fmtflags oldFlags = is . flags ();
       is >> std::hex;

       for (int i = 0; i < 8; i ++ )
        CIPv6::parseColonHex ( is, addr[i], i > 0 );

       if ( is )
        for ( int i = 0; i < 8; i ++ )
         x . m_Addr[i] = addr[i];

       is . flags ( oldFlags );
       return is;
     }
    //---------------------------------------------------------------------------------------------
   private:
    static void              parseColonHex                 ( std::istream    & is,
                                                             std::uint16_t   & n,
                                                             bool              colon )
     {
       int  x;
       char c;

       if ( colon &&
            ( ! (is >> c ) || c != ':' ) )
        {
          is . setstate ( std::ios::failbit );
          return;
        }
       if ( ! ( is >> std::hex >> x ) || x < 0 || x > 65535 )
        {
          is . setstate ( std::ios::failbit );
          return;
        }
       n = x;
     }
    //---------------------------------------------------------------------------------------------
    std::uint16_t            m_Addr[8];
 };

#endif /* __common_h__34805723904562903456203495629034__ */
