/* -*- C++ -*- $Id$ */

// ******  Code generated by the The ACE ORB (TAO) IDL Compiler *******
// TAO and the TAO IDL Compiler have been developed by the Center for
// Distributed Object Computing at Washington University, St. Louis.
//
// Information about TAO is available at:
//                 http://www.cs.wustl.edu/~schmidt/TAO.html

#ifndef MESSENGERI_H_
#define MESSENGERI_H_

#include "MessengerS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include <tao/RTCORBA/RTCORBA.h>

//Class Messenger_i
class  Messenger_i : public virtual POA_Messenger
{
public:
  //Constructor
  Messenger_i(CORBA::ORB_ptr orb);

  //Destructor
  virtual ~Messenger_i();

  virtual CORBA::Boolean send_message(const char* user_name,
                                      const char* subject,
                                      char*& message);

private:
  RTCORBA::Current_var rt_current_;
};


#endif /* MESSENGERI_H_  */
