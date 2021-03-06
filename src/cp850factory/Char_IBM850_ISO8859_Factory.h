// -*- C++ -*-

//=============================================================================
/**
 *  @file    Char_IBM850_ISO8859_Factory.h
 *
 *  A tempate instantiation of the codeset translator factory from TAO. This
 *  one loads a modified version of ACE's IBM850 (EBCDIC) to ISO8859 (ASCII)
 *  translator.
 *
 *  @author   Phil Mesnier <mesnier_p@ociweb.com>
 */
//=============================================================================


#ifndef CHAR_IBM850_ISO8859_FACTORY_H
#define CHAR_IBM850_ISO8859_FACTORY_H

#include /**/ "ace/pre.h"
#include "ace/Service_Config.h"
#include "tao/Codeset/Codeset_Translator_Factory.h"

#include "IBM850_ISO8859_export.h"
#include "Char_IBM850_ISO8859_Translator.h"

typedef TAO_Codeset_Translator_Factory_T<IBM850_ISO8859> Char_IBM850_ISO8859_Factory;

ACE_STATIC_SVC_DECLARE_EXPORT (IBM850_ISO8859, Char_IBM850_ISO8859_Factory)
ACE_FACTORY_DECLARE (IBM850_ISO8859, Char_IBM850_ISO8859_Factory)

#include /**/ "ace/post.h"
#endif /* TAO_CHAR_IBM850_ISO8859_FACTORY_H */
