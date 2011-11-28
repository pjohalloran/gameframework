# Location of bin, libs and the name of the project.
#BIN				= ../../bin
#LIBS			= ../../libs
#TEST_NAME		= unittests

# Where to look for the tests
TESTS            = ./*.h

# Where the CxxTest distribution is unpacked
CXXTESTDIR       = ../../3rdParty/cxxtest-3.10.1

# Check CXXTESTDIR
!if !exist($(CXXTESTDIR)/cxxtestgen.pl)
!error Please fix CXXTESTDIR
!endif

# cxxtestgen needs Perl or Python
!if defined(PERL)
CXXTESTGEN       = $(PERL) $(CXXTESTDIR)/cxxtestgen.pl
!elseif defined(PYTHON)
CXXTESTGEN       = $(PYTHON) $(CXXTESTDIR)/cxxtestgen.py
!else
!error You must define PERL or PYTHON
!endif

# The arguments to pass to cxxtestgen
#  - ParenPrinter is the way MSVC likes its compilation errors
#  - --have-eh/--abort-on-fail are nice when you have them
CXXTESTGEN_FLAGS =        \
	--gui=Win32Gui        \
	--runner=ParenPrinter \
	--have-eh             \
	--abort-on-fail
#CXXTESTGEN_FLAGS =        \
#	--error-printer

# How to generate the test runner, `unittests.cpp'
unittests.cpp: $(TESTS)
	$(CXXTESTGEN) $(CXXTESTGEN_FLAGS) -o $@ $(TESTS)

# How to run the tests, which should be in DIR\unittests.exe
unittests: $(DIR)\unittests.exe
	$(DIR)\unittests.exe
