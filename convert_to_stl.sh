#!/bin/bash

# Copyright Charles Shapiro, 2 Oct 2016

#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <http://www.gnu.org/licenses/>.



# Constants -- change these to debug this script
EXEC=./3mf2stl
TMPDIR=/tmp/$0.$$
STEMNAME=$(basename $1 .3mf)
TMPZIP=${STEMNAME}.$$
OUTFILE=${STEMNAME}.stl

Usage() 
{
  echo Usage: $0 infile
  cat <<EOU
  Convert the .3mf infile to a .stl outfile.
EOU
}
#
# Main Line
#
# echo $#

if [ $# -ne 1 ]
then
   Usage $0
   exit 1
fi

if [ ! -f $1 ]
then
   echo I cannot see the file [$1].
   Usage $0
   exit 1
fi

mkdir ${TMPDIR}
cp $1 ${TMPDIR}
( cd $TMPDIR ; unzip $1 ; rm $1) > /dev/null
if [ -f /tmp/${TMPZIP} ]
then
   rm /tmp/${TMPZIP}
fi
(cd $TMPDIR ; zip -r ../${TMPZIP} . ) > /dev/null

${EXEC} -i /tmp/$TMPZIP -o ${OUTFILE}

if [ -f ${OUTFILE} ]
then
   echo Converted $1 into ${OUTFILE}
   rm /tmp/${TMPZIP}
   rm -rf ${TMPDIR}
else
   echo Oops. Something bad happened.  You might have to look at the source!
fi
