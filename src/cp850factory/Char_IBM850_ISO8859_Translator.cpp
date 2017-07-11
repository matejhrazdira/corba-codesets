// -*- C++ -*-

//=============================================================================
/**
 *  @file    Char_IBM850_ISO8859_Translator.cpp
 *
 *  Defines the arrays required to convert between ISO8859 (aka
 *  Latin/1) and IBM850 (aka EBCDIC).
 *
 *  @author Jim Rogers (jrogers@viasoft.com)
 */
//=============================================================================


#include "Char_IBM850_ISO8859_Translator.h"
#include "ace/OS_Memory.h"
#include "ace/OS_NS_string.h"

// ****************************************************************

IBM850_ISO8859::IBM850_ISO8859 (void)
{
}

IBM850_ISO8859::~IBM850_ISO8859 (void)
{
}

ACE_CDR::Boolean
IBM850_ISO8859::read_char (ACE_InputCDR &in,
                            ACE_CDR::Char &x)
{
  if (this->read_1 (in, reinterpret_cast<ACE_CDR::Octet*> (&x)))
    {
      x = to_IBM850[(int) x];
      return 1;
    }
  return 0;
}

ACE_CDR::Boolean
IBM850_ISO8859::read_string (ACE_InputCDR& in,
                              ACE_CDR::Char *& x)
{
  ACE_CDR::ULong len;

  in.read_ulong (len);

  if (len > 0)
    {
      //      ACE_DEBUG ((LM_DEBUG,"read_string: len = %u\n",len));
      ACE_NEW_RETURN (x,
                      ACE_CDR::Char[len],
                      0);

      if (this->read_char_array (in, x, len))
        return 1;

      delete [] x;
    }

  x = 0;
  return 0;
}

ACE_CDR::Boolean
IBM850_ISO8859::read_char_array (ACE_InputCDR& in,
                                  ACE_CDR::Char* x,
                                  ACE_CDR::ULong len)
{
  if (this->read_array (in,
                        x,
                        ACE_CDR::OCTET_SIZE,
                        ACE_CDR::OCTET_ALIGN,
                        len))
    {
      for (ACE_CDR::ULong i = 0; i != len; ++i)
        {
          x[i] = (unsigned char)to_IBM850[(unsigned char)x[i]];
        }

      return 1;
    }

  return 0;
}

ACE_CDR::Boolean
IBM850_ISO8859::write_char (ACE_OutputCDR& out,
                             ACE_CDR::Char x)
{
  return this->write_1 (out,
                        reinterpret_cast<const ACE_CDR::Octet*> (&from_IBM850[(int) x]));
}

ACE_CDR::Boolean
IBM850_ISO8859::write_string (ACE_OutputCDR& out,
                               ACE_CDR::ULong len,
                               const ACE_CDR::Char* x)
{
  if (out.write_ulong (len + 1))
    {
      return this->write_char_array (out, x, len + 1);
    }

  return 0;
}

ACE_CDR::Boolean
IBM850_ISO8859::write_char_array (ACE_OutputCDR& out,
                                   const ACE_CDR::Char* x,
                                   ACE_CDR::ULong len)
{
  char *buf;

  if (this->adjust (out, len, 1, buf) == 0)
    {
      ACE_OS::memcpy (buf, x, len);

      for (ACE_CDR::ULong i = 0; i != len; ++i)
        {
          buf[i] = (unsigned char)from_IBM850[(unsigned char)buf[i]];
        }

      return 1;
    }

  this->good_bit(out, 0);
  return 0;
}

// ****************************************************************

char to_IBM850[257] =
{
		"\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f"
		"\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f"
		"\x20\x21\x22\x23\x24\x25\x26\x27\x28\x29\x2a\x2b\x2c\x2d\x2e\x2f"
		"\x30\x31\x32\x33\x34\x35\x36\x37\x38\x39\x3a\x3b\x3c\x3d\x3e\x3f"
		"\x40\x41\x42\x43\x44\x45\x46\x47\x48\x49\x4a\x4b\x4c\x4d\x4e\x4f"
		"\x50\x51\x52\x53\x54\x55\x56\x57\x58\x59\x5a\x5b\x5c\x5d\x5e\x5f"
		"\x60\x61\x62\x63\x64\x65\x66\x67\x68\x69\x6a\x6b\x6c\x6d\x6e\x6f"
		"\x70\x71\x72\x73\x74\x75\x76\x77\x78\x79\x7a\x7b\x7c\x7d\x7e\x7f"
		"\x3f\x3f\x3f\x3f\x3f\x3f\x3f\x3f\x3f\x3f\x3f\x3f\x3f\x3f\x3f\x3f"
		"\x3f\x3f\x3f\x3f\x3f\x3f\x3f\x3f\x3f\x3f\x3f\x3f\x3f\x3f\x3f\x3f"
		"\xff\xad\xbd\x9c\xcf\xbe\xdd\xf5\xf9\xb8\xa6\xae\xaa\xf0\xa9\xee"
		"\xf8\xf1\xfd\xfc\xef\xe6\xf4\xfa\xf7\xfb\xa7\xaf\xac\xab\xf3\xa8"
		"\xb7\xb5\xb6\xc7\x8e\x8f\x92\x80\xd4\x90\xd2\xd3\xde\xd6\xd7\xd8"
		"\xd1\xa5\xe3\xe0\xe2\xe5\x99\x9e\x9d\xeb\xe9\xea\x9a\xed\xe8\xe1"
		"\x85\xa0\x83\xc6\x84\x86\x91\x87\x8a\x82\x88\x89\x8d\xa1\x8c\x8b"
		"\xd0\xa4\x95\xa2\x93\xe4\x94\xf6\x9b\x97\xa3\x96\x81\xec\xe7\x98"
};

char from_IBM850[257] =
{
		"\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f"
		"\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f"
		"\x20\x21\x22\x23\x24\x25\x26\x27\x28\x29\x2a\x2b\x2c\x2d\x2e\x2f"
		"\x30\x31\x32\x33\x34\x35\x36\x37\x38\x39\x3a\x3b\x3c\x3d\x3e\x3f"
		"\x40\x41\x42\x43\x44\x45\x46\x47\x48\x49\x4a\x4b\x4c\x4d\x4e\x4f"
		"\x50\x51\x52\x53\x54\x55\x56\x57\x58\x59\x5a\x5b\x5c\x5d\x5e\x5f"
		"\x60\x61\x62\x63\x64\x65\x66\x67\x68\x69\x6a\x6b\x6c\x6d\x6e\x6f"
		"\x70\x71\x72\x73\x74\x75\x76\x77\x78\x79\x7a\x7b\x7c\x7d\x7e\x7f"
		"\xc7\xfc\xe9\xe2\xe4\xe0\xe5\xe7\xea\xeb\xe8\xef\xee\xec\xc4\xc5"
		"\xc9\xe6\xc6\xf4\xf6\xf2\xfb\xf9\xff\xd6\xdc\xf8\xa3\xd8\xd7\x3f"
		"\xe1\xed\xf3\xfa\xf1\xd1\xaa\xba\xbf\xae\xac\xbd\xbc\xa1\xab\xbb"
		"\x3f\x3f\x3f\x7c\x2b\xc1\xc2\xc0\xa9\x3f\x3f\x3f\x3f\xa2\xa5\x2b"
		"\x2b\x2b\x2b\x2b\x2d\x2b\xe3\xc3\x3f\x3f\x3f\x3f\x3f\x3f\x3f\xa4"
		"\xf0\xd0\xca\xcb\xc8\x3f\xcd\xce\xcf\x2b\x2b\x3f\x3f\xa6\xcc\x3f"
		"\xd3\xdf\xd4\xd2\xf5\xd5\xb5\xfe\xde\xda\xdb\xd9\xfd\xdd\xaf\xb4"
		"\xad\xb1\x3f\xbe\xb6\xa7\xf7\xb8\xb0\xa8\xb7\xb9\xb3\xb2\x3f\xa0"};
