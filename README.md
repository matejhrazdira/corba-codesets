corba-codesets
==============

**corba-codesets** is a short example for codeset handling with ACE+TAO ORB.

Losely based on example from `ACE_wrappers/TAO/tests/CodeSets`.

HOW-TO
------

1. Patch ACE codeset library.
    - List of codesets can be found e.g. [here](https://ftp.gwdg.de/pub/misc/opengroup/code_set_registry/code_set_registry1.2f.txt)
    - Put required codesets to `ACE_wrappers/ace/Codeset_Registry_db.cpp`, e.g.: `{"IBM-850 (CCSID 04946); Multilingual IBM PC Data-190","CP850",0x10021352,1,{0x0011},1}`
    - Rebuild ACE+TAO
1. Use svc.conf as shown in the example.
