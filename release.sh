#!/bin/sh

DIR=/tmp
VER=`grep SLLT_VER_STR include/version.h | cut -f3 | sed s/\"//g`
NAME=spinnaker_tools_$VER

RELEASE=$DIR/$NAME

copy ()
{
  dir=$1
  shift 1
  args=$*

  mkdir -p $RELEASE/$dir
  cd $dir
  cp -pP $args $RELEASE/$dir
  cd - > /dev/null
}

echo "# Making $RELEASE.tgz"

mkdir -p $RELEASE

if [ ! -d $RELEASE ]
then
  echo "Not a directory"
  exit 1
fi

rm -rf $RELEASE/*

copy .         setup README

copy make      *.make
copy include   *.h
copy sark      *.c *.s make_arm make_gnu Makefile
copy spin1_api *.c make_arm make_gnu Makefile
copy lib       sark.sct sark.lnk
copy boot      sark.struct spin[1-5].conf scamp.boot

copy docs      ybug/ybug.pdf gdb-spin/gdb-spin.pdf spin1_api/spin1_api.pdf sark/sark.pdf

copy tools     ybug bmpc bmpstat tubotron tubogrid gdb-spin
copy tools     arm2gas h2asm mkaplx mkbuild mkbin

copy tools/SpiNN *.pm

copy apps/sdping    sdping.c sdping.ybug sdp_ping.pl sdp_recv.pl Makefile
copy apps/pt_demo   *.c pt_demo.ybug README pt_trig drawer.linux Makefile
copy apps/heat_demo heat_demo.c heat_demo.ybug visualiser.linux Makefile

for APP in hello interrupt simple ring life random data_abort gdb-test
do
  copy apps/$APP *.c *.ybug Makefile
done

cd $DIR
tar czf $NAME.tgz $NAME
cd - > /dev/null
