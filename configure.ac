AC_INIT([soundbooze-alsa], [1.0])
AC_CONFIG_SRCDIR(main.c)

AC_MSG_NOTICE([SoundBooze])
echo "----------------------------"
echo "UDUK™ Free as an AIR License"
echo "----------------------------"
echo ""
AC_PROG_CC
AC_LANG(C)
AC_CHECK_HEADERS(pthread.h)
AC_CHECK_HEADERS(alsa/asoundlib.h)
AC_CHECK_LIB(pthread, pthread_create)
AC_CHECK_LIB(asound, snd_pcm_open)

AC_OUTPUT(Makefile)
