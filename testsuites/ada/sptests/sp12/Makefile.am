include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am

include $(top_srcdir)/ada.am

noinst_PROGRAMS = ada_sp12
ada_sp12_SOURCES = sp12.adb config.h sptest.adb sptest.ads
ada_sp12_SOURCES += init.c

ada_sp12$(EXEEXT): sp12.adb init.$(OBJEXT)
	$(GNATCOMPILE) -margs -a $< -o $@

scndir = $(rtems_ada_testsdir)
dist_scn_DATA = ada_sp12.scn

