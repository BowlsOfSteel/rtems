include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am

include $(top_srcdir)/ada.am

noinst_PROGRAMS = ada_tm29
ada_tm29_SOURCES = tm29.adb config.h tmtest.adb tmtest.ads
ada_tm29_SOURCES += init.c

ada_tm29$(EXEEXT): tm29.adb init.$(OBJEXT)
	$(GNATCOMPILE) -margs -a $< -o $@

