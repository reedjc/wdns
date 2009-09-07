AC_PREREQ([2.64])
AC_INIT([wreck], [0.1], [info@sie.isc.org])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([1.10 foreign])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AC_PROG_CC
AC_PROG_CC_C99
AM_PROG_CC_C_O
AC_PROG_INSTALL
AC_PROG_LIBTOOL

AC_CHECK_HEADER([ldns/ldns.h])
AC_CHECK_LIB([ldns], [ldns_wire2pkt],
    [AC_DEFINE([HAVE_LIBLDNS], [1], [Define to 1 if libldns works.])]
)

AC_CHECK_HEADER([pcap.h])
AC_CHECK_LIB([pcap], [pcap_loop],
    [AC_DEFINE([HAVE_LIBPCAP], [1], [Define to 1 if libpcap works.])]
)

AC_CHECK_HEADERS([arpa/inet.h stdbool.h stdint.h stdlib.h string.h unistd.h],
    ,
    [AC_MSG_ERROR([required header file not found])]
)

AC_ARG_ENABLE([debug], [AS_HELP_STRING([--enable-debug], [enable debugging])],
    [AC_DEFINE([DEBUG], [1], [Define to 1 to enable debugging.])]
)

AC_CONFIG_FILES([Makefile wreck/Makefile wreck/msg/Makefile])
AC_OUTPUT