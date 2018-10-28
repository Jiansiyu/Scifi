###### Hall A SDK Main SConscript File #####
###### Author:	Edward Brash (brash@jlab.org) May 2017

import re

Import ('baseenv')

######## ROOT Dictionaries #########

rootuserdict = baseenv.subst('$MAIN_DIR')+'/RootUserDict.C'
userheaders = Split("""
SciFi.h   SciFi_LinkDef.h
""")
baseenv.RootCint(rootuserdict,userheaders)
baseenv.Clean(rootuserdict,re.sub(r'\.C\Z','_rdict.pcm',rootuserdict))

#######  Start of main SConscript ###########

list = Split("""
SciFi.C
""")

sotarget = 'User'
srclib = baseenv.SharedLibrary(target = sotarget, source = list+[rootuserdict],\
                               LIBS = [''], LIBPATH = [''])
