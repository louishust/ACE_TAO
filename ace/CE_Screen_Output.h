/**
 *  @file    CE_Screen_Output.h
 *
 *  $Id$
 *
 *  @author Si Mong Park  <spark@ociweb.com>
 */
//=============================================================================

#ifndef ACE_CE_Screen_Output_h
#define ACE_CE_Screen_Output_h

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_HAS_WINCE)

#include "ace/OS.h"
#include "ace/Log_Msg_Callback.h"
#include "ace/Log_Record.h"

const ACE_TCHAR endl[] = ACE_LIB_TEXT("\r\n");
const ACE_TCHAR tab[]  = ACE_LIB_TEXT("\t");

/**
 * @class ACE_CE_Screen_Output
 *
 * @brief Replacement of text output for Windows CE.
 *
 * This class allows standard text output to be displayed on
 * text window for Windows CE.  Generally, all ACE output will
 * go through under CE if and only if user uses WindozeCE
 * implementation by using main_ce instead of main.
 * Also, for the easier debugging purpose, object pointer of
 * this class can be gotten from ACE_Log_Msg::msg_callback()
 * and then can be used directly by user just like cout stream.
 */
class ACE_Export ACE_CE_Screen_Output : public ACE_Log_Msg_Callback
{
public:
    /**
     * Ctor with HWND specified.
     */
    ACE_CE_Screen_Output(HWND hEdit);

    /**
     * Default Ctor
     */
    ACE_CE_Screen_Output();

    /**
     * Default Dtor
     */
    virtual ~ACE_CE_Screen_Output();

    /**
     * Implementation of pure virtual function from ACE_Log_Msg_Callback.
     */
    virtual void log (ACE_Log_Record &log_record);

    /**
     * Interface to specify active window handle.
     */
    void SetOutputWindow(HWND hWnd);

    /**
     * Clears text screen.
     */
    void clear();

    /**
     * << operator that performs actual print out.
     *
     * Note: This is the only one operator that performs
     *       output.  All other perators convert the type and
     *       use this operator underneath.
     */
    ACE_CE_Screen_Output& operator << (ACE_TCHAR*);
    ACE_CE_Screen_Output& operator << (const ACE_TCHAR*);

    ACE_CE_Screen_Output& operator << (ACE_ANTI_TCHAR* output);
    ACE_CE_Screen_Output& operator << (const ACE_ANTI_TCHAR* output);

    ACE_CE_Screen_Output& operator << (char output);
    ACE_CE_Screen_Output& operator << (unsigned char output);

    ACE_CE_Screen_Output& operator << (unsigned short output);

    ACE_CE_Screen_Output& operator << (int output);
    ACE_CE_Screen_Output& operator << (unsigned int output);

    ACE_CE_Screen_Output& operator << (float output);

    ACE_CE_Screen_Output& operator << (long output);
    ACE_CE_Screen_Output& operator << (unsigned long output);

    ACE_CE_Screen_Output& operator << (FILE* pFile);

private:
    /**
     * Copy Ctor
     */
    ACE_CE_Screen_Output(ACE_CE_Screen_Output&);

    HWND handler_;

    /**
     * File pointer that used to save output to file.
     * This class does not own the file handler pointer.
     */
    FILE* pFile_;
};

#endif  // ACE_HAS_WINCE
#endif  // ACE_CE_Screen_Output_h
